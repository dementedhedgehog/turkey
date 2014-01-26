#ifndef MODEL_H
#define MODEL_H

#include <list>

//#include "cell.h"
#include "position.h"

#include "model/i_state.h"
#include "model/game_state.h"


class IntroState : public IState {
 public:
    State get_state() { return State::INTRO; }
    void update() { }
};


class FatalErrorState : public IState {
 public:
    State get_state() { return State::FATAL_ERROR; }    
    void update() { }
};

class IStateListener {
public:
    //virtual state_changing(IState * from_state, IState * to_state) = 0;
    virtual void state_changed(State old_state, State current_state) = 0;
};
    


/**
 * Model for MVC
 *
 */
class Model {
private:
    IntroState * intro_state;
    GameState * game_state;
    FatalErrorState * fatal_error_state;
    
    IState * current_state;

    // a list of things to notify when the game state changes
    std::list<IStateListener*> listeners;

public: 
    Model();    
    ~Model();    

    void toggle_fullscreen();
    void change_state(State to_state);
    void register_state_listener(IStateListener * listener);    

    GameState * get_game_state();

    // change the positions of the game objects, do collision detection etc.
    void update();
};
#endif
