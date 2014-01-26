#ifndef I_STATE_H
#define I_STATE_H

enum class State {INTRO, GAME, FATAL_ERROR};    

class IState {
 public:
    virtual ~IState() {};    
    virtual State get_state() = 0;
    virtual void update() = 0;
};

#endif
