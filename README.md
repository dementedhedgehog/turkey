turkey
======

Doth thou have a turkey?

Current dependencies.. 
   python2.7 library
   SDL2
   SDL_image
   SDL_tff
   SDL_mixer


Build and install SDL2 (or just install binaries).

    http://www.libsdl.org/download-2.0.php


Build and install the associate SDL_* libs from the internets, e.g.

      hg clone http://hg.libsdl.org/SDL_mixer/
      cd SDL_mixer
      ./autogen.sh
      ./configure
      make
      sudo make install

Python you just need the development stuff installed.


troubleshooting
---------------

Check the output of ./configure and install any missing libs otherwise it might fail to run properly, e.g. if you don't have the right image libs it might fail to read jpgs.

Sound doesn't work on my work machine but it does at home.  There's a flag to turn it off in src/view/view.cpp till I work out what's up.

If you're getting memory errors on linux run: gdb, run turkey, kill the turkey prog, where.
(not literally)


      


