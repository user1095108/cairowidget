#ifndef CAIRO_SVG_UTILS_HPP
# define CAIRO_SVG_UTILS_HPP
# pragma once

struct NSVGImage;
struct _cairo;

void draw_svg_image(_cairo*, struct NSVGimage const*, double, double);
void draw_svg_image(_cairo*, struct NSVGimage const*, double, double,
  double, double);

#endif // CAIRO_SVG_UTILS_HPP
