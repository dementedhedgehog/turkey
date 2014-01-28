
#include <stdlib.h>

#include "model/collision_detectors/brute_force_collision_detector.h"



void BruteForceCollisionDetector::detect_collisions(
        const std::list<GameObj*> game_objs, 
        std::list<collision_t> & collisions) {

    // clear the list so there are no collisions left.
    collisions.clear();

    // for each game obj
    std::list<GameObj*>::const_iterator i;
    std::list<GameObj*>::const_iterator j;
    GameObj * game_obj_i;
    GameObj * game_obj_j;
    for (i = game_objs.begin(); i != game_objs.end(); i++) {         

        // 
        game_obj_i = *i;

        for (j = i; j != game_objs.end(); j++) {         

            // don't compare an element with itself.
            if (i == j) {
                continue;
            }
            
            // move stuff
            game_obj_j = *j;
            
            // check for a collision..
            if ((game_obj_i->half_width + game_obj_j->half_width) > 
                abs(game_obj_i->x - game_obj_j->x)) {
                
                if ((game_obj_i->half_height + game_obj_j->half_height) > 
                    abs(game_obj_i->y - game_obj_j->y)) {
                    
                    // collision!
                    collision_t c = std::make_pair(game_obj_i, game_obj_j);
                    collisions.push_back(c);
                }                
            }
        }
    }  
}

