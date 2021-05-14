TEMPLATE = app
TARGET = example3
INCLUDEPATH += . nanosvg/src

CONFIG += no_lflags_merge exceptions_off rtti_off stl thread warn_on c++latest strict_c++
CONFIG -= c++11 c++14 exceptions rtti

QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES +=                 \
  QT_USE_FAST_CONCATENATION\
  QT_USE_FAST_OPERATOR_PLUS\
  QT_NO_CAST_FROM_ASCII    \
  QT_NO_CAST_TO_ASCII      \
  QT_NO_CAST_FROM_BYTEARRAY

# Input
HEADERS += caironanosvg.hpp \
           CairoWidget.hpp \
           example3.cpp
SOURCES += caironanosvg.cpp \
           CairoWidget.cpp \
           example3.cpp
LIBS += -lcairo

*-g++* {
  QMAKE_CFLAGS = -Wall -Wextra -fno-stack-protector -fno-plt
  QMAKE_CXXFLAGS = $$QMAKE_CFLAGS

  unix:QMAKE_CXXFLAGS_DEBUG *= -fsanitize=address,undefined
  unix:QMAKE_LFLAGS_DEBUG *= -fsanitize=address,undefined

  QMAKE_CFLAGS_RELEASE *= -DNDEBUG
  QMAKE_CXXFLAGS_RELEASE *= -Ofast -DQT_NO_DEBUG_OUTPUT -DNDEBUG

  QMAKE_LFLAGS *= -fno-stack-protector -fuse-ld=gold
}
