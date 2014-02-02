/**
 * Shared sub view methods.
 *
 * MVC View for a part of the game.
 */
#include <iostream>
#include <SDL2/SDL_image.h>

#include "view/sub_view.h"

SubView::SubView(Model * model, SDL_Window * window, SDL_Renderer * renderer) {
    this->model = model;
    this->window = window;
    this->renderer = renderer;
}


