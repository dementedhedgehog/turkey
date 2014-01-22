#ifndef INTRO_COMPONENT_H
#define INTRO_COMPONENT_H
/*
 * Game component that plays the intro
 *
 */
#include "view/i_component.h"

class IntroComponent : public IComponent {
 private:
    SDL_Texture * background;


 public:
    // start displaying this component
    //void start(View & view);
    void init(SDL_Renderer * renderer);
    void render(SDL_Renderer * renderer, SDL_Window * window);
};

#endif 


