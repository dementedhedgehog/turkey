#ifndef FPS_H
#define FPS_H
/*
 * Frames per second counter.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*
 * Class for counting frames per second.
 */

class FPS {
 private:

    // state variables used to calculate the current fps
    unsigned int current_frame_count;

    // current frames per second - updated every fps_period
    float current_fps; 

    // time we started counting frames in millisecs
    unsigned int fps_start_time; 

    // text rendering stuff
    TTF_Font * fps_font;
    SDL_Surface * fps_text_surface;
    SDL_Texture * fps_text_texture;
    SDL_Color fps_font_color;

 public:

    // constructor
    FPS(TTF_Font * fps_font);

    // increment the count of the frames rendererd (do this each frame).
    void increment(SDL_Renderer * renderer);

    // return the frames per second since the last 
    float get_fps();
};

#endif 


