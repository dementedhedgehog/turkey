#ifndef INTRO_STATE_H
#define INTRO_STATE_H

#include "model/base_state.h"


class IntroState : public BaseState {
 private:
    Model * model;

 public:
    IntroState(Model * model);
    State get_state();
    void update();

    // handle a mouse event
    void handle_mouse(const int x, const int y, const Uint8 mouse_button_state);
};


#endif
