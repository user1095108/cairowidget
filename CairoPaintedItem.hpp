#include <QQuickPaintedItem>

struct _cairo;

class CairoPaintedItem: public QQuickPaintedItem
{
  _cairo* cr_{};

  uchar* d_{};
  int w_, h_;

public:
  explicit CairoPaintedItem(QQuickItem* = {});
  ~CairoPaintedItem() noexcept override;

protected:
  virtual void init(_cairo*, int, int);
  virtual void draw(_cairo*, int, int) = 0;

private:
  void paint(QPainter*) final;
};
