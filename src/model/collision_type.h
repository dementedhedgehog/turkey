#ifndef COLLISION_TYPE_H
#define COLLISION_TYPE_H

#include <iostream>

/**
 * Collision Types are a bit mask (an object can be hit from multiple sides at once 
 * (e.g. LEFT and TOP).
 *
 */
enum class CollisionType : unsigned int {
   // collisions that can be handled immediately
   TOP = 0, 
   RIGHT = 1, 
   BOTTOM = 2, 
   LEFT = 3, 

   // deferred collisions
   TOP_LEFT = 4,
   TOP_RIGHT = 5,
   BOTTOM_LEFT = 6,
   BOTTOM_RIGHT = 7,

   // whack collisions that don't need resolvers at the end.
      
   // potential collisions (broad phase)
   POTENTIAL = 8, // initial state

   NONE = 9 // definitely no collision
   //UNINITIALIZED = 10
};


CollisionType const ValidCollisionTypes[] = { 
    CollisionType::TOP, CollisionType::RIGHT, CollisionType::BOTTOM, CollisionType::LEFT, 
    CollisionType::TOP_LEFT, CollisionType::TOP_RIGHT, 
    CollisionType::BOTTOM_LEFT, CollisionType::BOTTOM_RIGHT };

// Note: we don't include potential and none in this count.
const int N_VALID_COLLISION_TYPES = 8;

inline const uint32_t as_int(CollisionType c) {
    return static_cast<uint32_t>(c);
}

inline const bool is_deferred(CollisionType c) {
    return (
        c == CollisionType::TOP_LEFT || 
        c == CollisionType::TOP_RIGHT ||
        c == CollisionType::BOTTOM_LEFT ||
        c == CollisionType::BOTTOM_RIGHT);
}


inline constexpr bool operator==(CollisionType c, CollisionType c2) {
    return static_cast<unsigned int>(c) == static_cast<unsigned int>(c2);
}


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
        strm << "No Collision";
    }
    else if (c == CollisionType::TOP) { 
        strm << "Top";
    }
    else if (c == CollisionType::BOTTOM) {
        strm << "Bottom";
    }
    else if (c == CollisionType::TOP_LEFT) {
        strm << "Top Left";
    }
    else if (c == CollisionType::TOP_RIGHT) {
        strm << "Top Right";
    }
    else if (c == CollisionType::BOTTOM_LEFT) {
        strm << "Bottom Left";
    }
    else if (c == CollisionType::BOTTOM_RIGHT) {
        strm << "Bottom Right";
    }
    else if (c == CollisionType::LEFT) {
        strm << "Left";
    }
    else if (c == CollisionType::RIGHT) {
        strm << "Right";
    }
    else if (c == CollisionType::POTENTIAL) {
        strm << "Potential";
    }
    /* else if (c == CollisionType::UNINITIALIZED) { */
    /*     strm << "Uninitialized"; */
    /* } */

    return strm;
}


#endif
