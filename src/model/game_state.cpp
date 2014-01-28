
#include "model/game_state.h"

GameState::GameState() {

    // add the character!
    this->character = nullptr;    
}

void GameState::add_game_obj(GameObj * game_obj) {
    game_objs.push_back(game_obj);
}


void GameState::add_character_game_obj(GameObj * game_obj) {
    game_objs.push_back(game_obj);
    this->character = game_obj;
}



std::list<GameObj*> const GameState::get_game_objs() const {
    return game_objs;
}


void GameState::update() {

    std::list<GameObj*>::iterator i;
    GameObj * game_obj;
    for (i = game_objs.begin(); i != game_objs.end(); i++) {         

        // move stuff
        game_obj = *i;
        game_obj->x += game_obj->x_vel;
        game_obj->y += game_obj->y_vel;
    }
}



//
// character movement
//
void GameState::start_moving_character_left() { if (character) character->x_vel = -1; }
void GameState::stop_moving_character_left() { if (character && character->x_vel < 0) character->x_vel = 0;}

void GameState::start_moving_character_right() { if (character) character->x_vel = +1;}
void GameState::stop_moving_character_right() { if (character && character->x_vel > 0) character->x_vel = 0;}

void GameState::start_moving_character_up() { if (character) character->y_vel = -1;}
void GameState::stop_moving_character_up() { if (character && character->y_vel < 0) character->y_vel = 0;}

void GameState::start_moving_character_down() { if (character) character->y_vel = +1;}
void GameState::stop_moving_character_down() { if (character && character->y_vel > 0) character->y_vel = 0;}
