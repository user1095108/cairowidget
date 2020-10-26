#include "Fl/Fl.h"
#include "Fl/fl_draw.h"

#include <cstdint>
#include <cstring>
#include <utility>
#include <type_traits>

#include "cairowidget.hpp"

//////////////////////////////////////////////////////////////////////////////
template <typename To, typename From>
static To bit_cast(From const* const src) noexcept
{
  static_assert(std::is_trivially_constructible_v<To>);
  To dst;
  std::memcpy(&dst, src, sizeof(To));
  return dst;
}

template <typename T, std::size_t ...I>
static constexpr auto swap_impl(T const v, std::index_sequence<I...>) noexcept
{
  return (
    (
      ((v >> (I * 8)) & std::uint8_t(~0)) << ((sizeof(T) - 1 - I) * 8)
    ) | ...
  );
}

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
static constexpr auto swap(T&& v) noexcept
{
  return swap_impl(std::forward<T>(v), std::make_index_sequence<sizeof(T)>());
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l)
{
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget()
{
  cairo_destroy(static_cast<cairo_t*>(user_data()));
}

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::draw()
{
  auto const wx(x()), wy(y()), ww(w()), wh(h());

  auto cr(static_cast<cairo_t*>(user_data()));
  auto surf(cr ? cairo_get_target(cr) : nullptr);

  if (!surf ||
    (cairo_image_surface_get_width(surf) != ww) ||
    (cairo_image_surface_get_height(surf) != wh))
  {
    // cr invalidated or not existing
    cairo_destroy(cr);

    // generate a cairo context
    surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, ww, wh);
    user_data(cr = cairo_create(surf));
    cairo_surface_destroy(surf);
  }

  if (cr)
  {
    cairo_save(cr);

    {
      uchar r, g, b;
      Fl::get_color(color(), r, g, b);

      cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
    }

    {
      cairo_translate(cr, wx, wy);

      //
      cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

      cairo_rectangle(cr, 0., 0., ww, wh);

      cairo_fill_preserve(cr);
      cairo_clip(cr);

      //
      cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

      d_(cr, ww, wh);
    }

    cairo_restore(cr);

    //
    cairo_surface_flush(surf);

    auto const converter([](void* const s, int const x, int const y,
      int w, uchar* buf) noexcept
      {
        auto const surf(static_cast<cairo_surface_t*>(s));
        auto const width(cairo_image_surface_get_width(surf));

        auto data(cairo_image_surface_get_data(surf) + (y * width + x) * 4);

        for (; w; buf += 4, data += 4, --w)
        {
          auto const tmp(swap(bit_cast<std::uint32_t>(data) << 8));
          std::memcpy(buf, &tmp, sizeof(tmp));
        }
      }
    );

    fl_draw_image(converter, surf, wx, wy, ww, wh, 4);
  }
}
