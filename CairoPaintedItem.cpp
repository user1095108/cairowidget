#include "cairo/cairo.h"

#include <QPainter>

#include "CairoPaintedItem.hpp"

struct CairoPaintedItem::S
{
  static inline std::size_t size_;
  static inline unsigned char* data_;
};

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
  auto d(S::data_);

  int const w(width()), h(height());

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
        delete [] d; S::data_ = d = new unsigned char[S::size_ = size];
      }

      d_ = d; // multiple instances

      //
      cairo_destroy(cr);

      auto const srf(cairo_image_surface_create_for_data(
        d, CAIRO_FORMAT_ARGB32, w, h, stride));
      cr_ = cr = cairo_create(srf);
      cairo_surface_destroy(srf);

      //
      init(cr, w, h);
    }

    //
    cairo_save(cr);

    draw(cr, w, h);

    cairo_restore(cr);
  }

  //
  p->setCompositionMode(QPainter::CompositionMode_Source);
  p->drawPixmap(QPoint{}, QPixmap::fromImage(
    {d, w, h, stride_, QImage::Format_ARGB32_Premultiplied}));
}
