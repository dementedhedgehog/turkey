#ifndef MODEL_H
#define MODEL_H

#include <list>

//#include "cell.h"
//#include "position.h"

#include "model/base_state.h"
#include "model/game_state.h"
#include "model/intro_state.h"
#include "model/camera.h"


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
    QuittingState * quitting_state;
    BaseState * current_state;

    // a list of things to notify when the game state changes
    std::list<IStateListener*> listeners;

    // do we have keyboard focus
    bool game_has_keyboard_focus;

    /* KeyboardHandler * keyboard_handler; */

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
    //void update(const Uint8 * key_states);
    void update();

    // handle a mouse event
    void handle_mouse(const int x, const int y, const Uint8 mouse_button_state);

    int clean_up();
};
#endif
