
#include "model/collision.h"

Collision::Collision(GameObj * a, GameObj * b) {
    this->a = a;
    this->b = b;
    collision_type = CollisionType::NONE;
}

// check for a collision between the projected position of a and the projected 
// position of b (both of which are movable).
bool Collision::check_for_projected_movable_collision() {
    this->collision_type = a->check_for_projected_movable_collision(b);
    return (this->collision_type != CollisionType::NONE);
}

// check for a collision between the projected position of a and the position of b 
// (only a is movable).
bool Collision::check_for_projected_fixed_collision() {
    this->collision_type = a->check_for_projected_fixed_collision(b);
    return (this->collision_type != CollisionType::NONE);
}

void Collision::resolve() {
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

