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
  std::size_t const ww(w()), wh(h());

  auto cr(cr_);
  cairo_surface_t* surf;

  if (!cr ||
    (cairo_image_surface_get_width(surf = cairo_get_target(cr)) != int(ww)) ||
    (cairo_image_surface_get_height(surf) != int(wh)))
  {
    // cr invalidated or not existing
    cairo_destroy(cr);

    // generate a cairo context
    cr_ = cr = cairo_create(surf =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, ww, wh));
    cairo_surface_destroy(surf);

    size_ = cairo_image_surface_get_stride(surf) * wh / 4;

    // some defaults
    cairo_set_line_width(cr, 1.);
    cairo_translate(cr, .5, .5);
  }

  cairo_save(cr);

  d_(cr, ww, wh);

  cairo_restore(cr);

  // shuffle the entire surface at once
  //cairo_surface_flush(surf);

  auto const src(reinterpret_cast<std::uint32_t*>(
    cairo_image_surface_get_data(surf)));

  // ARGB -> RGBx (selects bytes and places them MSB -> LSB)
  std::transform(std::execution::unseq, src, src + size_, src,
    [](auto const a) noexcept { return shuffle<2, 1, 0>(a); });

  //cairo_surface_mark_dirty(surf);

  fl_draw_image(reinterpret_cast<uchar*>(src), x(), y(), ww, wh, 4);
}
