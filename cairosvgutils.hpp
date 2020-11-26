#ifndef CAIRO_SVG_UTILS_HPP
# define CAIRO_SVG_UTILS_HPP
# pragma once

#include "cairo/cairo.h"

#include "nanosvg.h"

#include <string_view>

class Fl_Image;

void draw_svg_image(cairo_t*, struct NSVGimage*,
  double = 0, double = 0, double = 0, double = 0) noexcept;
void draw_svg_image(Fl_Image*, struct NSVGimage*,
  double = 0, double = 0) noexcept;

struct NSVGshape* find_svg_shape(struct NSVGimage*,
  std::string_view const&) noexcept;

void draw_svg_shape(cairo_t*, struct NSVGshape*) noexcept;
void draw_svg_shape(cairo_t*, struct NSVGimage*,
  std::string_view const&) noexcept;

//////////////////////////////////////////////////////////////////////////////
inline void draw_svg_shape(cairo_t* const cr, struct NSVGimage* const image, 
  std::string_view const& name) noexcept
{
  if (auto const shape(find_svg_shape(image, name)); shape)
  {
    draw_svg_shape(cr, shape);
  }
}

#endif // CAIRO_SVG_UTILS_HPP
