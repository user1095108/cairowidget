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

  int w_{}, h_{};

  unsigned char* d_;

  unsigned pixels_;

  using draw_t = std::function<void(cairo_t*, int, int)>;

  draw_t df_;
  draw_t if_;

  void draw() final;

public:
  CairoWidget(int, int, int, int, const char* = nullptr);
  ~CairoWidget() noexcept;

  //
  template <class U>
  void init(U&& u) noexcept(noexcept(if_ = std::forward<U>(u)))
  {
    if_ = std::forward<U>(u);
  }

  auto& draw() const noexcept;

  template <class U>
  void draw(U&& u) noexcept(noexcept(df_ = std::forward<U>(u)))
  {
    df_ = std::forward<U>(u);
  }
};

//////////////////////////////////////////////////////////////////////////////
inline auto& CairoWidget::draw() const noexcept
{
  return df_;
}

#endif // CAIROWIDGET_HPP
