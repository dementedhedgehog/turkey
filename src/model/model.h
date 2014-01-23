#ifndef MODEL_H
#define MODEL_H

<list>

#include "cell.h"
#include "position.h"


class IState {};

class IntroState : IState {

}

class GameState : IState {

}

IModelListener {
public:
    //virtual state_changing(IState * from_state, IState * to_state) = 0;
    virtual state_changed(IState * old_state, IState * current_state) = 0;
}
    


/**
 * Model for MVC
 *
 */
class Model {
private:
    IntroState * intro_state;
    GameState * game_state;
    
    IState * current_state;

    std::list<IStateListener*> listeners;

public: 
    Model();    

    void toggle_fullscreen();

    void change_state(IState * to_state);


};


#endif
