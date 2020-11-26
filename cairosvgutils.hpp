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

struct NSVGshape* find_svg_shape(struct NSVGimage*) noexcept;
void draw_svg_shape(cairo_t*, struct NSVGshape*) noexcept;

#endif // CAIRO_SVG_UTILS_HPP
