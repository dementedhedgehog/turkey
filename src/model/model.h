#ifndef MODEL_H
#define MODEL_H

#include <map>

#include "cell.h"
#include "position.h"

typedef std::map<Position, Cell * > CellMap;

/**
 * Model for MVC
 *
 */
class Model : private CellMap {
 private:
    CellMap map;

 public: 
    Model();    
};


#endif
