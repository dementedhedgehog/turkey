
#include "view/sound_manager.h"

#include <iostream>

#include "view/utils.h"


int SoundManager::init() {

    // play sound 
    sound_enabled = false;

    // returns 0 on success, -1 on error
    int result = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) ;

    // sound_enabled && 
    if (result == -1) {
        log_msg("Mix_OpenAudio: \n" + (std::string)Mix_GetError() + "\n");
    }
    return result;
}


bool SoundManager::load() {

    // load the music 
    if (sound_enabled) {

        music = Mix_LoadMUS("./res/beat.wav"); 
        // if there was a problem loading the music 
        if (music == NULL) { 
            return 8; 
        } 
        // load the sound effects 
        scratch = Mix_LoadWAV("./res/scratch.wav"); 
        high = Mix_LoadWAV("./res/high.wav"); 
        med = Mix_LoadWAV("./res/medium.wav"); 
        low = Mix_LoadWAV("./res/low.wav"); 
        // if there was a problem loading the sound effects 
        if ((scratch == NULL) || (high == NULL) || (med == NULL) || (low == NULL)) { 
            return false; 
        }

    }

    return true;
}


// bool SoundManager::load(std::string fname) {


// }

//         case SDLK_0:
//             current_view->key_0_down();

//             // if 0 was pressed stop the music 
//             if (sound_enabled) {
//                 Mix_HaltMusic(); 
//             }
//             break;

//         case SDLK_1: 
//             current_view->key_1_down();

//             // if 1 was pressed play the scratch effect 
//             if (sound_enabled) {
//                 if (Mix_PlayChannel(-1, scratch, 0 ) == -1) { 
//                     return 1; 
//                 } 
//             }
//             break;

//         case SDLK_2: 
//             current_view->key_2_down();

//             // if 2 was pressed play the high hit effect 
//             if (sound_enabled) {
//                 if (Mix_PlayChannel(-1, high, 0) == -1) { 
//                     return 1; 
//                 } 
//             }
//             break;

//         case SDLK_3: 
//             current_view->key_3_down();

//             if (sound_enabled) {
//                 // if 3 was pressed play the medium hit effect 
//                 if (Mix_PlayChannel(-1, med, 0) == -1) {
//                     return 1; 
//                 } 
//             }
//             break;

//         case SDLK_4: 
//             current_view->key_4_down();

//             if (sound_enabled) {
//                 // if 4 was pressed play the low hit effect 
//                 if (Mix_PlayChannel(-1, low, 0) == -1) { 
//                     return 1; 
//                 } 
//             }
//             break;
                            
//         case SDLK_9: 
//             current_view->key_9_down();

//             // if 9 was pressed and there is no music playing 
//             if (sound_enabled) {
//                 if (Mix_PlayingMusic() == 0) {
//                     // play the music 
//                     if (Mix_PlayMusic(music, -1) == -1) { 
//                         return 1; 
//                     } 
//                 }
//             }
//             else { 
//                 // music is being played...
//                 if (sound_enabled) {
                                    
//                     // if the music is paused 
//                     if (Mix_PausedMusic() == 1) { 
//                         // resume the music 
//                         Mix_ResumeMusic(); 
//                     } 
//                     // If the music is currently playing 
//                     else { 
//                         // pause the music 
//                         Mix_PauseMusic(); 
//                     } 
//                 }
//             }
//             break;




int SoundManager::clean_up() {
    if (sound_enabled) {
        Mix_FreeChunk(scratch); 
        Mix_FreeChunk(high); 
        Mix_FreeChunk(med); 
        Mix_FreeChunk(low); 

        // clean up the music 
        Mix_FreeMusic(music); 
    }

    return 0;
}
