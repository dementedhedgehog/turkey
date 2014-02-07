#ifndef I_GAME_OBJ_MANAGER_H
#define I_GAME_OBJ_MANAGER_H

#include "model/game_obj.h"

class IGameObjManager {    
 public:
    virtual void add_game_obj(GameObj * game_obj) = 0;    
};

#endif 
