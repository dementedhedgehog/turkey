/*
 * Frames per second counter.
 */
#include <iostream>

#include "view/fps.h"
#include "view/utils.h"


const unsigned int FRAME_RATE_PERIOD = 1000; // milliseconds


FPS::FPS(TTF_Font * fps_font) {

    // state variables used to calculate the current fps
    current_frame_count = 0;

    // current frames per second - updated every fps_period
    current_fps = 0.0f;
    
    // font colour
    fps_font_color = {0,0,0};
    fps_text_surface = NULL;
    fps_text_texture = NULL;
    this->fps_font = fps_font;

    // time we started counting frames in millisecs
    fps_start_time = 0;
}


void FPS::increment(SDL_Renderer * renderer) {
    unsigned int time_now = SDL_GetTicks();
    unsigned int delta_time = time_now - fps_start_time;
        
    // increment the number of frames rendered
    current_frame_count += 1;

    // if it's time to update the fps count then 
    if (delta_time > FRAME_RATE_PERIOD) {

        // change the current_fps 
        current_fps = current_frame_count / FRAME_RATE_PERIOD;

        // FIXME: remove this!
        //std::cout << "fps: " << get_fps() << std::endl;


        // FIXME WE NEED 
        //   method to blat dynamic floats (digits to texture then render).
        //   method to blat static strings (to texture then render)
        // 

        // FIXME: this is slow.. 
        // render on the GPU by making textures for each digit and displaying them 
        fps_text_surface = TTF_RenderText_Solid(
            fps_font, 
            // This doesn't work because of a bug in the current version of mingw/gcc!!
            // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=52015
            // std::to_string(current_frame_count).c_str(), 
            "FIX THE FPS STUFF!!!", 
            fps_font_color);

        if (!fps_text_surface) {
            // handle error here, perhaps print TTF_GetError at least
            log_ttf_error("Problem displaying fps text!");
        } 
        else {

            fps_text_texture = SDL_CreateTextureFromSurface(renderer, fps_text_surface);
            if (!fps_text_texture) {
                log_sdl_error("SDL_CreateTextureFromSurface: ");
                //return 4;
            }        
            else {
                std::cout << "Draw FPS XXXX" << std::endl;
            }
                
            // draw the title
            SDL_Rect src = { 0, 0, fps_text_surface->w, fps_text_surface->h };
            SDL_Rect dest = { 30, 30, fps_text_surface->w, fps_text_surface->h};
            SDL_RenderCopy(renderer, fps_text_texture, &src, &dest);

            // perhaps we can reuse it, but I assume not for simplicity.
            SDL_FreeSurface(fps_text_surface);
            SDL_DestroyTexture(fps_text_texture);
        }
        


        // and reset the fps counters            
        current_frame_count = 0;
        fps_start_time = time_now;
    }
}



// float FPS::get_fps() {
//     return current_frame_count;
// }
