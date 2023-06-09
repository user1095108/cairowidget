![screenshot.png](screenshot.png?raw=true)
# cairowidget
This code illustrates one possible way to use cairo in your FLTK and Qt projects. It does not require support for cairo or SVG to be compiled into FLTK or Qt and shows an alternative way to render SVG using cairo and NanoSVG.

I also explored support for cairo's OpenGL backend (`Cairo_Gl_Window`). This could make for fast renders of large interactive GUI controls, but the backend had considerable bugs.

Qt markedly outperforms FLTK in rendering performance.

# build instructions

    git submodule update --init
    g++ -std=c++20 -Ofast -I./nanosvg/src example1.cpp cairowidget.cpp caironanosvg.cpp -o example1 -lfltk -lcairo

or

    git submodule update --init
    qmake examples.pro
    make
