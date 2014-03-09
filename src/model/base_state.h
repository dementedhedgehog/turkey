#ifndef BASE_STATE_H
#define BASE_STATE_H
/**
 * The game has a bunch of parts.. the intro screen, high scores, the game, etc.
 * We call these "states".  This class represents the shared behaviour of these "states",
 */

#include <SDL2/SDL.h>

#include "model/keyboard_handler.h"

// forward declaration
class Model;

enum class State {INTRO, GAME, FATAL_ERROR, QUITTING};    

class BaseState {
 protected:
    // every 
    KeyboardHandler * keyboard_handler;

 public:
    BaseState() {
        keyboard_handler = new KeyboardHandler();
    }

    virtual ~BaseState() {};    
    virtual State get_state() = 0;
    virtual void update() = 0;

    // handle a mouse event
    virtual void handle_mouse(const int x, const int y, const Uint8 mouse_button_state) = 0;
};


class FatalErrorState : public BaseState {
 public:
    State get_state() { return State::FATAL_ERROR; }    
    //void update(const Uint8 * key_states) { }
    void update() { }
    void handle_mouse(const int x, const int y, const Uint8 mouse_button_state) {};
};



class QuittingState : public BaseState {
 public:
    State get_state() { return State::QUITTING; }    
    void update() { }
    void handle_mouse(const int x, const int y, const Uint8 mouse_button_state) {};
};

#endif
