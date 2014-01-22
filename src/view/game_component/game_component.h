#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H
/*
 * Game component that playes the side scroller game itself.
 *
 */
#include "view/i_component.h"

class GameComponent : public IComponent {
 public:
    // start displaying this component
    void init(SDL_Renderer * renderer);
    //void start(View & view);
    void render(SDL_Renderer * renderer, SDL_Window * window);
};

#endif 


