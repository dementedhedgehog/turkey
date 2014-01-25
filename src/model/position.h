#ifndef POSITION_H
#define POSITION_H

//#include "cell.h"


/**
 * Position for MVC
 *
 */
class Position {    
    int x;
    int y;

 public:
    Position(const int x, const int y);

    // required for mapping
    inline bool operator< (const Position other) const { 
        return (x < other.x) ? true : (y < other.y);
    }
};


#endif
