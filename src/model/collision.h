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

    // calculate the projected move using the speculative contacts approach
    /* inline void calc_projected_move() { */
    /*     collision_type = a->calc_projected_move(b); */
    /* } */

    /* inline void penetration_resolution() { */
    /*     //a.penetration_resolution(b); no op for the moment */
    /*     // FIXME... thinking about this.. */
    /* } */

    inline bool check() {
        this->collision_type = a->collides_with(b);
        return (this->collision_type != NONE);
    }
};

#endif
