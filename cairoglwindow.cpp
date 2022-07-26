#include "FL/Fl.H"
#include "FL/fl_draw.H"

#include "cairo-gl.h"

#include <cassert>

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
Cairo_Gl_Window::~Cairo_Gl_Window() { cairo_destroy(cr_); }

//////////////////////////////////////////////////////////////////////////////
void Cairo_Gl_Window::draw()
{
  auto const w{this->w()}, h{this->h()};

  auto cr(cr_);
  auto surf(surf_);

  if (!context_valid())
  {
    Fl_Window::make_current();

    //
    auto const device(cairo_glx_device_create(fl_display,
      static_cast<GLXContext>(context())));
    cairo_gl_device_set_thread_aware(device, false);
    assert(cairo_device_status(device) == CAIRO_STATUS_SUCCESS);

    //
    cairo_destroy(cr);
    cr_ = cr = cairo_create(surf_ = surf =
      cairo_gl_surface_create_for_window(device, fl_window, w, h));
    cairo_device_destroy(device);
    cairo_surface_destroy(surf);
    assert(CAIRO_STATUS_SUCCESS == cairo_surface_status(surf_));
    assert(CAIRO_STATUS_SUCCESS == cairo_status(cr));

    //
    i_(cr, w, h);
  }
  else if (!valid())
  {
    cairo_gl_surface_set_size(surf, w, h);
  }

  //
  cairo_save(cr);

  d_(cr, w, h);

  cairo_restore(cr);

  //
  cairo_surface_flush(surf);
}
