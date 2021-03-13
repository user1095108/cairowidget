#include "FL/fl_draw.H"

#include <cassert>

#include <algorithm>

#include <execution>

#include <memory>

#include "shuffler.hpp"

#include "cairowidget.hpp"

struct CairoWidget::S
{
  // all drawing is done on the main thread, hence a static pixel buffer
  static inline std::size_t datasize_;
  static inline std::unique_ptr<unsigned char[]> data_;
};

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l),
  d_{[](cairo_t*, int, int) noexcept {}},
  i_{[](cairo_t* const cr, int, int) noexcept
    {
      cairo_set_line_width(cr, 1.);
      cairo_translate(cr, .5, .5);
    }
  }
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
  auto cr(cr_);

  auto d(S::data_.get());

  auto const w(this->w()), h(this->h());

  if ((w != w_) || (h != h_))
  {
    w_ = w; h_ = h;

    //
    auto const stride(cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, w));

    {
      auto const datasize(std::size_t(h) * std::size_t(stride));

      if (pixels_ = datasize / 4; S::datasize_ < datasize)
      {
        S::data_.reset(d = new unsigned char[S::datasize_ = datasize]);
      }
    }

    //
    cairo_destroy(cr);

    auto const surf(cairo_image_surface_create_for_data(d,
      CAIRO_FORMAT_RGB24, w, h, stride));
    assert(CAIRO_STATUS_SUCCESS == cairo_surface_status(surf));
    cr_ = cr = cairo_create(surf);
    assert(CAIRO_STATUS_SUCCESS == cairo_status(cr));
    cairo_surface_destroy(surf);

    //
    i_(cr, w, h);
  }

  //
  cairo_save(cr);

  d_(cr, w, h);

  cairo_restore(cr);

  //
  //cairo_surface_flush(surf);

  auto const src(reinterpret_cast<std::uint32_t*>(d));

  // ARGB -> RGBx (selects bytes and places them MSB -> LSB),
  std::transform(std::execution::unseq, src, src + pixels_, src,
    (std::uint32_t(&)(std::uint32_t))(shuffler::shuffle<2, 1, 0>));

  //cairo_surface_mark_dirty(surf);

  fl_draw_image(d, x(), y(), w, h, 4);
}
