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
#include "control/control.h"
#include "view/view.h"
#include "shared/scripting.h"



int main(int argc, char **argv){
    int result;

    Scripting * scripting = new Scripting();
    Model * model = new Model();
    Control * control = new Control(model);
    View * view = new View(control);
    

    scripting->print_time();

    // init the UI libraries
    result = view->init_sdl();
    if (result != 0) {
        // something failed!
        return result;
    }

    view->display_window();
    view->render();
    view->msg_loop();
    view->clean_up();

    delete view;
    delete control;
    delete model;
    delete scripting;

	return 0;
}
