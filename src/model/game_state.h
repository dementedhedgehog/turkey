#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>

#include "model/base_state.h"
#include "model/i_game_obj_manager.h"
#include "model/collision.h"
#include "model/camera.h"
#include "model/particle_system.h"


class GameState : public BaseState, public IGameObjManager {
 private:
    Model * model;

    // a list of all the game objects that we might want to render..
    // this includes the character and fixed objects
    std::list<GameObj*> game_objs;

    // a list of game objects that can move..
    // this includes characters, monsters, and other stuff, 
    // but not game_objs with fixed_position = true.
    // (we use this in an optimization of the collision detection).
    std::list<GameObj*> moveable_game_objs;

    // and a list for game objs that don't move
    std::list<GameObj*> immoveable_game_objs;

    // and a list for game objs that we check for a time to live
    std::list<GameObj*> ttl_game_objs;
    
    // the main dude that the player controls (he's also in the game_objs list).
    // the character also appears in the list of game_objs and the moveable_game_objs
    GameObj * character;

    // a list of potential collisions between two moveable objects
    // (returned by the first pass of the collision detector).
    std::list<Collision*> potential_moveable_collisions;

    // a list of potential collisions between a moveable object and a fixed object
    // (returned by the first pass of the collision detector).
    std::list<Collision*> potential_fixed_collisions;

    // deferred fixed collisions 
    // deferred because they happen after any other collisions.
    //std::list<Collision*> deferred_fixed_collisions;

    //
    // Keyboard state.
    //

    // is the game paused?
    bool paused;

    // need this to avoid turning the pause off/on/off/on.. strobe-style
    bool pause_key_pressed;

    // use this to edge trigger dumping debug data to stdout
    bool rctrl_key_pressed;

    /* // is the player jumping (space held down?) */
    /* bool jumping; */

    /* // need this to avoid strobing the jump button */
    /* bool jump_key_pressed; */

    // a particle system!
    ParticleSystem * particle_system;

    //
    // Stuff used for timing movement
    // 

    // last time we moved the game objs in milliseconds.
    Uint32 last_time_updated;

    // return the delta time since the last state update
    inline float get_time_since_last_update();

    //
    // Internal methods
    //

    // work out where moveable objects would move to without collisions 
    // and update the bounding boxes
    // scale movement to delta t
    float calc_initial_projected_move(const float delta_time);

    // fast and stupid first stage of collision detection 
    void detect_potential_collisions_brute_force(
        const std::list<GameObj*> game_objs, 
        std::list<Collision*> & potential_moveable_collisions,
        std::list<Collision*> & potential_fixed_collisions);
        
    //
    // Camera
    //
    Camera * camera;

    // clean up old game objects
    inline void age_ttl_game_objs(float delta_time);
    inline void remove_dead_game_objs();

    
public:
    // constructor
    GameState(Model * model);

    // get the current game state
    State get_state() { return State::GAME; }    

    // a list of game objects which we render
    std::list<GameObj*> const get_game_objs() const;    

    // add a game object
    void add_game_obj(GameObj * game_obj);

    // add a special game object.. the players sprite
    void add_character_game_obj(GameObj * game_obj);

    // add a particle system to the game
    void add_particle_system(ParticleSystem * particle_system);

    // handle keyboard input
    //inline void handle_keyboard(const Uint8 * key_states);

    // handle mouse button events
    inline void handle_mouse(const int x, const int y, const Uint8 mouse_button_state);

    // update the positions of all the game objects based on their velocity
    //void update(const Uint8 * key_states);
    void update();

    // is the game paused?
    bool is_paused() const { return paused; }

    // turn pause on and off
    void toggle_pause();

    // debugging: dump game object info to std out.
    void dbg_dump_objects();

    // return the current camera position
    void get_camera_position(float * camera_x, float * camera_y) const;

};
#endif
