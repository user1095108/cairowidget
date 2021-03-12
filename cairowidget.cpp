#include "FL/fl_draw.H"

#include <cassert>

#include <algorithm>

#include <execution>

#include "shuffler.hpp"

#include "cairowidget.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l),
  cr_(cairo_create(surf_ =
    cairo_image_surface_create(CAIRO_FORMAT_RGB24, 0, 0)))
{
  cairo_surface_destroy(surf_);
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
  auto surf(surf_);

  if ((cairo_image_surface_get_width(surf) != ww) ||
    (cairo_image_surface_get_height(surf) != wh))
  {
    // cr invalidated or not existing
    cairo_destroy(cr);

    // generate a cairo context
    cr_ = cr = cairo_create(surf_ = surf =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, ww, wh));
    assert(CAIRO_STATUS_SUCCESS == cairo_surface_status(surf));
    assert(CAIRO_STATUS_SUCCESS == cairo_status(cr));
    cairo_surface_destroy(surf);

    size_ = std::size_t(cairo_image_surface_get_stride(surf)) * wh / 4;

    //
    i_(cr, ww, wh);
  }

  //
  cairo_save(cr);

  d_(cr, ww, wh);

  cairo_restore(cr);

  // shuffle the entire surface at once
  //cairo_surface_flush(surf);

  auto const src(reinterpret_cast<std::uint32_t*>(
    cairo_image_surface_get_data(surf)));

  // ARGB -> RGBx (selects bytes and places them MSB -> LSB),
  std::transform(std::execution::unseq, src, src + size_, src,
    (std::uint32_t(&)(std::uint32_t))(shuffler::shuffle<2, 1, 0>));

  //cairo_surface_mark_dirty(surf);

  fl_draw_image(reinterpret_cast<uchar*>(src), x(), y(), ww, wh, 4);
}
