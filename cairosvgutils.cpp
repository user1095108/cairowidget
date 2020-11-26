#include "Fl/Fl_Image.H"

#include <cassert>

#include <algorithm>

#include <bit>

#include <iterator>

#include <array>

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "cairosvgutils.hpp"

//////////////////////////////////////////////////////////////////////////////
constexpr auto to_rgba(unsigned int const c) noexcept
{
  return std::array<double, 4>{
    (c & 0xff) / 255.,
    ((c >> 8) & 0xff) / 255.,
    ((c >> 16) & 0xff) / 255.,
    ((c >> 24) & 0xff) / 255.
  };
}

inline auto inverse(float const* const t) noexcept
{
  auto const invdet(1. / (double(t[0]) * t[3] - double(t[2]) * t[1]));

  return std::array<double, 6>{
    t[3] * invdet, -t[1] * invdet,
    -t[2] * invdet, t[0] * invdet,
    (double(t[2]) * t[5] - double(t[3]) * t[4]) * invdet,
    (double(t[1]) * t[4] - double(t[0]) * t[5]) * invdet
  };
}

//////////////////////////////////////////////////////////////////////////////
void draw_svg_shape(cairo_t* const cr, struct NSVGshape* const shape) noexcept
{
  cairo_new_path(cr);

  for (auto path(shape->paths); path; path = path->next)
  {
    {
      //cairo_new_sub_path(cr);
      auto p(path->pts);

      cairo_move_to(cr, p[0], p[1]);

      auto const end(p + 2 * path->npts);

      for (p += 2; end != p; p += 6)
      {
        cairo_curve_to(cr, p[0], p[1], p[2], p[3], p[4], p[5]);
      }
    }

    if (path->closed)
    {
      cairo_close_path(cr);
    }
  }

  // fill
  switch (auto const type(shape->fill.type); type)
  {
    case NSVG_PAINT_NONE:
      break;

    case NSVG_PAINT_COLOR:
    case NSVG_PAINT_LINEAR_GRADIENT:
    case NSVG_PAINT_RADIAL_GRADIENT:
    {
      switch (shape->fillRule)
      {
        case NSVG_FILLRULE_NONZERO:
          cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
          break;

        case NSVG_FILLRULE_EVENODD:
          cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
          break;

        default:
          assert(0);
      }

      cairo_pattern_t* pat{};

      switch (type)
      {
        case NSVG_PAINT_COLOR:
          {
            auto const c(to_rgba(shape->fill.color));
            cairo_set_source_rgba(cr, c[0], c[1], c[2],
              shape->opacity * c[3]);
          }

          cairo_fill_preserve(cr);

          goto stroke;

        case NSVG_PAINT_LINEAR_GRADIENT:
          {
            auto const t(inverse(shape->fill.gradient->xform));

            pat = cairo_pattern_create_linear(t[4], t[5],
              t[4] + t[2], t[5] + t[3]);

            break;
          }

        case NSVG_PAINT_RADIAL_GRADIENT:
          {
            auto& g(*shape->fill.gradient);

            auto const t(inverse(g.xform));

            auto const r(t[0]);
            pat = cairo_pattern_create_radial(g.fx * r, g.fy * r, 0.,
              t[4], t[5], r);

            break;
          }

        default:
          assert(0);
      }

      assert(pat);

      auto& g(*shape->fill.gradient);

      switch (g.spread)
      {
        case NSVG_SPREAD_PAD:
          cairo_pattern_set_extend(pat, CAIRO_EXTEND_PAD);
          break;

        case NSVG_SPREAD_REFLECT:
          cairo_pattern_set_extend(pat, CAIRO_EXTEND_REFLECT);
          break;

        case NSVG_SPREAD_REPEAT:
          cairo_pattern_set_extend(pat, CAIRO_EXTEND_REPEAT);
          break;

        default:
          assert(0);
      }

      {
        auto const ns(g.nstops);

        for (int i{}; ns != i; ++i)
        {
          auto& stop(g.stops[i]);

          auto const c(to_rgba(stop.color));
          cairo_pattern_add_color_stop_rgba(pat, stop.offset,
            c[0], c[1], c[2], shape->opacity * c[3]);
        }
      }

      cairo_set_source(cr, pat);

      cairo_fill_preserve(cr);

      cairo_pattern_destroy(pat);

      break;
    }

    default:
      assert(0);
  }

  // stroke
  stroke:
  switch (shape->stroke.type)
  {
    case NSVG_PAINT_NONE:
      break;

    case NSVG_PAINT_COLOR:
      {
        auto const c(to_rgba(shape->stroke.color));
        cairo_set_source_rgba(cr, c[0], c[1], c[2],
          shape->opacity * c[3]);
      }

      if (auto const count(shape->strokeDashCount); count)
      {
        double dashes[sizeof(shape->strokeDashArray) /
          sizeof(*shape->strokeDashArray)];

        std::copy(shape->strokeDashArray, shape->strokeDashArray + count,
          std::begin(dashes));

        cairo_set_dash(cr, dashes, shape->strokeDashCount,
          shape->strokeDashOffset);
      }

      switch (shape->strokeLineCap)
      {
        case NSVG_CAP_BUTT:
          cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
          break;

        case NSVG_CAP_ROUND:
          cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
          break;

        case NSVG_CAP_SQUARE:
          cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
          break;

        default:
          assert(0);
      }

      switch (shape->strokeLineJoin)
      {
        case NSVG_JOIN_MITER:
          cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
          cairo_set_miter_limit(cr, shape->miterLimit);
          break;

        case NSVG_JOIN_ROUND:
          cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND);
          break;

        case NSVG_JOIN_BEVEL:
          cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);
          break;

        default:
          assert(0);
      }

      cairo_set_line_width(cr, shape->strokeWidth);

      cairo_stroke_preserve(cr);

      break;

    default:
      assert(0);
  }
}

//////////////////////////////////////////////////////////////////////////////
void draw_svg_image(cairo_t* const cr, struct NSVGimage* const image,
  double const x, double const y, double const w, double const h) noexcept
{
  cairo_save(cr);

  cairo_translate(cr, x, y);

  // preserve aspect ratio
  if (w && h)
  {
    auto const sm(std::min(w / image->width, h / image->height));
    cairo_scale(cr, sm, sm);
  }

  // draw shapes
  for (auto shape(image->shapes); shape; shape = shape->next)
  {
    if (NSVG_FLAGS_VISIBLE & shape->flags)
    {
      draw_svg_shape(cr, shape);
    }
  }

  cairo_restore(cr);
}

//////////////////////////////////////////////////////////////////////////////
void draw_svg_image(Fl_Image* const fli, struct NSVGimage* const image,
  double const x, double const y) noexcept
{
  assert(4 == fli->d()); // we are converting icons, hence 4 channels
  auto const w(fli->w()), h(fli->h());

  auto const surf(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h));
  auto const cr(cairo_create(surf));
  cairo_surface_destroy(surf);

  //
  cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

  draw_svg_image(cr, image, x, y, w, h);

  //
  auto dst(const_cast<char*>(fli->data()[0]));
  auto src(cairo_image_surface_get_data(surf));

  for (auto const end(src + 4 * w * h); end != src; src += 4, dst += 4)
  {
    // ARGB (cairo) -> RGBA (fltk)
    if constexpr (std::endian::little == std::endian::native)
    {
      dst[0] = src[2]; dst[1] = src[1]; dst[2] = src[0]; dst[3] = src[3];
    }
    else if constexpr (std::endian::big == std::endian::native)
    {
      dst[0] = src[1]; dst[1] = src[2]; dst[2] = src[3]; dst[3] = src[0];
    }
  }

  //
  cairo_destroy(cr);
}
