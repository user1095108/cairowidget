#include "Fl/Fl.h"
#include "Fl/fl_draw.h"

#include "cairo-gl.h"

#include <cassert>

#include <iostream>

#include "cairowindow.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoWindow::CairoWindow(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Gl_Window(x, y, w, h, l)
{
  mode(FL_SINGLE | FL_RGB);
}

//////////////////////////////////////////////////////////////////////////////
CairoWindow::CairoWindow(int const w, int const h, const char* const l) :
  Fl_Gl_Window(w, h, l)
{
  mode(FL_SINGLE | FL_RGB);
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

  if (!context_valid())
  {
    Fl_Window::make_current();

    auto const device(cairo_glx_device_create(fl_display,
      static_cast<GLXContext>(context())));
    cairo_gl_device_set_thread_aware(device, false);
    assert(cairo_device_status(device) == CAIRO_STATUS_SUCCESS);

    auto const surf(cairo_gl_surface_create_for_window(device, fl_window,
      w, h));
    cairo_device_destroy(device);
    assert(cairo_surface_status(surf) == CAIRO_STATUS_SUCCESS);

    cairo_destroy(cr);
    cr_ = cr = cairo_create(surf);
    cairo_surface_destroy(surf);
    assert(cairo_status(cr) == CAIRO_STATUS_SUCCESS);
  }
  else if (!valid())
  {
    cairo_gl_surface_set_size(cairo_get_target(cr), w, h);
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

      d_(cr, w, h);
    }

    cairo_restore(cr);

    cairo_surface_flush(cairo_get_target(cr));
  }
}
