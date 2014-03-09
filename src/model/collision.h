#ifndef COLLISION_H
#define COLLISION_H

#include "model/game_obj.h"




class Collision {
    //private:
 public: // DEBUGGING

    GameObj * a;
    GameObj * b;

    CollisionType collision; 

    // FIXME: later on we'll have state here..
    // some collisions just go away after they happen (e.g. being hit with a bullet)
    // so we won't need to spend too much time resolving them.

 public:
    Collision(GameObj * a, GameObj * b);

    // check for a collision between the projected position of a and the projected 
    // position of b (when both a and b are movable).
    bool check_for_projected_movable_collision();

    // check for a collision between the projected position of a and the position of b 
    // (when only a is movable).
    //bool check_for_projected_fixed_collision();
    //CollisionType check_for_projected_fixed_collision();
    bool has_projected_fixed_collision();

    // resolve a collision by changing the state of the game objects involved in the 
    // collision
    void resolve();

    // 
    void to_stream(std::ostream &strm) const {
        strm << "Collision: " << collision << ": " << a << " --> " << b;
    }

};


inline std::ostream& operator<<(std::ostream &strm, const Collision & c) {
    c.to_stream(strm);
    return strm;
}

inline std::ostream& operator<<(std::ostream &strm, const Collision * c) {
    c->to_stream(strm);
    return strm;
}

#endif
