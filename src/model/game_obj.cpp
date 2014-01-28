
#include <SDL2/SDL_image.h>

#include "model/game_obj.h"


GameObj::GameObj(int x, int y, SDL_Texture * texture) {
    this->x = x;
    this->y = y;
    this->texture = texture;

    // FIXME: this will get replaced with info from the sprite
	SDL_Rect dst;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    half_width = dst.w / 2;
    half_height = dst.h / 2;

    x_vel = 0;
    y_vel = 0;
    
}
