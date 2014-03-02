
#include <vector>

#include "collision_resolution_manager.h"

int CollisionResolutionManager::FROM_OBJ_OFFSET = 
    GameObjType::get_num_game_obj_types() * N_COLLISION_TYPES;
int CollisionResolutionManager::TO_OBJ_OFFSET = N_COLLISION_TYPES;

collision_resolver * CollisionResolutionManager::collision_resolvers = nullptr;


void CollisionResolutionManager::init() {

    // there's a collision resolver for each (game obj type, game obj type, collision type)

    // FIXME: work out a way to seperate movable and fixed game obj ids?
    uint32_t n_resolvers = 
        GameObjType::get_num_game_obj_types() *      
        GameObjType::get_num_game_obj_types() * 
        N_COLLISION_TYPES;

        
    // CollisionResolutionManager::FROM_OBJ_OFFSET = 
    //     GameObjType::get_num_game_obj_types() * N_COLLISION_TYPES;
    // CollisionResolutionManager::TO_OBJ_OFFSET = N_COLLISION_TYPES;

    CollisionResolutionManager::collision_resolvers = 
        (collision_resolver*) malloc(n_resolvers * sizeof(collision_resolver));
}


void CollisionResolutionManager::clean_up() {
    free(CollisionResolutionManager::collision_resolvers);
    CollisionResolutionManager::collision_resolvers = nullptr;
}


void CollisionResolutionManager::add(
    GameObjType * g1, GameObjType * g2, CollisionType c, collision_resolver resolver)
{    
    int index = 
        g1->get_moveable_id() * CollisionResolutionManager::FROM_OBJ_OFFSET + 
        g2->get_id() * CollisionResolutionManager::TO_OBJ_OFFSET +
        as_int(c);
            
    *(collision_resolvers + index) = resolver;
}


collision_resolver CollisionResolutionManager::get(
    GameObjType * g1, GameObjType * g2, CollisionType c)
{    
    int index = 
        g1->get_moveable_id() * CollisionResolutionManager::FROM_OBJ_OFFSET + 
        g2->get_id() * CollisionResolutionManager::TO_OBJ_OFFSET +
        as_int(c);
    return *(collision_resolvers + index);
}    



