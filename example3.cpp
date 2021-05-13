#include <QApplication>

#include "caironanosvg.hpp"

#include "CairoWidget.hpp"

struct NSVGimage* image{};

//////////////////////////////////////////////////////////////////////////////
void example(cairo_t* const cr, int const w, int const h) noexcept
{
  cairo_set_source_rgb(cr, 220 / 255., 220 / 255., 220 / 255.);

  cairo_paint(cr);

  if (image)
  {
    draw_svg_image(cr, image, 0, 0, w, h);
  }
}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  image = nsvgParseFromFile("nanosvg/example/23.svg", "px", 96);

  QApplication app(argc, argv);

  CairoWidget w;

  w.init([](auto const cr, auto, auto) noexcept
    {
      cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
    }
  );

  w.draw(example);

  w.show();

  return app.exec();
}
