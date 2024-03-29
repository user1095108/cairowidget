#include <QQuickPaintedItem>

struct _cairo;

class CairoPaintedItem: public QQuickPaintedItem
{
  struct S;

  _cairo* cr_{};

  int w_{}, h_;
  unsigned char* d_;
  int stride_;

public:
  using QQuickPaintedItem::QQuickPaintedItem;
  ~CairoPaintedItem() override;

  void reinit() { if (cr_) init(cr_, w_, h_); }

  virtual void init(_cairo*, int, int); // for capture
  virtual void draw(_cairo*, int, int) = 0; // for capture

private:
  void paint(QPainter*) final;
};
