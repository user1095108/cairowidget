#ifndef CAIROWIDGET_HPP
# define CAIROWIDGET_HPP
# pragma once

#include "cairo/cairo.h"

#include "Fl/Fl_Widget.h"

#include <functional>

class CairoWidget: public Fl_Widget
{
  struct S;

  using draw_t = std::function<void(cairo_t*, int, int)>;
  draw_t d_{[](cairo_t*, int, int) noexcept {}};

  void draw() final;

public:
  CairoWidget(int, int, int, int, const char* = nullptr);
  ~CairoWidget();

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
