
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
    this->jumping = false;

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
              << "potential_collider?: " << (bool)potential_collider << ", "
              << "projected pos: (" << px << ", " << py << "), "
              << "velocity: (" << x_vel_per_sec << ", " << y_vel_per_sec << "), "
              << "acc x: " << x_acc_per_sec << ", " 
              << "dec x: " << x_dec_per_sec << ", " 
              << "acc y: " << y_acc_per_sec << "." << std::endl 
              << "\taabb: (" << ax << "," << ay << "," << bx << "," << by << ")" 
              << std::endl;
}

// returns true if the aabb boxes intersect
bool GameObj::potentially_collides_with(GameObj * other_game_obj) {
    // If this objects top edge is below the others bottom edge,
    // then this object is totally below the other object.
    // (remember that y increases downwards in sdl).
    if (ay > other_game_obj->by) {
        //std::cout << " below " << std::endl;
        return false;
    }
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (ax > other_game_obj->bx) return false;

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (by < other_game_obj->ay) {
        //std::cout << " above " << std::endl;
        return false;
    }

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (bx < other_game_obj->ax) return false;

    // assume all objects aren't going to be 
    potential_collider = true;
    other_game_obj->potential_collider = true;

    // may be a collision!
    return true;
}


//
// Check whether the projected position of this object will collide with the projected 
// position of another movable object.  
//
collision_type_t GameObj::check_for_projected_movable_collision(GameObj * other_game_obj) {
    assert(other_game_obj->movable);

    // std::cout << " *** check collides with " << std::endl;

    // If this objects top edge is below the others bottom edge,
    // then this object is totally below the other object.
    // (remember that y increases downwards in sdl).
    if (py - half_height > other_game_obj->py + other_game_obj->half_height) {
        //std::cout << " below " << std::endl;
        return NONE;
    }
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (px - half_width > other_game_obj->px + other_game_obj->half_width) {
        //std::cout << " right " << std::endl;
        return NONE;
    }

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (py + half_height < other_game_obj->py - other_game_obj->half_height) {
        //std::cout << " above " << std::endl;
        return NONE;
    }


    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (px + half_width < other_game_obj->px - other_game_obj->half_width) {
        //std::cout << " left " << std::endl;
        return NONE;
    }

    // may be a collision!
    //std::cout << " collision " << std::endl;

    return BOTTOM;  // FIXME: this is bull shit .. I just want to see it run.
}


//
// Check whether the projected position of this object will collide with the fixed position 
// of a fixed object.
//
collision_type_t GameObj::check_for_projected_fixed_collision(GameObj * other_game_obj) {
    // std::cout << " *** check collides fixed with " << std::endl;
    // dump_position("a");
    // other_game_obj->dump_position("b");
    // std::cout << std::endl;
    // std::cout << std::endl;

    // If this objects top edge is below the others bottom edge,
    // then this object is totally below the other object.
    // (remember that y increases downwards in sdl).
    if (py - half_height > other_game_obj->y + other_game_obj->half_height) {
        //std::cout << " below " << std::endl;
        return NONE;
    }
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (px - half_width > other_game_obj->x + other_game_obj->half_width) {
        //std::cout << " right " << std::endl;
        return NONE;
    }

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (py + half_height < other_game_obj->y - other_game_obj->half_height) {
        //std::cout << " above " << std::endl;
        return NONE;
    }

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (px + half_width < other_game_obj->x - other_game_obj->half_width) {
        //std::cout << " left " << std::endl;
        return NONE;
    }

    // may be a collision!
    //std::cout << " collision " << std::endl;
    return BOTTOM;  // FIXME: this is bull shit .. I just want to see it run.
}

// 
// Before we think about collisions we do a rough projection to see what pairs of 
// objects we have to be careful about when it comes to detecting collisions.
//
float GameObj::calc_initial_projected_move(float delta_time_in_secs) {

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
    float dx = x_vel_per_sec * delta_time_in_secs;
    float dy = y_vel_per_sec * delta_time_in_secs; 

    // calculate the move distanc
    float move_distance = sqrt(dx*dx + dy*dy);

    // assume all objects aren't going to be 
    potential_collider = false;

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

    return move_distance;        
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



// calculate where the object will be after the next time stp assuming 
// no collisions happen
void GameObj::calc_projected_delta_position(const float step_t) {
    px = x + step_t * x_vel_per_sec;
    py = y + step_t * y_vel_per_sec;
}


// move the object along the movement vector using a parametric equation
void GameObj::move(const float step_t) {
    x += step_t * x_vel_per_sec;
    y += step_t * y_vel_per_sec;
}

