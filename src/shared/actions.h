#ifndef ACTIONS_H
#define ACTIONS_H

#include "model/model.h"
#include "model/keyboard_handler.h"
#include "view/view.h"
#include "view/utils.h"


/* class PrintAction: public IAction { */
/*  public: */
/*     void do_action() { */
/*         std::cout << " PRINT " << std::endl; */
/*     } */
/* }; */

class ToggleFullscreen: public IAction {
    SDL_Window * window;
    
 public:
    ToggleFullscreen(SDL_Window * window) { this->window = window; }

    void do_action() {
        toggle_fullscreen(window);
    }
};


class Quit: public IAction {
    View * view; 
 public:
    Quit(View * view) { this->view = view; }

    void do_action() {
        view->quit();
    }
};

#endif
