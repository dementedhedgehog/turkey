/*
 *
 * A Simple Side-Scroller Game.
 *
 */

// required to be the first line by the python docs
//#include <Python.h>

#include <iostream>
#include <string>

// #include <stdio.h>
// #include <execinfo.h>
// #include <signal.h>
// // #include <stdlib.h>
// // #include <unistd.h>


#include "model/model.h"
#include "view/view.h"
#include "shared/scripting.h"


// void error_handler(int sig) {
//     void *array[10];
//     size_t size;

//     // get void*'s for all entries on the stack
//     size = backtrace(array, 10);

//     // print out all the frames to stderr
//     std::cerr <<  "Error: signal " << sig << ":" << std::endl;
//     backtrace_symbols_fd(array, size, STDERR_FILENO);
//     exit(1);
// }


int main(int argc, char **argv){
    int result;

    // install an exception handler
    // signal(SIGSEGV, error_handler);   

    Model * model = new Model();
    View * view = new View();

    // the view gets change notifications from the model
    model->register_state_listener(view);

    // init the UI libraries or die
    if ((result = view->init_sdl()) != 0) {
        log_msg("init sdl failed!");
        return result;
    }

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

    // clean up
    delete view;
    delete model;
    delete scripting;

    // done.
	return 0;
}
