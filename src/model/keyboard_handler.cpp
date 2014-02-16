
#include "model/keyboard_handler.h"


// c++ needs static class members to be defined in two places!
std::vector<KeyMap*> KeyboardHandler::global_key_actions;

// constructor
KeyboardHandler::KeyboardHandler() {
    keys = SDL_GetKeyboardState(NULL);
}
