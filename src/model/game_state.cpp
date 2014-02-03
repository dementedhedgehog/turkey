
#include <iostream>
#include <cassert>

#include "model/game_state.h"
#include "model/model.h"

const int CHARACTER_MOVE = 8;

const int N_SPECULATIVE_CONTACTS_ITERATIONS = 1;

GameState::GameState(Model * model) {
    this->model = model;    

    // add the character!
    this->character = nullptr;   

    // pause state
    paused = false;
    pause_key_pressed = false;

    // character jumping state
    jumping = false;
    jump_key_pressed = false;

    // start timing movement... now (movement = velocity * time).
    last_time_updated = SDL_GetTicks();
}

void GameState::add_game_obj(GameObj * game_obj) {
    game_objs.push_back(game_obj);
    if (game_obj->movable) {
        movable_game_objs.push_back(game_obj);
    }
    else {
        immovable_game_objs.push_back(game_obj);
    }
}


void GameState::add_character_game_obj(GameObj * game_obj) {
    assert(game_obj->movable);
    this->add_game_obj(game_obj);
    this->character = game_obj;
}



std::list<GameObj*> const GameState::get_game_objs() const {
    return game_objs;
}


void GameState::update(const Uint8 * key_states) {

    // work out the user commands
    handle_keyboard(key_states);

    // don't update any positions if the game is paused
    if (paused) {
        //std::cout << "paused " << std::endl;
        return;
    }
    
    // also use contact points?

    // calculate the positions that movable objects would be in after moving if nothing 
    // effected their movement.  This method also calculates the AABB bounding boxes
    // for all movable objects (aka aabb rects).   We use this information to optimize 
    // collision detection in an approach called the Bounding Box Optimization.
    float max_distance = calc_projected_movement();

    // get a list of possible collisions (i.e. a list of objects whose aabb rects overlap)
    // all possible collisions will be in this list.. i.e. it might contain false positives 
    // but no false negatives.
    detect_potential_collisions_brute_force(game_objs, potential_collisions);
    if (potential_collisions.size() > 0) {
        std::cout << "collisions: " << potential_collisions.size()  << std::endl;
    };
    
    // For each pairwise collision move the object as far as it can.
    // do this a number of times to avoid jitter..
    // for (int i = 0; i < N_SPECULATIVE_CONTACTS_ITERATIONS; i++) {
       
    // how many steps do we have to take to get per pixel testing?
    float dt = 1.0 / max_distance;
        
    GameObj * game_obj;
    std::list<GameObj*>::iterator i;
    for (float t = 0.0f; t < 1.001; t += dt) {

        // step any movable objects that might be in a collision
        for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
            game_obj = *i;
            
            if (game_obj->potential_collider) {
                game_obj->step_movement(dt);
            }
        }
        
        // for each collision
        for (auto j = potential_collisions.begin(); j != potential_collisions.end(); j++) {
            Collision * collision = *j;            

            // check for a collision..
            if (collision->check()) {

                // FIXME: resolve collision here!
                std::cout << "COLLISION OCCURRED !!!!!!!!!!!!!!!!!!!!!!" << std::endl;

                // handle the collision
                collision->resolve();
            }

            // move stuff
            std::cout << "collision!" << std::endl;
        }       
    }
    // }

    // now move any movable objects that can't be in a collision
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
        game_obj = *i;
            
        if (!game_obj->potential_collider) {
            game_obj->move();
        }
    }

    //commit_changes_in_positions();
    //if (character) character->dump_position("character");
}    


// calculate the positions that movable objects would be in after moving if nothing 
// effected their movement.  This method also calculates the AABB bounding boxes
// for all movable objects.
//
// Returns the maximum possible distance moved which we use to determine the number
// of iterations to make.
float GameState::calc_projected_movement() {

    // We want objects to move at a constant rate irrespective of the number of 
    // frames per second so we scale the velocities and accelerations by the time 
    // since the last render.
    //
    // Get the time ellapsed since the last update
    // (for constant speed in time ... not effected by frames per second).
    Uint32 time_now = SDL_GetTicks();
    float delta_time = (time_now - last_time_updated) / 1000.0f; // in seconds

    // remember that now is the new last time we moved the objects!
    last_time_updated = time_now;
    
    // for each movable game object
    std::list<GameObj*>::iterator i;
    float max_distance = 0.0;
    float distance;
    GameObj * game_obj;
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
        // calculate where the movable object would move to if collisions don't happen
        game_obj = *i;
        distance = game_obj->calc_projected_delta_position(delta_time);
        max_distance = fmax(max_distance, distance);
    }

    return max_distance;
}


// commits the current position of the movable objects to their projected, 
// collision-resolved positions.
// void GameState::commit_changes_in_positions() {

//     // for each movable game object
//     std::list<GameObj*>::iterator i;
//     GameObj * game_obj;
//     for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
//         // set the game objects current position to the projected position 
//         // after we've taken into account all the collisions.
//         game_obj = *i;
//         game_obj->commit_change_in_position();
//     }
// }

void GameState::handle_keyboard(const Uint8 * key_states) {

    // Process player keyboard input?
    bool move_player_request = false;
    if (model->has_keyboard_focus()) {

        // move left?
        if (key_states[SDL_SCANCODE_LEFT]) { 
            if (character) {
                character->accelerate_left();
                move_player_request = true;
            }
        }

        // move right?
        if (key_states[SDL_SCANCODE_RIGHT]) { 
            if (character) {
                character->accelerate_right();
                move_player_request = true;
            }
        }
 
        // Jump if not already jumping and the jump key was released earlier
        if (key_states[SDL_SCANCODE_SPACE]) {
            if (!jumping && !jump_key_pressed) { 
                if (character) character->jump();
                jumping = true;
                jump_key_pressed = true;
            }
        }
        else {
            // jump key released (logic to avoid strobing the jump key)
            jump_key_pressed = false;
        }

        // when the P button is pressed the game is paused
        // (avoid toggling the pause button off and on again really quickly)
        if (key_states[SDL_SCANCODE_P]) {
            if (!pause_key_pressed) {
                paused = not paused;
                pause_key_pressed = true;
            }            
        }
        else {
            // pause key is released! next time it's pressed we toggle pause
            pause_key_pressed = false;
        }            
 
        // Decelerate the player's sideways movement if left or right wasn't pressed
        if (!move_player_request) {
            if (character) character->decelerate(); 
        }
        
        // Dump debug info when right control is pressed
        if (key_states[SDL_SCANCODE_RCTRL]) {
            if (!rctrl_key_pressed) { 
                rctrl_key_pressed = true;
                if (character) character->dump_position("character");
                
                std::cout << "--------- " << std::endl;
                std::cout << "movable " << movable_game_objs.size() << std::endl;
                std::cout << "all " << game_objs.size() << std::endl;
               }
        }
        else {
            // jump key released (logic to avoid strobing the jump key)
            rctrl_key_pressed = false;
        }
    }
}


/*
 * Detect potential collisions based on the aabb bounding boxes..
 *
 * FIXME: later on (when we do particle systems) we should use a quadtree to 
 *        do this instead of brute force? (maybe).
 */
void GameState::detect_potential_collisions_brute_force(
        const std::list<GameObj*> game_objs, 
        std::list<Collision*> & collisions) {

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "---" << std::endl;
        
    // clear the list so there are no collisions left.
    collisions.clear();

    // make a collision object for each pair of things that might collide 
    // (if two movable objs might collide there's still only one collision).
    std::list<GameObj*>::const_iterator i;
    std::list<GameObj*>::const_iterator j;
    GameObj * game_obj_i;
    GameObj * game_obj_j;
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {         
        game_obj_i = *i;

        // collisions of movable objects with other movable objects
        for (j = std::next(i); j != movable_game_objs.end(); j++) {
            //for (j = i + 1; j != movable_game_objs.end(); j++) {
            game_obj_j = *j;

            // sanity checks
            assert(game_obj_i != game_obj_j);
            assert(game_obj_j->movable);

            // check for a collision..
            if (game_obj_i->potentially_collides_with(game_obj_j)) {
                
                // collision!
                Collision * c = new Collision(game_obj_i, game_obj_j);
                collisions.push_back(c);
            }
        }

        // collisions of movable objects with immovable objects
        for (j = immovable_game_objs.begin(); j != immovable_game_objs.end(); j++) {
            game_obj_j = *j;

            // sanity checks
            assert(game_obj_i != game_obj_j);
            assert(!game_obj_j->movable);
            
            // check for a collision..
            if (game_obj_i->potentially_collides_with(game_obj_j)) {
                
                // collision!
                Collision * c = new Collision(game_obj_i, game_obj_j);
                collisions.push_back(c);
            }
        }
    }  
    std::cout << "---" << std::endl;
}

