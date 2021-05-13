TEMPLATE = app
TARGET = example3
INCLUDEPATH += . nanosvg/src

CONFIG += no_lflags_merge exceptions_off rtti_off stl thread warn_on c++latest strict_c++
CONFIG -= c++11 c++14 exceptions rtti

QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Input
HEADERS += caironanosvg.hpp \
           CairoWidget.hpp \
           example3.cpp
SOURCES += caironanosvg.cpp \
           CairoWidget.cpp \
           example3.cpp
LIBS += -lcairo
