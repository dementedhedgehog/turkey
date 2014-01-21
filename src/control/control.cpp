
#include "model/model.h"
#include "control.h"

Control::Control(Model * model) {
    this->model = model;
}

Model const * const Control::get_model() {
    return model;
}
