#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SDL2/SDL_mixer.h"

class SoundManager {
 private:

    // the music that will be played 
    Mix_Music * music; 

    // the sound effects that will be used 
    Mix_Chunk * scratch; 
    Mix_Chunk * high; 
    Mix_Chunk * med; 
    Mix_Chunk * low; 

    // turn on sound (for development)
    bool sound_enabled;

 public:

    // initialize the sound manager
    int init();

    // load the sounds
    bool load();

    // clean up all sound resources
    int clean_up();
};

#endif
