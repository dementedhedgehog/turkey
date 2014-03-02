#ifndef GAME_OBJ_TYPE_H
#define GAME_OBJ_TYPE_H

#include <cstdint>
#include <string>
#include <vector>

class GameObjType {
 private:
    const int id;
    const int moveable_id;
    //const bool moveable; // FIXME: redendant equivalent to moveable_id == -1    
    

    static std::vector<std::string> game_obj_type_names;

    GameObjType * parent;

    // static list of game object type names
    std::vector<std::string> & get_names();

 public:
    
    const int get_id() { return id; };
    const int get_moveable_id() { return moveable_id; };

    std::string get_name();
    GameObjType(const std::string name, const bool moveable);
    const int as_int();

    inline bool is_moveable() { return moveable_id != -1; };

    // return the number of game objects
    static uint32_t get_num_game_obj_types();
    static uint32_t get_num_movable_game_obj_types();
};


extern GameObjType * BLOCK;
extern GameObjType * CHARACTER;
extern GameObjType * FIREBALL;

#endif
