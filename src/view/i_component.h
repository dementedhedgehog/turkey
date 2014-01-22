#ifndef I_COMPONENT_H
#define I_COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "view/utils.h"


/*
 * Interface for Components.
 *
 * Components are parts of the game.. e.g. the help screens, the intro screen, the inventory screen the 
 * game screen etc.
 */
//#include "view/view.h"

class IComponent {
 public:

    // Adding a virtual destructor allows you to pass pointer ownership to another party without exposing the 
    // concrete derived class. The destructor doesn't have to do anything, because the interface doesn't have 
    // any concrete members
    virtual ~IComponent() {};

    // increment the count of the frames rendererd (do this each frame).
    //virtual void start(View & view) = 0;
    virtual void init(SDL_Renderer * renderer) = 0;
    virtual void render(SDL_Renderer * renderer, SDL_Window * window) = 0;

};

#endif 


