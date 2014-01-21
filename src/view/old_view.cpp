
#include <iostream>
#include <cassert>

#include "view.h"
#include "utils.h"

// screen size
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

// grid size..
const int GRID_CELL_WIDTH  = 16;
const int GRID_CELL_HEIGHT = GRID_CELL_WIDTH;

// game name string
const char * GAME_NAME = "Turkey";

// event used to trigger a redraw
const int USER_EVENT_DRAW_FRAME = 1;

// number of frames per second we want to draw
// FIXME: get this from a configuration
//const uint TARGET_FRAMES_PER_SECOND = 90;
const uint TARGET_FRAMES_PER_SECOND = 30;


View::View(Control * control) {
    control = control;

    // determine how long to wait between frames
    ms_per_frame = 1000 / TARGET_FRAMES_PER_SECOND;
    assert(ms_per_frame > 15);

    // init character position 
    // FIXME: move this stuff into a sprite or some other object
    character_pos = {3, 3}; 
    character_width = 35;
    character_height = 36; 

    // enable fps counter?
    //fps = new FPS();
}


std::string View::getName() {
    return "Edgar";
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
        return 1;
    }

    // success
    return 0;
}


// show an sdl window
int View::display_window() {

    // create the window
    window = SDL_CreateWindow(GAME_NAME, 
        100, 100, SCREEN_WIDTH, SCREEN_HEIGHT,
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

    // FIXME: all this should be done using config files? 
    background = loadTexture("./res/dungen.png", renderer);
    background_parallax = loadTexture("./res/dungen_parallax.png", renderer);
    if (background == nullptr || background_parallax == nullptr) {
        logSDLError(std::cout, "LoadBMP");
        return 4;
    }

    // load the main character image
    character = loadTexture("./res/dwarf_sprite.png", renderer);

    // draw the screen
    render();

    // success
    return 0; 
}


// draw to the screen
int View::render() {

    // clear the display
    SDL_RenderClear(renderer);

    // tile background
    int backgroundWidth, backgroundHeight;
    SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
    renderTexture(background, renderer, 0, 0);
    renderTexture(background, renderer, backgroundWidth, 0);
    renderTexture(background, renderer, 0, backgroundHeight);
    renderTexture(background, renderer, backgroundWidth, backgroundHeight);

    // draw parallax background
    int iW, iH;
    SDL_QueryTexture(background_parallax, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(background_parallax, renderer, x, y);

    // draw the character    
    x = GRID_CELL_WIDTH * character_pos.x - character_width / 2;
    y = GRID_CELL_HEIGHT * character_pos.y - character_height / 2;
    renderTexture(character, renderer, x, y);

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

    // move stuff
    character_pos.x += character_xvel;
    character_pos.y += character_yvel;

    // std::cout << "pos " << character_pos.x << "," << character_pos.y 
    //           << "vel " << character_xvel << "," << character_yvel << std::endl;


    // success
    return 0;
}


// free everything.
int View::clean_up() {

    // clean up the sdl objects
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(background_parallax); 
    SDL_DestroyTexture(character); 
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
    bool done = false;
    int result = 0;
    SDL_TimerID timer_id;

    // set up the timer to send an event to the event loop periodically to trigger a 
    // frame redraw
    timer_id = SDL_AddTimer(ms_per_frame, &push_redraw_timer_event, this);

    std::cout <<  ms_per_frame << std::endl;
    // return 0;

    // loop handling messages till someone exists the game 
    while (!done && SDL_WaitEvent(&event)) {
        switch(event.type) {
            
            // handle timer events!
            case SDL_USEREVENT:

                switch (event.user.code) {
                    case USER_EVENT_DRAW_FRAME: {
                        uint64_t delta_t;

                        delta_t = uint64_t(SDL_GetTicks()) - uint64_t(event.user.data1);
                        // FIXME:
                        std::cout << "draw " << delta_t << std::endl;

                        // move objects
                        move();

                        // draw a single frame
                        render();
                        break;
                    }
            
                    default: {
                        // weird should never get here! fail fast!
                        std::cout << "unknown user event! " << event.user.code << std::endl;
                        result = 2;
                        done = true;
                    }
                }
                break;
                
            case SDL_KEYDOWN: {
                // Handle any key presses here.
                //std::cout << "key pressed" << std::endl;

                /* Check the SDLKey values and move change the coords */
                switch (event.key.keysym.sym) {

                    case SDLK_LEFT:
                    case SDLK_d:
                        character_xvel = -1;
                        std::cout << "left" << std::endl;
                        break;

                    case SDLK_RIGHT:
                    case SDLK_a:
                        character_xvel = +1;
                        std::cout << "-----------------------------------" << std::endl;
                        std::cout << "right" << std::endl;
                        break;

                    case SDLK_UP:
                    case SDLK_w:
                        character_yvel = -1;
                        break;

                    case SDLK_DOWN:
                    case SDLK_x:
                        character_yvel = +1;
                        break;

                    case SDLK_ESCAPE:
                        done = true;
                        break;

                    default:
                        break;
                }
                break;
            }

            /* We must also use the SDL_KEYUP events to zero the x */
            /* and y velocity variables. But we must also be       */
            /* careful not to zero the velocities when we shouldn't*/
            case SDL_KEYUP: {
                switch( event.key.keysym.sym ){

                    case SDLK_LEFT:
                    case SDLK_a:
                        /* We check to make sure the character is moving */
                        /* to the left. If it is then we zero the    */
                        /* velocity. If the character is moving to the   */
                        /* right then the right key is still press   */
                        /* so we don't tocuh the velocity            */
                        if (character_xvel < 0) character_xvel = 0;
                        std::cout << "XXX left" << std::endl;
                        break;

                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (character_xvel > 0) character_xvel = 0;
                        std::cout << "XXX right" << std::endl;
                        break;

                    case SDLK_UP:
                    case SDLK_w:
                        if (character_yvel < 0) character_yvel = 0;
                        break;

                    case SDLK_DOWN:
                    case SDLK_x:
                        if (character_yvel > 0) character_yvel = 0;
                        break;

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
                done = true;
                break;
            }
                
            default: {
                std::cout << "unknown event" << std::endl;
                break;
            }                
        }            
    }

    // clean up the timer
    if (!SDL_RemoveTimer(timer_id)) {
        // failure!
        logSDLError(std::cout, "msg loop, freeing timer");
        result = 1;        
    }

    // success
    return result;
}


// Timer callback, run on a separate thread from the UI thread!
unsigned int View::push_redraw_timer_event(unsigned int interval, void* param)
{
    // Create a user event to call the game loop.
    SDL_Event event;
    
    event.type = SDL_USEREVENT;
    event.user.code = USER_EVENT_DRAW_FRAME;
    event.user.data1 = (void*)(uint64_t)SDL_GetTicks();
    //event.user.data1 = 0;
    event.user.data2 = 0;    
    if (SDL_PushEvent(&event)) {
        std::cout << "Event queue is full, dropping refresh!" << std::endl;
    }
    else {
        std::cout << "pushed event" << std::endl;
    }

    return interval;
}
