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

    // Axis Aligned Bounding Box *including* Move!! (used for collision detection)
    // Note: not quite a bounding box for the object because it's really the box for the 
    // object and the object if it were moved to where it wants to be (ignoring collisions).
    float ax, ay, bx, by;

    // is the player jumping (space held down?)
    bool jumping;

    // if this flag is true the game object will be removed
    bool dead;

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

    // the time-to-live for the game object
    // if this is >= 0.0 then the ttl will be reduced every time the state is updated
    // when the ttl goes from being positive to negative it is removed for good.
    float ttl_in_secs;

    //
    // Scaled Movement Variables
    // 

    // projected movement position (where we want to move in the next time step
    // assuming there aren't going to be any collisions).
    float px;
    float py;

    // stepped axis aligned bounding box..
    // This is the bounding box of the object after we have stepped it.
    // recalculate this after changing px or py
    float pax, pay, pbx, pby;

   
    // Axis

    // if a game object is a potential collider (or collidee) *and* movable then
    // we will need to iterate it's movement at collision detection time.
    // movable objects that aren't potential colliders can just jump to their potential 
    // displacement. Immovable game objects are never colliders.
    bool potential_collider;
    
    // constructor
    GameObj(float x, float y, SDL_Texture * texture = nullptr, bool movable = false); 

    
    //bool contains_point(float x, float y);

    // print position, speed, acceleration info to stdout
    void dump_position(std::string name);


    //
    // Instructions to change the movement of the object
    //
    
    // change in speed when there's no direction input
    void decelerate();

    // moving the game object
    void accelerate_left();
    void accelerate_right();
    void jump();

    //
    // Methods used in collision detection
    //

    // move the object in the velocity direction
    void move(const float step_t);

    // move the object along the movement vector using a parametric equation
    float calc_initial_projected_move(float delta_time_in_secs);

    // work out (hypothetically) where this object would move to
    void calc_projected_move(const float step_t);

    // calculate the position of this object after moving if nothing effected its movement.  
    // This method also calculates the AABB bounding boxes for all movable objects.
    // (Faster than the initial projected move method).
    void  calc_projected_delta_position(float delta_time_in_secs);
    //void calc_projected_delta_move(float delta_time_in_secs);

    // return true if this object *potentially* collides with the other object 
    // this is the fast and rough check for a potential collision
    bool potentially_collides_with(GameObj * other_game_obj);

    // Check whether the projected position of this object will collide with the projected 
    // position of another movable object.
    CollisionType check_for_projected_movable_collision(GameObj * other_game_obj);

    // Check whether the projected position of this object will collide with the fixed 
    // position of a fixed object.
    CollisionType check_for_projected_fixed_collision(GameObj * other_game_obj);

    // set the velocity of the game object 
    // use these to slow or stop the object when colliding.    
    void collision_set_x_velocity(float x_vel_per_sec);
    void collision_set_y_velocity(float y_vel_per_sec);
};

#endif
