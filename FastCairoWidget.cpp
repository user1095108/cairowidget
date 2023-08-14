#include <QBackingStore>

#include "cairo/cairo.h"

#include "CairoWidget.hpp"

//a fast but unreliable alternative implementation////////////////////////////
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
  setAttribute(Qt::WA_DontCreateNativeAncestors);
  setAttribute(Qt::WA_NativeWindow);
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget() { cairo_destroy(cr_); }

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::paintEvent(QPaintEvent*)
{
  auto cr(cr_);

  auto const w(width()), h(height());

  if ((w != w_) || (h != h_))
  {
    w_ = w; h_ = h;

    //
    cairo_destroy(cr);

    auto const img(static_cast<QImage*>(backingStore()->paintDevice()));

    auto const srf(
      cairo_image_surface_create_for_data(
        img->bits(),
        CAIRO_FORMAT_ARGB32,
        w,
        h,
        img->bytesPerLine()
      )
    );

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
