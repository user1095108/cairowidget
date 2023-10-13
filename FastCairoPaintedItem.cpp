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
CairoPaintedItem::~CairoPaintedItem() { cairo_destroy(cr_); }

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

  int const w(width()), h(height());

  auto const img(static_cast<QImage*>(p->device()));

  if (auto const d(img->bits()); (w != w_) || (h != h_) || (d != d_))
  {
    w_ = w; h_ = h; d_ = d;

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

  //
  cairo_save(cr);

  draw(cr, w, h);

  cairo_restore(cr);

  //
  if (QImage::Format_RGBA8888_Premultiplied == img->format()) img->rgbSwap();
}
