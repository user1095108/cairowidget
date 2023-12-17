#include <cassert>
#include <climits>
#include <cstdio>
#include <cstdint>
#include <cstring>

#include <algorithm> // std::copy
#include <execution>
#include <utility> // std::index_sequence

#include <array>
#include <string_view>

#include "cairo/cairo.h"

#define NANOSVG_ALL_COLOR_KEYWORDS
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/src/nanosvg.h"

#include "caironanosvg.hpp"

//////////////////////////////////////////////////////////////////////////////
inline auto inverse(float const* const f0) noexcept
{
  std::array<float, 6> f1{
    f0[3], -f0[1],
    -f0[2], f0[0],
    f0[2] * f0[5] - f0[3] * f0[4],
    f0[1] * f0[4] - f0[0] * f0[5]
  };

  std::transform(
    std::execution::unseq,
    f1.cbegin(),
    f1.cend(),
    f1.begin(),
    [invdet(1.f / (f0[0] * f0[3] - f0[2] * f0[1]))](auto const f) noexcept
    {
      return f * invdet;
    }
  );

  return f1;
}

inline auto to_rgba(std::uint32_t const c) noexcept
{
  std::array<double, 4> r{
    double(std::uint8_t(c)),
    double(std::uint8_t(c >> 8)),
    double(std::uint8_t(c >> 16)),
    double(std::uint8_t(c >> 24))
  };

  constexpr auto k{1. / 255.};

  std::transform(
    std::execution::unseq,
    r.cbegin(),
    r.cend(),
    r.begin(),
    [](auto const d) noexcept
    {
      return k * d;
    }
  );

  return r;
}

//////////////////////////////////////////////////////////////////////////////
inline void draw_svg_shape(cairo_t* const cr,
  struct NSVGshape const* const shape)
{
  cairo_new_path(cr);

  for (auto path(shape->paths); path; path = path->next)
  {
    {
      auto p(path->pts);

      //cairo_new_sub_path(cr);
      cairo_move_to(cr, p[0], p[1]);

      auto const end(p + 2 * path->npts);

      for (p += 2; end != p; p += 6)
      {
        cairo_curve_to(cr, p[0], p[1], p[2], p[3], p[4], p[5]);
      }
    }

    if (path->closed) cairo_close_path(cr);
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

            cairo_set_source_rgba(
              cr,
              c[0], c[1], c[2],
              shape->opacity * c[3]
            );
          }

          cairo_fill_preserve(cr);

          goto stroke;

        case NSVG_PAINT_LINEAR_GRADIENT:
          {
            auto const t(inverse(shape->fill.gradient->xform));

            pat = cairo_pattern_create_linear(
              t[4],
              t[5],
              t[2] + t[4],
              t[3] + t[5]
            );

            break;
          }

        case NSVG_PAINT_RADIAL_GRADIENT:
          {
            auto const& g(*shape->fill.gradient);

            auto const t(inverse(g.xform));
            auto const r(-t[0]);

            pat = cairo_pattern_create_radial(
              g.fx * r,
              g.fy * r,
              .0,
              t[4],
              t[5],
              t[0]
            );

            break;
          }

        default:
          assert(0);
      }

      assert(pat);
      auto const& g(*shape->fill.gradient);

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

        for (decltype(g.nstops) i{}; ns != i; ++i)
        {
          auto const& stop(g.stops[i]);

          auto const c(to_rgba(stop.color));

          cairo_pattern_add_color_stop_rgba(
            pat,
            stop.offset,
            c[0], c[1], c[2],
            shape->opacity * c[3]
          );
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

        cairo_set_source_rgba(cr, c[0], c[1], c[2], shape->opacity * c[3]);
      }

      if (auto const count(shape->strokeDashCount); count)
      {
        double dashes[
          sizeof(shape->strokeDashArray) / sizeof(*shape->strokeDashArray)
        ];

        std::copy(
          shape->strokeDashArray,
          shape->strokeDashArray + count,
          dashes
        );

        cairo_set_dash(
          cr,
          dashes,
          count,
          shape->strokeDashOffset
        );
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
void draw_svg_image(cairo_t* const cr, struct NSVGimage const* const image,
  double const w, double const h)
{
  {
    auto const sm(std::min(w / image->width, h / image->height));

    cairo_translate(
      cr,
      .5 * (w - sm * image->width),
      .5 * (h - sm * image->height)
    );

    cairo_scale(cr, sm, sm);
  }

  // draw shapes
  for (auto shape(image->shapes); shape; shape = shape->next)
  {
    draw_svg_shape(cr, shape);
  }
}

//////////////////////////////////////////////////////////////////////////////
void draw_svg_image(cairo_t* const cr, struct NSVGimage const* const image,
  double const x, double const y, double const w, double const h)
{
  cairo_save(cr);

  {
    auto const sm(std::min(w / image->width, h / image->height));

    cairo_translate(
      cr,
      x + .5 * (w - sm * image->width),
      y + .5 * (h - sm * image->height)
    );

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
inline struct NSVGshape const* find_svg_shape(
  struct NSVGimage const* const image,
  std::string_view const& name) noexcept
{
  for (auto shape(image->shapes); shape; shape = shape->next)
  {
    if (!std::strncmp(shape->id, name.data(), name.size())) return shape;
  }

  return {};
}

//////////////////////////////////////////////////////////////////////////////
void draw_svg_shape(cairo_t* const cr, struct NSVGimage const* const image, 
  std::string_view const& name) noexcept
{
  if (auto const shape(find_svg_shape(image, name)); shape)
  {
    draw_svg_shape(cr, shape);
  }
}
