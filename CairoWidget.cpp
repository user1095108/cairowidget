#include <memory>

#include <QPainter>

#include "CairoWidget.hpp"

struct CairoWidget::S
{
  static inline std::size_t datasize_;
  static inline std::unique_ptr<unsigned char[]> data_;
};

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(QWidget* const p, Qt::WindowFlags const wf):
  QWidget(p, wf),
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
CairoWidget::~CairoWidget() noexcept
{
  cairo_destroy(cr_);
}

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::paintEvent(QPaintEvent*)
{
  auto d(S::data_.get());

  auto const w(width()), h(height());

  {
    auto cr(cr_);

    if ((w != w_) || (h != h_) || (d != d_))
    {
      w_ = w; h_ = h;

      //
      auto const stride(stride_ =
        cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, w));

      if (auto const datasize(h * std::size_t(stride));
        S::datasize_ < datasize)
      {
        S::data_.reset(d = new unsigned char[S::datasize_ = datasize]);
      }

      d_ = d;

      //
      cairo_destroy(cr);

      auto const surf(cairo_image_surface_create_for_data(d,
        CAIRO_FORMAT_ARGB32, w, h, stride));
      cr_ = cr = cairo_create(surf);
      cairo_surface_destroy(surf);

      //
      if_(cr, w, h);
    }

    //
    cairo_save(cr);

    df_(cr, w, h);

    cairo_restore(cr);
  }

  //
  //cairo_surface_flush(surf);

  QPainter painter(this);

  painter.setCompositionMode(QPainter::CompositionMode_Source);
//painter.drawImage(0, 0, QImage(d, w, h, stride_, QImage::Format_ARGB32));
  painter.drawPixmap(0, 0, QPixmap::fromImage(
    QImage(d, w, h, stride_, QImage::Format_ARGB32)));
}
