
#include <iostream>
#include <cassert>

#include "view/view.h"
#include "view/utils.h"
#include "view/fps.h"

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


View::View() {

    // determine how long to wait between frames
    ms_per_frame = 1000 / FRAMES_PER_SECOND;
    ms_per_move = 1000 / MOVES_PER_SECOND;

    // set the default value to run in fullscreen mode 
    //fullscreen = false;    

    // enable fps counter?
    fps = NULL;

    font_manager = FontManager();
    
    // play sound 
    sound_enabled = false;

    // set this true to stop looping in the message loop and quit the application
    finished = false;

    //
    current_view = NULL;
}

// View::~View() {

// }



// start up sdl
int View::init_sdl() {

    // init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_sdl_error("SDL_Init");
		return 1;
	}
    
    // init the img reading library
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        log_sdl_error("IMG_Init");
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

    font_manager.init();

    // setup the game views

    // success
    return 0;
}


// show an sdl window
int View::display_window(Model * model) {

    // keep a referemce to the model
    this->model = model;

    // create the window
    window = SDL_CreateWindow(GAME_NAME, 
        SCREEN_POS_X, SCREEN_POS_Y, 
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        log_sdl_error("CreateWindow");
        return 2;
    }

    // get a renderer for the window
    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        log_sdl_error("CreateRenderer");
        return 3;
    }

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

    // init the various game views
    intro_view = new IntroView(model, window, renderer);
    game_view = new GameView(model, window, renderer);
    current_view = intro_view;        

    // init the various renderers
    int result;
    if ((result = intro_view->init()) != 0) {
        return result;
    }

    if ((result = game_view->init()) != 0) {
        return result;
    }
        
    // draw the screen
    render();

    // success
    return 0; 
}


// draw to the screen
int View::render() {

    // clear the display
    SDL_RenderClear(renderer);

    // draw the screen for the current view
    current_view->render(); 
    
    // increment frames per second 
    if (fps) {
        fps->increment(renderer);
    }

    // display what we've just rendered
    SDL_RenderPresent(renderer);
        
    // success
    return 0;
}



// free everything.
int View::clean_up() {
   
    // clean up the view specific resources
    intro_view->clean_up();
    game_view->clean_up();

    // cleanup the sound effects 
    if (sound_enabled) {
        Mix_FreeChunk(scratch); 
        Mix_FreeChunk(high); 
        Mix_FreeChunk(med); 
        Mix_FreeChunk(low); 

        // clean up the music 
        Mix_FreeMusic(music); 
    }

    // clean up the font manager
    font_manager.clean_up();

    // clean up the ttf library
    TTF_Quit();
   
    // clean up the img library
    IMG_Quit();

    // clean up the sdl objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();

    // clean up the frame-per-second counter
    if (fps) {
        delete fps;
        fps = NULL;
    }

    // success
    return 0; 
}


int View::msg_loop() {    
    SDL_Event event;
    uint time_now;
    uint last_frame_time = SDL_GetTicks();
    uint last_move_time = last_frame_time;  

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


    // loop handling messages till someone exists the game 
    while (!finished) {

        // It's fine to poll here!  (trust me).
        while (!finished && SDL_PollEvent(&event)) {

            switch (event.type) {            
                case SDL_KEYDOWN: {
                    // Handle global key presses here.
                    
                    /* Check the SDLKey values and move change the coords */
                    switch (event.key.keysym.sym) {

                        case SDLK_F1: {
                            // should toggle fullscreen in all views
                            toggle_fullscreen(window);
                            break;
                        }

                        case SDLK_ESCAPE: {
                            // exit the game in all views
                            // don't overload this.
                            quit();
                            break;
                        }
                            
                        default:
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

                case SDL_WINDOWEVENT_FOCUS_GAINED: {
                    //SDL_Log("Window %d gained keyboard focus", event->window.windowID);
                    model->set_keyboard_focus(true);
                    break;
                }

                case SDL_WINDOWEVENT_FOCUS_LOST: {
                    //SDL_Log("Window %d lost keyboard focus", event->window.windowID);
                    model->set_keyboard_focus(false);
                    break;
                }
                        
                default: {
                    std::cout << "unknown event " << event.type << std::endl;
                    break;
                }                
            }
        }

        //
        // FIXME: I think we want to tune these two rates on the fly to get nice behaviour
        //
            
        // try to keep a constant number of moves per second
        time_now = SDL_GetTicks();
        
        if (time_now - last_move_time >= ms_per_move) {

            // pass the keyboard state to the current view for polling
            const Uint8 * key_states = SDL_GetKeyboardState(NULL);
                
            // move objects, calculate collisions, update stats etc
            model->update(key_states);

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
    return 0;
}


void View::state_changed(State old_state, State current_state) {
    
    std::cout << "state changed!!!!!!!!!!!!!!!!" << std::endl;
        
    switch (current_state) {
        case State::INTRO:
            std::cout << "intro!!" << std::endl;
            current_view = intro_view;
            break;
            
        case State::GAME:
            std::cout << "render!!" << std::endl;
            current_view = game_view;
            break;

        case State::FATAL_ERROR:
        default:
            // log an error and bail
            log_sdl_error("Unknown State!");
            finished = true;
            break;
    }
}



SDL_Renderer * View::get_renderer() {
    return renderer;
}


void View::debug_set_draw_fps(const bool draw_fps) { 

    // turn on fps (if it isn't already on)
    if (draw_fps) {
        if (fps == NULL) {        
            fps = new FPS(font_manager.get_fps_font());
        }
    }
    // turn off fps (if it's already on)
    else {
        if (fps) {
            delete fps;
            fps = NULL;
        }
    }
} 
