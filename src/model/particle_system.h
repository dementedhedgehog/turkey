#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "model/i_game_obj_manager.h"
#include "constants.h"



class ParticleSystem  {
    friend class GameObj;

    SDL_Texture * texture;

 public:

    ParticleSystem(SDL_Texture * texture) {
        this->texture = texture;
    };


    void generate_particles(IGameObjManager * i_game_obj_manager, float x, float y) {        
        int n_particles = rand() % 5 + 5;
        
        GameObj * game_obj;
        for (int i = 0; i < n_particles; i++) {

            // create a particle
            const bool movable = true;
            game_obj = new GameObj(x, y, texture, movable);

            // particles explode out from (x, y) point
            game_obj->x_vel_per_sec = rand() % 1000 - 500;
            game_obj->y_vel_per_sec = rand() % 1000 - 500;

            // make particles float a bit
            game_obj->y_acc_per_sec = 0.5 * game_obj->y_acc_per_sec;

            // set a time to live
            game_obj->ttl_in_secs = 1.3f * (rand() % 100)/100.0f + 0.3f;
            
            // add it to the game
            i_game_obj_manager->add_game_obj(game_obj);
        }
    }
};

#endif
