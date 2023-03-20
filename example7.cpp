#include <QtQml>
#include <QtQuick>

#include "cairo/cairo.h"

#include "CairoPaintedItem.hpp"

class SampleItem: public CairoPaintedItem
{
  qreal const m_radius{.42}, m_line_width{.05};

public:
  using CairoPaintedItem::CairoPaintedItem;

  void init(cairo_t* const cr, int const w, int const h)
  {
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

    if (w >= h)
    {
      cairo_translate(cr, .5 * (w - h), 0.);
      cairo_scale(cr, h, h);
    }
    else
    {
      cairo_translate(cr, 0., .5 * (h - w));
      cairo_scale(cr, w, w);
    }

    cairo_translate(cr, .5, .5);
  }

  void paint(cairo_t* const cr, int, int)
  {
    cairo_set_source_rgba(cr, .337, .612, .117, .9);   // green
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);

    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    cairo_set_source_rgb(cr, .0, .0, .0);

    // scale to unit square and translate (0, 0) to be (.5, .5), i.e.
    // the center of the window
    cairo_set_line_width(cr, m_line_width);

    cairo_arc(cr, 0, 0, m_radius, 0, 2 * M_PI);
    cairo_save(cr);
    cairo_set_source_rgba(cr, 1., 1., 1., .8);
    cairo_fill_preserve(cr);
    cairo_restore(cr);
    cairo_stroke_preserve(cr);
    cairo_clip(cr);

    //clock ticks
    for (int i = 0; i != 12; ++i)
    {
      qreal inset = .05;

      cairo_save(cr);
      cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

      if (i % 3)
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
};

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  QGuiApplication app(argc, argv);

  //
  qmlRegisterType<SampleItem>("cairowidget", {}, {}, "SampleItem");

  //
  QQmlApplicationEngine engine;
  engine.load(QStringLiteral("qrc:///Main.qml"));

  //
  return app.exec();
}
