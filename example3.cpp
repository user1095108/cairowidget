#include "FL/Fl.h"

#include "Fl/Fl_Box.h"

#include "FL/Fl_Double_Window.h"

#include <memory>

#include "cairowindow.hpp"

//////////////////////////////////////////////////////////////////////////////
void example(cairo_t* const cr, int const w, int const h) noexcept
{
  // we need this
  cairo_scale(cr, w, h);

  // code from https://www.cairographics.org/tutorial/
  {
    auto const radpat(cairo_pattern_create_radial(.25, .25, .1, .5, .5, .5));
    cairo_pattern_add_color_stop_rgb(radpat, 0,  1., .8, .8);
    cairo_pattern_add_color_stop_rgb(radpat, 1,  .9, .0, .0);

    for (int i=1; i!=10; ++i)
      for (int j=1; j!=10; ++j)
        cairo_rectangle(cr, i/10. - .04, j/10. - .04, .08, .08);

    cairo_set_source(cr, radpat);
    cairo_fill(cr);

    cairo_pattern_destroy(radpat);
  }

  {
    auto const linpat(cairo_pattern_create_linear(.25, .35, .75, .65));

    cairo_pattern_add_color_stop_rgba(linpat, .00, 1, 1, 1, 0);
    cairo_pattern_add_color_stop_rgba(linpat, .25, 0, 1, 0, .5);
    cairo_pattern_add_color_stop_rgba(linpat, .50, 1, 1, 1, 0);
    cairo_pattern_add_color_stop_rgba(linpat, .75, 0, 0, 1, .5);
    cairo_pattern_add_color_stop_rgba(linpat,  1., 1, 1, 1, 0);

    cairo_rectangle(cr, .0, .0, 1., 1.);
    cairo_set_source(cr, linpat);
    cairo_fill(cr);

    cairo_pattern_destroy(linpat);
  }
}

//////////////////////////////////////////////////////////////////////////////
int main()
{
  auto const win(std::make_unique<Fl_Double_Window>(724, 700, "example"));
  win->resizable(win.get());

  auto const cw(new CairoWindow(0, 0, win->w(), win->h()));
  cw->resizable(cw);

  cw->draw(example);

  new Fl_Box(FL_EMBOSSED_BOX, 0, 0, win->w(), 40, "Text from label");

  cw->end();

  win->end();

  win->show();

  return Fl::run();
}
