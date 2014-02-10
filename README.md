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


You need the libasound2-dev files installed if you're building SDL from scratch to get sound working on unix machines run:
 sudo yum install alsa-lib-devel



      
building on windows
-------------------

 o) mkdir C:\turkey
 i) cd C:\turkey

 i) Checking out with git.. note there's a bug in the latest git so get the older 
    version without the bug:

       git-1.8.4 preview.

    clone the turkey repo from the cmd line thus:     
       git clone https://github.com/dementedhedgehog/turkey.git

   So now you'll have C:\turkey\turkey\..lots..of..files


 ii) Install mingw in c:\mingw 
 iii) Make sure c:/mingw/bin in in the PATH environment variable

 iv) Download the sdl development libraries.. here
        http://www.libsdl.org/download-2.0.php
    get this one:
        http://www.libsdl.org/release/SDL2-devel-2.0.1-mingw.tar.gz

   gunzip it and untar it into c:\turkey    



 v) run mingw32-make
 

     

