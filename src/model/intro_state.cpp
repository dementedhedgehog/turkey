
#include <iostream>

#include <SDL2/SDL.h>

#include "model/intro_state.h"
#include "model/model.h"

//
// Actions
//

class SkipIntro: public IAction {
    Model * model;

    // need this to avoid strobing the jump button
    bool skip_key_pressed;
    
 public:
    SkipIntro(Model * model) { this->model = model; }

    void do_action() {
        if (!skip_key_pressed) {
            skip_key_pressed = true;
            model->change_state(State::GAME);        
        }
    }

    bool do_debounce() { return true; };
    void undo_action() {
        // jump key released (logic to avoid strobing the jump key)
        skip_key_pressed = false;
    };

};


//
//
//
IntroState::IntroState(Model * model) {
    this->model = model;    
    keyboard_handler->add_action(new SkipIntro(model), SDL_SCANCODE_SPACE);
    keyboard_handler->add_action(new SkipIntro(model), SDL_SCANCODE_X);
}

State IntroState::get_state() { return State::INTRO; };

void IntroState::update() { 
    keyboard_handler->do_actions();
}


void IntroState::handle_mouse(const int x, const int y, const Uint8 mouse_button_state) {
    if (mouse_button_state & SDL_BUTTON(1)) {
        std::cout << "Mouse Button 1(left) is pressed.\n" << x << " , " << y << std::endl;
    }                            
}
