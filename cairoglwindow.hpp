#ifndef CAIROGLWINDOW_HPP
# define CAIROGLWINDOW_HPP
# pragma once

#include "cairo.h"

#include "FL/Fl_Gl_Window.H"

#include "cairosurfacedevice.hpp"

#include <functional>

class Cairo_Gl_Window: public Fl_Gl_Window
{
  struct S;

  cairo_t* cr_{};
  cairo_surface_t* surf_{};

  std::unique_ptr<CairoSurfaceDevice> surface_device_{new CairoSurfaceDevice};

  using draw_t = std::function<void(cairo_t*, int, int)>;
  draw_t d_{[](cairo_t*, int, int) noexcept {}};

  draw_t i_{[](cairo_t* const cr, int, int) noexcept
    {
      cairo_set_line_width(cr, 1.);
      cairo_translate(cr, .5, .5);
    }
  };

  void draw() final;

public:
  Cairo_Gl_Window(int, int, const char* = nullptr);
  Cairo_Gl_Window(int, int, int, int, const char* = nullptr);
  ~Cairo_Gl_Window();

  //
  auto ctx() const noexcept;

  template <class U>
  void init(U&& u) noexcept(noexcept(i_ = std::forward<U>(u)))
  {
    i_ = std::forward<U>(u);
  }

  auto& draw() const noexcept;

  template <class U>
  void draw(U&& u) noexcept(noexcept(d_ = std::forward<U>(u)))
  {
    d_ = std::forward<U>(u);
  }
};

//////////////////////////////////////////////////////////////////////////////
inline auto Cairo_Gl_Window::ctx() const noexcept
{
  return cr_;
}

//////////////////////////////////////////////////////////////////////////////
inline auto& Cairo_Gl_Window::draw() const noexcept
{
  return d_;
}

#endif // CAIROWINDOW_HPP
