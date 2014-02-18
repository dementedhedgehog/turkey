
#include "view/sound_manager.h"

#include <iostream>

#include "view/utils.h"


int SoundManager::init() {

    // play sound 
    sound_enabled = true;

    // returns 0 on success, -1 on error
    int result = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) ;
    if (result == -1) {
        log_msg(" *** Mix_OpenAudio failed to initialize! ***: " + 
                (std::string)Mix_GetError() + "\n");
        sound_enabled = false;
        result = 0;            
    }
    return result;
}


Mix_Music * SoundManager::load_music(const std::string fname) {
    Mix_Music * music = nullptr;

    // load the music 
    if (sound_enabled) {
        music = Mix_LoadMUS(fname.c_str()); 
        // if there was a problem loading the music 
        if (music == NULL) { 
            log_msg("Problem loading music file: " + fname);
        } 
        else {
            // add it to our list of music to free
            all_music.push_back(music);
        }
    }

    return music;
}


void SoundManager::play_music(Mix_Music * music) {    
    // play music forever, fading in over 2 seconds
    if (sound_enabled) {
        if(Mix_FadeInMusic(music, -1, 2000)==-1) {
            // well, there's no music, but most games don't break without music
            // so we'll carry on.
            log_mix_error("Mix_FadeInMusic:\n");
        }
    }
}

// bool SoundManager::load() {

//     // load the music 
//     if (sound_enabled) {

//         music = Mix_LoadMUS("./res/beat.wav"); 
//         // if there was a problem loading the music 
//         if (music == NULL) { 
//             return 8; 
//         } 
//         // load the sound effects 
//         scratch = Mix_LoadWAV("./res/scratch.wav"); 
//         high = Mix_LoadWAV("./res/high.wav"); 
//         med = Mix_LoadWAV("./res/medium.wav"); 
//         low = Mix_LoadWAV("./res/low.wav"); 
//         // if there was a problem loading the sound effects 
//         if ((scratch == NULL) || (high == NULL) || (med == NULL) || (low == NULL)) { 
//             return false; 
//         }

//     }

//     return true;
// }




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
    if (!sound_enabled) {
        return 0;
    }
        // Mix_FreeChunk(scratch); 
        // Mix_FreeChunk(high); 
        // Mix_FreeChunk(med); 
        // Mix_FreeChunk(low); 

        // // clean up the music 
        // Mix_FreeMusic(music); 

    // clean up all the textures we've allocated
    for (std::vector<Mix_Music *>::iterator i = all_music.begin(); i != all_music.end(); i++) {
        Mix_FreeMusic(*i); 
    }

    Mix_CloseAudio();

    return 0;
}
