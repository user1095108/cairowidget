// https://www.djmaster.com/freepascal/demos/clock.php
#include <QtMath>

#include <QApplication>
#include <QTime>
#include <QTimer>

#include "cairo/cairo.h"

#include "CairoWidget.hpp"

class ClockWidget final: public CairoWidget
{
public:
  explicit ClockWidget()
  {
    init([](auto const cr, auto, auto) noexcept
      {
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
      }
    );

    draw([&](auto const cr, int const ww, int const wh)
      {
        cairo_set_source_rgb(cr, 1., 1., 1.);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr);

        cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

        cairo_scale(cr, ww, wh);

        // Set line properties
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
        cairo_set_line_width(cr, .1);

        // Translate to the center of the rendering context
        cairo_translate(cr, .5, .5);

        // Draw the context white
        //cairo_set_source_rgba(cr, 1., 1., 1., 1.);
        cairo_arc(cr, 0, 0, .4, 0, 2 * M_PI);
        cairo_fill(cr);

        // Draw a black clock outline
        cairo_set_source_rgba(cr, 0, 0, 0, 1);
        cairo_arc(cr, 0, 0, .4, 0, 2 * M_PI);
        cairo_stroke(cr);

        qreal h, m, s;

        {
          auto const ct(QTime::currentTime());

          h = (ct.hour() + ct.minute() / 60) * M_PI / 6;
          m = ct.minute() * M_PI / 30;
          s = ct.second() * M_PI / 30;
        }

        // Draw a white dot on the current second
        cairo_set_source_rgba(cr, 1, 1, 1, .6);
        cairo_arc(cr, qSin(s) * .4, -qCos(s) * .4, .05, 0, 2 * M_PI);
        cairo_fill(cr);

        // Draw the minutes indicator
        cairo_set_source_rgba(cr, .2, .2, 1., .6);
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, qSin(m) * .4, -qCos(m) * .4);
        cairo_stroke(cr);

        // Draw the hours indicator
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, qSin(h) * .2, -qCos(h) * .2);
        cairo_stroke(cr);
      }
    );

    //
    auto t(new QTimer(this));
    connect(t, &QTimer::timeout, this, QOverload<>::of(&ClockWidget::update));
    t->start(1000);
  }
};

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  ClockWidget w;
  w.resize(w.height(), w.height());
  w.show();

  return app.exec();
}
