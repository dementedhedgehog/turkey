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
    
    GameObjType * parent;

    // static list of game object type names
    static std::vector<std::string> game_obj_type_names;
    std::vector<std::string> & get_names() const;

    /* static std::vector<GameObjType> game_obj_types; */
    /* static std::vector<GameObjType> movable_game_obj_types; */



 public:
    // constructor
    GameObjType(const std::string name, const bool moveable);
    
    // ids
    const int get_id() const { return id; };
    const int get_moveable_id() const { return moveable_id; };

    std::string get_name() const;
    const int as_int() const;

    inline bool is_moveable() { return moveable_id != -1; };

    // return the number of game objects
    static uint32_t get_num_game_obj_types();
    static uint32_t get_num_movable_game_obj_types();


    static int get_last_moveable_id();
    static int get_last_id();

    /* void to_stream(std::ostream &strm) const { */
    /*     strm << get_name(); */
    /* } */


    static void initialize();

    static GameObjType * get_type_for_id(const int id);
    static GameObjType * get_moveable_type_for_id(const int moveable_id);
};


inline std::ostream& operator<<(std::ostream &strm, const GameObjType & g) {
    strm << g.get_name();
    //g.to_stream(strm);
    return strm;
}

inline std::ostream& operator<<(std::ostream &strm, const GameObjType * g) {
    strm << g->get_name();
    //g->to_stream(strm);
    return strm;
}


extern GameObjType * BLOCK;
extern GameObjType * CHARACTER;
extern GameObjType * FIREBALL;

#endif
