
#include <iostream>
#include <cassert>

#include "game_obj_type.h"

// note we rely on these being contiguous
int last_id = 0;
int last_moveable_id = 0;

std::vector<GameObjType*> game_obj_types;
std::vector<GameObjType*> moveable_game_obj_types;

// GameObjType * BLOCK = new GameObjType("block", false);
// GameObjType * CHARACTER = new GameObjType("character", true);
// GameObjType * FIREBALL = new GameObjType("fireball", true);

GameObjType * BLOCK;
GameObjType * CHARACTER;
GameObjType * FIREBALL;


void GameObjType::initialize() {
    BLOCK = new GameObjType("block", false);
    CHARACTER = new GameObjType("character", true);
    FIREBALL = new GameObjType("fireball", true);
}


// return the number of game objects
uint32_t GameObjType::get_num_game_obj_types() {
    return last_id;
}

uint32_t GameObjType::get_num_movable_game_obj_types() {
    return last_moveable_id;
}

// avoids the static initialization fiasco
// http://www.parashift.com/c++-faq-lite/static-init-order.html
std::vector<std::string> & GameObjType::get_names() const {
    static std::vector<std::string> names;
    return names;
}

GameObjType::GameObjType(const std::string name, const bool moveable) : 
    id(last_id++), moveable_id(moveable ? last_moveable_id++: -1)
{
    get_names().push_back(std::string(name));
    game_obj_types.push_back(this);
    if (moveable) { 
        moveable_game_obj_types.push_back(this);
    }
}

const int GameObjType::as_int() const {
    return id;
}

std::string GameObjType::get_name() const {
    return get_names()[id];
}

int GameObjType::get_last_moveable_id() {
    return last_moveable_id;
}

int GameObjType::get_last_id() {
    return last_id;
}


GameObjType * GameObjType::get_type_for_id(const int id) {
    //return & game_obj_types.get(id);
    return game_obj_types[id];
}

GameObjType * GameObjType::get_moveable_type_for_id(const int moveable_id) {
    // check that initialize has been called
    assert(moveable_game_obj_types.size() > 0);
    return moveable_game_obj_types[moveable_id];


}

