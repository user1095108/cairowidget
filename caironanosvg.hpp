#ifndef CAIRO_SVG_UTILS_HPP
# define CAIRO_SVG_UTILS_HPP
# pragma once

#include <cstring>

#include <string_view>

#include "cairo/cairo.h"

#include "nanosvg/src/nanosvg.h"

void draw_svg_image(cairo_t*, struct NSVGimage*,
  double = 0, double = 0, double = 0, double = 0) noexcept;

auto find_svg_shape(struct NSVGimage*, std::string_view const&) noexcept;

void draw_svg_shape(cairo_t*, struct NSVGshape*) noexcept;
void draw_svg_shape(cairo_t*, struct NSVGimage*,
  std::string_view const&) noexcept;

//////////////////////////////////////////////////////////////////////////////
inline auto find_svg_shape(struct NSVGimage* const image,
  std::string_view const& name) noexcept
{
  for (auto shape(image->shapes); shape; shape = shape->next)
  {
    if (!std::strncmp(shape->id, name.data(), name.size()))
    {
      return shape;
    }
  }

  return static_cast<struct NSVGshape*>(nullptr);
}

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
