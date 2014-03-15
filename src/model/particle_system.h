#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "model/i_game_obj_manager.h"
#include "model/game_obj_type.h"
#include "constants.h"



class ParticleSystem  {
    friend class GameObj;

    SDL_Texture * texture;

 public:

    ParticleSystem(SDL_Texture * texture) {
        this->texture = texture;
    };


    void generate_particles(IGameObjManager * i_game_obj_manager, float x, float y) {        
        //int n_particles = rand() % 5 + 5;
        int n_particles = rand() % 10 + 5;
        //int n_particles = 1;
        
        GameObj * game_obj;
        for (int i = 0; i < n_particles; i++) {

            // create a particle
            //const bool movable = true;
            game_obj = new GameObj(x, y, FIREBALL, texture); //, movable);

            // particles explode out from (x, y) point
            game_obj->x_velocity = rand() % 1000 - 500;
            game_obj->y_velocity = rand() % 1000 - 500;

            // make particles float a bit
            game_obj->y_acc_per_sec = 0.5 * game_obj->y_acc_per_sec;

            // set a time to live
            game_obj->ttl = 1.3f * (rand() % 100)/100.0f + 0.3f;
            
            // add it to the game
            i_game_obj_manager->add_game_obj(game_obj);
        }
    }
};

#endif
