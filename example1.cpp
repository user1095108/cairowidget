#include "FL/Fl.H"

#include "Fl/Fl_Box.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Image_Surface.H"

#include "cairowidget.hpp"

#include "cairo/cairo-pdf.h"
#include "cairo/cairo-svg.h"

#include <memory>

enum Capture
{
  PDF,
  PNG,
  SVG
};

//////////////////////////////////////////////////////////////////////////////
template <enum Capture C>
void capture(CairoWidget const& wi, char const* const filename)
{
  auto const w(wi.w()), h(wi.h());

  cairo_surface_t* surf;

  if constexpr (PDF == C)
  {
    surf = cairo_pdf_surface_create(filename, w, h);
  }
  else if constexpr (PNG == C)
  {
    surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h);
  }
  else
  {
    surf = cairo_svg_surface_create(filename, w, h);
  }

  auto const cr(cairo_create(surf));
  cairo_surface_destroy(surf);

  {
    uchar r, g, b;
    Fl::get_color(wi.color(), r, g, b);

    cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
  }

  {
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_paint(cr);

    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    wi.draw()(cr, w, h);
  }

  if constexpr (PNG == C)
  {
    cairo_surface_write_to_png(surf, filename);
  }

  cairo_destroy(cr);
}

//////////////////////////////////////////////////////////////////////////////
void capture(Fl_Widget* const wi, char const* const filename)
{
  auto const w(wi->w()), h(wi->h());

  // capture
  Fl_Image_Surface fis(w, h);
  fis.set_current();
  fis.draw(wi);

  Fl_Display_Device::display_device()->set_current();

  // convert
  auto const surf(cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h));

  auto src(fis.image()->data()[0]);
  auto dst(cairo_image_surface_get_data(surf));

  for (auto pixels(w * h); pixels--; src += 3, dst += 4)
  {
    dst[0] = src[2];
    dst[1] = src[1];
    dst[2] = src[0];
  }

  // save and cleanup
  cairo_surface_write_to_png(surf, filename);
  cairo_surface_destroy(surf);
}

//////////////////////////////////////////////////////////////////////////////
void example(cairo_t* const cr, int const w, int const h) noexcept
{
  cairo_paint(cr);

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

  auto const ex(new CairoWidget(0, 0, win->w(), win->h()));
  ex->draw(example);

  new Fl_Box(FL_EMBOSSED_BOX, 0, 0, win->w(), 40, "Text from label");

  win->end();

  win->show();

  capture(win.get(), "capture.png");

  capture<PDF>(*ex, "example.pdf");
  capture<PNG>(*ex, "example.png");
  capture<SVG>(*ex, "example.svg");

  return Fl::run();
}
