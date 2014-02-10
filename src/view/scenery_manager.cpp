
#include <algorithm> 

#include "view/scenery_manager.h"
#include "view/utils.h"



int SceneryManager::render_background(SDL_Renderer * renderer, float camera_x, float camera_y) {

    Scenery * scenery;
    for (std::vector<Scenery*>::iterator i = backgrounds.begin(); i != backgrounds.end(); i++) {
        scenery = *i;


        std::cout << "  XX" << (scenery->x - (camera_x * scenery->scroll_rate)) << ", " 
                  << (scenery->y - (camera_y * scenery->scroll_rate)) << std::endl;

        std::cout << "  XX" << scenery->x << " cam " << camera_x  << "sr " << scenery->scroll_rate << std::endl;

        
      
        render_texture(scenery->texture, renderer, 
            scenery->x - (camera_x * scenery->scroll_rate), 
            scenery->y - (camera_y * scenery->scroll_rate));        
    }

    return 0; // success
}

bool scenery_sort_fn(Scenery * s1, Scenery * s2) { 
    return (s1->scroll_rate < s2->scroll_rate); 
}

int SceneryManager::add_background(SDL_Texture * texture, 
    float scroll_rate,
    float x, float y) {

    Scenery * scenery = new Scenery(texture, scroll_rate, x, y);
    backgrounds.push_back(scenery);

    // we don't really care about adding backgrounds takes at the moment
    // (this is very inefficent)
    std::sort(backgrounds.begin(), backgrounds.end(), scenery_sort_fn);

    return 0; // success
}
