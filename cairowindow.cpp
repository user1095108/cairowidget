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
  mode(FL_DOUBLE | FL_RGB);
}

//////////////////////////////////////////////////////////////////////////////
CairoWindow::CairoWindow(int const w, int const h, const char* const l) :
  Fl_Gl_Window(w, h, l)
{
  mode(FL_DOUBLE | FL_RGB);
}

//////////////////////////////////////////////////////////////////////////////
CairoWindow::~CairoWindow()
{
  cairo_surface_destroy(surf_);
}

//////////////////////////////////////////////////////////////////////////////
void CairoWindow::draw()
{
  auto const w{this->w()}, h{this->h()};

  if (!context_valid())
  {
    Fl_Window::make_current();

    auto const device(cairo_glx_device_create(fl_display,
      static_cast<GLXContext>(context())));
    cairo_gl_device_set_thread_aware(device, false);
    assert(cairo_device_status(device) == CAIRO_STATUS_SUCCESS);

    surf_ = cairo_gl_surface_create_for_window(device, fl_window, w, h);
    cairo_device_destroy(device);
    assert(cairo_surface_status(surf_) == CAIRO_STATUS_SUCCESS);
  }
  else if (!valid())
  {
    cairo_gl_surface_set_size(surf_, w, h);
    assert(cairo_surface_status(surf_) == CAIRO_STATUS_SUCCESS);
  }

  {
    auto const cr(cairo_create(surf_));
    assert(cairo_status(cr) == CAIRO_STATUS_SUCCESS);

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

    cairo_destroy(cr);

    cairo_surface_flush(surf_);
  }
}
