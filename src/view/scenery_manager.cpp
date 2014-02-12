
#include <algorithm> 

#include "view/scenery_manager.h"
#include "view/utils.h"



int SceneryManager::render_background(SDL_Renderer * renderer, float camera_x, float camera_y) {
    Scenery * scenery;
    for (std::vector<Scenery*>::iterator i = backgrounds.begin(); i != backgrounds.end(); i++) {
        scenery = *i;
             
        render_texture(scenery->texture, renderer, 
            scenery->x - (camera_x * scenery->scroll_rate), 
            scenery->y - (camera_y * scenery->scroll_rate));        
    }
    return 0; // success
}


int SceneryManager::render_foreground(SDL_Renderer * renderer, float camera_x, float camera_y) {
    Scenery * scenery;
    for (std::vector<Scenery*>::iterator i = foregrounds.begin(); i != foregrounds.end(); i++) {
        scenery = *i;
             
        render_texture(scenery->texture, renderer, 
            scenery->x - (camera_x * scenery->scroll_rate), 
            scenery->y - (camera_y * scenery->scroll_rate));        
    }
    return 0; // success
}

bool scenery_sort_fn(Scenery * s1, Scenery * s2) { 
    return (s1->scroll_rate < s2->scroll_rate); 
}

int SceneryManager::add_scenery(float scroll_rate, float x, float y, SDL_Texture * texture) {
    Scenery * scenery = new Scenery(texture, scroll_rate, x, y);

    if (scroll_rate <= 1.0f) {
        backgrounds.push_back(scenery);

        // we don't really care about adding backgrounds takes at the moment
        // (this is very inefficent)
        std::sort(backgrounds.begin(), backgrounds.end(), scenery_sort_fn);
    }
    else {
        foregrounds.push_back(scenery);

        // we don't really care about adding foregrounds takes at the moment
        // (this is very inefficent)
        std::sort(foregrounds.begin(), foregrounds.end(), scenery_sort_fn);
    }
    return 0; // success
}
