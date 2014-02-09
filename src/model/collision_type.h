#ifndef COLLISION_TYPE_H
#define COLLISION_TYPE_H

#include <iostream>

/**
 * Collision Types are a bit mask (an object can be hit from multiple sides at once 
 * (e.g. LEFT and TOP).
 *
 */
enum class CollisionType : unsigned int {
    NONE = 0, TOP = 1, RIGHT = 2, BOTTOM = 4, LEFT = 8, POTENTIAL = 16

};

/* static const char * names[] = { "top", "right", "bottom" }; */


//inline constexpr CollisionType operator std::string (CollisionType c) const {
/* inline constexpr operator std::string (CollisionType c)  { */
/*     return names[static_cast<unsigned int>(this)]; */
/* } */


inline constexpr CollisionType operator|(CollisionType c, CollisionType c2) {
    return static_cast<CollisionType>(
        static_cast<unsigned int>(c) | static_cast<unsigned int>(c2));
}

inline constexpr CollisionType operator&(CollisionType c, CollisionType c2) {
    return static_cast<CollisionType>(
        static_cast<unsigned int>(c) & static_cast<unsigned int>(c2));
}


inline std::ostream& operator<<(std::ostream &strm, const CollisionType &c) {

    if (c == CollisionType::NONE) {
        strm << "None";
    }
    else {

        if ((c & CollisionType::TOP) != CollisionType::NONE) {
            strm << "Top";
        }
        else if ((c & CollisionType::BOTTOM) != CollisionType::NONE) {
            strm << "Bottom";
        }

        if ((c & CollisionType::LEFT) != CollisionType::NONE) {
            strm << "Left";
        }
        else if ((c & CollisionType::RIGHT) != CollisionType::NONE) {
            strm << "Right";
        }
        else if ((c & CollisionType::POTENTIAL) != CollisionType::NONE) {
            strm << "Potential";
        }
    }

    return strm;
}


#endif
