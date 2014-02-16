
#include <SDL2/SDL_image.h>

#include "constants.h"
#include "model/game_obj.h"

//
// c++ requires us to initalize static variables outside classes
//

// default gravity
float GameObj::default_gravity = 0.5f;

// maximum speeds
float GameObj::default_x_max_velocity = 5.0f;
float GameObj::default_y_max_velocity = 10.0f;

// side ways movement
float GameObj::default_x_acceleration = 0.55f;
// side ways deceleration
float GameObj::default_x_deceleration = 0.8f;

// set jump and gravity forces
float GameObj::default_jump_velocity = 8.f;


 
GameObj::GameObj(float x, float y, SDL_Texture * texture, bool movable) {

    // init the position
    this->x = x;
    this->y = y;

    // is this a movable object?
    this->movable = movable;    
    this->jumping = false;

    // set this to true to clean this up
    // we use this in a mark and sweep garbage collection
    this->dead = false;

    // should we decelerate the game object?
    this->do_decelerate = false;

    // a time to live value
    this->ttl_in_secs = -1.0;

    // draw this
    this->texture = texture;

    // FIXME: this will get replaced with info from the sprite
	SDL_Rect dst;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    half_width = dst.w / 2.0;
    half_height = dst.h / 2.0;

    // we calculate the AABB bounding boxes now
    // movable objects get their bounding boxes calculated on the fly
    // this is really just for the fixed game objects
    ax = x - half_width;
    ay = y - half_height;
    bx = x + half_width;
    by = y + half_height;

    // do this so we have these values for fixed game objects as well as movable game objects
    // (makes things simpler for the collision detection stuff).
    pax = ax;
    pbx = bx;
    pay = ay;
    pby = by;

    // sanity checks
    assert(ax < bx);
    assert(ay < by);
    
    // initial velocity
    x_vel_per_sec = 0.0;
    y_vel_per_sec = 0.0;

    // clamp speed to these values.
    x_max_vel_per_sec = GameObj::default_x_max_velocity * MOVEMENT_SCALE;
    y_max_vel_per_sec = GameObj::default_y_max_velocity * MOVEMENT_SCALE;    
    
    // acceleration and deceleration
    x_acc_per_sec = GameObj::default_x_acceleration * MOVEMENT_SCALE;
    x_dec_per_sec = GameObj::default_x_deceleration * MOVEMENT_SCALE;

     // set jump and gravity forces
    y_jump_start_vel_per_sec = GameObj::default_jump_velocity * MOVEMENT_SCALE;
    y_acc_per_sec = GameObj::default_gravity * MOVEMENT_SCALE;

    // {
    //     { 5,  0  }, { 15, 0  }, // Top of head
    //     { 5,  40 }, { 15, 40 }, // Feet
    //     { 0,  10 }, { 0,  30 }, // Left arm
    //     { 20, 10 }, { 20, 30 }  // Right arm
    // };
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
    if (ay > other_game_obj->by) { return false; }
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (ax > other_game_obj->bx) return false;

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (by < other_game_obj->ay) { return false; }

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
CollisionType GameObj::check_for_projected_movable_collision(GameObj * other_game_obj) {
    assert(other_game_obj->movable);

    // If this objects top edge is below the others bottom edge, then this object is totally 
    // below the other object (remember that y increases downwards in sdl).
    if (pay > other_game_obj->pby) { return CollisionType::NONE; }
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (pax > other_game_obj->pbx) { return CollisionType::NONE; }

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (pby < other_game_obj->pay) { return CollisionType::NONE; }

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (pbx < other_game_obj->pax) { return CollisionType::NONE; }

    // may be a collision!
    return CollisionType::POTENTIAL;  
}


//
// Check whether the projected position of this object will collide with the fixed position 
// of a fixed object.
//
CollisionType GameObj::check_for_projected_fixed_collision(GameObj * other_game_obj) {
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (pax > other_game_obj->pbx) { return CollisionType::NONE; }

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (pbx < other_game_obj->pax) { return CollisionType::NONE; }

    // If this objects top edge is below the others bottom edge, then this object is 
    // totally below the other object (remember that y increases downwards in sdl).
    if (pay > other_game_obj->pby) { return CollisionType::NONE; }

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (pby < other_game_obj->pay) { return CollisionType::NONE; }
    
    // we know we've had a collision work out the relative velocity between 
    // this game object and the other game object
    // float vx = x_vel_per_sec - other_game_obj->x_vel_per_sec;
    // float vy = y_vel_per_sec + other_game_obj->y_vel_per_sec;

    // std::cout << vx << ", " << vy << std::endl;

    // check where we'd get the collision
    CollisionType result = CollisionType::NONE;
    if (x_vel_per_sec > 0) {
        // moving left
        if (pbx > other_game_obj->ax) {
            result = result | CollisionType::LEFT;
        }
    }
    else {            
        // moving right
        if (pax < other_game_obj->bx) {
            result = result | CollisionType::RIGHT;
            //std::cout << " collision " << result << std::endl;
        }
    }

    if (y_vel_per_sec > 0) {
        // moving down
        if (pby > other_game_obj->ay) {
            result = result | CollisionType::BOTTOM;
        }
    }
    else {            
        // moving up
        if (pay < other_game_obj->by) {
            result = result | CollisionType::TOP;
        }
    }


    // if (px < x) {
    //     result = result | CollisionType::LEFT;
    // } 
    // else {
    //     result = result | CollisionType::RIGHT;
    // }

    // if (py < y) {
    //     result = result | CollisionType::TOP;
    // }
    // else {
    //     result = result | CollisionType::BOTTOM;
    // }

    // may be a collision!
    return result;
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



void GameObj::decelerate_character() {
    if (do_decelerate) {
        if (x_vel_per_sec < 0) x_vel_per_sec += x_dec_per_sec;
        if (x_vel_per_sec > 0) x_vel_per_sec -= x_dec_per_sec;

        // Deceleration may produce a speed that is greater than zero but
        // smaller than the smallest unit of deceleration. These lines ensure
        // that the player does not keep travelling at slow speed forever after
        // decelerating.
        if (x_vel_per_sec > 0 && x_vel_per_sec < x_dec_per_sec) {
            x_vel_per_sec = 0.0f;

            // don't slow down sideways movement till we start moving again
            do_decelerate = false;    
        }
        if (x_vel_per_sec < 0 && x_vel_per_sec > - x_dec_per_sec) {
            x_vel_per_sec = 0.0f;

            // don't slow down sideways movement till we start moving again
            do_decelerate = false;    
        }
    }
}

void GameObj::reset_decelerate_flag() { 
    // slow down sideways movement for the character
    do_decelerate = true;    
}


void GameObj::accelerate_left() { 
    x_vel_per_sec -= x_acc_per_sec;
    if (x_vel_per_sec < -x_max_vel_per_sec) x_vel_per_sec = -x_max_vel_per_sec;         
    do_decelerate = false;    
}

void GameObj::accelerate_right() {
    x_vel_per_sec += x_acc_per_sec;        
    if (x_vel_per_sec > x_max_vel_per_sec) x_vel_per_sec = x_max_vel_per_sec;        
    do_decelerate = false;    
}

void GameObj::move_up() { 
    y_vel_per_sec -= 40.0f;
}

void GameObj::move_down() { 
    y_vel_per_sec += 40.0f;
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

    pax = px - half_width;
    pbx = px + half_width;
    pay = py - half_height;
    pby = py + half_height;
}


// move the object along the movement vector using a parametric equation
void GameObj::move(const float step_t) {
    x += step_t * x_vel_per_sec;
    y += step_t * y_vel_per_sec;
}


// set the velocity of the game object (use these when colliding).
void GameObj::collision_set_x_velocity(float x_vel_per_sec) {
    assert(this->x_vel_per_sec >= x_vel_per_sec);
    
    //if (abs(this->x_vel_per_sec) > ZERO) {
    //py *= x_vel_per_sec / this->x_vel_per_sec;    
    this->x_vel_per_sec = x_vel_per_sec;    
}

void GameObj::collision_set_y_velocity(float y_vel_per_sec) {
    assert(this->y_vel_per_sec >= y_vel_per_sec);

    //if (abs(this->y_vel_per_sec) > ZERO) {
    //py *= y_vel_per_sec / this->y_vel_per_sec;    
    this->y_vel_per_sec = y_vel_per_sec;    
}


// void GameObj::set_default_gravity(const float gravity) {
//     GameObj::default_gravity = gravity;
// }
