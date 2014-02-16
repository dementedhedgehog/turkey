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
    Collision(GameObj * a, GameObj * b);

    // check for a collision between the projected position of a and the projected 
    // position of b (when both a and b are movable).
    bool check_for_projected_movable_collision();

    // check for a collision between the projected position of a and the position of b 
    // (when only a is movable).
    bool check_for_projected_fixed_collision();

    // resolve a collision by changing the state of the game objects involved in the 
    // collision
    void resolve();
};

#endif
