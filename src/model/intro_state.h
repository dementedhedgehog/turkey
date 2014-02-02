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
};


#endif
