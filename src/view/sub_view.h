#ifndef I_SUB_VIEW_H
#define I_SUB_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "model/model.h"

class SubView;

/*
 * Interface for SubViews.
 *
 * SubViews are parts of the game.. e.g. the help screens, the intro screen, the inventory 
 * screen the game screen etc.  They are views in the MVC sense of the word.
 */

class SubView {
 protected:
    Model * model;
    SDL_Window * window;
    SDL_Renderer * renderer;
    
public:
    SubView(Model * model, SDL_Window * window, SDL_Renderer * renderer);

    // initalize the libraries
    virtual int init() = 0;

    // render to the window
    virtual void render() = 0;

    // clean up resources
    virtual void clean_up() = 0;
    
    // mouse event
    virtual void mouse_button() = 0;
};

#endif 


