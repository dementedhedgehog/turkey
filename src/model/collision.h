#ifndef COLLISION_H
#define COLLISION_H

#include "model/game_obj.h"

class Collision {
 private:
    GameObj * a;
    GameObj * b;

    CollisionType collision_type; 

    // FIXME: later on we'll have state here..
    // some collisions just go away after they happen (e.g. being hit with a bullet)
    // so we won't need to spend too much time resolving them.

 public:
    Collision(GameObj * a, GameObj * b) {
        this->a = a;
        this->b = b;
        collision_type = CollisionType::NONE;
    }

    // check for a collision between the projected position of a and the projected 
    // position of b (both of which are movable).
    inline bool check_for_projected_movable_collision() {
        this->collision_type = a->check_for_projected_movable_collision(b);
        return (this->collision_type != CollisionType::NONE);
    }

    // check for a collision between the projected position of a and the position of b 
    // (only a is movable).
    inline bool check_for_projected_fixed_collision() {
        this->collision_type = a->check_for_projected_fixed_collision(b);
        return (this->collision_type != CollisionType::NONE);
    }

    inline void resolve() {
        // FIXME: use a collision handling lookup here?
        // separate resolves for collisions with dynamic and static objects?
        
        // stop it falling through the floor?
        if (((collision_type & CollisionType::BOTTOM) != CollisionType::NONE) && !b->movable) {

            //a->y_vel_per_sec = 0.0f;
            a->collision_set_y_velocity(0.0f);
            a->jumping = false;
        }

        /* if (((collision_type & CollisionType::LEFT) != CollisionType::NONE) && !b->movable) { */
        /*     std::cout << "XXX" << std::endl; */
        /*     a->collision_set_x_velocity(0.0f); */
        /*     //a->x_vel_per_sec = 0.0f; */
        /*     //a->jumping = false; */
        /* } */
    }
};

#endif
