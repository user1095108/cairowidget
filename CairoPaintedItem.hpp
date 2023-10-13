#include <QQuickPaintedItem>

struct _cairo;

class CairoPaintedItem: public QQuickPaintedItem
{
  struct S;

  _cairo* cr_{};

  uchar* d_{};
  int w_, h_;
  int stride_;

public:
  explicit CairoPaintedItem(QQuickItem* = {});
  ~CairoPaintedItem() override;

protected:
  virtual void init(_cairo*, int, int);
  virtual void draw(_cairo*, int, int) = 0;

  void reinit() { if (cr_) init(cr_, w_, h_); }

private:
  void paint(QPainter*) final;
};
