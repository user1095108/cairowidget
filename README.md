![screenshot.png](screenshot.png?raw=true)
# cairowidget
This code illustrates one possible way to use cairo in your FLTK project. It does not require support for cairo or SVG to be compiled into FLTK and shows an alternative way to render SVG using cairo.

I also explored support for cairo's OpenGL backend (`Cairo_Gl_Window`). This could make for fast renders of large interactive GUI controls, but the backend has considerable bugs. Nevertheless, should these bugs ever be fixed, we will have the option of rendering everything through cairo.
# build instructions
    git submodule init
    git submodule update
    g++ -std=c++2a -Ofast -I./nanosvg/src example1.cpp cairowidget.cpp caironanosvg.cpp -o example1 -lfltk -lcairo
