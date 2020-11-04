#include "Fl/Fl.H"
#include "Fl/fl_draw.H"

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
  Fl_Group::current(this);
}

//////////////////////////////////////////////////////////////////////////////
CairoWindow::CairoWindow(int const w, int const h, const char* const l) :
  Fl_Gl_Window(w, h, l)
{
  mode(FL_SINGLE | FL_RGB);
  Fl_Group::current(this);
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

    // some defaults
    cairo_set_line_width(cr, 1.);
    cairo_translate(cr, .5, .5);

    int attrs;
    Fl::get_font_name(fl_font(), &attrs);

    auto const cr(ctx());

    cairo_select_font_face(cr,
      Fl::get_font(fl_font()),
      attrs & FL_ITALIC ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL,
      attrs & FL_BOLD ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, fl_size());
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

      if (d_)
      {
        d_(cr, w, h);
      }
    }

    cairo_restore(cr);

    //
    if (children())
    {
      surface_device_->set_current();

      Fl_Group::draw_children();

      Fl_Display_Device::display_device()->set_current();
    }

    //
    cairo_surface_flush(cairo_get_target(cr));
  }
}
