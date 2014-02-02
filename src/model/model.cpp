
#include <iostream>
#include <assert.h>

#include "model.h"



Model::Model() {
    intro_state = new IntroState(this);
    game_state = new GameState(this);
    fatal_error_state = new FatalErrorState();
    current_state = intro_state;

    game_has_keyboard_focus = true;
}

Model::~Model() {
    delete intro_state;
    delete game_state;
    delete fatal_error_state;
}

void Model::set_keyboard_focus(const bool has_focus) {
    game_has_keyboard_focus = has_focus;
}

const bool Model::has_keyboard_focus() {
    return game_has_keyboard_focus;
}

void Model::change_state(State to_state) {
    assert(current_state != NULL);

    std::cout << " state changed! " << std::endl;

    State old_state_enum = this->current_state->get_state();

    switch (to_state) {
        case State::GAME:
            current_state = game_state;
            break;

        case State::INTRO:
            // fall through .. we should never get here?
        default:
            current_state = fatal_error_state;
    }
    
    for (std::list<IStateListener*>::iterator i = listeners.begin(); i != listeners.end(); i++)
    {
        (*i)->state_changed(old_state_enum, current_state->get_state());
    }    
}

void Model::register_state_listener(IStateListener * state_listener) {
    listeners.push_back(state_listener);    
}

GameState * Model::get_game_state() {
    return game_state;
}

void Model::update(const Uint8 * key_states) {
    current_state->update(key_states);
}
