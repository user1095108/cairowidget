TEMPLATE = app
TARGET = example4

CONFIG += no_lflags_merge exceptions_off rtti_off stl thread warn_on c++17 strict_c++

QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES +=                 \
  QT_USE_FAST_CONCATENATION\
  QT_USE_FAST_OPERATOR_PLUS\
  QT_NO_CAST_FROM_ASCII    \
  QT_NO_CAST_TO_ASCII      \
  QT_NO_CAST_FROM_BYTEARRAY

# Input
HEADERS += CairoWidget.hpp \
           example4.cpp
SOURCES += CairoWidget.cpp \
           example4.cpp
LIBS += -lcairo
