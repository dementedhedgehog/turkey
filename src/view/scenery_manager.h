#ifndef SCENERY_MANAGER_H
#define SCENERY_MANAGER_H

#include <vector>

#include "SDL2/SDL.h"

class Scenery {
 public:
    SDL_Texture * texture;
    float scroll_rate;
    float x, y;

 public:
    // constructor
    Scenery(SDL_Texture * texture, float scroll_rate, float x, float y) {
        this->texture = texture; 
        this->scroll_rate = scroll_rate;
        this->x = x;
        this->y = y;
    }
};


class SceneryManager {
    friend class Scenery;

 private:
    
    // a list of all scenery elements sorted back to front
    // FIXME: we will almost certainly want to do this a bit smarter..
    //        e.g loading and unloading background textures on demand
    //        but this will do for now.
    std::vector<Scenery*> backgrounds;
    std::vector<Scenery*> foregrounds;

 public:

    int render_background(SDL_Renderer * renderer, float camera_x, float camera_y);
    int render_foreground(SDL_Renderer * renderer, float camera_x, float camera_y);

    // if scroll_rate <= 1.0 the scenery will appear in the background
    // otherwise it will appear in the foreground
    int add_scenery(float scroll_rate, float x, float y, SDL_Texture * texture);
};

#endif
