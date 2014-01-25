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
#include "control/control.h"
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

    Scripting * scripting = new Scripting();
    Model * model = new Model();
    Control * control = new Control(model);
    View * view = new View(control);

    // the view gets change notifications from the model
    model->register_state_listener(view);
    
    scripting->print_time();

    // init the UI libraries
    result = view->init_sdl();
    if (result != 0) {
        // something failed!
        return result;
    }

    // display the window or die
    if ((result = view->display_window()) != 0) { return result; }

    view->render();
    view->msg_loop();
    view->clean_up();

    delete view;
    delete control;
    delete model;
    delete scripting;

	return 0;
}
