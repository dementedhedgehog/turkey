
#include <assert.h>

#include "model/model.h"
#include "control.h"

Control::Control(Model * model) {
    this->model = model;
    assert(model != NULL);
}

Model const * const Control::get_model() {
    return model;
}


void Control::toggle_fullscreen() {
    model->toggle_fullscreen();
}

void Control::change_state(State new_state) {
    assert(model != NULL);
    model->change_state(new_state);
}
