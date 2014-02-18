#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <vector>
#include <string>

#include "SDL2/SDL_mixer.h"

class SoundManager {
 private:


    // keep a list of all music used
    std::vector<Mix_Music*> all_music;

    /* // the sound effects that will be used  */
    /* Mix_Chunk * scratch;  */
    /* Mix_Chunk * high;  */
    /* Mix_Chunk * med;  */
    /* Mix_Chunk * low;  */

    // turn on sound (for development)
    bool sound_enabled;

 public:

    // initialize the sound manager
    int init();

    // load the sounds
    //bool load();

    // load music (which is different from sound effects)
    Mix_Music * load_music(const std::string fname); 

    // play music
    void play_music(Mix_Music * music); 


    // clean up all sound resources
    int clean_up();
};

#endif
