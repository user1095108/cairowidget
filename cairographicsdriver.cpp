#include "FL/Fl.H"
#include "FL/fl_draw.H"

#include "FL/math.h"

#include <string>

#include "cairographicsdriver.hpp"

struct CairoGraphicsDriver::S
{
};

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::color(uchar const r, uchar const g, uchar const b)
{
  Fl_Graphics_Driver::color(fl_rgb_color(r, g, b));

  cairo_set_source_rgb(ctx(), r / 255., g / 255., b / 255.);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::color(Fl_Color const c)
{
  Fl_Graphics_Driver::color(c);

  uchar r, g, b;
  Fl::get_color(c, r, g, b);

  cairo_set_source_rgb(ctx(), r / 255., g / 255., b / 255.);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::point(int const x, int const y)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
	cairo_close_path(cr);

	cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::rect(int const x, int const y,
  int const w, int const h)
{
  auto const cr(ctx());

  cairo_rectangle(cr, x, y, w - 1, h - 1);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::rectf(int const x, int const y,
  int const w, int const h)
{
  auto const cr(ctx());

  cairo_rectangle(cr, x, y, w, h);

  cairo_fill(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::line(int const x1, int const y1,
  int const x2, int const y2)
{
  auto const cr(ctx());

  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::line(int const x1, int const y1,
  int const x2, int const y2, int const x3, int const y3)
{
  auto const cr(ctx());

  cairo_move_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_line_to(cr, x3, y3);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::xyline(int const x, int const y, int const x1)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x1, y);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::xyline(int const x, int const y, int const x1,
  int const y2)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x1, y);
  cairo_line_to(cr, x1, y2);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::xyline(int const x, int const y,
  int const x1, int const y2, int const x3)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x1, y);
  cairo_line_to(cr, x1, y2);
  cairo_line_to(cr, x3, y2);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::yxline(int const x, int const y, int const y1)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x, y1);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::yxline(int const x, int const y, int const y1,
  int const x2)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x, y1);
  cairo_line_to(cr, x2, y1);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::yxline(int const x, int const y, int const y1,
  int const x2, int const y3)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x, y1);
  cairo_line_to(cr, x2, y1);
  cairo_line_to(cr, x2, y3);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::loop(int const x0, int const y0,
  int const x1, int const y1, int const x2, int const y2)
{
  auto const cr(ctx());

  cairo_new_path(cr);

  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);

  cairo_close_path(cr);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::loop(int const x0, int const y0,
  int const x1, int const y1, int const x2, int const y2,
  int const x3, int const y3)
{
  auto const cr(ctx());

  cairo_new_path(cr);

  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_line_to(cr, x3, y3);

  cairo_close_path(cr);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::polygon(int const x0, int const y0,
  int const x1, int const y1, int const x2, int const y2)
{
  auto const cr(ctx());

  cairo_new_path(cr);

  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);

  cairo_close_path(cr);

  cairo_fill(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::polygon(int const x0, int const y0,
  int const x1, int const y1, int const x2, int const y2,
  int const x3, int const y3)
{
  auto const cr(ctx());

  cairo_new_path(cr);

  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_line_to(cr, x3, y3);

  cairo_close_path(cr);

  cairo_fill(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::push_clip(int x, int y, int w, int h)
{
}

//////////////////////////////////////////////////////////////////////////////
int CairoGraphicsDriver::clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H)
{
  X = x; Y = y; W = w; H = h;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
int CairoGraphicsDriver::not_clipped(int x, int y, int w, int h)
{
  return 1;
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::push_no_clip()
{
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::pop_clip()
{
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::circle(double const x, double const y,
  double const r)
{
  auto const cr(ctx());

  cairo_arc(cr, x, y, r, 0., 2 * M_PI);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::arc(int const x, int const y,
  int const w, int const h, double const a1, double const a2)
{
  auto const cr(ctx());

  cairo_save(cr);

  cairo_new_sub_path(cr);

  auto const hw(w / 2.), hh(h / 2.);
  cairo_translate(cr, x + hw, y + hh);
  cairo_scale(cr, hw, hh);
  cairo_arc(cr, 0., 0., 1., 0., 2 * M_PI);

  cairo_stroke(cr);

  cairo_restore(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::pie(int x, int y, int w, int h, double a1, double a2)
{
  // FIXME: implement this
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::line_style(int style, int width, char* dashes)
{
}

//////////////////////////////////////////////////////////////////////////////
Fl_Bitmask CairoGraphicsDriver::create_bitmask(int w, int h, const uchar *array)
{
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::delete_bitmask(Fl_Bitmask bm)
{
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::font(Fl_Font const face, Fl_Fontsize const fsize)
{
  Fl_Graphics_Driver::font(face, fsize);

  int attrs;
  Fl::get_font_name(face, &attrs);

  auto const cr(ctx());

  cairo_select_font_face(cr,
    Fl::get_font(face),
    attrs & FL_ITALIC ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
    attrs & FL_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, fsize);
  cairo_font_extents(cr, &cfe_);
}

//////////////////////////////////////////////////////////////////////////////
double CairoGraphicsDriver::width(char const* const str, int const n)
{
  std::string const tmp(str, n);

  cairo_text_extents_t cte;
  cairo_text_extents(ctx(), tmp.c_str(), &cte);

  return cte.width;
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::draw(char const* const str, int const n,
  int const x, int const y)
{
  std::string const tmp(str, n);

  auto const cr(ctx());

  cairo_move_to(cr, x, y);

  cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

  cairo_text_path(ctx(), tmp.c_str());
  cairo_fill(ctx());

  cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
}
