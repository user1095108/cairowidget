#ifndef CAIRO_SURFACE_DEVICE_HPP
# define CAIRO_SURFACE_DEVICE_HPP
# pragma once

#include <memory>

#include "cairographicsdriver.hpp"

class CairoSurfaceDevice : public Fl_Surface_Device
{
  std::unique_ptr<CairoGraphicsDriver> driver_;

public:
  CairoSurfaceDevice();
};

#endif // CAIRO_SURFACE_DEVICE_HPP
