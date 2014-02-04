#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <string>
#include <iostream>
#include <cmath>
#include <cassert>

#include <SDL2/SDL.h>

#include "model/collision_type.h"


// FIXME: get rid of dx/dy duplicate data that stops us changing the velocities?

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

    // Axis Aligned Bounding Box (used for collision detection)
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
    /* float dx; */
    /* float dy; */

    // if a game object is a potential collider (or collidee) *and* movable then
    // we will need to iterate it's movement at collision detection time.
    // movable objects that aren't potential colliders can just jump to their potential 
    // displacement. Immovable game objects are never colliders.
    bool potential_collider;

    // distance to be moved, i.e. from (x, y) to (dx, dy)
    //float move_distance;

    // t is a parametric value between 0.0 and 1.0 that represents how 
    // far along the (dx, dy) vector we can safely move in one movement cycle
    //float t;
    

    // constructor
    GameObj(float x, float y, SDL_Texture * texture = nullptr, bool movable = false); 

    
    bool contains_point(float x, float y);

    // print position, speed, acceleration info to stdout
    void dump_position(std::string name);

    // moving the game object
    void accelerate_left();
    void accelerate_right();
    void jump();


    //
    // Instructions to change the movement of the object
    //
    
    // change in speed when there's no direction input
    void decelerate();

    //
    // Methods used in collision detection
    //

    // move the object in the velocity direction
    void move(float step_t);

    // calculate the position of this object after moving if nothing effected its movement.  
    // This method also calculates the AABB bounding boxes for all movable objects.
    float calc_projected_delta_position(float delta_time_in_secs);

    // return true if this object *potentially* collides with the other object 
    // this is the fast and rough check for a potential collision
    bool potentially_collides_with(GameObj * other_game_obj);

    // return true if this object *collides* with the other object 
    collision_type_t collides_with(GameObj * other_game_obj);

    // set the position of the object to the current projected position 
    // (after collisions have been resolved).
    void commit_change_in_position();

    // move the object along the movement vector using a parametric equation
    float calc_projected_move(GameObj * other_game_obj);


    /* // this is supposed to be the accurate collision detection */
    /* collision_type_t check_for_collision(GameObj * other_game_obj); */

};

#endif
