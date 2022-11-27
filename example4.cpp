// https://developer-old.gnome.org/gtkmm-tutorial/stable/sec-drawing-clock-example.html.en
#include <QtMath>

#include <QApplication>
#include <QTime>
#include <QTimer>

#include "cairo/cairo.h"

#include "CairoWidget.hpp"

class ClockWidget final: public CairoWidget
{
  qreal const m_radius{.42}, m_line_width{.05};

public:
  explicit ClockWidget()
  {
    init([](auto const cr, auto, auto) noexcept
      {
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);
      }
    );

    draw([&](auto const cr, int const w, int const h)
      {
        cairo_save(cr);
        cairo_set_source_rgba(cr, .337, .612, .117, .9);   // green
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr);
        cairo_restore(cr);

        // scale to unit square and translate (0, 0) to be (.5, .5), i.e.
        // the center of the window
        cairo_scale(cr, w, h);
        cairo_translate(cr, .5, .5);
        cairo_set_line_width(cr, m_line_width);

        cairo_arc(cr, 0, 0, m_radius, 0, 2 * M_PI);
        cairo_save(cr);
        cairo_set_source_rgba(cr, 1., 1., 1., .8);
        cairo_fill_preserve(cr);
        cairo_restore(cr);
        cairo_stroke_preserve(cr);
        cairo_clip(cr);

        //clock ticks
        for (int i = 0; i < 12; i++)
        {
          qreal inset = .05;

          cairo_save(cr);
          cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

          if(i % 3 != 0)
          {
            inset *= .8;
            cairo_set_line_width(cr, .03);
          }

          cairo_move_to(
            cr,
            (m_radius - inset) * qCos (i * M_PI / 6),
            (m_radius - inset) * qSin (i * M_PI / 6));
          cairo_line_to(
            cr,
            m_radius * qCos (i * M_PI / 6),
            m_radius * qSin (i * M_PI / 6));
          cairo_stroke(cr);
          cairo_restore(cr); /* stack-pen-size */
        }

        qreal minutes, hours, seconds;

        {
          auto const ct(QTime::currentTime());

          // compute the angles of the indicators of our clock
          minutes = ct.minute() * M_PI / 30;
          hours = ct.hour() * M_PI / 6;
          seconds= ct.second() * M_PI / 30;
        }

        cairo_save(cr);
        cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

        // draw the seconds hand
        cairo_save(cr);
        cairo_set_line_width(cr, m_line_width / 3);
        cairo_set_source_rgba(cr, .7, .7, .7, .8); // gray
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, qSin(seconds) * (m_radius * .9),
          -qCos(seconds) * (m_radius * .9));
        cairo_stroke(cr);
        cairo_restore(cr);

        // draw the minutes hand
        cairo_set_source_rgba(cr, .117, .337, .612, .9);   // blue
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, qSin(minutes + seconds / 60) * (m_radius * .8),
          -qCos(minutes + seconds / 60) * (m_radius * .8));
        cairo_stroke(cr);

        // draw the hours hand
        cairo_set_source_rgba(cr, .337, .612, .117, .9);   // green
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, qSin(hours + minutes / 12.) * (m_radius * .5),
          -qCos(hours + minutes / 12.) * (m_radius * .5));
        cairo_stroke(cr);
        cairo_restore(cr);

        // draw a little dot in the middle
        cairo_arc(cr, 0, 0, m_line_width / 3., 0, 2 * M_PI);
        cairo_fill(cr);
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
