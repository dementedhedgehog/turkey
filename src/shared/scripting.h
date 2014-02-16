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
    GameState * game_state;

    View * view;
    SDL_Renderer * renderer;
    //GameView * game_view;
    ImageManager * image_manager;

public:
    Scripting(Model * model, View * view);
    ~Scripting();

    int init(char * program_fname);
    
    // run the initialization script 
    int run_initialize_levels_script();
};

#endif
