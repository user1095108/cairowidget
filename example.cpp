#include "FL/Fl.h"

#include "FL/Fl_Double_Window.h"

#include "cairowidget.hpp"

#include "cairo/cairo-pdf.h"
#include "cairo/cairo-svg.h"

#include <memory>

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
void pdf_capture(CairoWidget const& wi, char const* const filename)
{
  auto const w(wi.w()), h(wi.h());
  auto const surf(cairo_pdf_surface_create(filename, w, h));
  auto const cr(cairo_create(surf));

  {
    uchar r, g, b;
    Fl::get_color(wi.color(), r, g, b);

    cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
  }

  {
    cairo_rectangle(cr, 0., 0., w, h);
    cairo_fill(cr);
  }

  wi.draw()(cr, w, h);

  {
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
  }
}

//////////////////////////////////////////////////////////////////////////////
void png_capture(CairoWidget const& wi, char const* const filename)
{
  auto const w(wi.w()), h(wi.h());
  auto const surf(cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h));
  auto const cr(cairo_create(surf));

  {
    uchar r, g, b;
    Fl::get_color(wi.color(), r, g, b);

    cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
  }

  {
    cairo_rectangle(cr, 0., 0., w, h);
    cairo_fill(cr);
  }

  wi.draw()(cr, w, h);
  cairo_surface_write_to_png(surf, filename);

  {
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
  }
}

//////////////////////////////////////////////////////////////////////////////
void svg_capture(CairoWidget const& wi, char const* const filename)
{
  auto const w(wi.w()), h(wi.h());
  auto const surf(cairo_svg_surface_create(filename, w, h));
  auto const cr(cairo_create(surf));

  {
    uchar r, g, b;
    Fl::get_color(wi.color(), r, g, b);

    cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
  }

  {
    cairo_rectangle(cr, 0., 0., w, h);
    cairo_fill(cr);
  }

  wi.draw()(cr, w, h);

  {
    cairo_destroy(cr);
    cairo_surface_destroy(surf);
  }
}

//////////////////////////////////////////////////////////////////////////////
int main()
{
  auto const win(std::make_unique<Fl_Double_Window>(724, 700, "example"));
  win->resizable(win.get());

  auto const ex(new CairoWidget(0, 0, win->w(), win->h()));

  win->end();

  ex->draw(example);

  pdf_capture(*ex, "example.pdf");
  png_capture(*ex, "example.png");
  svg_capture(*ex, "example.svg");

  win->show();

  return Fl::run();
}
