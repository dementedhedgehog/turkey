

#include "model/game_obj.h"


// GameObj::GameObj(int x, int y) {
//     this->x = x;
//     this->y = y;

//     x_vel = 0;
//     y_vel = 0;
// }

GameObj::GameObj(int x, int y, SDL_Texture * texture) {
    this->x = x;
    this->y = y;
    this->texture = texture;

    x_vel = 0;
    y_vel = 0;
    
}
