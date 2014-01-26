#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>

#include "model/i_state.h"
#include "model/game_obj.h"


class GameState : public IState {

    // a list of game objects that we might want to render..
    std::list<GameObj*> game_objs;

    // the main dude that the player controls
    GameObj * character;

 public:
    GameState();
    State get_state() { return State::GAME; }    

    std::list<GameObj*> const get_game_objs() const;    

    void update();

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
