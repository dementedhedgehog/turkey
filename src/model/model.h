#ifndef MODEL_H
#define MODEL_H

#include <list>

//#include "cell.h"
//#include "position.h"

#include "model/i_state.h"
#include "model/game_state.h"
#include "model/intro_state.h"


class FatalErrorState : public IState {
 public:
    State get_state() { return State::FATAL_ERROR; }    
    void update(const Uint8 * key_states) { }
};

class IStateListener {
public:
    //virtual state_changing(IState * from_state, IState * to_state) = 0;
    virtual void state_changed(State old_state, State current_state) = 0;

    // Adding a virtual destructor allows you to pass pointer ownership to another party 
    // without exposing the concrete derived class. The destructor doesn't have to do 
    // anything, because the interface doesn't have any concrete members
    virtual ~IStateListener() {};
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

    // do we have keyboard focus
    bool game_has_keyboard_focus;

public: 
    Model();    
    ~Model();    

    //void toggle_fullscreen();
    void change_state(State to_state);
    void register_state_listener(IStateListener * listener);    

    GameState * get_game_state();

    void set_keyboard_focus(const bool has_focus);
    const bool has_keyboard_focus();

    // change the positions of the game objects, do collision detection etc.
    void update(const Uint8 * key_states);
};
#endif
