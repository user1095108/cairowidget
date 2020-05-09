#ifndef CAIROWIDGET_HPP
# define CAIROWIDGET_HPP
# pragma once

#include "cairo/cairo.h"

#include "Fl/Fl_Widget.h"

class CairoWidget : public Fl_Widget
{
  struct win_info
  {
    cairo_t* cr;

    int w;
    int h;

    cairo_surface_t* surf;
  };

public:
  CairoWidget(int, int, int, int, const char* = nullptr);
  ~CairoWidget();

  virtual void draw(cairo_t*, int, int) noexcept = 0;

private:
  void draw() final;
};

//////////////////////////////////////////////////////////////////////////////
inline CairoWidget::CairoWidget(int const x, int const y,
  int const w, int const h, const char* const l) :
  Fl_Widget(x, y, w, h, l)
{
}

#endif // CAIROWIDGET_HPP
