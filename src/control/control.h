#ifndef CONTROL_H
#define CONTROL_H

#include <string>

#include "model/model.h"


/**
 * Control for MVC
 *
 */
class Control {
 private:
    Model * model;
    
    
 public:
    
    Control(Model * model);    

    Model const * const get_model();

    void toggle_fullscreen();
};


#endif
