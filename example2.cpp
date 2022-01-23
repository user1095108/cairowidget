#include "FL/Fl.H"
#include "FL/Fl_Box.H"

#include "cairo/cairo.h"

#include "nanosvg/src/nanosvg.h"

#include "caironanosvg.hpp"

#include "cairoglwindow.hpp"

struct NSVGimage* image{};

//////////////////////////////////////////////////////////////////////////////
void example(cairo_t* const cr, int const w, int const h) noexcept
{
  cairo_set_source_rgb(cr, 220 / 255., 220 / 255., 220 / 255.);

  cairo_paint(cr);

  if (image)
  {
    draw_svg_image(cr, image, 0, 40, w, h - 40);
  }
}

//////////////////////////////////////////////////////////////////////////////
int main()
{
  image = nsvgParseFromFile("nanosvg/example/23.svg", "px", 96);

  auto const win(new Cairo_Gl_Window(724, 700));
  win->resizable(*win);

  new Fl_Box(FL_EMBOSSED_BOX, 0, 0, win->w(), 40, "Text from label");

  win->end();

  win->init([](auto const cr, auto, auto) noexcept
    {
      cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
    }
  );

  win->draw(example);

  win->show();

  return Fl::run();
}
