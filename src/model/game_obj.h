#ifndef GAME_OBJ_H
#define GAME_OBJ_H

/*
 * Class that represents all collidable objects 
 * this includes the main character, monsters, some pieces of terrain, and items.
 *
 */
class GameObj {
 public:
    // pos center
    int x;
    int y;

    int x_vel;
    int y_vel;

    // bounding box radius 
    // (i.e. for the moment they're squares with side length 2r centered at x,y)
    
    GameObj(int x, int y);
};

#endif
