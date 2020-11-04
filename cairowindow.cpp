#include "cairo/cairo-features.h"

#if defined(CAIRO_HAS_XLIB_SURFACE)
#  include "cairo/cairo-xlib.h"
#elif defined(CAIRO_HAS_WIN32_SURFACE)
#  include "cairo/cairo-win32.h"
#elif defined(CAIRO_HAS_QUARTZ_SURFACE)
#  include "cairo/cairo-quartz.h"
#endif

#include "Fl/Fl.H"
#include "Fl/fl_draw.H"

#include <cassert>

#include "cairowindow.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoWindow::CairoWindow(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Window(x, y, w, h, l)
{
}

//////////////////////////////////////////////////////////////////////////////
CairoWindow::CairoWindow(int const w, int const h, const char* const l) :
  Fl_Window(w, h, l)
{
}

//////////////////////////////////////////////////////////////////////////////
CairoWindow::~CairoWindow()
{
  cairo_destroy(cr_);
}

//////////////////////////////////////////////////////////////////////////////
void CairoWindow::draw()
{
  auto const w{this->w()}, h{this->h()};

  auto cr(cr_);

  if (!cr || (w != w_) || (h != h_))
  {
    Fl_Window::make_current();

    // generate a cairo context
#if defined(CAIRO_HAS_XLIB_SURFACE)
    auto const surf(cairo_xlib_surface_create(fl_display,
      fl_window, fl_visual->visual, w, h));
#elif defined(CAIRO_HAS_WIN32_SURFACE)
    auto const surf(cairo_win32_surface_create(static_cast<HDC>(fl_gc)));
#elif defined(CAIRO_HAS_QUARTZ_SURFACE)
    auto const surf(cairo_quartz_surface_create_for_cg_context(
      static_cast<CGContext*>(fl_gc), w, h));
#endif
    assert(cairo_surface_status(surf) == CAIRO_STATUS_SUCCESS);

    cairo_destroy(cr);

    w_ = w; h_ = h;

    cr_ = cr = cairo_create(surf);
    cairo_surface_destroy(surf);
    assert(cairo_status(cr) == CAIRO_STATUS_SUCCESS);

    // some defaults
    cairo_set_line_width(cr, 1.);
    cairo_translate(cr, .5, .5);
  }

  {
    cairo_save(cr);

    {
      uchar r, g, b;
      Fl::get_color(color(), r, g, b);

      cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
    }

    {
      cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

      cairo_paint(cr);

      cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

      if (d_)
      {
        d_(cr, w, h);
      }
    }

    cairo_restore(cr);

    //
    Fl_Group::draw_children();

    //cairo_surface_flush(cairo_get_target(cr));
  }
}
