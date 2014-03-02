#ifndef COLLISION_RESOLUTION_MANAGER_H
#define COLLISION_RESOLUTION_MANAGER_H

#include <array>

#include "collision_type.h"
#include "game_obj_type.h"
#include "game_obj.h"


// add a game object
//bool add_collision_resolution(GameObjType got1, GameObjType got2, CollisionType c2);

// prototype for collision resolving functions.
typedef void (* collision_resolver) (
    GameObj * movable_game_obj, 
    GameObj * other_game_obj, 
    CollisionType collision);



class CollisionResolutionManager {
 private:
    // some dynamically allocated three dimensional array!
    static collision_resolver * collision_resolvers;

    static int FROM_OBJ_OFFSET;
    static int TO_OBJ_OFFSET;

 public:

    static void init(); 
    static void clean_up();
    static void add(GameObjType * g1, GameObjType * g2, 
        CollisionType c, collision_resolver resolver);
    static collision_resolver get(GameObjType * g1, GameObjType * g2, CollisionType c);
};

#endif
