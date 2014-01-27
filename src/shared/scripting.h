#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "model/model.h"
#include "view/view.h"

/*
 * Object that provides access to python scripts..
 *
 */
class Scripting {
private:
    Model * model;
    View * view;

public:
    Scripting(Model * model, View * view);
    ~Scripting();

    int init(char * program_fname);
    
    // run the initialization script 
    int run_initialize_levels_script();
};

#endif
