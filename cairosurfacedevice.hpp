#ifndef CAIRO_SURFACE_DEVICE_HPP
# define CAIRO_SURFACE_DEVICE_HPP
# pragma once

#include "FL/Fl_Device.H"

#include <memory>

class CairoGraphicsDriver;

class CairoSurfaceDevice final: public Fl_Surface_Device
{
  std::unique_ptr<Fl_Graphics_Driver> driver_;

public:
  CairoSurfaceDevice();
  ~CairoSurfaceDevice() final;
};

#endif // CAIRO_SURFACE_DEVICE_HPP
