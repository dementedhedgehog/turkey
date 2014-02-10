#ifndef INTRO_VIEW_H
#define INTRO_VIEW_H
/*
 * Game view that plays the intro
 *
 */
#include "model/model.h"
#include "view/base_view.h"

class IntroView : public BaseView {
 private:
    SDL_Texture * background;

    // example text
    // FIXME: add to its own object
    SDL_Surface * title_text;
    SDL_Texture * title_texture;

    std::vector<SDL_Texture*> * frames;

    // an example of a ttf font
    TTF_Font * font;  // FIXME get the font from a loader object..

 public:
    IntroView(Model * model, SDL_Window * window, SDL_Renderer * renderer, 
        ImageManager * image_manager, FontManager * font_manager, SoundManager * sound_manager);

    // start displaying this view
    int init();

    // draw to the screen
    int render();
 
    // clean up any resources
    int clean_up();

    // a mouse event occurred
    void mouse_button();
};

#endif 


