#include <FL/Fl.H>
#include <FL/fl_draw.H>

#include <FL/math.h>

#include "cairographicsdriver.hpp"

struct CairoGraphicsDriver::S
{
};

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::color(uchar const r, uchar const g, uchar const b)
{
  Fl_Graphics_Driver::color(fl_rgb_color(r, g, b));
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::color(Fl_Color const c)
{
  Fl_Graphics_Driver::color(c);

  uchar r, g, b;
  Fl::get_color(c, r, g, b);

  auto const cr(ctx());

  cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::point(int x, int y)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
	cairo_close_path(cr);

	cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::rect(int x, int y, int w, int h)
{
  int x1 = x+w-1, y1 = y+h-1;
  xyline(x, y, x1);
  xyline(x, y1, x1);
  yxline(x, y, y1);
  yxline(x1, y, y1);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::rectf(int x, int y, int w, int h)
{
  auto const cr(ctx());

  cairo_rectangle(cr, x, y, w, h);

  cairo_fill(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::line(int x, int y, int x1, int y1)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);
  cairo_line_to(cr, x1, y1);

  cairo_stroke(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::line(int x, int y, int x1, int y1, int x2, int y2)
{
  line(x, y, x1, y1);
  line(x1, y1, x2, y2);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::xyline(int x, int y, int x1)
{
  line(x, y, x1, y);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::xyline(int x, int y, int x1, int y2)
{
  xyline(x, y, x1);
  yxline(x1, y, y2);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::xyline(int x, int y, int x1, int y2, int x3)
{
  xyline(x, y, x1);
  yxline(x1, y, y2);
  xyline(x1, y2, x3);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::yxline(int x, int y, int y1)
{
  line(x, y, x, y1);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::yxline(int x, int y, int y1, int x2)
{
  yxline(x, y, y1);
  xyline(x, y1, x2);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::yxline(int x, int y, int y1, int x2, int y3)
{
  yxline(x, y, y1);
  xyline(x, y1, x2);
  yxline(x2, y1, y3);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::loop(int x0, int y0, int x1, int y1, int x2, int y2)
{
  line(x0, y0, x1, y1);
  line(x1, y1, x2, y2);
  line(x2, y2, x0, y0);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
  line(x0, y0, x1, y1);
  line(x1, y1, x2, y2);
  line(x2, y2, x3, y3);
  line(x3, y3, x0, y0);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::polygon(int x0, int y0, int x1, int y1, int x2, int y2)
{
  auto const cr(ctx());

  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_line_to(cr, x0, y0);

  cairo_fill(cr);
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
  auto const cr(ctx());

  cairo_move_to(cr, x0, y0);
  cairo_line_to(cr, x1, y1);
  cairo_line_to(cr, x2, y2);
  cairo_line_to(cr, x3, y3);
  cairo_line_to(cr, x0, y0);

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
void CairoGraphicsDriver::circle(double x, double y, double r)
{
  begin_loop();
  double X = r;
  double Y = 0;
  fl_vertex(x+X,y+Y);

  double rx = fabs(transform_dx(r, r));
  double ry = fabs(transform_dy(r, r));

  double circ = M_PI*0.5*(rx+ry);
  int segs = circ * 360 / 1000;  // every line is about three pixels long
  if (segs<16) segs = 16;

  double A = 2*M_PI;
  int i = segs;

  if (i) {
    double epsilon = A/i;                       // Arc length for equal-size steps
    double cos_e = cos(epsilon);        // Rotation coefficients
    double sin_e = sin(epsilon);
    do {
      double Xnew =  cos_e*X + sin_e*Y;
      Y = -sin_e*X + cos_e*Y;
      fl_vertex(x + (X=Xnew), y + Y);
    } while (--i);
  }
  end_loop();
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::arc(int xi, int yi, int w, int h, double a1, double a2)
{
  if (a2<=a1) return;

  double rx = w/2.0;
  double ry = h/2.0;
  double x = xi + rx;
  double y = yi + ry;
  double circ = M_PI*0.5*(rx+ry);
  int i, segs = circ * (a2-a1) / 1000;  // every line is about three pixels long
  if (segs<3) segs = 3;

  int px, py;
  a1 = a1/180*M_PI;
  a2 = a2/180*M_PI;
  double step = (a2-a1)/segs;

  int nx = x + sin(a1)*rx;
  int ny = y - cos(a1)*ry;
  for (i=segs; i>0; i--) {
    a1+=step;
    px = nx; py = ny;
    nx = x + sin(a1)*rx;
    ny = y - cos(a1)*ry;
    line(px, py, nx, ny);
  }
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
}

//////////////////////////////////////////////////////////////////////////////
double CairoGraphicsDriver::width(char const* const str, int const n)
{
  cairo_text_extents_t cte;

  auto const cr(ctx());

  std::string tmp(str, n);
  cairo_text_extents(cr, tmp.c_str(), &cte);

  return cte.width;
}

//////////////////////////////////////////////////////////////////////////////
int CairoGraphicsDriver::descent()
{
  cairo_font_extents_t cfe;

  auto const cr(ctx());

  cairo_font_extents(cr, &cfe);

  return cfe.descent;
}

//////////////////////////////////////////////////////////////////////////////
int CairoGraphicsDriver::height()
{
  cairo_font_extents_t cfe;

  auto const cr(ctx());

  cairo_font_extents(cr, &cfe);

  return cfe.height;
}

//////////////////////////////////////////////////////////////////////////////
void CairoGraphicsDriver::draw(char const* const str, int const n,
  int const x, int const y)
{
  auto const cr(ctx());

  cairo_move_to(cr, x, y);

  cairo_show_text(cr, str);
}
