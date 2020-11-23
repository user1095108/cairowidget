#ifndef CAIRO_SVG_UTILS_HPP
# define CAIRO_SVG_UTILS_HPP
# pragma once

#include "cairo/cairo.h"

#include "Fl/Fl_Image.H"

#include "nanosvg.h"

void draw_svg_image(cairo_t*, struct NSVGimage*,
  double = 0, double = 0, int = 0, int = 0) noexcept;
void draw_svg_image(Fl_Image*, struct NSVGimage*,
  double = 0, double = 0) noexcept;

#endif // CAIRO_SVG_UTILS_HPP
