
#include <SDL2/SDL_image.h>

#include "model/game_obj.h"

// Used to set acceleration and speed
const float SCALE = 60.f;  // pixels 
//const float SCALE_SQUARED = 60.f;  // pixels 
 
GameObj::GameObj(float x, float y, SDL_Texture * texture, bool movable) {

    // init the position
    this->x = x;
    this->y = y;

    // is this a movable object?
    this->movable = movable;    

    // draw this
    this->texture = texture;

    // FIXME: this will get replaced with info from the sprite
	SDL_Rect dst;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    half_width = dst.w / 2.0;
    half_height = dst.h / 2.0;

    // we calculate the AABB bounding boxes now
    // movable objects get their bounding boxes calculated on the fly
    ax = x - half_width;
    ay = y - half_height;
    bx = x + half_width;
    by = y + half_height;

    // sanity checks
    assert(ax < bx);
    assert(ay < by);
    
    // initial velocity
    x_vel_per_sec = 0.0;
    y_vel_per_sec = 0.0;

    // clamp speed to these values.
    x_max_vel_per_sec = 5.0f * SCALE;
    y_max_vel_per_sec = 10.0f * SCALE;    
    
    // acceleration and deceleration
    x_acc_per_sec = 0.2f * SCALE;
    x_dec_per_sec = 0.3f * SCALE;

     // set jump and gravity forces
    y_jump_start_vel_per_sec = 8.f * SCALE;
    y_acc_per_sec = 0.5f * SCALE;
}

// bool GameObj::contains_point(float x, float y) {    
//     return (
//         (x > (this->x - half_width)) &&
//         (x < (this->x + half_width)) && 
//         (y < (this->y + half_height)) && 
//         (y < (this->y + half_height)));
// }


// bool GameObj::potentially_contains_point(float x, float y) {    
//     return ((x > ax) && (x < ay) && (y < ay) && (y < by));
// }

// debug .. dump the game objects position to std out
void GameObj::dump_position(std::string name) {
    std::cout << name 
              << " pos: (" << x << ", " << y << "), "
              << "delta pos: (" << dx << ", " << dy << "), "
              << "velocity: (" << x_vel_per_sec << ", " << y_vel_per_sec << "), "
              << "acc x: " << x_acc_per_sec << ", " 
              << "dec x: " << x_dec_per_sec << ", " 
              << "acc y: " << y_acc_per_sec << "." << std::endl 
              << "\taabb: (" << ax << "," << ay << "," << bx << "," << by << ")" 
              << std::endl;
}


bool GameObj::potentially_collides_with(GameObj * other_game_obj) {

    // If this objects top edge is below the others bottom edge,
    // then this object is totally below the other object.
    // (remember that y increases downwards in sdl).
    if (ay > other_game_obj->by) return false;
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (ax > other_game_obj->bx) return false;

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (by < other_game_obj->ay) return false;

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (bx < other_game_obj->ax) return false;

    // may be a collision!
    return true;
}

void GameObj::calc_projected_delta_position(float delta_time_in_secs) {

    // should only ever call this on movable objects
    assert(movable);

    // sanity checks
    assert(ax < bx);
    assert(ay < by);

    // apply the force of gravity
    y_vel_per_sec += y_acc_per_sec;
    if (y_vel_per_sec > y_max_vel_per_sec) y_vel_per_sec = y_max_vel_per_sec;
    if (y_vel_per_sec < -y_max_vel_per_sec) y_vel_per_sec = -y_max_vel_per_sec;

    // work out the project change in position
    dx = x_vel_per_sec * delta_time_in_secs;
    dy = y_vel_per_sec * delta_time_in_secs; 

    // reset the parametric t value for the projected move 
    // later on we try and find the largest t value without having a collision.
    t = 1.0f;

    // we need to recalculate this every time we move
    ax = x - half_width;
    ay = y - half_height;
    bx = x + half_width;
    by = y + half_height;

    // now work out the new aabb box including the displacement in position
    // (used by the bounding boxes optimization for collision detection)
    ax = fmin(ax, ax + dx);
    ay = fmin(ay, ay + dy);
    bx = fmax(bx, bx + dx);
    by = fmax(by, by + dy);

    // sanity checks
    assert(ax < bx);
    assert(ay < by);
}        

void GameObj::decelerate() {
    if (x_vel_per_sec < 0) x_vel_per_sec += x_dec_per_sec;      
    if (x_vel_per_sec > 0) x_vel_per_sec -= x_dec_per_sec;

    // Deceleration may produce a speed that is greater than zero but
    // smaller than the smallest unit of deceleration. These lines ensure
    // that the player does not keep travelling at slow speed forever after
    // decelerating.
    if (x_vel_per_sec > 0 && x_vel_per_sec < x_dec_per_sec) {
        x_vel_per_sec = 0.0f;
    }
    if (x_vel_per_sec < 0 && x_vel_per_sec > - x_dec_per_sec) {
        x_vel_per_sec = 0.0f;
    }
}

void GameObj::accelerate_left() { 
    x_vel_per_sec -= x_acc_per_sec;
    if (x_vel_per_sec < -x_max_vel_per_sec) x_vel_per_sec = -x_max_vel_per_sec;         
}

void GameObj::accelerate_right() {
    x_vel_per_sec += x_acc_per_sec;        
    if (x_vel_per_sec > x_max_vel_per_sec) x_vel_per_sec = x_max_vel_per_sec;        
}

void GameObj::jump() {
    // y velocity is clamped later (we have to apply gravity)
    y_vel_per_sec = -y_jump_start_vel_per_sec;
}

void GameObj::commit_change_in_position() {
    x += t * dx;
    y += t * dy;
}
