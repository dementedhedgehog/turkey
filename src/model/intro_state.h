#ifndef INTRO_STATE_H
#define INTRO_STATE_H

#include "model/i_state.h"

// forward declaration
//class Model;

class IntroState : public IState {
 private:
    Model * model;

 public:
    IntroState(Model * model);
    State get_state();
    void update(const Uint8 * key_states);

    // handle a mouse event
    void handle_mouse(const int x, const int y, const Uint8 mouse_button_state);
};


#endif
