
#include <iostream>

#include "game_obj_type.h"


GameObjType * BLOCK = new GameObjType("block", false);
GameObjType * CHARACTER = new GameObjType("character", true);
GameObjType * FIREBALL = new GameObjType("fireball", true);

// note we rely on these being contiguous
int last_id = 0;
int last_moveable_id = 0;

// return the number of game objects
uint32_t GameObjType::get_num_game_obj_types() {
    return last_id;
}

uint32_t GameObjType::get_num_movable_game_obj_types() {
    return last_moveable_id;
}

// avoids the static initialization fiasco
// http://www.parashift.com/c++-faq-lite/static-init-order.html
std::vector<std::string> & GameObjType::get_names() {
    static std::vector<std::string> names;
    return names;
}

GameObjType::GameObjType(const std::string name, const bool moveable) : 
    id(last_id++), moveable_id(moveable ? last_moveable_id++: -1)
{
    get_names().push_back(std::string(name));
}

const int GameObjType::as_int() {
    return id;
}

std::string GameObjType::get_name() {
    return get_names()[id];
}

