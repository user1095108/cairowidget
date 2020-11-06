#include "Fl/Fl.h"
#include "Fl/fl_draw.h"

#include <cstdint>

#include "cairowidget.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l)
{
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget()
{
  cairo_destroy(cr_);
}

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::draw()
{
  auto const ww(w()), wh(h());

  auto cr(cr_);
  cairo_surface_t* surf;

  if (!cr ||
    (cairo_image_surface_get_width(surf = cairo_get_target(cr)) != ww) ||
    (cairo_image_surface_get_height(surf) != wh))
  {
    // cr invalidated or not existing
    cairo_destroy(cr);

    // generate a cairo context
    cr_ = cr = cairo_create(surf =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, ww, wh));
    cairo_surface_destroy(surf);

    // some defaults
    cairo_set_line_width(cr, 1.);
    cairo_translate(cr, .5, .5);
  }

  if (cr)
  {
    cairo_save(cr);

    d_(cr, ww, wh);

    cairo_restore(cr);

    //
    //cairo_surface_flush(surf);

#if defined(__GNUC__)
    auto const converter([](void* const s, int const x, int const y,
      int w, uchar* buf) noexcept
      {
        auto const surf(static_cast<cairo_surface_t*>(s));

        auto dst(reinterpret_cast<std::uint32_t*>(buf));
        auto src(reinterpret_cast<std::uint32_t*>(
          cairo_image_surface_get_data(surf) +
          (y * cairo_image_surface_get_stride(surf))) + x);

        while (w--)
        {
          *dst++ = __builtin_bswap32(*src++ << 8);
        }
      }
    );

    fl_draw_image(converter, surf, x(), y(), ww, wh, 4);
#else
    auto const converter([](void* const s, int const x, int const y,
      int w, uchar* dst) noexcept
      {
        auto const surf(static_cast<cairo_surface_t*>(s));

        auto src(cairo_image_surface_get_data(surf) +
          (y * cairo_image_surface_get_stride(surf)) + x * 4);

        for (; w--; src += 4, dst += 3)
        {
          dst[0] = src[2];
          dst[1] = src[1];
          dst[2] = src[0];
        }
      }
    );

    fl_draw_image(converter, surf, x(), y(), ww, wh, 3);
#endif
  }
}
