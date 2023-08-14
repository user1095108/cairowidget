#include <QPainter>
#include <QScopedArrayPointer>

#include "cairo/cairo.h"

#include "CairoWidget.hpp"

struct CairoWidget::S
{
  static inline std::size_t size_;
  static inline QScopedArrayPointer<unsigned char> data_;
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
CairoWidget::~CairoWidget() { cairo_destroy(cr_); }

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

      if (auto const size(h * std::size_t(stride)); S::size_ < size)
      {
        S::data_.reset(d = new unsigned char[S::size_ = size]);
      }

      d_ = d; // multiple instances

      //
      cairo_destroy(cr);

      auto const srf(cairo_image_surface_create_for_data(d,
        CAIRO_FORMAT_ARGB32, w, h, stride));
      cr_ = cr = cairo_create(srf);
      cairo_surface_destroy(srf);

      //
      if_(cr, w, h);
    }

    //
    cairo_save(cr);

    df_(cr, w, h);
    //cairo_surface_flush(srf);

    cairo_restore(cr);
  }

  //
  QPainter painter(this);

  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.drawPixmap(QPoint{}, QPixmap::fromImage(
      {d, w, h, stride_, QImage::Format_ARGB32_Premultiplied}
    )
  );
}
