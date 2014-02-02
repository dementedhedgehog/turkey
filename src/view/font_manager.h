#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


class FontManager {
 private:
    // an example of a ttf font
    TTF_Font * hobbiton_font; 


 public:

    TTF_Font * get_fps_font();    

    void init();
    void clean_up();
};


#endif
