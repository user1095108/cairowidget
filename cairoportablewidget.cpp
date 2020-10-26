#include "Fl/Fl.h"
#include "Fl/fl_draw.h"

#include <cstdint>
#include <cstring>

#include "cairowidget.hpp"

//////////////////////////////////////////////////////////////////////////////
struct CairoWidget::S
{
  static void free_cairo_resources(cairo_t* const cr) noexcept
  {
    cairo_surface_destroy(cr ? cairo_get_target(cr) : nullptr);
    cairo_destroy(cr);
  }
};

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l)
{
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget()
{
  S::free_cairo_resources(static_cast<cairo_t*>(user_data()));
}

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::draw()
{
  auto cr(static_cast<cairo_t*>(user_data()));
  auto surf(cr ? cairo_get_target(cr) : nullptr);

  if (auto const ww(w()), wh(h()); !surf ||
    ((cairo_image_surface_get_width(surf) != ww) &&
    (cairo_image_surface_get_height(surf) != wh)))
  {
    // cr invalidated or not existing
    S::free_cairo_resources(cr);

    // generate a cairo context
    if ((surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, ww, wh)))
    {
      // fill out wi and set cr
      user_data(cr = cairo_create(surf));
    }
    else
    {
      user_data(cr = {});
    }
  }

  if (cr)
  {
    cairo_save(cr);

    {
      uchar r, g, b;
      Fl::get_color(color(), r, g, b);

      cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
    }

    auto const wx(x()), wy(y()), ww(w()), wh(h());

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

    auto const converter([](void* const s, int const x, int const y,
      int w, uchar* const buf) noexcept
      {
        auto const surf(static_cast<cairo_surface_t*>(s));

        auto const data(cairo_image_surface_get_data(surf));
        auto const width(cairo_image_surface_get_width(surf));

        for (auto src(reinterpret_cast<std::uint32_t*>(data) +
          (y * width) + x), dst(reinterpret_cast<std::uint32_t*>(buf));
          w; --w)
        {
          *dst++ = __builtin_bswap32(*src++ << 8);
        }
      }
    );

    cairo_surface_flush(surf);
    fl_draw_image(converter, surf, wx, wy, ww, wh, 4);
  }
}
