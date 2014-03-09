#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>


//
// Implements customizable keyboard mapping.
//

class IAction {
 public:
    inline void perform_action() { do_action(); };
    virtual void do_action() = 0;

    // overload these if you only want to receive a signal when the key is pressed
    virtual bool do_debounce() { return false; };
    virtual void undo_action() {};
};


//
// Class that abstracts away the debounce logic
// (just using it for jump at the moment).
//
class IDebouncedAction: public IAction {
    // need this to avoid strobing the button
    bool key_pressed;

 protected:
    /* bool is_pressed() { return key_pressed; }; */
    /* void pressed() { key_pressed = true; }; */

 public:

    IDebouncedAction(): key_pressed(false) {};

    inline void perform_action() { 
        if (!key_pressed) {
            do_action();
            key_pressed = true;
        }
    }

    virtual void do_action()  {} ;
    //virtual bool xdo_action() = 0;

    // overload these if you only want to receive a signal when the key is pressed
    virtual bool do_debounce() { return true; };
    void undo_action() {
        // jump key released (logic to avoid strobing the jump key)
        key_pressed = false;
    };
};



// a mapping from keys to action
class KeyMap {
 private:
    IAction * action;
    SDL_Scancode k0, k1, k2;

 public:

    KeyMap(IAction * action, 
        SDL_Scancode k0, 
        SDL_Scancode k1 = SDL_SCANCODE_UNKNOWN, 
        SDL_Scancode k2 = SDL_SCANCODE_UNKNOWN) {
        this->k0 = k0;
        this->k1 = k1;
        this->k2 = k2;
        this->action = action;
    }

    void do_action(const Uint8 * keys) {

        // check k0 was pressed
        if (!keys[k0]) {
            return;
        }
        
        // if k1 is required check it
        if (k1 != SDL_SCANCODE_UNKNOWN) {
            if (!keys[k1]) {
                return;
            }
            
            // if k1 and k2 are required then check k2
            if (k2 != SDL_SCANCODE_UNKNOWN && !keys[k2]) {
                return;
            }
        }
        
        // action triggered
        action->perform_action();
    }

    void undo_action(const Uint8 * keys) {

        // check k0 was NOT pressed
        if (!keys[k0]) {

            // debounce action
            action->undo_action();
            return;
        }
        
        // if k1 is required check it
        if (k1 != SDL_SCANCODE_UNKNOWN) {
            // k1 is NOT pressed
            if (!keys[k1]) {

                // debounce action
                action->undo_action();
                return;
            }
            
            // if k1 and k2 are required then check k2
            if (k2 != SDL_SCANCODE_UNKNOWN && !keys[k2]) {
                // k2 is NOT pressed
                // debounce action
                action->undo_action();
                return;
            }
        }        
    }


};

// a set of key->action mappings
class KeyboardHandler {
 private:    
    static std::vector<KeyMap*> global_key_actions;
    std::vector<KeyMap*> key_actions;

    // actions that we need to do only when the button is pressed!
    // not each time we check after it's down
    std::vector<KeyMap*> debounce_key_actions;
    const Uint8 * keys;

 public:

    // constructor
    KeyboardHandler();
    
    int add_action(IAction * action, SDL_Scancode k0, 
        SDL_Scancode k1 = SDL_SCANCODE_UNKNOWN, 
        SDL_Scancode k2 = SDL_SCANCODE_UNKNOWN) {

        KeyMap * key_map = new KeyMap(action, k0, k1, k2);
        key_actions.push_back(key_map);       

        // some actions are edge triggered (we do them when the key is hit once,
        // and don't repeat the action till the key is unpressed and then pressed 
        // again
        if (action->do_debounce()) {
            debounce_key_actions.push_back(key_map);                   
        }
        return 0; // success
    }


    static int add_global_action(IAction * action, SDL_Scancode k0, 
        SDL_Scancode k1 = SDL_SCANCODE_UNKNOWN, 
        SDL_Scancode k2 = SDL_SCANCODE_UNKNOWN) {

        KeyMap * key_map = new KeyMap(action, k0, k1, k2);
        global_key_actions.push_back(key_map);       
        return 0; // success
    }
    
    int do_actions() {

        std::vector<KeyMap*>::iterator i;
        KeyMap * key_map;
        for (i = key_actions.begin(); i != key_actions.end(); i++) {
            key_map = *i;
            key_map->do_action(keys);
        }        

        for (i = global_key_actions.begin(); i != global_key_actions.end(); i++) {
            key_map = *i;
            key_map->do_action(keys);
        }        

        // check for debounce actions
        for (i = debounce_key_actions.begin(); i != debounce_key_actions.end(); i++) {
            key_map = *i;
            key_map->undo_action(keys);
        }        

        return 0; // success
    }
    
    
    int clean_up() {
        std::vector<KeyMap*>::iterator i;
        KeyMap * key_map;
        for (i = key_actions.begin(); i != key_actions.end(); i++) {
            key_map = *i;
            delete key_map;
        }
        return 0; // success
    }
};


#endif
