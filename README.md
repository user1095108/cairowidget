![screenshot.png](screenshot.png?raw=true)
# cairowidget
This code illustrates one possible way to use cairo in your FLTK **and** Qt projects. It does not require support for cairo or SVG to be compiled into FLTK or Qt and shows an alternative way to render SVG using cairo and NanoSVG.

I also explored support for cairo's OpenGL backend (`Cairo_Gl_Window`). This could make for fast renders of large interactive GUI controls, but the backend has considerable bugs. Nevertheless, should these ever be fixed, we will have the option of rendering everything through cairo.

Qt markedly outperforms FLTK in rendering performance.

# build instructions
First do:

    git submodule init
    git submodule update

then

    g++ -std=c++2a -Ofast -I./nanosvg/src example1.cpp cairowidget.cpp caironanosvg.cpp -o example1 -lfltk -lcairo

or

    qmake
    make
