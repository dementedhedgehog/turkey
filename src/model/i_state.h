#ifndef I_STATE_H
#define I_STATE_H

#include <SDL2/SDL.h>

// forward declaration
class Model;

enum class State {INTRO, GAME, FATAL_ERROR};    

class IState {
 public:
    virtual ~IState() {};    
    virtual State get_state() = 0;
    virtual void update(const Uint8 * key_states) = 0;

    // handle a mouse event
    virtual void handle_mouse(const int x, const int y, const Uint8 mouse_button_state) = 0;
};

#endif
