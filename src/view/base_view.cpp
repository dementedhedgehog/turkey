/**
 * Shared sub view methods.
 *
 * MVC View for a part of the game.
 */
#include <iostream>
#include <SDL2/SDL_image.h>

#include "view/base_view.h"

BaseView::BaseView(Model * model, 
                   SDL_Window * window, 
                   SDL_Renderer * renderer,
                   ImageManager * image_manager, 
                   FontManager * font_manager, 
                   SoundManager * sound_manager) {
    this->model = model;
    this->window = window;
    this->renderer = renderer;
    this->image_manager = image_manager;
    this->font_manager = font_manager;
    this->sound_manager = sound_manager;
}


ImageManager * BaseView::get_image_manager() {
    return image_manager;
}

SoundManager * BaseView::get_sound_manager() {
    return sound_manager;
}
