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

    pixels_ = cairo_image_surface_get_stride(surf) * wh / 4;
  }

  cairo_save(cr);

  d_(cr, ww, wh);

  cairo_restore(cr);

  // shuffle the entire surface at once, could be a source of bugs:
  // no flushing, no marking
  //cairo_surface_flush(surf);

  auto const src(reinterpret_cast<std::uint32_t*>(
    cairo_image_surface_get_data(surf)));

  // ARGB -> RGBx
  std::transform(std::execution::unseq, src, src + pixels_, src,
    [](auto const a) noexcept { return shuffle<2, 1, 0>(a); });

  //cairo_surface_mark_dirty(surf);

  fl_draw_image(reinterpret_cast<uchar*>(src), x(), y(), ww, wh, 4);
}
