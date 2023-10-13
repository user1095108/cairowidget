#ifndef CAIROWIDGET_HPP
# define CAIROWIDGET_HPP
# pragma once

#include <QWidget>

#include <functional>

struct _cairo;

class CairoWidget: public QWidget
{
  struct S;

  _cairo* cr_{};

  int w_{}, h_;
  unsigned char* d_;
  int stride_;

  using draw_t = std::function<void(_cairo*, int, int)>;

  draw_t df_;
  draw_t if_;

public:
  explicit CairoWidget(QWidget* = {}, Qt::WindowFlags = {});
  ~CairoWidget() override;

  //
  template <class U>
  void init(U&& u) noexcept(noexcept(if_ = std::forward<U>(u)))
  {
    if_ = std::forward<U>(u);
  }

  auto& draw() const noexcept;

  template <class U>
  void draw(U&& u) noexcept(noexcept(df_ = std::forward<U>(u)))
  {
    df_ = std::forward<U>(u);
  }

  void reinit() { if (cr_) if_(cr_, w_, h_); }

private:
  void paintEvent(QPaintEvent*) final;
};

//////////////////////////////////////////////////////////////////////////////
inline auto& CairoWidget::draw() const noexcept { return df_; }

#endif // CAIROWIDGET_HPP
