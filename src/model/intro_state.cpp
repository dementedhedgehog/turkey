
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


void IntroState::handle_mouse(const int x, const int y, const Uint8 mouse_button_state) {
    if (mouse_button_state & SDL_BUTTON(1)) {
        std::cout << "Mouse Button 1(left) is pressed.\n" << x << " , " << y << std::endl;
    }                            
}
