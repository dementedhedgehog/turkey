
#include <cmath>

#include <SDL2/SDL_image.h>


#include "constants.h"
#include "model/game_obj.h"
#include "shared/dump_trace.h"

const float EPSILON = 0.01f;
//
// c++ requires us to initalize static variables outside classes
//

// default gravity
float GameObj::default_gravity = 0.5f * MOVEMENT_SCALE;

// maximum speeds
float GameObj::default_x_max_velocity = 5.0f * MOVEMENT_SCALE;
float GameObj::default_y_max_velocity = 10.0f * MOVEMENT_SCALE;

// side ways movement
float GameObj::default_x_acceleration = 0.55f * MOVEMENT_SCALE;
// side ways deceleration
float GameObj::default_x_deceleration = 0.8f * MOVEMENT_SCALE;

// set jump and gravity forces
float GameObj::default_jump_velocity = 8.f * MOVEMENT_SCALE;

// give each game object a unique id.
int GameObj::last_id = 0;

 
GameObj::GameObj(float x, float y, GameObjType * type, SDL_Texture * texture) {

    // unique id
    this->id = last_id++;

    // init the position
    this->x = x;
    this->y = y;

    // is this a movable object?
    //this->movable = movable;    
    this->jumping = false;

    // set this to true to clean this up
    // we use this in a mark and sweep garbage collection
    this->dead = false;

    // should we decelerate the game object?
    this->do_decelerate = false;

    // a time to live value
    this->ttl = -1.0;

    // draw this
    this->texture = texture;

    // save the game objects type
    this->type = type;

    // FIXME: this will get replaced with info from the sprite
	SDL_Rect dst;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    half_width = dst.w / 2.0;
    half_height = dst.h / 2.0;

    // initial velocity
    x_velocity = 0.0;
    y_velocity = 0.0;

    // clamp speed to these values.
    x_max_velocity = GameObj::default_x_max_velocity;
    y_max_velocity = GameObj::default_y_max_velocity;    
    
    // acceleration and deceleration
    x_acc_per_sec = GameObj::default_x_acceleration;
    x_dec_per_sec = GameObj::default_x_deceleration;

     // set jump and gravity forces
    y_jump_start_velocity = GameObj::default_jump_velocity;
    y_acc_per_sec = GameObj::default_gravity;

    // sets the aabb boxes
    move(0.0f);

    // do this so we have these values for fixed game objects as well as movable game objects
    // (makes things simpler for the collision detection stuff).
    pax = ax;
    pbx = bx;
    pay = ay;
    pby = by;
}

// debug .. dump the game objects position to std out
void GameObj::dump_position(std::string name) {
    std::cout << name 
              << " pos: (" << x << ", " << y << "), "
              << "potential_collider?: " << (bool)potential_collider << ", "
              << "projected pos: (" << px << ", " << py << "), "
              << "velocity: (" << x_velocity << ", " << y_velocity << "), "
              << "acc x: " << x_acc_per_sec << ", " 
              << "dec x: " << x_dec_per_sec << ", " 
              << "acc y: " << y_acc_per_sec << "." << std::endl 
              << "\taabb: (" << ax << "," << ay << "," << bx << "," << by << ")" 
              << std::endl;
}

// returns true if the aabb boxes intersect
bool GameObj::potentially_collides_with(GameObj * other) {
    // If this objects top edge is below the others bottom edge,
    // then this object is totally below the other object.
    // (remember that y increases downwards in sdl).
    if (pay > other->pby) { return false; }
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (pax > other->pbx) return false;

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (pby < other->pay) { return false; }

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (pbx < other->pax) return false;

    // may be a collision!
    return true;
}


//
// Check whether the projected position of this object will collide with the projected 
// position of another movable object.  
//
CollisionType GameObj::check_for_projected_movable_collision(GameObj * other) {
    assert(false);

    // assert(other->is_moveable());

    // // If this objects top edge is below the others bottom edge, then this object is totally 
    // // below the other object (remember that y increases downwards in sdl).
    // if (pay > other->pby) { return CollisionType::NONE; }
        
    // // If this objects left edge is to the right of the others right edge,
    // // then this object is totally to the right of the other object.
    // if (pax > other->pbx) { return CollisionType::NONE; }

    // // If this objects bottom edge is above the others top edge,
    // // then this object is totally above the other object.
    // if (pby < other->pay) { return CollisionType::NONE; }

    // // If this objects right edge is to the left of the others left edge,
    // // then this object is totally to the left of the other object.
    // if (pbx < other->pax) { return CollisionType::NONE; }

    // may be a collision!
    return CollisionType::POTENTIAL;  
}


//
// Check whether the projected position of this object will collide with the fixed position 
// of a fixed object.
//
CollisionType GameObj::check_for_projected_fixed_collision(GameObj * other) {
        
    // If this objects left edge is to the right of the others right edge,
    // then this object is totally to the right of the other object.
    if (pax > other->pbx) { 
        // std::cout << "1"  << std::endl;    
        return CollisionType::NONE; }

    // If this objects right edge is to the left of the others left edge,
    // then this object is totally to the left of the other object.
    if (pbx < other->pax) {
        // std::cout << "2"  << std::endl;
        return CollisionType::NONE; }

    // If this objects top edge is below the others bottom edge, then this object is 
    // totally below the other object (remember that y increases downwards in sdl).
    if (pay > other->pby) {
        // std::cout << "3"  << std::endl;
        return CollisionType::NONE; }

    // If this objects bottom edge is above the others top edge,
    // then this object is totally above the other object.
    if (pby < other->pay) { 
        // std::cout << "4"  << std::endl;
        return CollisionType::NONE; }
    
    // the type of collision depends only on this objects movement and the relative positions 
    // and sizes of the objects
    if (x_velocity > 0) {
        if (y_velocity > 0) {  // moving right and down

            // moving down collision is either BOTTOM, RIGHT or BOTTOM_RIGHT
            if (bx < other->ax) {      // this is left of other
                if (by < other->ay) {  // this is above other
                    //std::cout << "moving right and down BR" << std::endl;
            
                    return CollisionType::BOTTOM_RIGHT; 
                }
                else {                          // this is not above other
                    if (ay < other->by) {  // this is not below other
                        //std::cout << "moving right and down R" << std::endl;
                        return CollisionType::RIGHT; 
                    }
                }
            }
            else {                              // this is right of other
                if (by < other->ay) {  // this is above other
                    //std::cout << "moving right and down B" << std::endl;
                    return CollisionType::BOTTOM;                     
                }
            }
        }
        else { // moving right and up
            //std::cout << "moving right and up, " << y_velocity << std::endl;

            // moving right and up collision is either TOP, RIGHT or TOP_RIGHT
            if (bx < other->ax) {      // this is left of other 
                if (ay > other->by) {  // this is below other
                    std::cout << "moving right and up TR" << std::endl;
                    return CollisionType::TOP_RIGHT; 
                }
                else {                          // this is not below other
                    if (by > other->ay) {  // this is not above other
                        //std::cout << "moving right and up R" << std::endl;
                        return CollisionType::RIGHT; 
                    }
                }
            }
            else {                              // this is right of other
                if (ay > other->by) {  // this is below other
                    //std::cout << "moving right and up TOP" << std::endl;
                    return CollisionType::TOP; 
                }
            }
        }
    }
    else { // moving left

        if (y_velocity > 0.0) { // moving left and down
            //std::cout << "moving left and down" << std::endl;

            // moving left and down collision is either BOTTOM, LEFT or BOTTOM_LEFT
            if (ax > other->bx) {       // this is right of other 
                if (by < other->ay) {   // this is above other
                    //std::cout << "moving left and down BL" << std::endl;
                    return CollisionType::BOTTOM_LEFT; 
                }
                else {                           // this is not above other
                    if (ay < other->by) {  // this is not below other
                        //std::cout << "moving left and down L" << std::endl;
                        return CollisionType::LEFT; 
                    }                    
                }
            }
            else {
                if (by < other->ay) {   // this is above other
                    //std::cout << "moving left and down B" << std::endl;
                    return CollisionType::BOTTOM;                     
                }
            }
        }
        else { 
            //std::cout << "moving left and up" << std::endl;

            // moving left and up collision is either TOP, LEFT or TOP_LEFT
            if (ax > other->bx) {       // this is right of other
                if (ay > other->by) {   // this is below other
                    //std::cout << "moving left and up TL" << std::endl;
                    return CollisionType::TOP_LEFT; 
                }
                else {                              // this is not below other
                    if (by > other->ay) {  // this is not above other
                        //std::cout << "moving left and up TL" << std::endl;
                        return CollisionType::LEFT;  
                    }
                }
            }
            else {
                if (ay > other->by) {   // this is below other 
                    //std::cout << "moving left and up T" << std::endl;
                    return CollisionType::TOP; 
                }
            }        
        }
    }
    return CollisionType::NONE; 
}

// 
// Before we think about collisions we do a rough projection to see what pairs of 
// objects we have to be careful about when it comes to detecting collisions.
// 
//   delta_time is in seconds
//
float GameObj::calc_initial_projected_move(float delta_time) {

    // should only ever call this on movable objects
    assert(is_moveable());

    // apply the force of gravity
    y_velocity += y_acc_per_sec;
    if (y_velocity > y_max_velocity) y_velocity = y_max_velocity;
    if (y_velocity < -y_max_velocity) y_velocity = -y_max_velocity;

    // work out the project change in position
    float dx = x_velocity * delta_time;
    float dy = y_velocity * delta_time; 

    // calculate the move distance
    float move_distance = sqrt(dx*dx + dy*dy);

    // now work out the new aabb box including the displacement in position
    // (used by the bounding boxes optimization for collision detection)
    pax = fmin(ax, ax + dx);
    pay = fmin(ay, ay + dy);
    pbx = fmax(bx, bx + dx);
    pby = fmax(by, by + dy);

    return move_distance;        
}        



void GameObj::decelerate_character() {
    if (do_decelerate) {
        if (x_velocity < 0) x_velocity += x_dec_per_sec;
        if (x_velocity > 0) x_velocity -= x_dec_per_sec;

        // Deceleration may produce a speed that is greater than zero but
        // smaller than the smallest unit of deceleration. These lines ensure
        // that the player does not keep travelling at slow speed forever after
        // decelerating.
        if (abs(x_velocity) < x_dec_per_sec) {
            x_velocity = 0.0f;

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
    x_velocity -= x_acc_per_sec;
    if (x_velocity < -x_max_velocity) x_velocity = -x_max_velocity;         
    do_decelerate = false;    
}

void GameObj::accelerate_right() {
    x_velocity += x_acc_per_sec;        
    if (x_velocity > x_max_velocity) x_velocity = x_max_velocity;        
    do_decelerate = false;    
}

void GameObj::move_up() { 
    //y_velocity -= 40.0f;
    y_velocity -= 10.0f;
    if (y_velocity > -y_max_velocity) y_velocity = -y_max_velocity;        
    // y_velocity -= x_acc_per_sec;
    // if (y_velocity < -y_max_velocity) y_velocity = -y_max_velocity;         
    //do_decelerate = false;    
}

void GameObj::move_down() { 
    //y_velocity += 40.0f;
    y_velocity += 10.0f;
    if (y_velocity > y_max_velocity) y_velocity = y_max_velocity;        
    // y_velocity += x_acc_per_sec;
    // if (y_velocity < y_max_velocity) y_velocity = -y_max_velocity;         
}

void GameObj::jump() {
    // y velocity is clamped later (we have to apply gravity)
    y_velocity = -y_jump_start_velocity;
    jumping = true;
}


// calculate where the object will be after the next time step assuming 
// no collisions happen
void GameObj::calc_projected_delta_position(const float step_t) {
    px = x + step_t * x_velocity;
    py = y + step_t * y_velocity;

    pax = fmin(ax, px - half_width);
    pbx = fmax(bx, px + half_width);
    pay = fmin(ay, py - half_height);
    pby = fmax(by, py + half_height);

    // sanity checks
    assert(pax < pbx);
    assert(pay < pby);
}


// move the object along the movement vector using a parametric equation
void GameObj::move(const float step_t) {
    x += step_t * x_velocity;
    y += step_t * y_velocity;

    // we need to recalculate this every time we move
    ax = x - half_width;
    ay = y - half_height;
    bx = x + half_width;
    by = y + half_height;

    // sanity checks
    assert(ax < bx);
    assert(ay < by);
}


// set the velocity of the game object (use these when colliding).
void GameObj::collision_set_x_velocity(float x_velocity) {
    assert(abs(this->x_velocity) >= abs(x_velocity));
    
    //if (abs(this->x_velocity) > ZERO) {
    //py *= x_velocity / this->x_velocity;    
    this->x_velocity = x_velocity;    
}

void GameObj::collision_set_y_velocity(float y_velocity) {
    assert(abs(this->y_velocity) >= abs(y_velocity));

    //if (abs(this->y_velocity) > ZERO) {
    //py *= y_velocity / this->y_velocity;    
    this->y_velocity = y_velocity;    
}


// void GameObj::set_default_gravity(const float gravity) {
//     GameObj::default_gravity = gravity;
// }
