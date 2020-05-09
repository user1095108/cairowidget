#include "cairo/cairo-features.h"

#if defined(CAIRO_HAS_XLIB_SURFACE)
#  include "cairo/cairo-xlib.h"
#elif defined(CAIRO_HAS_WIN32_SURFACE)
#  include "cairo/cairo-win32.h"
#elif defined(CAIRO_HAS_QUARTZ_SURFACE)
#  include "cairo/cairo-quartz.h"
#endif

#include "Fl/Fl.h"

#include "Fl/Fl_Window.h"

#include "Fl/fl_draw.h"

#include <cassert>

#include "cairowidget.hpp"

//////////////////////////////////////////////////////////////////////////////
struct CairoWidget::S
{
  static void free_cairo_resources(win_info* const wi) noexcept
  {
    assert(wi);
    cairo_surface_destroy(wi->surf);
    cairo_destroy(wi->cr);
  }
};

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w,
  int const h, const char* const l) :
  Fl_Widget(x, y, w, h, l)
{
  assert(window());

  if (auto const win(window()); !win->user_data())
  {
    win->user_data(new win_info);
  }
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget()
{
  auto const win(window());
  assert(win);

  if (auto const wi(static_cast<win_info*>(win->user_data())); wi->cr)
  {
    S::free_cairo_resources(wi), delete wi, win->user_data({});
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
    assert(wi);

    if (auto const ww(win->w()), wh(win->h()); (ww == wi->w) && (wh == wi->h))
    {
      cr = wi->cr;
    }
    else
    {
      // obtain a valid pointer to win_info
      if (wi->cr)
      {
        S::free_cairo_resources(wi);
      }

      // generate a cairo context
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
        // fill out wi and set cr
        wi->cr = cr = cairo_create(wi->surf = surf);

        wi->w = ww, wi->h = wh;
      }
      else
      {
        // fill out wi and set cr
        wi->surf = {}, wi->cr = cr = {};
      }
    }
  }

  assert(cr);

  if (cr)
  {
    cairo_save(cr);

    {
      uchar r, g, b;
      Fl::get_color(color(), r, g, b);

      cairo_set_source_rgb(cr, r / 255., g / 255., b / 255.);
    }

    {
      cairo_translate(cr, x(), y());

      //
      cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

      auto const ww(w()), wh(h());

      cairo_rectangle(cr, 0, 0, ww, wh);

      cairo_fill_preserve(cr);
      cairo_clip(cr);

      //
      cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

      // assuming d_ is set
      d_(cr, ww, wh);
    }

    cairo_restore(cr);
  }
}
