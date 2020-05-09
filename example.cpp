#include "FL/Fl.h"

#include "FL/Fl_Double_Window.h"

#include "cairowidget.hpp"

#include <memory>

class ExampleWidget : public CairoWidget
{
public:
  using CairoWidget::CairoWidget;

  void draw(cairo_t* const cr, int const w, int const h)
  {
    cairo_scale(cr, w, h);

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
  }
};

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  auto const win(std::make_unique<Fl_Double_Window>(724, 700, "example"));

  auto const ex(std::make_unique<ExampleWidget>(0, 0, 724, 700));

  win->end();

  win->resizable(ex.get());

  win->show(argc, argv);

  return Fl::run();
}
