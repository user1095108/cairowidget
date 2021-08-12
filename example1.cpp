#include <bit>

#include <execution>

#include "FL/Fl.H"

#include "FL/Fl_Box.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Image_Surface.H"

#include "cairo/cairo.h"

#include "cairo/cairo-pdf.h"
#include "cairo/cairo-ps.h"
#include "cairo/cairo-svg.h"

#include "nanosvg/src/nanosvg.h"

#include "pixeliterator.hpp"

#include "caironanosvg.hpp"

#include "cairowidget.hpp"

enum Capture
{
  EPS,
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

  if constexpr (EPS == C)
  {
    surf = cairo_ps_surface_create(filename, w, h);
    cairo_ps_surface_set_eps(surf, true);
  }
  else if constexpr (PDF == C)
  {
    surf = cairo_pdf_surface_create(filename, w, h);
  }
  else if constexpr (PNG == C)
  {
    surf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h);
  }
  else if constexpr (SVG == C)
  {
    surf = cairo_svg_surface_create(filename, w, h);
  }

  auto const cr(cairo_create(surf));
  cairo_surface_destroy(surf);

  wi.draw()(cr, w, h);

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

  //
  auto const surf(cairo_image_surface_create(CAIRO_FORMAT_RGB24, w, h));

  //cairo_surface_flush(surf);

  // convert
  auto src(fis.image()->data()[0]);
  auto dst(cairo_image_surface_get_data(surf));

  // RGB (fltk) -> xRGB (cairo)
  std::transform(std::execution::unseq,
    make_pixel_iterator<3>(src),
    make_pixel_iterator<3>(src + 3 * w * h),
    make_pixel_iterator<4>(dst),
    [](auto&& s) noexcept
    {
      decltype(make_pixel_iterator<4>(dst))::value_type d;

      if constexpr (std::endian::little == std::endian::native)
      {
        d[0] = s[2]; d[1] = s[1]; d[2] = s[0];
      }
      else if constexpr (std::endian::big == std::endian::native)
      {
        d[1] = s[0]; d[2] = s[1]; d[3] = s[2];
      }

      return d;
    }
  );

  //cairo_surface_mark_dirty(surf);

  // save and cleanup
  cairo_surface_write_to_png(surf, filename);

  cairo_surface_destroy(surf);
}

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
int main()
{
  image = nsvgParseFromFile("nanosvg/example/23.svg", "px", 96);

  auto const win(new Fl_Double_Window(724, 700, "example1"));

  new Fl_Box(FL_EMBOSSED_BOX, 0, 0, win->w(), 40, "Text from label");

  auto const ex(new CairoWidget(0, 40, win->w(), win->h() - 40));

  win->resizable(ex);

  win->end();

  ex->init([](auto const cr, auto, auto) noexcept
    {
      cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
    }
  );

  ex->draw(example);

  win->show();

  capture(win, "capture1.png");

  capture<EPS>(*ex, "capture2.eps");
  capture<PDF>(*ex, "capture2.pdf");
  capture<PNG>(*ex, "capture2.png");
  capture<SVG>(*ex, "capture2.svg");

  return Fl::run();
}
