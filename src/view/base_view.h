#ifndef BASE_VIEW_H
#define BASE_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "model/model.h"
#include "view/image_manager.h"
#include "view/font_manager.h"
#include "view/sound_manager.h"

/*
 * Base view for all the other views.. provides access to the various resource managers etc.
 *
 */

class BaseView {
 protected:
    Model * model;
    SDL_Window * window;
    SDL_Renderer * renderer;

    // the image manager
    ImageManager * image_manager;

    // the font manager
    FontManager * font_manager;

    // the sound manager
    SoundManager * sound_manager;
    
public:
    BaseView(
        Model * model, 
        SDL_Window * window, SDL_Renderer * renderer,
        ImageManager * image_manager, 
        FontManager * font_manager, 
        SoundManager * sound_manager);

    // initalize the libraries
    virtual int init() = 0;

    // render to the window 
    // return 0 for success of some non-zero error code
    virtual int render() = 0;

    // clean up resources
    virtual int clean_up() = 0;
    
    // mouse event
    virtual void mouse_button() = 0;
};
#endif 


