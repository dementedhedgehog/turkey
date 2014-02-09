
#include <iostream>
#include <cassert>

#include "model/game_state.h"
#include "model/model.h"



GameState::GameState(Model * model) {
    this->model = model;    

    // add the character!
    this->character = nullptr;   

    // the camera
    camera = new Camera();

    // pause state
    paused = false;
    pause_key_pressed = false;

    // create a particle system
    particle_system = nullptr; //new ParticleSystem()

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

    // keep a special list of game objects which will be removed after a while
    if (game_obj->ttl_in_secs >= 0.0f) {
        ttl_game_objs.push_back(game_obj);
    }
}

void GameState::add_particle_system(ParticleSystem * particle_system) {
    //partoc.push_back(game_obj);
    this->particle_system = particle_system;
}


void GameState::add_character_game_obj(GameObj * game_obj) {
    assert(game_obj->movable);
    this->add_game_obj(game_obj);
    this->character = game_obj;
}


void GameState::age_ttl_game_objs(float delta_time) {
    std::list<GameObj*>::iterator i;
    GameObj * game_obj;

    // reduce the ttl for the game_obj.. 
    for (i = ttl_game_objs.begin(); i != ttl_game_objs.end(); ) {
        game_obj = *i;
        game_obj->ttl_in_secs -= delta_time;

        std::cout << "ttl " << game_obj->ttl_in_secs << std::endl;

        if (game_obj->ttl_in_secs <= 0.0f) {

            // this object is dead jim..
            // the next time remove_dead_game_objs() is called it'll be erased from everywhere
            game_obj->dead = true;

            // remove from the ttl list straight away if it's dead (since we're here)
            i = ttl_game_objs.erase(i);
        }
        else {
            // otherwise check the next ttl object
            i++;
        }
    }
}



void GameState::remove_dead_game_objs() {
    std::list<GameObj*>::iterator i;
    GameObj * game_obj;

    for (i = movable_game_objs.begin(); i != movable_game_objs.end();) {
        game_obj = *i;
        if (game_obj->dead) {
            i = movable_game_objs.erase(i);
        }
        else {
            i++;
        }
    }

    for (i = immovable_game_objs.begin(); i != immovable_game_objs.end();) {
        game_obj = *i;
        if (game_obj->dead) {
            i = immovable_game_objs.erase(i);
        }
        else {
            i++;
        }
    }

    // cull game objects whose time to live is over before worrying about collisions
    for (i = game_objs.begin(); i != game_objs.end();) {
        game_obj = *i;
        if (game_obj->dead) {

            std::cout << "DEAD " << std::endl;
            i = game_objs.erase(i);
            delete game_obj;
        }
        else {
            i++;
        }
    }
}



std::list<GameObj*> const GameState::get_game_objs() const {
    return game_objs;
}



float GameState::get_time_since_last_update() {
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

    // and return the time since the last update in seconds
    return delta_time;
}


void GameState::update(const Uint8 * key_states) {
    std::list<GameObj*>::iterator i;
    GameObj * game_obj;

    // work out the user commands
    handle_keyboard(key_states);

    // don't update any positions if the game is paused
    if (paused) {
        return;
    }
    
    // first time through don't bother doing anything (no way to get a meaningful delta time)
    if (last_time_updated == 0) {
        last_time_updated = SDL_GetTicks();
        return;
    }

    // FIXME: use contact points?

    // get the time elapsed since the last update 
    float delta_time = get_time_since_last_update();

    // 
    age_ttl_game_objs(delta_time);

    remove_dead_game_objs();
    
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
       
    // how many steps do we have to take to get per pixel testing?
    float dt = delta_time / max_distance;

    // any moving object that can't possibly be in a collision can be moved 
    // its entire movement straight away.. no need to take incremental steps
    // looking for a collision. (we don't step these so it's faster)
    for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
        game_obj = *i;
        if (!game_obj->potential_collider) {
            game_obj->move(delta_time);
        }
    }
        
    // For each pairwise collision move the object as far as it can.
    // do this a number of times to avoid jitter..
    for (float t = 0.0f; t < delta_time; t += dt) {

        // calculate the projected positions of movable objects that might be in a collision
        for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
            game_obj = *i;
            
            if (game_obj->potential_collider) {
                // this object might be in a collision so work out where it's going
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

            if (key_states[SDL_SCANCODE_LALT] || key_states[SDL_SCANCODE_LALT]) {
                // alt left pans the camera right
                camera->pan_left();
            }
            else if (character) {
                character->accelerate_left();
                move_player_request = true;
            }
        }

        // move right?
        if (key_states[SDL_SCANCODE_RIGHT]) { 

            if (key_states[SDL_SCANCODE_LALT] || key_states[SDL_SCANCODE_LALT]) {
                // alt right pans the camera right
                camera->pan_right();
            }
            else if (character) {
                // right arrow moves the camera right
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


        // move up (camera only)?
        if (key_states[SDL_SCANCODE_UP]) { 

            if (key_states[SDL_SCANCODE_LALT] || key_states[SDL_SCANCODE_LALT]) {
                // alt right pans the camera right
                camera->pan_up();
            }
        }

        // move down (camera only)?
        if (key_states[SDL_SCANCODE_DOWN]) { 

            if (key_states[SDL_SCANCODE_LALT] || key_states[SDL_SCANCODE_LALT]) {
                // alt right pans the camera right
                camera->pan_down();
            }
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


void GameState::handle_mouse(const int x, const int y, const Uint8 mouse_button_state) {
    if (mouse_button_state & SDL_BUTTON(1)) {
        std::cout << "Mouse Button 1(left) is pressed.\n" << x << " , " << y << std::endl;

        particle_system->generate_particles(this, x, y);
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


// return the current camera position
void GameState::get_camera_position(float * camera_x, float * camera_y) const {
    camera->get_camera_position(camera_x, camera_y);
}
