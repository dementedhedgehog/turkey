#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <utility> // for pair
#include <list> 

#include <SDL2/SDL.h>

#include "model/game_obj.h"

// a bit of short hand
typedef std::pair<GameObj*, GameObj*> collision_t;

 
/*
 * Collision detector interface.
 */
class ICollisionDetector {
 public:

    // fill the list of collisions in, in place
    virtual void detect_collisions(
        const std::list<GameObj*> game_objs, 
        std::list<collision_t> & collisions) = 0;
};

#endif
