
#include "model.h"

Model::Model() : CellMap() {
    map[Position(10, 10)] = new Cell(STONE);
}



// int const * const * Model::get_map() {
//     return map;
// }
    
