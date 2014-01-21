
#include "cell.h"

Cell::Cell(const cell_type_t cell_type) {
    type = cell_type;
}
    


const cell_type_t Cell::get_cell_type() {
    return type;
}
