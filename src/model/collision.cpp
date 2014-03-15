
#include "model/collision.h"
#include "model/collision_resolution_manager.h"




Collision::Collision(GameObj * a, GameObj * b) {
    this->a = a;
    this->b = b;
    //collision = CollisionType::NONE;
    collision = CollisionType::POTENTIAL;
}

// check for a collision between the projected position of a and the projected 
// position of b (both of which are movable).
bool Collision::check_for_projected_movable_collision() {
    this->collision = a->check_for_projected_movable_collision(b);
    return (this->collision != CollisionType::NONE);
}

// check for a collision between the projected position of a and the position of b 
// (only a is movable).
bool Collision::has_projected_fixed_collision() {
    this->collision = a->check_for_projected_fixed_collision(b);
    //return (this->collision_type != CollisionType::NONE);
    return this->collision != CollisionType::NONE;
}

void Collision::resolve() {
    // FIXME: use a collision handling lookup here?
    // separate resolves for collisions with dynamic and static objects?

    // get the collision resolver for this collision
    //collision_resolver resolver = CollisionResolutionManager::get(a->type, b->type, collision);
    CollisionResolver * resolver = CollisionResolutionManager::get(
        a->type, b->type, collision);

    if (resolver != nullptr) {
        std::cout << a->type << " --> " << b->type << " " <<  collision 
                  << " = " << resolver->get_name() << std::endl;
    
        // resolve the collision
        //resolver(a, b, collision);
        resolver->resolve(a, b, collision);

    }
    else {
        std::cout << "No resolver configured for collision: " 
                  << a->get_type_name() << " -> " << b->get_type_name() << " at " 
                  << collision << std::endl;
        exit(1);
    } 
}

