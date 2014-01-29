#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>

#include "model/i_state.h"
#include "model/game_obj.h"
#include "model/i_collision_detector.h"


class GameState : public IState {

    // a list of game objects that we might want to render..
    std::list<GameObj*> game_objs;

    // the main dude that the player controls
    GameObj * character;

    ICollisionDetector * collision_detector;
    std::list<collision_t> collisions;

    // is the game paused?
    bool paused;

 public:
    GameState();
    State get_state() { return State::GAME; }    

    std::list<GameObj*> const get_game_objs() const;    

    void add_game_obj(GameObj * game_obj);
    void add_character_game_obj(GameObj * game_obj);

    // update the positions of all the game objects based on their velocity
    void update();

   // toggle game pause
    void pause();

    // character movement
    void start_moving_character_left();
    void stop_moving_character_left();

    void start_moving_character_right();
    void stop_moving_character_right();

    void start_moving_character_up();
    void stop_moving_character_up();

    void start_moving_character_down();
    void stop_moving_character_down();
};


#endif
