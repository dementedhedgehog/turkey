
#include <iostream>
#include <assert.h>

#include "model.h"



Model::Model() {
    intro_state = new IntroState();
    game_state = new GameState();
    fatal_error_state = new FatalErrorState();
    current_state = intro_state;
}

Model::~Model() {
    delete intro_state;
    delete game_state;
    delete fatal_error_state;
}

void Model::toggle_fullscreen() {
    // FIXME: WE SHOULD GET THIS STATE FROM SDL RATHER THAN DUPLICATE IT!??
    // BUT WE NEED TO SAVE IT?
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
