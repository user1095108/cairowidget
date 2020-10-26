#include "cairo/cairo-features.h"

#if defined(CAIRO_HAS_XLIB_SURFACE)
#  include "cairo/cairo-xlib.h"
#elif defined(CAIRO_HAS_WIN32_SURFACE)
#  include "cairo/cairo-win32.h"
#elif defined(CAIRO_HAS_QUARTZ_SURFACE)
#  include "cairo/cairo-quartz.h"
#endif

#include "Fl/Fl.h"
#include "Fl/fl_draw.h"

#include "Fl/Fl_Window.h"

#include "cairowidget.hpp"

struct win_info
{
  cairo_t* cr;

  int w;
  int h;

  Fl_Callback* c;
  void* ud;
};

//////////////////////////////////////////////////////////////////////////////
CairoWidget::CairoWidget(int const x, int const y, int const w, int const h,
  const char* const l) :
  Fl_Widget(x, y, w, h, l)
{
  // latch onto top window
  if (auto const win(top_window()); win && !win->user_data())
  {
    win->callback([](auto const w, void* const d)
      {
        auto const wi(static_cast<win_info*>(d));

        cairo_destroy(wi->cr);

        auto const p(std::make_pair(wi->c, wi->ud));

        w->user_data(p.second);

        delete wi;

        p.first(w, p.second);
      },
      new win_info{{}, {}, {}, win->callback(), win->user_data()}
    );
  }
}

//////////////////////////////////////////////////////////////////////////////
CairoWidget::~CairoWidget()
{
}

//////////////////////////////////////////////////////////////////////////////
void CairoWidget::draw()
{
  cairo_t* cr;

  {
    auto const win(top_window());
    auto const wi(static_cast<win_info*>(win->user_data()));

    if (auto const ww(win->w()), wh(win->h()); (ww == wi->w) && (wh == wi->h))
    {
      cr = wi->cr;
    }
    else
    {
      // cr invalidated or not existing
      cairo_destroy(wi->cr);

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
        wi->cr = cr = cairo_create(surf);
        cairo_surface_destroy(surf);

        wi->w = ww, wi->h = wh;
      }
      else
      {
        wi->cr = cr = {};
      }
    }
  }

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

      cairo_rectangle(cr, 0., 0., ww, wh);

      cairo_fill_preserve(cr);
      cairo_clip(cr);

      //
      cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

      d_(cr, ww, wh);
    }

    cairo_restore(cr);
  }
}
