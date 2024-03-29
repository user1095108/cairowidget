// https://www.youtube.com/watch?v=PMZ9BFgane4&t=1175s&ab_channel=KevinO%27Kane
#include <QtMath>

#include <QApplication>
#include <QTime>
#include <QTimer>

#include "cairo/cairo.h"

#include "CairoWidget.hpp"

class ClockWidget final: public CairoWidget
{
  double k_;

public:
  explicit ClockWidget()
  {
    init([&](auto const cr, int const w, int const h) noexcept
      {
        cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

        if (w >= h)
        {
          k_ = 1. / h;

          cairo_translate(cr, .5 * (w - h), 0.);
          cairo_scale(cr, h, h);
        }
        else
        {
          k_ = 1. / w;

          cairo_translate(cr, 0., .5 * (h - w));
          cairo_scale(cr, w, w);
        }

        cairo_translate(cr, .5, .5);
      }
    );

    draw([&](auto const cr, auto, auto)
      {
        cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
        cairo_paint(cr);

        cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

        //
        int h, m, s;

        {
          auto const ct(QTime::currentTime());

          h = ct.hour() % 12;
          m = ct.minute();
          s = ct.second();
        }

        // hash marks
        cairo_set_source_rgb(cr, .5, .5, 1.);
        cairo_set_line_width(cr, k_ * 2.5);

        for (int i{}; i != 60; i += 5)
        {
          double const X((i - 3) * M_PI / 6);

          double x0, y0;
          cairo_arc(cr, 0., 0., .5, -M_PI, X);
          cairo_get_current_point(cr, &x0, &y0);

          double x1, y1;
          cairo_arc(cr, 0., 0., .5 - k_ * 10. , -M_PI, X);
          cairo_get_current_point(cr, &x1, &y1);

          cairo_new_path(cr);
          cairo_move_to(cr, x0, y0);
          cairo_line_to (cr, x1, y1);

          cairo_stroke(cr);
        }

        // needles
        auto const draw_needle(
          [&](double const len, double const pos) noexcept
          {
            cairo_arc(cr, 0., 0., len, -M_PI, pos);

            double x, y;
            cairo_get_current_point(cr, &x, &y);

            cairo_new_path(cr);
            cairo_move_to(cr, 0., 0.);
            cairo_line_to(cr, x, y);
            cairo_stroke(cr);
          }
        );

        cairo_set_source_rgb(cr, .5, 1., .5);
        cairo_set_line_width(cr, k_ * 3.);
        draw_needle(.3, (h - 3 + m / 60) * (M_PI / 6));

        cairo_set_source_rgb(cr, 1., 0., 0.);
        cairo_set_line_width(cr, k_ * 2.);
        draw_needle(.45, (m - 15) * (M_PI / 30));

        // second needle
        cairo_set_source_rgb(cr, 1., 1., .3);
        cairo_set_line_width(cr, k_ * 2.5);

        {
          double const pos((s - 15) * (M_PI / 30));

          for (int i{}; i != 60; ++i)
          {
            if (double const X((i - 15) * (M_PI / 30)); X >= pos)
            {
              break;
            }
            else if (i % 5)
            {
              double x0, y0;
              cairo_arc(cr, 0., 0., .5, -M_PI, X);
              cairo_get_current_point(cr, &x0, &y0);

              double x1, y1;
              cairo_arc(cr, 0., 0., .5 - k_ * 3., -M_PI, X);
              cairo_get_current_point(cr, &x1, &y1);

              cairo_new_path(cr);
              cairo_move_to(cr, x0, y0);
              cairo_line_to(cr, x1, y1);

              cairo_stroke(cr);
            }
          }

          cairo_set_line_width(cr, k_ * 1.);
          draw_needle(.5, pos);
        }

        // spindle
        cairo_arc(cr, 0., 0., k_ * 4., -M_PI, M_PI);
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
