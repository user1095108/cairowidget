#include <QPainter>

#include "cairo/cairo.h"

#include "CairoPaintedItem.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoPaintedItem::CairoPaintedItem(QQuickItem* const p):
  QQuickPaintedItem(p)
{
  setRenderTarget(QQuickPaintedItem::Image);
}

//////////////////////////////////////////////////////////////////////////////
CairoPaintedItem::~CairoPaintedItem() noexcept { cairo_destroy(cr_); }

//////////////////////////////////////////////////////////////////////////////
void CairoPaintedItem::init(cairo_t* const cr, int, int)
{
  cairo_set_line_width(cr, 1.);
  cairo_translate(cr, .5, .5);
}

//////////////////////////////////////////////////////////////////////////////
void CairoPaintedItem::paint(QPainter* const p)
{
  auto cr(cr_);

  auto const w(width()), h(height());

  {
    auto const img(static_cast<QImage*>(p->device()));
    auto const d(img->bits());

    if ((d != d_) || (w != w_) || (h != h_))
    {
      d_ = d; w_ = w; h_ = h;

      //
      cairo_destroy(cr);

      auto const srf(
        cairo_image_surface_create_for_data(
          d,
          CAIRO_FORMAT_ARGB32,
          w,
          h,
          img->bytesPerLine()
        )
      );

      cr_ = cr = cairo_create(srf);
      cairo_surface_destroy(srf);

      init(cr, w, h);
    }
  }

  //
  cairo_save(cr);

  paint(cr, w, h);

  cairo_restore(cr);
}
