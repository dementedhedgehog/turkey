#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <string>
#include <iostream>
#include <cmath>
#include <cassert>

#include <SDL2/SDL.h>

#include "model/collision_type.h"
#include "model/game_obj_type.h"


// FIXME: get rid of dx/dy duplicate data that stops us changing the velocities?

/*
 * Class that represents all collidable objects 
 * this includes the main character, monsters, some pieces of terrain, and items.
 *
 */
class GameObj {
 public:

    GameObjType * type;

    static int last_id;

    int id;

    //
    // Scripts can set this stuff
    // 

    // maximum speeds
    static float default_x_max_velocity;
    static float default_y_max_velocity;

    // side ways movement
    static float default_x_acceleration;
    // side ways deceleration
    static float default_x_deceleration;

    // set jump and gravity forces
    static float default_jump_velocity;
    static float default_gravity;


    // current position
    float x;
    float y;

    // make this a function of the sprite (and calculate and store it),
    float half_height;
    float half_width;

    // FIXME: place holder for a sprite..
    SDL_Texture * texture;

    // does the game object move?
    // if the game_obj is not movable then velocities, accleration etc are not used.
    // (we use this field to optimize collision detection).    
    //bool movable; 

    inline bool is_moveable() const { return type->is_moveable(); };

    // Axis Aligned Bounding Box 
    float ax, ay, bx, by;

    // is the player jumping (space held down?)
    bool jumping;

    // flag used for character movement
    bool do_decelerate;

    // if this flag is true the game object will be removed
    bool dead;

    //
    // FPS Invariant Movement Variables
    //

    // current velocity, this is the delta position per second.
    float x_velocity;
    float y_velocity;

    // use these to clamp max velocity
    float x_max_velocity;
    float y_max_velocity;

    // acceleration when running
    float x_acc_per_sec; 
    // decelaration when you stop running
    float x_dec_per_sec; 

    // acceleration up/down (due to gravity + jumping)
    float y_acc_per_sec; 
    float y_jump_start_velocity;

    // the time-to-live for the game object
    // if this is >= 0.0 then the ttl will be reduced every time the state is updated
    // when the ttl goes from being positive to negative it is removed for good.
    float ttl;

    //
    // Stepped Movement Variables
    // We use these to iterate objects towards their final positions
    // 

    // projected movement position (where we want to move in the next time step
    // assuming there aren't going to be any collisions).
    float px;
    float py;

    // Axis Aligned Bounding Box *including* Projected Move!! (used for collision detection)
    // Note: not quite a bounding box for the object because it's really the box for the 
    // object and the object if it were moved to where it wants to be (ignoring collisions).
    float pax, pay, pbx, pby;

   
    // Axis

    // if a game object is a potential collider (or collidee) *and* movable then
    // we will need to iterate it's movement at collision detection time.
    // movable objects that aren't potential colliders can just jump to their potential 
    // displacement. Immovable game objects are never colliders.
    bool potential_collider;

    
    
    // constructor
    GameObj(float x, float y, GameObjType * type, 
        SDL_Texture * texture = nullptr); //, bool movable = false); 

    
    //bool contains_point(float x, float y);

    // print position, speed, acceleration info to stdout
    void dump_position(std::string name);


    //
    // Instructions to change the movement of the object
    //
    
    // change in speed when there's no direction input
    void reset_decelerate_flag();

    // decelerate character if required
    void decelerate_character();

    // moving the game object
    void accelerate_left();
    void accelerate_right();
    void move_up();
    void move_down();
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
    void collision_set_x_velocity(float x_velocity);
    void collision_set_y_velocity(float y_velocity);

    // set the gravity
    static void set_default_gravity(const float gravity);

    void to_stream(std::ostream &strm) const {
        strm << type->get_name()  << " " << id << " collider? " << potential_collider << ",";
        /* strm << type->get_name() << " (" << self.id; */

        /* if (is_moveable()) { */
        /*     strm << ", " << self.moveable_id; */
        /* } */
        //strm << ") aabb: (" << ax << "," << ay << "," << bx << "," << by << ")";
        strm << " aabb: (" << ax << "," << ay << "," << bx << "," << by << ")";

        if (is_moveable()) {
            strm << " vel(" << x_velocity << ", " << y_velocity << ")";
            strm << " paabb: (" << pax << "," << pay << "," << pbx << "," << pby << ")";
        }
    }

    const std::string get_type_name() const { return type->get_name(); };
};


inline std::ostream& operator<<(std::ostream &strm, const GameObj & g) {
    g.to_stream(strm);
    return strm;
}

inline std::ostream& operator<<(std::ostream &strm, const GameObj * g) {
    g->to_stream(strm);
    return strm;
}


#endif
