#include "cairo/cairo.h"
#include "FL/fl_draw.H"

#include <cstdint>
#include <execution>

#include "shuffler.hpp"

#include "cairowidget.hpp"

struct CairoWidget::S
{
  static inline std::size_t size_;
  static inline unsigned char* data_;
};

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l),
  df_{[](auto, auto, auto) noexcept {}},
  if_{[](auto const cr, auto, auto) noexcept
    {
      cairo_set_line_width(cr, 1.);
      cairo_translate(cr, .5, .5);
    }
  }
{
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget() { cairo_destroy(cr_); }

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::draw()
{
  auto d(S::data_);

  auto const w(this->w()), h(this->h());

  {
    auto cr(cr_);

    if ((w != w_) || (h != h_) || (d != d_))
    {
      w_ = w; h_ = h;

      //
      auto const stride(cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, w));

      {
        auto const size(h * std::size_t(stride));

        if (pixels_ = size / 4; S::size_ < size)
        {
          delete [] d; S::data_ = d = new unsigned char[S::size_ = size];
        }
      }

      d_ = d; // multiple instances

      //
      cairo_destroy(cr);

      auto const srf(cairo_image_surface_create_for_data(
        d, CAIRO_FORMAT_RGB24, w, h, stride));
      cr_ = cr = cairo_create(srf);
      cairo_surface_destroy(srf);

      //
      if_(cr, w, h);
    }

    //
    cairo_save(cr);

    df_(cr, w, h);

    cairo_restore(cr);
  }

  //
  auto const src(reinterpret_cast<std::uint32_t*>(d));

  // ARGB (native), RGB are at indices (2, 1, 0) -> RGBx conversion
  // le: (2, 1, 0) -> (0, 1, 2); be: (2, 1, 0) -> (3, 2, 1)
  std::transform(std::execution::unseq, src, src + pixels_, src,
    (std::uint32_t(&)(std::uint32_t))(shuffler::shuffle<2, 1, 0>));

  fl_draw_image(d, x(), y(), w, h, 4);
}
