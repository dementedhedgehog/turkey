
#include <iostream>

#include <SDL2/SDL.h>

#include "model/intro_state.h"
#include "model/model.h"

IntroState::IntroState(Model * model) {
    this->model = model;    
}

State IntroState::get_state() { return State::INTRO; };

void IntroState::update(const Uint8 * key_states) { 
    if (key_states[SDL_SCANCODE_RCTRL]) { 
        model->change_state(State::GAME);
    }
}
