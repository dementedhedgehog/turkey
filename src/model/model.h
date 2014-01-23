#ifndef MODEL_H
#define MODEL_H

#include <list>

#include "cell.h"
#include "position.h"


enum class State {INTRO, GAME, FATAL_ERROR};    

class IState {
 public:
    virtual ~IState() {};    
    virtual State get_state()  { return State::FATAL_ERROR; }; //= 0;
};


class IntroState : public IState {
 public:
    State get_state() { return State::INTRO; }
};

class GameState : public IState {
 public:
    State get_state() { return State::GAME; }    
};

class FatalErrorState : public IState {
 public:
    State get_state() { return State::FATAL_ERROR; }    
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

    std::list<IStateListener*> listeners;

public: 
    Model();    
    ~Model();    

    void toggle_fullscreen();
    void change_state(State to_state);
    void register_state_listener(IStateListener * listener);    
};
#endif
