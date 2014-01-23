
#include "model.h"

Model::Model() {

    listeners = new std::list<IStateListener*>();

}

void Model::toggle_fullscreen() {
    // FIXME: WE SHOULD GET THIS STATE FROM SDL RATHER THAN DUPLICATE IT!??
    // BUT WE NEED TO SAVE IT?
}

void Model::change_state(IState * to_state) {

    IState * old_state = current_state;
    current_state = to_state;

    for(std::list<IStateListener*>::iterator iter = listeners.begin(); iter != listeners.end(); iter++)
    {
        (*iter)->state_changed(old_state, current_state);
    }    
}
