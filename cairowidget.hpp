#ifndef CAIROWIDGET_HPP
# define CAIROWIDGET_HPP
# pragma once

#include "cairo/cairo.h"

#include "FL/Fl_Widget.H"

#include <functional>

class CairoWidget: public Fl_Widget
{
  struct S;

  cairo_t* cr_{};

  using draw_t = std::function<void(cairo_t*, int, int)>;
  draw_t d_{[](cairo_t*, int, int) noexcept {}};

  void draw() final;

public:
  CairoWidget(int, int, int, int, const char* = nullptr);
  ~CairoWidget();

  auto& draw() const noexcept;

  template <class U>
  void draw(U&& u) noexcept(noexcept(d_ = std::forward<U>(u)))
  {
    d_ = std::forward<U>(u);
  }
};

//////////////////////////////////////////////////////////////////////////////
inline auto& CairoWidget::draw() const noexcept
{
  return d_;
}

#endif // CAIROWIDGET_HPP
