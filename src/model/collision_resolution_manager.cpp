
#include <vector>

#include "collision_resolution_manager.h"

static int FROM_OBJ_OFFSET;
static int TO_OBJ_OFFSET;

// int CollisionResolutionManager::FROM_OBJ_OFFSET = 
//     GameObjType::get_num_game_obj_types() * N_VALID_COLLISION_TYPES;
// int CollisionResolutionManager::TO_OBJ_OFFSET = N_VALID_COLLISION_TYPES;

//collision_resolver * CollisionResolutionManager::collision_resolvers = nullptr;
static CollisionResolver ** collision_resolvers = nullptr;
//CollisionResolver ** CollisionResolutionManager::collision_resolvers = nullptr;


void CollisionResolutionManager::init() {

    // there's a collision resolver for each (game obj type, game obj type, collision type)

    FROM_OBJ_OFFSET = GameObjType::get_num_game_obj_types() * N_VALID_COLLISION_TYPES;
    TO_OBJ_OFFSET = N_VALID_COLLISION_TYPES;


    // FIXME: work out a way to seperate movable and fixed game obj ids?
    uint32_t n_resolvers = 
        GameObjType::get_num_game_obj_types() *      
        GameObjType::get_num_game_obj_types() * 
        N_VALID_COLLISION_TYPES;

    //CollisionResolutionManager::collision_resolvers = 
    collision_resolvers = 
        (CollisionResolver**) malloc(n_resolvers * sizeof(CollisionResolver*));
    //(collision_resolver*) malloc(n_resolvers * sizeof(collision_resolver));
}


void CollisionResolutionManager::clean_up() {
    //free(CollisionResolutionManager::collision_resolvers);
    free(collision_resolvers);
    //CollisionResolutionManager::collision_resolvers = nullptr;
    collision_resolvers = nullptr;
}


// void CollisionResolutionManager::add(
//     GameObjType * g1, GameObjType * g2, CollisionType c, collision_resolver resolver)
void CollisionResolutionManager::add(
    GameObjType * g1, GameObjType * g2, CollisionType c, CollisionResolver * resolver)
{    
    int index = 
        // g1->get_moveable_id() * CollisionResolutionManager::FROM_OBJ_OFFSET + 
        // g2->get_id() * CollisionResolutionManager::TO_OBJ_OFFSET +
        g1->get_moveable_id() * FROM_OBJ_OFFSET + 
        g2->get_id() * TO_OBJ_OFFSET +
        as_int(c);
            
    *(collision_resolvers + index) = resolver;
}


//collision_resolver CollisionResolutionManager::get(
CollisionResolver * CollisionResolutionManager::get(
    GameObjType * g1, GameObjType * g2, CollisionType c)
{    
    int index = 
        // g1->get_moveable_id() * CollisionResolutionManager::FROM_OBJ_OFFSET + 
        // g2->get_id() * CollisionResolutionManager::TO_OBJ_OFFSET +
        g1->get_moveable_id() * FROM_OBJ_OFFSET + 
        g2->get_id() * TO_OBJ_OFFSET +
        as_int(c);
    return *(collision_resolvers + index);
}    



