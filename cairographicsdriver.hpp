#ifndef CAIRO_GRAPHICS_DRIVER_HPP
# define CAIRO_GRAPHICS_DRIVER_HPP
# pragma once

#include "FL/Fl.H"
#include "FL/Fl_Device.H"

#include "cairowindow.hpp"

class CairoGraphicsDriver : public Fl_Graphics_Driver
{
  struct S;

public:
  using Fl_Graphics_Driver::Fl_Graphics_Driver;

  //
  cairo_t* ctx() const noexcept;

  //
  void point(int x, int y);
  void rect(int x, int y, int w, int h);
//  void focus_rect(int x, int y, int w, int h);
  void rectf(int x, int y, int w, int h);
  void line(int x, int y, int x1, int y1);
  void line(int x, int y, int x1, int y1, int x2, int y2);
  void xyline(int x, int y, int x1);
  void xyline(int x, int y, int x1, int y2);
  void xyline(int x, int y, int x1, int y2, int x3) ;
  void yxline(int x, int y, int y1) ;
  void yxline(int x, int y, int y1, int x2) ;
  void yxline(int x, int y, int y1, int x2, int y3) ;
  void loop(int x0, int y0, int x1, int y1, int x2, int y2) ;
  void loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) ;
  void polygon(int x0, int y0, int x1, int y1, int x2, int y2) ;
  void polygon(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) ;
//  // --- clipping
  void push_clip(int x, int y, int w, int h) ;
  int clip_box(int x, int y, int w, int h, int &X, int &Y, int &W, int &H) ;
  int not_clipped(int x, int y, int w, int h) ;
  void push_no_clip() ;
  void pop_clip() ;
//  Fl_Region clip_region();              // has default implementation
//  void clip_region(Fl_Region r);        // has default implementation
//  void restore_clip();
//  // --- implementation is in src/fl_vertex.cxx which includes src/drivers/xxx/Fl_xxx_Graphics_Driver_vertex.cxx
//  void push_matrix();
//  void pop_matrix();
//  void mult_matrix(double a, double b, double c, double d, double x, double y);
//  void rotate(double d);
//  void scale(double x, double y);
//  void scale(double x);
//  void translate(double x,double y);
//  void begin_points();
//  void begin_line();
//  void begin_loop();
//  void begin_polygon();
//  void begin_complex_polygon() ;
//  double transform_x(double x, double y);
//  double transform_y(double x, double y);
//  double transform_dx(double x, double y);
//  double transform_dy(double x, double y);
//  void transformed_vertex(double xf, double yf) ;
//  void vertex(double x, double y) ;
//  void end_points() ;
//  void end_line() ;
//  void end_loop() ;
//  void end_polygon() ;
//  void end_complex_polygon() ;
//  void gap() ;
  void circle(double x, double y, double r) ;
//  // --- implementation is in src/fl_arc.cxx which includes src/drivers/xxx/Fl_xxx_Graphics_Driver_arc.cxx if needed
//  void arc(double x, double y, double r, double start, double end);
//  // --- implementation is in src/fl_arci.cxx which includes src/drivers/xxx/Fl_xxx_Graphics_Driver_arci.cxx
  void arc(int x, int y, int w, int h, double a1, double a2) ;
  void pie(int x, int y, int w, int h, double a1, double a2) ;
//  // --- implementation is in src/fl_curve.cxx which includes src/drivers/xxx/Fl_xxx_Graphics_Driver_curve.cxx if needed
//  void curve(double X0, double Y0, double X1, double Y1, double X2, double Y2, double X3, double Y3);
//  // --- implementation is in src/fl_line_style.cxx which includes src/cfg_gfx/xxx_line_style.cxx
  void line_style(int style, int width=0, char* dashes=0) ;
//  // --- implementation is in src/fl_color.cxx which includes src/cfg_gfx/xxx_color.cxx
//  void color(Fl_Color c) { color_ = c; }
//  Fl_Color color() { return color_; }
  void color(uchar r, uchar g, uchar b) final;
  void color(Fl_Color c) final;

//  // --- implementation is in src/fl_font.cxx which includes src/drivers/xxx/Fl_xxx_Graphics_Driver_font.cxx
  void draw(const char *str, int n, int x, int y) ;
//  void draw(const char *str, int n, float x, float y) { draw(str, n, (int)(x+0.5), (int)(y+0.5));}
//  void draw(int angle, const char *str, int n, int x, int y) { draw(str, n, x, y); }
//  void rtl_draw(const char *str, int n, int x, int y) { draw(str, n, x, y); }
//  /** Returns non-zero if the graphics driver possesses the \p feature */
//  int has_feature(driver_feature feature) { return 0; }
    void font(Fl_Font face, Fl_Fontsize fsize);
//  Fl_Font font() {return font_; }
//  Fl_Fontsize size() {return size_; }
  double width(const char *str, int n);
//  double width(unsigned int c) { char ch = (char)c; return width(&ch, 1); }
  int height();
  int descent();
//  Fl_Font_Descriptor *font_descriptor() { return font_descriptor_;}
//  void font_descriptor(Fl_Font_Descriptor *d) { font_descriptor_ = d;}
//  // --- implementation is in src/fl_image.cxx which includes src/drivers/xxx/Fl_xxx_Graphics_Driver_font.cxx
  Fl_Bitmask create_bitmask(int w, int h, const uchar *array) ;
//  void cache(Fl_Pixmap *img) { return 0; }
//  void cache(Fl_Bitmap *img) { return 0; }
//  void uncache(Fl_RGB_Image *img, fl_uintptr_t &id_, fl_uintptr_t &mask_) { }
  void delete_bitmask(Fl_Bitmask bm) ;
//  void draw_image(const uchar* buf, int X,int Y,int W,int H, int D=3, int L=0) {}
//  void draw_image_mono(const uchar* buf, int X,int Y,int W,int H, int D=1, int L=0) {}
//  void draw_image(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=3) {}
//  void draw_image_mono(Fl_Draw_Image_Cb cb, void* data, int X,int Y,int W,int H, int D=1) {}
//  /** \brief Draws an Fl_RGB_Image object to the device.
//   *
//   Specifies a bounding box for the image, with the origin (upper left-hand corner) of
//   the image offset by the cx and cy arguments.
//   */
//  void draw(Fl_RGB_Image * rgb,int XP, int YP, int WP, int HP, int cx, int cy) {}
//  /** \brief Draws an Fl_Pixmap object to the device.
//   *
//   Specifies a bounding box for the image, with the origin (upper left-hand corner) of
//   the image offset by the cx and cy arguments.
//   */
//  void draw(Fl_Pixmap * pxm,int XP, int YP, int WP, int HP, int cx, int cy) {}
//  /** \brief Draws an Fl_Bitmap object to the device.
//   *
//   Specifies a bounding box for the image, with the origin (upper left-hand corner) of
//   the image offset by the cx and cy arguments.
//   */
//  void draw(Fl_Bitmap *bm, int XP, int YP, int WP, int HP, int cx, int cy) {}
//  void copy_offscreen(int x, int y, int w, int h, Fl_Offscreen pixmap, int srcx, int srcy);
//
//  /** Sets the value of the driver-specific graphics context. */
//  void gc(void*) {}
//  /** Returns the driver-specific graphics context, of NULL if there's none. */
//  void *gc(void) {return NULL;}
//  /** Support for pixmap drawing */
//  uchar **mask_bitmap() { return 0; }
//  /** Support for pixmap drawing */
//  void mask_bitmap(uchar **) {}
};

//////////////////////////////////////////////////////////////////////////////
inline cairo_t* CairoGraphicsDriver::ctx() const noexcept
{
  return static_cast<CairoWindow*>(Fl_Window::current())->ctx();
}

#endif // CAIRO_GRAPHICS_DRIVER_HPP
