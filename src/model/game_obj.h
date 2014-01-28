#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <SDL2/SDL.h>

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

    int half_height;
    int half_width;

    int x_vel;
    int y_vel;

    // FIXME: place holder for a sprite..
    SDL_Texture * texture; 

    // bounding box radius 
    // (i.e. for the moment they're squares with side length 2r centered at x,y)
    // FIXME: replace with sprite instance    
    GameObj(int x, int y, SDL_Texture * texture = nullptr); 
    //GameObj(int x, int y);
};

#endif
