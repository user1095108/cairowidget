#include "FL/Fl.h"

#include "FL/Fl_Double_Window.h"

#include "cairowidget.hpp"

#include <memory>

void example(cairo_t* const cr, int const w, int const h) noexcept
{
  // we need this
  cairo_scale(cr, w, h);

  // code from https://www.cairographics.org/tutorial/
  int i, j;
  cairo_pattern_t *radpat, *linpat;

  radpat = cairo_pattern_create_radial(0.25, 0.25, 0.1,  0.5, 0.5, 0.5);
  cairo_pattern_add_color_stop_rgb(radpat, 0,  1.0, 0.8, 0.8);
  cairo_pattern_add_color_stop_rgb(radpat, 1,  0.9, 0.0, 0.0);

  for (i=1; i<10; i++)
      for (j=1; j<10; j++)
          cairo_rectangle(cr, i/10.0 - 0.04, j/10.0 - 0.04, 0.08, 0.08);
  cairo_set_source(cr, radpat);
  cairo_fill(cr);

  linpat = cairo_pattern_create_linear(0.25, 0.35, 0.75, 0.65);
  cairo_pattern_add_color_stop_rgba(linpat, 0.00,  1, 1, 1, 0);
  cairo_pattern_add_color_stop_rgba(linpat, 0.25,  0, 1, 0, 0.5);
  cairo_pattern_add_color_stop_rgba(linpat, 0.50,  1, 1, 1, 0);
  cairo_pattern_add_color_stop_rgba(linpat, 0.75,  0, 0, 1, 0.5);
  cairo_pattern_add_color_stop_rgba(linpat, 1.00,  1, 1, 1, 0);

  cairo_rectangle(cr, 0.0, 0.0, 1, 1);
  cairo_set_source(cr, linpat);
  cairo_fill(cr);

  cairo_pattern_destroy(linpat);
  cairo_pattern_destroy(radpat);

/*
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, 1, 1);
  cairo_move_to(cr, 1, 0);
  cairo_line_to(cr, 0, 1);
  cairo_set_line_width(cr, .2);
  cairo_stroke(cr);

  cairo_rectangle(cr, 0, 0, .5, .5);
  cairo_set_source_rgba(cr, 1, 0, 0, .8);
  cairo_fill(cr);

  cairo_rectangle(cr, 0, .5, .5, .5);
  cairo_set_source_rgba(cr, 0, 1, 0, .6);
  cairo_fill(cr);

  cairo_rectangle(cr, .5, 0, .5, .5);
  cairo_set_source_rgba(cr, 0, 0, 1, .4);
  cairo_fill(cr);
*/
}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  auto const win(std::make_unique<Fl_Double_Window>(724, 700, "example"));

  auto const ex(std::make_unique<CairoWidget>(0, 0, win->w(), win->h()));

  ex->draw(example);

  win->end();

  win->resizable(ex.get());

  win->show(argc, argv);

  return Fl::run();
}
