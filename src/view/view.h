#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL_mixer.h"

#include <string>

#include "model/model.h"
#include "view/utils.h"
#include "view/fps.h"
#include "view/font_manager.h"
#include "view/intro_view/intro_view.h"
#include "view/game_view/game_view.h"

/**
 * View for MVC
 *
 */
class View : public IStateListener {
 private:

    // determines how many frames per second we get
    uint ms_per_frame;

    // determines how many moves per second we get
    uint ms_per_move;
 
    // fps counter (null if we're not counting fps).
    FPS * fps;

    // turn on sound (for development)
    bool sound_enabled;

    // the font manager
    FontManager font_manager;

    // the sdl renderer
    SDL_Renderer * renderer;

    // the game window
    SDL_Window * window;

    // the music that will be played 
    Mix_Music * music; 

    // the sound effects that will be used 
    Mix_Chunk * scratch; 
    Mix_Chunk * high; 
    Mix_Chunk * med; 
    Mix_Chunk * low; 

    // all the game views
    IntroView * intro_view;
    GameView * game_view;

    // the current game view
    SubView * current_view;
    
    // set this to true when we want to exit the message loop and hence the application
    bool finished;

    // reference to the model
    Model * model;

       
 public:    

    // constructor
    View();

    /* // access the renderer */
    SDL_Renderer * get_renderer();

    // start up sdl
    int init_sdl();

    // show an sdl window
    int display_window(Model * model);

    // draw to a window
    int render();

    // enter the message loop... we stay in here till the player quits
    int msg_loop();

    // free everything.
    int clean_up();    

    // notification that the state has changed
    // (required by the IStateListener interface)
    void state_changed(State old_state, State current_state);


    //
    // Python API stuff.
    // expose these methods to the python api
    //
    
    // sets that flag that determines whether cell grid is drawn
    void debug_set_draw_grid(const bool draw_grid) { 
        game_view->debug_set_draw_grid(draw_grid); 
    }

    // sets that flag that determines whether frames per second is drawn
    void debug_set_draw_fps(const bool draw_fps);

    void quit() { finished = true; }
    // load a texture
    /* SDL_Texture * pyapi_load_texture(const std::string &fname) { */
    /*     load_texture(fname, renderer); */
    /* } */
};

#endif
