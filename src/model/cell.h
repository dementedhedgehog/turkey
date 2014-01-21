#ifndef CELL_H
#define CELL_H

enum cell_type_t {
    EMPTY, 
    STONE    
}; 


/*
 * One of the cells in the 2D side scroller "grid".
 *
 */
class Cell {
 private:
    cell_type_t type; 

 public: 
    Cell(const cell_type_t cell_type);

    const cell_type_t get_cell_type();
};


#endif
