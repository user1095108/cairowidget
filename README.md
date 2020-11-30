![screenshot.png](screenshot.png?raw=true)
# cairowidget
cairo widget for FLTK. This code illustrates one possible way to use cairo in your project. There is an official FLTK solution available, but support for that is not compiled into the library in many distributions and it may not be what you want anyway.

I also explored support for cairo's OpenGL backend (`Cairo_Gl_Window`). This could make for fast renders of large interactive GUI controls, but the backend has considerable bugs. Nevertheless, should these bugs ever be fixed, we will have the option of rendering everything through cairo.
# build instructions
    git submodule init
    git submodule update
    g++ -std=c++2a -Ofast -I./nanosvg/src example1.cpp cairowidget.cpp cairosvgutils.cpp -o example1 -lfltk -lcairo
