
#include <iostream>
#include <cassert>

#include "model/game_state.h"
#include "model/model.h"

const int CHARACTER_MOVE = 12;


GameState::GameState(Model * model) {
    this->model = model;    

    // add the character!
    this->character = nullptr;   

    // pause state
    paused = false;
    pause_key_pressed = false;

    // character jumping state
    jump_key_pressed = false;

    // start timing movement... now (movement = velocity * time).
    last_time_updated = 0; 
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


static int count = 0;

void GameState::update(const Uint8 * key_states) {

    // work out the user commands
    handle_keyboard(key_states);

    // don't update any positions if the game is paused
    if (paused) {
        //std::cout << "paused " << std::endl;
        return;
    }
    
    // first time through don't bother doing anything (no way to get a meaningful delta time)
    if (last_time_updated == 0) {
        last_time_updated = SDL_GetTicks();
        return;
    }

    // FIXME: use contact points?

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

    //if (character) character->dump_position("character");

    // calculate the positions that movable objects would be in after moving if nothing 
    // effected their movement.  This method also calculates the AABB bounding boxes
    // for all movable objects (aka aabb rects).   We use this information to optimize 
    // collision detection in an approach called the Bounding Box Optimization.
    float max_distance = calc_initial_projected_move(delta_time);

    // get a list of possible collisions (i.e. a list of objects whose aabb rects overlap)
    // all possible collisions will be in this list.. i.e. it might contain false positives 
    // but no false negatives.
    detect_potential_collisions_brute_force(
        game_objs, 
        potential_movable_collisions,
        potential_fixed_collisions);
    // if (potential_fixed_collisions.size() > 0 || potential_movable_collisions.size() > 0) {
    //     std::cout << "potential collisions: " 
    //               << potential_movable_collisions.size() + potential_fixed_collisions.size()  
    //               << std::endl;
    // };
    
    // For each pairwise collision move the object as far as it can.
    // do this a number of times to avoid jitter..
       
    // how many steps do we have to take to get per pixel testing?
    float dt = delta_time / max_distance;

    // FIXME: handle max_distance = 0 (no possible collisions)??
    // if (character) character->dump_position("character");
    // std::cout << "MAX_DISTANCE " << max_distance << std::endl;
    // std::cout << "DELTA_TIME " << delta_time << std::endl;
    // std::cout << "DT " << dt << std::endl << std::endl << std::endl;
    //exit(4);
        
    GameObj * game_obj;
    std::list<GameObj*>::iterator i;
    //for (float t = 0.0f; t < 1.001; t += dt) {
    for (float t = 0.0f; t < delta_time; t += dt) {
        // std::cout << count << std::endl;

        // calculate the projected positions of movable objects that might be in a collision
        for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
            game_obj = *i;
            
            if (game_obj->potential_collider) {
                game_obj->calc_projected_delta_position(dt);
            }
        }
        
        // find and resolve each *movable* collision
        for (auto j = potential_movable_collisions.begin(); 
                  j != potential_movable_collisions.end(); 
                  j++) {
            Collision * collision = *j;

            // check for a collision..
            if (collision->check_for_projected_movable_collision()) {

                // FIXME: resolve collision here!
                // std::cout << "COLLISION OCCURRED !!!!!!!!!!!!!!!!!!!!!!" << std::endl;

                // handle the collision
                collision->resolve();
            }
        }       

        // find and resolve each *fixed* collision
        for (auto j = potential_fixed_collisions.begin(); 
                  j != potential_fixed_collisions.end(); 
                  j++) {
            Collision * collision = *j;            

            // check for a collision..
            if (collision->check_for_projected_fixed_collision()) {

                // FIXME: resolve collision here!
                // std::cout << "COLLISION OCCURRED !!!!!!!!!!!!!!!!!!!!!!" << std::endl;

                // handle the collision
                collision->resolve();
            }
        }       

        // now move the objects to their projected position 
        // (unless they've been involved in a collision in which case the collision 
        // resolution might have changed stuff a bit).
        for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
            game_obj = *i;
            
            if (game_obj->potential_collider) {
                game_obj->move(dt);
            }
        }
    }

    count += 1;
    // if (count > 16) {
    //     //exit(3);
    //     paused = true;
    // }

    // now move any movable objects that can't be in a collision
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
        game_obj = *i;
            
        if (!game_obj->potential_collider) {
            game_obj->move(delta_time);
        }
    }
}    


// calculate the positions that movable objects would be in after moving if nothing 
// effected their movement.  This method also calculates the AABB bounding boxes
// for all movable objects.
//
// Returns the maximum possible distance moved which we use to determine the number
// of iterations to make.
float GameState::calc_initial_projected_move(const float delta_time) {
    
    // for each movable game object
    std::list<GameObj*>::iterator i;
    float max_distance = 0.0;
    float distance;
    GameObj * game_obj;
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
        // calculate where the movable object would move to if collisions don't happen
        game_obj = *i;
        distance = game_obj->calc_initial_projected_move(delta_time);
        max_distance = fmax(max_distance, distance);
    }

    return max_distance;
}


//
// Deal with user input
//
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
            if (!character->jumping && !jump_key_pressed) { 
                if (character) character->jump();
                character->jumping = true;
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
        std::list<Collision*> & moving_collisions,
        std::list<Collision*> & fixed_collisions) {

    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << "---" << std::endl;
        
    // clear the list so there are no collisions left.
    moving_collisions.clear();
    fixed_collisions.clear();

    // make a collision object for each pair of things that might collide 
    // (if two movable objs might collide there's still only one collision).
    std::list<GameObj*>::const_iterator i;
    std::list<GameObj*>::const_iterator j;
    GameObj * game_obj_i;
    GameObj * game_obj_j;
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {         
        game_obj_i = *i;

        // collisions of movable objects with other movable objects
        // (we handle collisions between a pair of moving objects differently from
        // a collision between a moving object and a fixed object).
        for (j = std::next(i); j != movable_game_objs.end(); j++) {
            game_obj_j = *j;

            // sanity checks
            assert(game_obj_i != game_obj_j);
            assert(game_obj_j->movable);

            // check for a collision..
            if (game_obj_i->potentially_collides_with(game_obj_j)) {
                
                // collision!
                Collision * c = new Collision(game_obj_i, game_obj_j);
                moving_collisions.push_back(c);
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
                fixed_collisions.push_back(c);
            }
        }
    }  
    //std::cout << "---" << std::endl;
}

