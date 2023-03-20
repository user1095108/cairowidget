#include <QBackingStore>

#include "cairo/cairo.h"

#include "CairoWidget.hpp"

//an unreliable, but fast, alternative implementation/////////////////////////
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
CairoWidget::~CairoWidget() noexcept { cairo_destroy(cr_); }

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::paintEvent(QPaintEvent*)
{
  auto const w(width()), h(height());

  {
    auto cr(cr_);

    if ((w != w_) || (h != h_))
    {
      w_ = w; h_ = h;

      //
      auto const stride(stride_ =
        cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, w));

      //
      cairo_destroy(cr);

      auto const surf(cairo_image_surface_create_for_data(
        static_cast<QImage*>(backingStore()->paintDevice())->bits(),
        CAIRO_FORMAT_ARGB32,
        w,
        h,
        stride)
      );
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

  //cairo_surface_flush(surf);
}
