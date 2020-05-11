#ifndef CAIROWIDGET_HPP
# define CAIROWIDGET_HPP
# pragma once

#include "cairo/cairo.h"

#include "Fl/Fl_Widget.h"

#include <functional>

class CairoWidget: public Fl_Widget
{
public:
  using draw_t = std::function<void(cairo_t*, int, int)>;

private:
  struct S;

  struct win_info
  {
    cairo_t* cr;

    int w;
    int h;

    cairo_surface_t* surf;

    Fl_Callback* c;
  };

  draw_t d_;

  void draw() override;

public:
  CairoWidget(int, int, int, int, const char* = nullptr);

  auto& draw() const noexcept;
  void draw(draw_t const&);
};

//////////////////////////////////////////////////////////////////////////////
inline void CairoWidget::draw(draw_t const& d)
{
  d_ = d;
}

inline auto& CairoWidget::draw() const noexcept
{
  return d_;
}

#endif // CAIROWIDGET_HPP
