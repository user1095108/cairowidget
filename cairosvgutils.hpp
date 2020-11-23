#ifndef CAIRO_SVG_UTILS_HPP
# define CAIRO_SVG_UTILS_HPP
# pragma once

#include "cairo/cairo.h"

#include "nanosvg.h"

void draw_svg_image(cairo_t*, struct NSVGimage*,
  int = 0, int = 0, int = 0, int = 0) noexcept;

#endif // CAIRO_SVG_UTILS_HPP
