#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <string>
#include <iostream>
#include <cmath>
#include <cassert>

#include <SDL2/SDL.h>

#include "model/collision_type.h"


/*
 * Class that represents all collidable objects 
 * this includes the main character, monsters, some pieces of terrain, and items.
 *
 */
class GameObj {
 public:

    // current position
    float x;
    float y;

    float half_height;
    float half_width;

    // FIXME: place holder for a sprite..
    SDL_Texture * texture;

    // does the game object move?
    // if the game_obj is not movable then velocities, accleration etc are not used.
    // (we use this field to optimize collision detection).    
    bool movable; 

    // Bounding Box (used for collision detection)
    // Note: not quite a bounding box for the object because it's really the box for the 
    // object and the object if it were moved to where it wants to be (ignoring collisions).
    float ax, ay, bx, by;

    //
    // FPS Invariant Movement Variables
    //

    // current velocity
    float x_vel_per_sec;
    float y_vel_per_sec;

    // use these to clamp max velocity
    float x_max_vel_per_sec;
    float y_max_vel_per_sec;

    // acceleration when running
    float x_acc_per_sec; 
    // decelaration when you stop running
    float x_dec_per_sec; 

    // acceleration up/down (due to gravity + jumping)
    float y_acc_per_sec; 
    float y_jump_start_vel_per_sec;

    //
    // Scaled Movement Variables
    // 

    // change in position due to velocity and acceleration
    float dx;
    float dy;

    // t is a parametric value between 0.0 and 1.0 that represents how 
    // far along the (dx, dy) vector we can safely move in one movement cycle
    float t;

    // constructor
    GameObj(float x, float y, SDL_Texture * texture = nullptr, bool movable = false); 

    
    bool contains_point(float x, float y);

    // print position, speed, acceleration info to stdout
    void dump_position(std::string name);

    // moving the game object
    void accelerate_left();
    void accelerate_right();
    void jump();

    /* inline void apply_gravity() { */
    /* } */
    
    // change in speed when there's no direction input
    void decelerate();

    // calculate the position of this object after moving if nothing effected its movement.  
    // This method also calculates the AABB bounding boxes for all movable objects.
    void calc_projected_delta_position(float delta_time_in_secs);

    // return true if this object potentially collides with the other object 
    bool potentially_collides_with(GameObj * other_game_obj);

    // set the position of the object to the current projected position 
    // (after collisions have been resolved).
    void commit_change_in_position();

    // move the object along the movement vector using a parametric equation
    // 
    inline collision_type_t calc_projected_move(GameObj * other_game_obj) {

        // FIXME: Safe move optimization here!
        // there's an easy way to find a larger starting new_t value here
        float start_t = 0.0f;

        // We need to get the length of the dx/dy vector in order to step by 1 pixel 
        // at a time when doing our speculative collisions accurately to the pixel.
        // Note: also that we don't care about t values that have already had collisions!
        float dxdy_vector_length = (t - start_t) * sqrt(dx*dx + dy*dy); // in pixels 

        // how many steps do we have to take to get per pixel testing?
        float step = 1.0 / dxdy_vector_length;

        // step the parametric equation until we get to 1.0 or we get a collision
        float new_t;
        collision_type_t collision_type = NONE;
        for (new_t = start_t; new_t <= (t + 0.0001); new_t += step) {
            collision_type = projection_collides_with(other_game_obj, t);
            if (collision_type != NONE) {
                new_t -= step;

                if (new_t < t) {
                    // a closer collision has occurred
                    t = new_t;
                }
                break;
            }
        }

        // returns the type of collision or NONE
        return collision_type;
    }

    // this is supposed to be the accurate collision detection
    // FIXME: it's just using bounding boxes at the moment
    // FIXME: this will use contact points
    inline collision_type_t projection_collides_with(GameObj * other_game_obj, float t) {
        collision_type_t collision_type = NONE;
        
        // if moving left
        if (dx > 0) { 
            float tbx = bx + t * dx;
            
            // If this objects right edge is to the left of the others left edge,
            // then this object is totally to the left of the other object.
            if (tbx < other_game_obj->ax) return NONE;

            collision_type = collision_type && RIGHT;
        }
        else { // otherwise moving right
            float tax = ax + t * dx;
            
            // If this objects left edge is to the right of the others right edge,
            // then this object is totally to the right of the other object.
            if (tax > other_game_obj->bx) return NONE;

            collision_type = collision_type && LEFT;
        }

        // if moving down
        if (dy > 0) {
            float tby = by + t * dy;

            // If this objects bottom edge is above the others top edge,
            // then this object is totally above the other object.
            if (tby < other_game_obj->ay) return NONE;

            collision_type = collision_type && BOTTOM;
        }
        else { // otherwise moving up
            float tay = ay + t * dy;

            // If this objects top edge is below the others bottom edge,
            // then this object is totally below the other object.
            // (remember that y increases downwards in sdl).
            if (tay > other_game_obj->by) return NONE;
            
            collision_type = collision_type && TOP;
        }
        
        // otherwise there's been a collision with the real bounding box 
        // (not the aabb bounding box) of the other game object.
        return collision_type;        
    }
};

#endif
