/*
 *
 * A Simple Side-Scroller Game.
 *
 */

// required to be the first line by the python docs
//#include <Python.h>

#include <iostream>
#include <string>

#include "model/model.h"
#include "view/view.h"
#include "shared/scripting.h"


int main(int argc, char **argv){
    int result;

    Model * model = new Model();

    ImageManager * image_manager = new ImageManager();
    FontManager * font_manager = new FontManager();
    SoundManager * sound_manager = new SoundManager();
    View * view = new View();

    // the view gets change notifications from the model
    model->register_state_listener(view);

    // init the UI libraries or die
    if ((result = view->init()) != 0) {
        log_msg("init view failed!");
        return result;
    }

    // initialize the managers
    if ((result = font_manager->init()) != 0) {
        log_msg("init font manager failed!");
        return result;
    };
    if ((result = image_manager->init(view->get_window())) != 0) {
        log_msg("init image manager failed!");
        return result;
    };
    if ((result = sound_manager->init()) != 0) {
        log_msg("init sound manager failed!");
        return result;
    };

    // seed the random number generator
    srand(SDL_GetTicks());

    // display the window or die
    if ((result = view->display_window(model)) != 0) { 
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
    scripting->run_initialize_levels_script();

    // enter the message loop.. move, draw, check ui events, repeat
    // until the game is over
    view->msg_loop();

    // clean up the ui objects
    view->clean_up();

    // clean up the image manager
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

    // done.
	return 0;
}
