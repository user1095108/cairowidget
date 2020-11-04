#include "cairosurfacedevice.hpp"

//////////////////////////////////////////////////////////////////////////////
CairoSurfaceDevice::CairoSurfaceDevice():
  Fl_Surface_Device(nullptr),
  driver_(new CairoGraphicsDriver)
{
  driver(driver_.get());
}
