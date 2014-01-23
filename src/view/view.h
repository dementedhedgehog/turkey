#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL_mixer.h"

#include <string>

#include "control/control.h"
#include "view/fps.h"
#include "view/intro_component/intro_component.h"
#include "view/game_component/game_component.h"

/**
 * View for MVC
 *
 */
class View {
 private:

    // determines how many frames per second we get
    uint ms_per_frame;

    // determines how many moves per second we get
    uint ms_per_move;
 
    // fps counter
    FPS * fps;

    // turn on grid (for development)
    //bool grid_enabled;

    // turn on sound (for development)
    bool sound_enabled;

    // an example of a ttf font
    TTF_Font * font;

    // example text
    // FIXME: add to its own object
    SDL_Surface * title_text;
    SDL_Texture * title_texture;

    // the sdl renderer
    SDL_Renderer * renderer;

    // the game window
    SDL_Window * window;

    // run in fullscreen mode (use F1 to toggle).
    //bool fullscreen;

    // the MVC control object
    Control * control;

    // the music that will be played 
    Mix_Music * music; 

    // the sound effects that will be used 
    Mix_Chunk * scratch; 
    Mix_Chunk * high; 
    Mix_Chunk * med; 
    Mix_Chunk * low; 

    // all the game components
    IntroComponent * intro;
    GameComponent * game;

    // the current game component
    IComponent * current_component;
       
 public:    

    View(Control * control);

    // start up sdl
    int init_sdl();

    // show an sdl window
    int display_window();

    // draw to a window
    int render();

    // move things
    int move();

    // enter the message loop... we stay in here till the player quits
    int msg_loop();

    // toggle back and forth between full screen mode
    void toggle_fullscreen();

    // free everything.
    int clean_up();
};

#endif
