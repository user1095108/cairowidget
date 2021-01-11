#include "FL/fl_draw.H"

#include <cstdint>

#include <algorithm>

#include <execution>

#include <utility>

#include "shuffler.hpp"

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

  cairo_save(cr);

  d_(cr, ww, wh);

  cairo_restore(cr);

  //cairo_surface_flush(surf);

  auto const converter([](void* const s, int const x, int const y, int w,
    uchar* const buf) noexcept
    {
      auto const surf(static_cast<cairo_surface_t*>(s));

      auto const src(reinterpret_cast<std::uint32_t const*>(
        cairo_image_surface_get_data(surf) +
        (y * cairo_image_surface_get_stride(surf))) + x);

      // ARGB -> RGBx (selects bytes and places them MSB -> LSB)
#if defined(__cpp_rtti)
      std::transform(std::execution::unseq, src, src + w,
        reinterpret_cast<std::uint32_t*>(buf),
        [](auto const a) noexcept { return shuffle<2, 1, 0>(a); });
#else
      std::transform(src, src + w, reinterpret_cast<std::uint32_t*>(buf),
        [](auto const a) noexcept { return shuffle<2, 1, 0>(a); });
#endif // __cpp_rtti
    }
  );

  fl_draw_image(converter, surf, x(), y(), ww, wh, 4);
}
