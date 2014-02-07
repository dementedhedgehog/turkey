#ifndef INTRO_VIEW_H
#define INTRO_VIEW_H
/*
 * Game view that plays the intro
 *
 */
#include "model/model.h"
#include "view/sub_view.h"

class IntroView : public SubView {
 private:
    SDL_Texture * background;

    // example text
    // FIXME: add to its own object
    SDL_Surface * title_text;
    SDL_Texture * title_texture;

    // an example of a ttf font
    TTF_Font * font;  // FIXME get the font from a loader object..

 public:
    IntroView(Model * model, SDL_Window * window, SDL_Renderer * renderer);

    // start displaying this view
    int init();

    // draw to the screen
    void render();
 
    // clean up any resources
    void clean_up();

    // a mouse event occurred
    void mouse_button();
};

#endif 


