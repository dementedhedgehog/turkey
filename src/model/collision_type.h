#ifndef COLLISION_TYPE_H
#define COLLISION_TYPE_H

typedef unsigned int collision_type_t;

const collision_type_t NONE = 0;
const collision_type_t TOP = 1;
const collision_type_t LEFT = 2;
const collision_type_t RIGHT = 4;
const collision_type_t BOTTOM = 8;

#endif
