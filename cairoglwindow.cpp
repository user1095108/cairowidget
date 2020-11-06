#include "Fl/Fl.H"
#include "Fl/fl_draw.H"

#include "cairo-gl.h"

#include <cassert>

#include <iostream>

#include "cairoglwindow.hpp"

//////////////////////////////////////////////////////////////////////////////
Cairo_Gl_Window::Cairo_Gl_Window(int const x, int const y,
  int const w, int const h, const char* const l) :
  Fl_Gl_Window(x, y, w, h, l)
{
  mode(FL_SINGLE | FL_RGB);
  Fl_Group::current(this);
}

//////////////////////////////////////////////////////////////////////////////
Cairo_Gl_Window::Cairo_Gl_Window(int const w, int const h,
  const char* const l) :
  Fl_Gl_Window(w, h, l)
{
  mode(FL_SINGLE | FL_RGB);
  Fl_Group::current(this);
}

//////////////////////////////////////////////////////////////////////////////
Cairo_Gl_Window::~Cairo_Gl_Window()
{
  cairo_destroy(cr_);
}

//////////////////////////////////////////////////////////////////////////////
void Cairo_Gl_Window::draw()
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

    // some defaults
    cairo_set_line_width(cr, 1.);
    cairo_translate(cr, .5, .5);
  }
  else if (!valid())
  {
    cairo_gl_surface_set_size(cairo_get_target(cr), w, h);
  }

  {
    if (d_)
    {
      cairo_save(cr);

      d_(cr, w, h);

      cairo_restore(cr);
    }

    //
    if (children())
    {
      surface_device_->set_current();

      //cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

      Fl_Group::draw_children();

      Fl_Display_Device::display_device()->set_current();
    }

    //
    cairo_surface_flush(cairo_get_target(cr));
  }
}
