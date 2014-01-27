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
#include "view/intro_component/intro_component.h"
#include "view/game_component/game_component.h"

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
 
    // fps counter
    FPS * fps;

    // turn on grid (for development)
    //bool grid_enabled;

    // turn on sound (for development)
    bool sound_enabled;

    // the sdl renderer
    SDL_Renderer * renderer;

    // the game window
    SDL_Window * window;

    // run in fullscreen mode (use F1 to toggle).
    bool fullscreen;

    // the MVC control object
    //Control * control;

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
    Component * current_component;
    
    // set this to true when we want to exit the message loop and hence the application
    bool finished;

       
 public:    

    //View(Control * control);
    View();
    /* ~View(); */

    // access the renderer
    SDL_Renderer * get_renderer();

    // start up sdl
    int init_sdl();

    // show an sdl window
    int display_window(Model * model);

    // draw to a window
    int render();

    // enter the message loop... we stay in here till the player quits
    int msg_loop();

    // toggle back and forth between full screen mode
    void toggle_fullscreen();

    /* // load a texture. */
    /* SDL_Texture * load_texture(const std::string &file); */

    /* // render a texture */
    /* void render_texture(SDL_Texture *tex, int x, int y); */
    /* void render_texture(SDL_Texture *tex, SDL_Rect destination, SDL_Rect * clip); */
    /* void render_texture(SDL_Texture *tex, int x, int y, SDL_Rect * clip); */

    // free everything.
    int clean_up();

    // notification that the state has changed
    // (required by the IStateListener interface)
    void state_changed(State old_state, State current_state);
};

#endif
