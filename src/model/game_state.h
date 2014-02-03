#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>

#include "model/i_state.h"
#include "model/game_obj.h"
#include "model/collision.h"


class GameState : public IState {
 private:
    Model * model;

    // a list of all the game objects that we might want to render..
    // this includes the character and fixed objects
    std::list<GameObj*> game_objs;

    // a list of game objects that can move..
    // this includes characters, monsters, and other stuff, 
    // but not game_objs with fixed_position = true.
    // (we use this in an optimization of the collision detection).
    std::list<GameObj*> movable_game_objs;

    // and a list for game objs that don't move
    std::list<GameObj*> immovable_game_objs;
    
    // the main dude that the player controls (he's also in the game_objs list).
    // the character also appears in the list of game_objs and the movable_game_objs
    GameObj * character;

    // a list of potential collisions (returned by the first pass of the collision detector).
    std::list<Collision*> potential_collisions;

    // is the game paused?
    bool paused;
    // need this to avoid turning the pause off/on/off/on.. strobe-style
    bool pause_key_pressed;

    // is the player jumping (space held down?)
    bool jumping;
    // need this to avoid strobing the jump button
    bool jump_key_pressed;

    // last time we moved the game objs in milliseconds.
    Uint32 last_time_updated;

    // work out where movable objects would move to without collisions 
    // and update the bounding boxes
    // scale movement to delta t
    float calc_projected_movement(const float delta_time);

    // fast and stupid first stage of collision detection 
    void detect_potential_collisions_brute_force(
        const std::list<GameObj*> game_objs, 
        std::list<Collision*> & collisions);
    

    // set the current position to the projected position 
    // that has the positions resolved
    /* void commit_changes_in_positions(); */

    // use this to edge trigger dumping debug data to stdout
    bool rctrl_key_pressed;


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

    // handle keyboard input
    inline void handle_keyboard(const Uint8 * key_states);

    // update the positions of all the game objects based on their velocity
    void update(const Uint8 * key_states);

    // is the game paused?
    bool is_paused() const { return paused; }
};
#endif
