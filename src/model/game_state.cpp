
#include <iostream>
#include <cassert>

#include "model/game_state.h"
#include "model/model.h"

//
// Actions
//
class PanLeft: public IAction {
    Camera * camera;
    
 public:
    PanLeft(Camera * camera) { this->camera = camera; }

    void do_action() {
        camera->pan_left();
    }
};

class PanRight: public IAction {
    Camera * camera;
    
 public:
    PanRight(Camera * camera) { this->camera = camera; }

    void do_action() {
        camera->pan_right();
    }
};


class PanUp: public IAction {
    Camera * camera;
    
 public:
    PanUp(Camera * camera) { this->camera = camera; }

    void do_action() {
        camera->pan_up();
    }
};


class PanDown: public IAction {
    Camera * camera;
    
 public:
    PanDown(Camera * camera) { this->camera = camera; }

    void do_action() {
        camera->pan_down();
    }
};




// Character
class MoveLeft: public IAction {
    GameObj * character;
    
 public:
    MoveLeft(GameObj * character) { this->character = character; }

    void do_action() {
        character->accelerate_left();
    }
};


class MoveRight: public IAction {
    GameObj * character;
    
 public:
    MoveRight(GameObj * character) { this->character = character; }

    void do_action() {
        character->accelerate_right();
    }
};

class MoveUp: public IAction {
    GameObj * character;
    
 public:
    MoveUp(GameObj * character) { this->character = character; }

    void do_action() {
        character->move_up();
    }
};

class MoveDown: public IAction {
    GameObj * character;
    
 public:
    MoveDown(GameObj * character) { this->character = character; }

    void do_action() {
        character->move_down();
    }
};


class Jump: public IAction {
    GameObj * character;

    // need this to avoid strobing the jump button
    bool jump_key_pressed;
    
 public:
    Jump(GameObj * character) { 
        this->character = character; 

        // character jumping state
        jump_key_pressed = false;
    }

    void do_action() {
        //character->jump();

        if (!character->jumping && !jump_key_pressed) { 
            if (character) character->jump();
            character->jumping = true;
            jump_key_pressed = true;
        }
    }

    bool do_debounce() { return true; };
    void undo_action() {
        // jump key released (logic to avoid strobing the jump key)
        jump_key_pressed = false;
    };
};


class Pause: public IAction {
    GameState * game_state;

    // need this to avoid strobing the jump button
    bool pause_key_pressed;
    
 public:
    Pause(GameState * game_state) { 
        this->game_state = game_state; 

        // debounce the pause key state
        pause_key_pressed = false;
    }

    void do_action() {
        if (!pause_key_pressed) {
            pause_key_pressed = true;
            game_state->toggle_pause();
        }
    }

    bool do_debounce() { return true; };
    void undo_action() {
        // pause key released (logic to avoid strobing the pause key)
        pause_key_pressed = false;
    };
};





//
// Game State
//
GameState::GameState(Model * model) {
    this->model = model;    

    // add the character!
    this->character = nullptr;   

    // the camera
    camera = new Camera();

    // pause state
    paused = false;

    // create a particle system
    particle_system = nullptr; //new ParticleSystem()

    // start timing movement... now (movement = velocity * time).
    last_time_updated = 0;

    keyboard_handler->add_action(new PanRight(camera), SDL_SCANCODE_LEFT, SDL_SCANCODE_LALT);
    keyboard_handler->add_action(new PanRight(camera), SDL_SCANCODE_LEFT, SDL_SCANCODE_RALT);
    keyboard_handler->add_action(new PanLeft(camera), SDL_SCANCODE_RIGHT, SDL_SCANCODE_LALT);
    keyboard_handler->add_action(new PanLeft(camera), SDL_SCANCODE_RIGHT, SDL_SCANCODE_RALT);
    keyboard_handler->add_action(new PanUp(camera), SDL_SCANCODE_UP, SDL_SCANCODE_LALT);
    keyboard_handler->add_action(new PanUp(camera), SDL_SCANCODE_UP, SDL_SCANCODE_RALT);
    keyboard_handler->add_action(new PanDown(camera), SDL_SCANCODE_DOWN, SDL_SCANCODE_LALT);
    keyboard_handler->add_action(new PanDown(camera), SDL_SCANCODE_DOWN, SDL_SCANCODE_RALT);

    keyboard_handler->add_action(new Pause(this), SDL_SCANCODE_P);
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
    this->particle_system = particle_system;
}


void GameState::add_character_game_obj(GameObj * game_obj) {
    assert(game_obj->movable);
    add_game_obj(game_obj);
    character = game_obj;

    keyboard_handler->add_action(new MoveLeft(character), SDL_SCANCODE_LEFT);
    keyboard_handler->add_action(new MoveRight(character), SDL_SCANCODE_RIGHT);
    keyboard_handler->add_action(new MoveUp(character), SDL_SCANCODE_UP);
    keyboard_handler->add_action(new MoveDown(character), SDL_SCANCODE_DOWN);
    keyboard_handler->add_action(new Jump(character), SDL_SCANCODE_SPACE);

}


void GameState::age_ttl_game_objs(float delta_time) {
    std::list<GameObj*>::iterator i;
    GameObj * game_obj;

    // reduce the ttl for the game_obj.. 
    for (i = ttl_game_objs.begin(); i != ttl_game_objs.end(); ) {
        game_obj = *i;
        game_obj->ttl_in_secs -= delta_time;

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


//void GameState::update(const Uint8 * key_states) {
void GameState::update() {
    std::list<GameObj*>::iterator i;
    GameObj * game_obj;

    // reset the character deceleration (we decelerate unless the player is pressing
    // the left/right keys).
    if (character) character->reset_decelerate_flag(); 

    // work out the user commands
    keyboard_handler->do_actions();
    
    // Decelerate the player's sideways movement if left or right weren't pressed
    if (character) character->decelerate_character(); 
    
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

    // garbage collect dead objects
    age_ttl_game_objs(delta_time);
    remove_dead_game_objs();


    //
    // BROAD PHASE
    // determines which pairs of shapes need to be tested for collision
    //
    
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
        
    //
    // NARROW PHASE
    // determines collision results for each pair of potential collisions identified in the 
    // previous broad phase
    //
    
    // For each pairwise collision move the object as far as it can.
    //                                            do this a number of times to avoid jitter.. XXXX
    for (float t = 0.0f; t < delta_time; t += dt) {

        // calculate the projected positions of movable objects that might be in a collision
        for (i = movable_game_objs.begin(); i != movable_game_objs.end(); i++) {
            game_obj = *i;
            
            // this object might be in a collision so work out where it's going
            if (game_obj->potential_collider) {
                game_obj->calc_projected_delta_position(dt);
            }
        }
        
        // find and resolve each *movable* collision 
        // (collisions between two movable objects)
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
        // (collisions between a movable object and a fixed object)
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



void GameState::handle_mouse(const int x, const int y, const Uint8 mouse_button_state) {
    if (mouse_button_state & SDL_BUTTON(1)) {
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
}


// return the current camera position
void GameState::get_camera_position(float * camera_x, float * camera_y) const {
    camera->get_camera_position(camera_x, camera_y);
}


// toggle game pause
void GameState::toggle_pause() {
    paused = not paused;
}
