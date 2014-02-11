#ifndef VIEW_H
#define VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>

#include "model/model.h"
#include "view/utils.h"
#include "view/fps.h"
#include "view/image_manager.h"
#include "view/font_manager.h"
#include "view/sound_manager.h"
#include "view/base_view.h"
#include "view/intro_view.h"
#include "view/game_view.h"

/**
 * View for MVC
 *
 */
class View : public IStateListener, public BaseView {
 private:

    // determines how many frames per second we get
    uint32_t ms_per_frame;

    // determines how many moves per second we get
    uint32_t ms_per_move;
 
    // fps counter (null if we're not counting fps).
    FPS * fps;

    // all the game views
    IntroView * intro_view;
    GameView * game_view;

    // the current game view
    BaseView * current_view;
    
    // set this to true when we want to exit the message loop and hence the application
    bool finished;

       
 public:    

    // constructor
    View(Model * model, SDL_Window * window, SDL_Renderer * renderer,
         ImageManager * image_manager, FontManager * font_manager, SoundManager * sound_manager);

    /* // access the renderer */
    SDL_Renderer * get_renderer();

    // start up sdl
    int init();
    
    // mouse event
    void mouse_button() {};

    // show an sdl window
    int display_window();

    // draw to a window
    // return 0 for success of some non-zero error code
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

    // quit the application nicely.
    void quit();

    // access the window 
    // (the image renderer needs it).
    SDL_Window * get_window() { return window; };
};

#endif
