
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

// grid
const int N_CELLS_WIDE = 80;
const int N_CELLS_HIGH = 50;
// const int N_CELLS_WIDE = 40;
// const int N_CELLS_HIGH = 25;

// grid size..
const int GRID_CELL_WIDTH  = SCREEN_WIDTH / N_CELLS_WIDE;
const int GRID_CELL_HEIGHT = SCREEN_HEIGHT / N_CELLS_HIGH;
    

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

    // init character position 
    // FIXME: move this stuff into a sprite or some other object
    character_pos = {3, 3}; 
    character_width = 35;
    character_height = 36; 

    // set the default value to run in fullscreen mode 
    fullscreen = false;    

    // enable fps counter?
    //fps = new FPS();
    fps = NULL;
    
    // display the cell grid.. for development and debugging
    grid_enabled = true;

    // play sound 
    sound_enabled = false;

    character_xvel = 0;
    character_yvel = 0;

    // setup the game components
    intro = new IntroComponent();
    game = new GameComponent();
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

    // FIXME: all this should be done using config files? 
    background = loadTexture("./res/background.jpg", renderer);
    if (background == nullptr) {
        logSDLError(std::cout, "LoadBMPX");
        return 4;
    }


    // load font.ttf at size 16 into font
    //font = TTF_OpenFont("./res/verdana.ttf", 128);
    font = TTF_OpenFont("./res/hobbiton_brush_hand.ttf", 128);
    if (!font) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        return 5;
    }

    // 
    SDL_Color fg = { 255, 255, 0, 255 };
    //text = TTF_RenderText_Solid(font, "frog", fg);
    text = TTF_RenderText_Blended(font, "meow", fg);
    if (!text) {
        std::cout << "TTF_RenderText: " << TTF_GetError() << std::endl;
        return 6;
    }

    texture = SDL_CreateTextureFromSurface(renderer, text);
    if (!texture) {
        std::cout << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return 7;
    }        
        

    // background_parallax = loadTexture("./res/dungen_parallax.png", renderer);
    // if (background_parallax == nullptr) {
    //     logSDLError(std::cout, "LoadBMP");
    //     return 4;
    // }

    // load the main character image
    character = loadTexture("./res/dwarf.png", renderer);

    // load a block of stone..
    stone = loadTexture("./res/stone.png", renderer);

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
    current_component->init(renderer); // RENAME!!
    
    // draw the screen
    render();

    // success
    return 0; 
}


// draw to the screen
int View::render() {
    int x, y;

    // clear the display
    SDL_RenderClear(renderer);

    // tile background
    int backgroundWidth, backgroundHeight;
    SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
    renderTexture(background, renderer, 0, 0);
    // renderTexture(background, renderer, backgroundWidth, 0);
    // renderTexture(background, renderer, 0, backgroundHeight);
    // renderTexture(background, renderer, backgroundWidth, backgroundHeight);

    // draw parallax background
    // int iW, iH;
    // SDL_QueryTexture(background_parallax, NULL, NULL, &iW, &iH);
    // int x = SCREEN_WIDTH / 2 - iW / 2;
    // int y = SCREEN_HEIGHT / 2 - iH / 2;
    // renderTexture(background_parallax, renderer, x, y);


    // draw the cell lines
    if (grid_enabled) {
        
        int sx;
        for (x = 0; x < N_CELLS_WIDE; x++) {
            sx = x * GRID_CELL_WIDTH;         
            SDL_RenderDrawLine(renderer, sx, 0, sx , SCREEN_HEIGHT);
        }

        int sy;
        for (y = 0; y < N_CELLS_HIGH; y++) {
            sy = y * GRID_CELL_HEIGHT;
            SDL_RenderDrawLine(renderer, 0, sy, SCREEN_WIDTH, sy);
        }
    }
                

    // hard coded dungen
    int sx;
    y = 17 * GRID_CELL_HEIGHT;
    for (x = 0; x < 15; x += 1) {
        sx = x * GRID_CELL_WIDTH;
        renderTexture(stone, renderer, sx, y);
    }

    y = 23 * GRID_CELL_HEIGHT;
    for (x = 26; x < 35; x += 1) {
        sx = x * GRID_CELL_WIDTH;
        renderTexture(stone, renderer, sx, y);
    }

    y = 13 * GRID_CELL_HEIGHT;
    for (x = 22; x < 27; x += 1) {
        sx = x * GRID_CELL_WIDTH;
        renderTexture(stone, renderer, sx, y);
    }


    // draw the character    
    x = GRID_CELL_WIDTH * character_pos.x - character_width / 2;
    y = GRID_CELL_HEIGHT * character_pos.y - character_height / 2;
    renderTexture(character, renderer, x, y);

    // draw some text
    //SDL_Rect destination_rect = { info->w, info->h, 0, 0 };
    //SDL_BlitSurface(info, 0, place, &destination_rect);
    //SDL_Rect dest = { 100, 100, 0, 0 };
    //SDL_BlitSurface(text, 0, place, &dest);
    //SDL_BlitSurface(text, NULL, screen, &dest);

    SDL_Rect src = { 0, 0, text->w, text->h };
    SDL_Rect dest = { 30, 30, text->w, text->h};
    // SDL_Rect dst = { 0, 0, 10, 10}; //textureX->w, textureX->h };
    //SDL_RenderCopy(renderer, texture, &src, &dst);
    SDL_RenderCopy(renderer, texture, &src, &dest);


    current_component->render(renderer, window); // RENAME!!

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

    // success
    return 0;
}


// free everything.
int View::clean_up() {

    SDL_FreeSurface(text);
    SDL_DestroyTexture(texture);

    // cleanup the sound effects 
    if (sound_enabled) {
        Mix_FreeChunk(scratch); 
        Mix_FreeChunk(high); 
        Mix_FreeChunk(med); 
        Mix_FreeChunk(low); 

        // clean up the music 
        Mix_FreeMusic(music); 
    }

    // clean up the font 
    TTF_CloseFont(font);
    font = NULL; // to be safe...
 	
    // clean up the ttf library
    TTF_Quit();
   
    // clean up the img library
    IMG_Quit();

    // clean up the sdl objects
    SDL_DestroyTexture(background);
    //SDL_DestroyTexture(background_parallax); 
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
    uint time_now;
    uint last_frame_time = SDL_GetTicks();
    uint last_move_time = last_frame_time;

    //std::cout <<  ms_per_frame << std::endl;
   
    // loop handling messages till someone exists the game 
    while (!done) {

        // It's fine to poll here!  (trust me).
        while (!done && SDL_PollEvent(&event)) {
            switch(event.type) {
            
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
                            //std::cout << "-----------------------------------" << std::endl;
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

                        case SDLK_F1:
                            std::cout << "toggle fullscreen" << std::endl;
                            toggle_fullscreen();
                            break;

                        case SDLK_ESCAPE: {
                            done = true;
                            break;
                        }

                        case SDLK_0: { 
                            // if 0 was pressed stop the music 
                            if (sound_enabled) {
                                Mix_HaltMusic(); 
                            }
                        }                            

                        case SDLK_1: { 
                            // if 1 was pressed play the scratch effect 
                            if (sound_enabled) {
                                if (Mix_PlayChannel(-1, scratch, 0 ) == -1) { 
                                    return 1; 
                                } 
                            }
                        } 

                        case SDLK_2: {
                            // if 2 was pressed play the high hit effect 
                            if (sound_enabled) {
                                if (Mix_PlayChannel(-1, high, 0) == -1) { 
                                    return 1; 
                                } 
                            }
                        } 

                        case SDLK_3: { 
                            if (sound_enabled) {
                                // if 3 was pressed play the medium hit effect 
                                if (Mix_PlayChannel(-1, med, 0) == -1) {
                                    return 1; 
                                } 
                            }
                        } 

                        case SDLK_4: { 
                            if (sound_enabled) {
                                // if 4 was pressed play the low hit effect 
                                if (Mix_PlayChannel(-1, low, 0) == -1) { 
                                    return 1; 
                                } 
                            }
                        } 

                        case SDLK_9: { 
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
                        }                        

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
                    std::cout << "unknown event " << event.type << std::endl;
                    break;
                }                
            }            
        }

        // try to keep a constant number of moves per second
        time_now = SDL_GetTicks();
        if (time_now - last_move_time >= ms_per_move) {

            // move objects
            move();

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



void View::toggle_fullscreen() {
    if (!fullscreen) {

        if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) == 0) {
            // success
            fullscreen = true;
        }
        else {
            // failure
            std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        }
    }
    else {
        if (SDL_SetWindowFullscreen(window, 0) == 0) {
            // success
            fullscreen = false;
        }
        else {
            // failure
            std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        }
    }
}


