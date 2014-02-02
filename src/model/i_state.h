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
};

#endif
