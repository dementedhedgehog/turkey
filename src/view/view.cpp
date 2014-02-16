
#include <iostream>
#include <cassert>

#include "view/view.h"
#include "view/utils.h"
#include "view/fps.h"

// number of frames per second we want to draw
// 60 FPS is a good number.. it's widely considered that you can't see any more
// and the frame rate is often capped by other the graphics card at 60.
const uint32_t FRAMES_PER_SECOND = 60;

// number of moves to perform a second (best effort).
const uint32_t MOVES_PER_SECOND = 60;


View::View(Model * model, SDL_Window * window, SDL_Renderer * renderer,
    ImageManager * image_manager, FontManager * font_manager, SoundManager * sound_manager) :
    IStateListener(),
    BaseView(model, window, renderer, image_manager, font_manager, sound_manager) {

    assert(image_manager != NULL);

    // determine how long to wait between frames
    ms_per_frame = 1000 / FRAMES_PER_SECOND;
    ms_per_move = 1000 / MOVES_PER_SECOND;

    // enable fps counter?
    fps = NULL;

    // set this true to stop looping in the message loop and quit the application
    finished = false;

    //
    current_view = NULL;
}

// start up sdl
int View::init() {


    // success
    return 0;
}


// show an sdl window
int View::display_window() {

    assert(image_manager);

    // init the various game views
    intro_view = new IntroView(model, window, renderer, image_manager, font_manager, sound_manager);
    game_view = new GameView(model, window, renderer, image_manager, font_manager, sound_manager);
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

    // clean up the sdl objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

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
    uint32_t time_now;
    uint32_t last_frame_time = SDL_GetTicks();
    uint32_t last_move_time = last_frame_time;  


    // loop handling messages till someone exists the game 
    while (!finished) {

        // It's fine to poll here!  (trust me).
        while (!finished && SDL_PollEvent(&event)) {

            switch (event.type) {            
                // case SDL_KEYDOWN: {
                //     // Handle global key presses here.
                    
                //     /* Check the SDLKey values and move change the coords */
                //     switch (event.key.keysym.sym) {

                //         // case SDLK_F1: {
                //         //     // should toggle fullscreen in all views
                //         //     // don't overload this.
                //         //     toggle_fullscreen(window);
                //         //     break;
                //         // }

                //         case SDLK_ESCAPE: {
                //             // exit the game in all views
                //             // don't overload this.
                //             quit();
                //             break;
                //         }
                            
                //         default:
                //             break;
                //     }
                //     break;
                // }
                                            
                case SDL_MOUSEBUTTONDOWN: {
                    // Handle mouse clicks here.
                    std::cout << "mouse button down" << std::endl;

                    // get info about the mouse event
                    int mouse_x, mouse_y;
                    Uint32 mouse_button_states;
                    mouse_button_states = SDL_GetMouseState(&mouse_x, &mouse_y);

                    // and pass it to the model
                    model->handle_mouse(mouse_x, mouse_y, mouse_button_states);
                    break;
                }
                    
                case SDL_QUIT: {
                    // closing the game window quits the game..
                    quit();
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
                    //std::cout << "unknown event " << event.type << std::endl;
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

            // // pass the keyboard state to the current view for polling
            // const Uint8 * key_states = SDL_GetKeyboardState(NULL);
                
            // // move objects, calculate collisions, update stats etc
            // model->update(key_states);

            // move objects, calculate collisions, update stats etc
            model->update();

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
            fps = new FPS(font_manager->get_fps_font());
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


// Set the finished flag.   The next time through the messsage 
// loop we'll drop out of it and clean up and exit the application.
void View::quit() { 
    finished = true; 
}

// expose this to python
int View::add_scenery(float scroll_rate, float x, float y, SDL_Texture * texture) 
{
    return game_view->add_scenery(scroll_rate, x, y, texture);
}


// // returns a list of textures loaded from a sprite sheet with a regular layout of sprites
// std::vector<SDL_Texture*> * View::load_textures_from_sprite_sheet_using_grid(
//     const std::string & fname, 
//     SDL_Renderer * renderer,
//     unsigned int width, // of frame in pixels
//     unsigned int height, // of frame in pixels
//     unsigned int n_frames_wide = 0,
//     unsigned int n_frames_high = 0) {

    

// }

// // load a list of textures from an image using irregular shaped sprites
// std::vector<SDL_Texture*> * View::load_textures_from_sprite_sheet_using_rects(
//     const std::string & fname, 
//     SDL_Renderer * renderer,
//     std::vector<SDL_Rect*> rects) {

// }

