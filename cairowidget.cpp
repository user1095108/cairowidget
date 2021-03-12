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
    cairo_image_surface_create(CAIRO_FORMAT_RGB24, 0, 0))),
  d_{[](cairo_t*, int, int) noexcept {}},
  i_{[](cairo_t* const cr, int, int) noexcept
    {
      cairo_set_line_width(cr, 1.);
      cairo_translate(cr, .5, .5);
    }
  }
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
  auto const w(this->w()), h(this->h());

  auto cr(cr_);
  
  if (auto surf(surf_); (cairo_image_surface_get_width(surf) != w) ||
    (cairo_image_surface_get_height(surf) != h))
  {
    //
    auto const stride(cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, w));

    {
      auto const newdatasize(h * stride);
      pixels_ = newdatasize / 4;

      if (datasize_ < newdatasize)
      {
        data_.reset(new unsigned char[datasize_ = newdatasize]);
      }
    }

    //
    cairo_destroy(cr);
    cr_ = cr = cairo_create(surf_ = surf =
      cairo_image_surface_create_for_data(data_.get(), CAIRO_FORMAT_RGB24,
      w, h, stride));
    cairo_surface_destroy(surf);
    assert(CAIRO_STATUS_SUCCESS == cairo_surface_status(surf));
    assert(CAIRO_STATUS_SUCCESS == cairo_status(cr));

    //
    i_(cr, w, h);
  }

  //
  cairo_save(cr);

  d_(cr, w, h);

  cairo_restore(cr);

  //
  //cairo_surface_flush(surf_);

  auto const src(reinterpret_cast<std::uint32_t*>(data_.get()));

  // ARGB -> RGBx (selects bytes and places them MSB -> LSB),
  std::transform(std::execution::unseq, src, src + pixels_, src,
    (std::uint32_t(&)(std::uint32_t))(shuffler::shuffle<2, 1, 0>));

  //cairo_surface_mark_dirty(surf_);

  fl_draw_image(reinterpret_cast<uchar*>(src), x(), y(), w, h, 4);
}
