#ifndef COLLISION_H
#define COLLISION_H

#include "model/game_obj.h"

class Collision {
 private:
    GameObj * a;
    GameObj * b;

    collision_type_t collision_type; 

    // FIXME: later on we'll have state here..
    // some collisions just go away after they happen (e.g. being hit with a bullet)
    // so we won't need to spend too much time resolving them.

 public:
    Collision(GameObj * a, GameObj * b) {
        this->a = a;
        this->b = b;
        collision_type = NONE;
    }

    // check for a collision between the projected position of a and the projected 
    // position of b (both of which are movable).
    inline bool check_for_projected_movable_collision() {
        this->collision_type = a->check_for_projected_movable_collision(b);
        return (this->collision_type != NONE);
    }

    // check for a collision between the projected position of a and the position of b 
    // (only a is movable).
    inline bool check_for_projected_fixed_collision() {
        this->collision_type = a->check_for_projected_fixed_collision(b);
        return (this->collision_type != NONE);
    }

    inline void resolve() {
        // FIXME.. lots and lots of work here 
        // need to use velocity? rather than dx/dy?

        /* std::cout << " RESOLVING! " << std::endl; */
        /* std::cout << " A! " << (collision_type == BOTTOM) << ", " << !b->movable << std::endl; */

        // stop it falling through the floor?
        if (collision_type == BOTTOM && !b->movable) {
             a->y_vel_per_sec = 0.0f;
             a->jumping = false;
        }
    }
};

#endif
