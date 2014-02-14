/*
 *
 * A Simple Side-Scroller Game.
 *
 */
#include <iostream>
#include <string>

#include "model/model.h"
#include "view/view.h"
#include "view/utils.h"
#include "shared/scripting.h"


int main(int argc, char **argv){
    int result;

    // log some info about about the run context
    log_debug_info();


    // init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_sdl_error("SDL_Init");
		return 1;
	}
    
    // init the img reading library
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        log_sdl_error("IMG_Init");
        return 2;
    }

    // init the font library
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: %s\n" << TTF_GetError() << std::endl;
        return 3;
    }

    // create the baseview window
    SDL_Window * window = SDL_CreateWindow(GAME_NAME, 
        SCREEN_POS_X, SCREEN_POS_Y, 
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        log_sdl_error("CreateWindow");
        return 2;
    }

    // create the baseview renderer for the window
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        log_sdl_error("CreateRenderer");
        return 3;
    }


    // initialize the font manager
    FontManager * font_manager = new FontManager();
    if ((result = font_manager->init()) != 0) {
        log_msg("init font manager failed!");
        return result;
    };

    // initialize the image manager
    ImageManager * image_manager = new ImageManager(window);
    if ((result = image_manager->init()) != 0) {
        log_msg("init image manager failed!");
        return result;
    };

    // initialize the sound
    SoundManager * sound_manager = new SoundManager();
    if ((result = sound_manager->init()) != 0) {
        log_msg("init sound manager failed!");
        return result;
    };

    // seed the random number generator
    srand(SDL_GetTicks());

    // create the model 
    Model * model = new Model();

    // create the view
    View * view = new View(model, window, renderer, image_manager, font_manager, sound_manager);

    // the view gets change notifications from the model
    model->register_state_listener(view);

    // init the UI libraries or die
    if ((result = view->init()) != 0) {
        log_msg("init view failed!");
        return result;
    }

    // display the window or die
    if ((result = view->display_window()) != 0) { 
        log_msg("display_window failed!");
        return result; 
    }

    // initialize the embedded python interpreter
    Scripting * scripting = new Scripting(model, view);
    if ((result = scripting->init(argv[0])) != 0) {
        log_msg("Failed to initialize the scripting library!");
        return result;
    };

    // initialize the model using python scripts
    if (scripting->run_initialize_levels_script() != 0) {
        log_msg("Failed to run the init python method!");
        return result;
    };



    //
    // Main Message Loop!
    //

    // enter the message loop.. move, draw, check ui events, repeat
    // until the game is over
    view->msg_loop();


    //
    // Cleaning up..
    //

    // clean up the model
    model->clean_up();

    // clean up the ui objects
    view->clean_up();

    // clean up the managers
    image_manager->clean_up();
    font_manager->clean_up();
    sound_manager->clean_up();
   
    // clean up
    delete view;
    delete model;
    delete scripting;
    delete image_manager;
    delete font_manager;
    delete sound_manager;

    // clean up the ttf library
    TTF_Quit();
   
    // clean up the img library
    IMG_Quit();

    // clean up the sdl library
	SDL_Quit();

    // done.
	return 0;
}
