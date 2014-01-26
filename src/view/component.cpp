/**
 * Shared component methods.
 *
 */
#include <iostream>
#include <SDL2/SDL_image.h>

#include "view/component.h"

Component::Component(Model * model, SDL_Window * window, SDL_Renderer * renderer) {
    this->model = model;
    this->window = window;
    this->renderer = renderer;
}


