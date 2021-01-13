#include "FL/fl_draw.H"

#include <cassert>

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

  decltype(surf_) surf;
  auto cr(cr_);

  if (!cr ||
    (cairo_image_surface_get_width(surf = surf_) != ww) ||
    (cairo_image_surface_get_height(surf) != wh))
  {
    // cr invalidated or not existing
    cairo_destroy(cr);

    // generate a cairo context
    cr_ = cr = cairo_create(surf_ = surf =
      cairo_image_surface_create(CAIRO_FORMAT_RGB24, ww, wh));
    assert(cairo_surface_status(surf) == CAIRO_STATUS_SUCCESS);
    assert(cairo_status(cr) == CAIRO_STATUS_SUCCESS);
    cairo_surface_destroy(surf);

    size_ = std::size_t(cairo_image_surface_get_stride(surf)) * wh;

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
  std::transform(std::execution::unseq, src, src + size_ / 4, src,
    [](auto const a) noexcept { return shuffle<2, 1, 0>(a); });

/*
  auto const src(cairo_image_surface_get_data(surf));

  std::transform(std::execution::par_unseq,
    pixel_iterator<unsigned char, 4>(src),
    pixel_iterator<unsigned char, 4>(src + size_),
    pixel_iterator<unsigned char, 4>(src),
    [](auto const& s) noexcept ->
      typename pixel_iterator<unsigned char, 4>::value_type
    {
      if constexpr (std::endian::little == std::endian::native)
      {
        // BGRA -> RGBx
        return {s[2], s[1], s[0]};
      }
      else if constexpr (std::endian::big == std::endian::native)
      {
        // ARGB -> RGBx
        return {s[1], s[2], s[3]};
      }
    }
  );
*/

  //cairo_surface_mark_dirty(surf);

  fl_draw_image(reinterpret_cast<uchar*>(src), x(), y(), ww, wh, 4);
}
