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
    
    // sets are sorted
    std::vector<Scenery*> backgrounds;
    //std::list<Scenery*> foreground;


 public:

    int render_background(SDL_Renderer * renderer, float camera_x, float camera_y);

    int add_background(SDL_Texture * texture, 
        float scroll_rate, 
        float x, float y);
    //int add_foreground(const std::string fname, float scroll_rate);

};

#endif
