
#include <iostream>
#include <cassert>

#include "view.h"
#include "utils.h"

// FIXME: get these values from a configuration

// screen size
const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 800;

// screen pos (from top left)
const int SCREEN_POS_X = 50;
const int SCREEN_POS_Y = 20;

// game name string
const char * GAME_NAME = "Turkey";

// number of frames per second we want to draw
// 60 FPS is a good number.. it's widely considered that you can't see any more
// and the frame rate is often capped by other the graphics card at 60.
const uint FRAMES_PER_SECOND = 60;

// number of moves to perform a second (best effort).
const uint MOVES_PER_SECOND = 60;


View::View(Control * control) {
    control = control;

    // determine how long to wait between frames
    ms_per_frame = 1000 / FRAMES_PER_SECOND;
    ms_per_move = 1000 / MOVES_PER_SECOND;

    // set the default value to run in fullscreen mode 
    //fullscreen = false;    

    // enable fps counter?
    //fps = new FPS();
    fps = NULL;
    
    // play sound 
    sound_enabled = false;

    // set this true to stop looping in the message loop and quit the application
    finished = false;

    // setup the game components
    intro = new IntroComponent(control);
    game = new GameComponent(control);
    current_component = intro;

}


// start up sdl
int View::init_sdl() {

    // init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
		return 1;
	}
    
    // init the img reading library
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        logSDLError(std::cout, "IMG_Init");
        return 2;
    }

    // init the font library
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: %s\n" << TTF_GetError() << std::endl;
        return 3;
    }

    // init tne SDL_mixer library - for sound 
    if (sound_enabled && (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)) { 
        std::cerr << "Mix_OpenAudio: \n" << Mix_GetError() << std::endl;
        return 4; 
    }

    // success
    return 0;
}


// show an sdl window
int View::display_window() {

    // create the window
    window = SDL_CreateWindow(GAME_NAME, 
        SCREEN_POS_X, SCREEN_POS_Y, 
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        return 2;
    }

    // get a renderer for the window
    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "CreateRenderer");
        return 3;
    }

    // background_parallax = loadTexture("./res/dungen_parallax.png", renderer);
    // if (background_parallax == nullptr) {
    //     logSDLError(std::cout, "LoadBMP");
    //     return 4;
    // }


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
            return 9; 
        }
    }
        

    // FIXME: 
    intro->init(renderer); // RENAME!!
    game->init(renderer); // RENAME!!
    
    // draw the screen
    render();

    // success
    return 0; 
}


// draw to the screen
int View::render() {

    // clear the display
    SDL_RenderClear(renderer);

    // draw the screen for the current component
    current_component->render(renderer, window); 

    // display what we've just rendered
    SDL_RenderPresent(renderer);

    // increment frames per second 
    if (fps) {
        fps->increment();
    }
        
    // success
    return 0;
}


// draw to the screen
int View::move() {
    current_component->move();

    // success
    return 0;
}


// free everything.
int View::clean_up() {

    // cleanup the sound effects 
    if (sound_enabled) {
        Mix_FreeChunk(scratch); 
        Mix_FreeChunk(high); 
        Mix_FreeChunk(med); 
        Mix_FreeChunk(low); 

        // clean up the music 
        Mix_FreeMusic(music); 
    }

 	
    // clean up the ttf library
    TTF_Quit();
   
    // clean up the img library
    IMG_Quit();

    // clean up the component specific resources
    intro->clean_up();
    game->clean_up();

    // clean up the sdl objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();

    // clean up the frame-per-second counter
    if (fps) {
        delete fps;
    }

    // success
    return 0; 
}


int View::msg_loop() {    
    SDL_Event event;
    int result = 0;
    uint time_now;
    uint last_frame_time = SDL_GetTicks();
    uint last_move_time = last_frame_time;
   
    // loop handling messages till someone exists the game 
    while (!finished) {

        // It's fine to poll here!  (trust me).
        while (!finished && SDL_PollEvent(&event)) {

            switch (event.type) {            
                case SDL_KEYDOWN: {
                    // Handle any key presses here.

                    /* Check the SDLKey values and move change the coords */
                    switch (event.key.keysym.sym) {

                        case SDLK_LEFT:
                            current_component->key_left_down();
                            break;

                        case SDLK_RIGHT:
                            current_component->key_right_down();
                            break;

                        case SDLK_UP:
                            current_component->key_up_down();
                            break;

                        case SDLK_DOWN:
                            current_component->key_down_down();
                            break;

                        case SDLK_a:
                            current_component->key_a_down();
                            break;

                        case SDLK_d:
                            current_component->key_d_down();
                            break;

                        case SDLK_w:
                            current_component->key_w_down();
                            break;

                        case SDLK_x:
                            current_component->key_x_down();
                            break;

                        case SDLK_F1:
                            // should toggle fullscreen
                            current_component->key_f1_down();
                            break;

                        case SDLK_ESCAPE: 
                            current_component->key_d_down();
                            // don't overload this.. handle it here?
                            finished = true;
                            break;

                        case SDLK_0:
                            current_component->key_0_down();

                            // if 0 was pressed stop the music 
                            if (sound_enabled) {
                                Mix_HaltMusic(); 
                            }
                            break;

                        case SDLK_1: 
                            current_component->key_1_down();

                            // if 1 was pressed play the scratch effect 
                            if (sound_enabled) {
                                if (Mix_PlayChannel(-1, scratch, 0 ) == -1) { 
                                    return 1; 
                                } 
                            }
                            break;

                        case SDLK_2: 
                            current_component->key_2_down();

                            // if 2 was pressed play the high hit effect 
                            if (sound_enabled) {
                                if (Mix_PlayChannel(-1, high, 0) == -1) { 
                                    return 1; 
                                } 
                            }
                            break;

                        case SDLK_3: 
                            current_component->key_3_down();

                            if (sound_enabled) {
                                // if 3 was pressed play the medium hit effect 
                                if (Mix_PlayChannel(-1, med, 0) == -1) {
                                    return 1; 
                                } 
                            }
                            break;

                        case SDLK_4: 
                            current_component->key_4_down();

                            if (sound_enabled) {
                                // if 4 was pressed play the low hit effect 
                                if (Mix_PlayChannel(-1, low, 0) == -1) { 
                                    return 1; 
                                } 
                            }
                            break;
                            
                        case SDLK_9: 
                            current_component->key_9_down();

                            // if 9 was pressed and there is no music playing 
                            if (sound_enabled) {
                                if (Mix_PlayingMusic() == 0) {
                                    // play the music 
                                    if (Mix_PlayMusic(music, -1) == -1) { 
                                        return 1; 
                                    } 
                                }
                            }
                            else { 
                                // music is being played...
                                if (sound_enabled) {
                                    
                                    // if the music is paused 
                                    if (Mix_PausedMusic() == 1) { 
                                        // resume the music 
                                        Mix_ResumeMusic(); 
                                    } 
                                    // If the music is currently playing 
                                    else { 
                                        // pause the music 
                                        Mix_PauseMusic(); 
                                    } 
                                }
                            }
                            break;

                        default:
                            break;
                    }
                    break;
                    

                    /* We must also use the SDL_KEYUP events to zero the x */
                    /* and y velocity variables. But we must also be       */
                    /* careful not to zero the velocities when we shouldn't*/
                    case SDL_KEYUP: {
                        switch (event.key.keysym.sym) {

                            case SDLK_LEFT:
                                current_component->key_left_up();
                                break;

                            case SDLK_RIGHT:
                                current_component->key_right_up();
                                break;

                            case SDLK_UP:
                                current_component->key_up_up();
                                break;

                            case SDLK_DOWN:
                                current_component->key_down_up();
                                break;

                            case SDLK_a:
                                current_component->key_a_up();
                                break;

                            case SDLK_d:
                                current_component->key_d_up();
                                break;

                            case SDLK_w:
                                current_component->key_w_up();
                                break;

                            case SDLK_x:
                                current_component->key_x_up();
                                break;
                            
                            default: 
                                std::cout << "we don't handle " << 
                                    SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                                break;
                        }
                        break;
                    }
                        
                    case SDL_MOUSEBUTTONDOWN: {
                        // Handle mouse clicks here.
                        std::cout << "mouse button down" << std::endl;
                        break;
                    }

                    case SDL_QUIT: {
                        // closing the game window quits the game..
                        finished = true;
                        break;
                    }
                        
                    default: {
                        std::cout << "unknown event " << event.type << std::endl;
                        break;
                    }                
                }
                break;
            }
        }
            
        // try to keep a constant number of moves per second
        time_now = SDL_GetTicks();
        if (time_now - last_move_time >= ms_per_move) {
                
            // move objects
            move(); // FIXME RENAME

            // reset the move time
            last_move_time = time_now;
        }
            
        // try and render at a constant number of frames per second
        if (time_now - last_frame_time >= ms_per_frame) {

            // draw a single frame
            render();

            // reset the render time
            last_frame_time = time_now;
        }

        // this delay is so we don't hog cpu usage if we don't need to.
        SDL_Delay(1);
    }

    // success
    return result;
}


void View::state_changed(State old_state, State current_state) {
    
    std::cout << "state changed!!!!!!!!!!!!!!!!" << std::endl;
        
    switch (current_state) {
        case State::INTRO:
            std::cout << "intro!!" << std::endl;
            current_component = intro;
            break;
            
        case State::GAME:
            std::cout << "render!!" << std::endl;
            current_component = game;
            break;

        case State::FATAL_ERROR:
        default:
            // log an error and bail
            logSDLError(std::cout, "Unknown State!");
            finished = true;
            //return 2;
            break;
    }
}

