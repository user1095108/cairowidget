#include "cairo/cairo-features.h"

#if defined(CAIRO_HAS_XLIB_SURFACE)
#  include <FL/x.H>
#  include "cairo/cairo-xlib.h"
#elif defined(CAIRO_HAS_WIN32_SURFACE)
#  include "cairo/cairo-win32.h"
#elif defined(CAIRO_HAS_QUARTZ_SURFACE)
#  include "cairo/cairo-quartz.h"
#endif

#include "Fl/Fl.h"
#include "Fl/fl_draw.h"

#include "Fl/Fl_Window.h"

#include <cassert>

#include "cairowidget.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget()
{
  if (auto const win(window()); win)
  {
    delete static_cast<win_info*>(win->user_data());
    win->user_data({});
  }
}

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::draw()
{
  cairo_t* cr;

  {
    auto const win(window());
    assert(win);

    auto wi(static_cast<win_info*>(win->user_data()));

    if (auto const ww(win->w()), wh(win->h());
      wi && (ww == wi->w) && (wh == wi->h))
    {
      cr = wi->cr;
    }
    else
    {
      if (wi)
      {
        cairo_surface_destroy(wi->surf);
        cairo_destroy(wi->cr);
      }
      else
      {
        win->user_data(wi = new win_info);
      }

#if defined(CAIRO_HAS_XLIB_SURFACE)
      if (auto const surf = cairo_xlib_surface_create(fl_display,
        fl_window, fl_visual->visual, ww, wh))
#elif defined(CAIRO_HAS_WIN32_SURFACE)
      if (auto const surf = cairo_win32_surface_create(
        static_cast<HDC>(fl_gc)))
#elif defined(CAIRO_HAS_QUARTZ_SURFACE)
      if (auto const surf = cairo_quartz_surface_create_for_cg_context(
        static_cast<CGContext*>(fl_gc), ww, wh))
#endif
      {
        wi->cr = cr = cairo_create(wi->surf = surf);

        wi->w = ww;
        wi->h = wh;
      }
      else
      {
        assert(0);
      }
    }
  }

  assert(cr);

  if (cr)
  {
    cairo_save(cr);

    cairo_translate(cr, x(), y());

    {
      uchar r, g, b;
      Fl::get_color(color(), r, g, b);

      cairo_set_source_rgb(cr, r / 255.0, g / 255.0, b / 255.0);
    }

    {
      //
      cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

      auto const ww(w());
      auto const wh(h());

      cairo_rectangle(cr, 0, 0, ww, wh);
      cairo_clip(cr);
      cairo_fill(cr);

      //
      cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

      //
      draw(cr, ww, wh);
    }

    cairo_restore(cr);
  }
}
