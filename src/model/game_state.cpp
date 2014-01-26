
#include "model/game_state.h"

GameState::GameState() {

    // add the character!
    this->character = new GameObj(3, 3);
    game_objs.push_back(character);    

    
    // hard code some objects (FIXME: prototyping hack, remove)
    int x, y; 
    GameObj * game_obj;
    
    y = 17;
    for (x = 0; x < 15; x += 1) {
        game_obj = new GameObj(x, y); 
        game_objs.push_back(game_obj);
    }

    y = 23;
    for (x = 26; x < 35; x += 1) {
        game_obj = new GameObj(x, y);
        game_objs.push_back(game_obj);
    }

    y = 13;
    for (x = 22; x < 27; x += 1) {
        game_obj = new GameObj(x, y);
        game_objs.push_back(game_obj);
    }    

    game_obj = new GameObj(20, 20);
    game_objs.push_back(game_obj);

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
void GameState::start_moving_character_left() { character->x_vel = -1;}
void GameState::stop_moving_character_left() { if (character->x_vel < 0) character->x_vel = 0;}

void GameState::start_moving_character_right() { character->x_vel = +1;}
void GameState::stop_moving_character_right() { if (character->x_vel > 0) character->x_vel = 0;}

void GameState::start_moving_character_up() { character->y_vel = -1;}
void GameState::stop_moving_character_up() { if (character->y_vel < 0) character->y_vel = 0;}

void GameState::start_moving_character_down() { character->y_vel = +1;}
void GameState::stop_moving_character_down() { if (character->y_vel > 0) character->y_vel = 0;}
