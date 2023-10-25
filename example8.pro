TEMPLATE = app
TARGET = example8

CONFIG += no_lflags_merge exceptions_off rtti_off stl thread warn_on c++latest strict_c++ no_keywords

QT += qml quick quickwidgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
DEFINES +=                 \
  QT_USE_FAST_CONCATENATION\
  QT_USE_FAST_OPERATOR_PLUS\
  QT_NO_CAST_FROM_ASCII    \
  QT_NO_CAST_TO_ASCII      \
  QT_NO_CAST_FROM_BYTEARRAY

RESOURCES = svg.qrc

# Input
HEADERS += FastCairoPaintedItem.hpp \
           example7.cpp
SOURCES += FastCairoPaintedItem.cpp \
           example7.cpp
LIBS += -lcairo
