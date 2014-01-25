
#include <iostream>

#include "control/control.h"
#include "view/game_component/game_component.h"


// grid
const int N_CELLS_WIDE = 80;
const int N_CELLS_HIGH = 50;
// const int N_CELLS_WIDE = 40;
// const int N_CELLS_HIGH = 25;

// grid size..
const int SCREEN_WIDTH  = 1280;  // FIXME: get this dynamically from the window
const int SCREEN_HEIGHT = 800;
const int GRID_CELL_WIDTH  = SCREEN_WIDTH / N_CELLS_WIDE;
const int GRID_CELL_HEIGHT = SCREEN_HEIGHT / N_CELLS_HIGH;
    


GameComponent::GameComponent(Control * control) {
    this->control = control;

    // init character position 
    // FIXME: move this stuff into a sprite or some other object
    character_pos = {3, 3}; 
    character_width = 35;
    character_height = 36; 
    character_xvel = 0;
    character_yvel = 0;

    // MOVE TO MODEL
    // display the cell grid.. for development and debugging
    grid_enabled = true;
}

const char * GameComponent::get_name_cstr() {
    return "Game";
}
    


int GameComponent::init(SDL_Renderer * renderer) {

    // FIXME: all this should be done using config files? 
    background = loadTexture("./res/background.jpg", renderer);
    if (background == nullptr) {
        logSDLError(std::cout, "LoadBMPX");
        return 1;
    }

    // load font.ttf at size 16 into font
    font = TTF_OpenFont("./res/hobbiton_brush_hand.ttf", 128);
    if (!font) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        return 2;
    }
       
    // 
    SDL_Color fg = { 255, 255, 0, 255 };
    title_text = TTF_RenderText_Blended(font, get_name_cstr(), fg);
    if (!title_text) {
        std::cout << "TTF_RenderText: " << TTF_GetError() << std::endl;
        return 3;
    }

    title_texture = SDL_CreateTextureFromSurface(renderer, title_text);
    if (!title_texture) {
        std::cout << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return 4;
    }        

    // load the main character image
    character = loadTexture("./res/dwarf.png", renderer);

    // load a block of stone..
    stone = loadTexture("./res/stone.png", renderer);

    // success
    return 0;
}


void GameComponent::render(SDL_Renderer * renderer, SDL_Window * window) {
    int x, y;

    // tile background
    //int backgroundWidth, backgroundHeight;
    //SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
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

    SDL_Rect src = { 0, 0, title_text->w, title_text->h };
    SDL_Rect dest = { 30, 30, title_text->w, title_text->h};
    // SDL_Rect dst = { 0, 0, 10, 10}; //textureX->w, textureX->h };
    //SDL_RenderCopy(renderer, texture, &src, &dst);
    SDL_RenderCopy(renderer, title_texture, &src, &dest);

    // // draw some text
    // SDL_Rect src = { 0, 0, text->w, text->h };
    // SDL_Rect dest = { 30, 30, text->w, text->h};
    // SDL_RenderCopy(renderer, texture, &src, &dest);

}


void GameComponent::move() {
    // move stuff
    character_pos.x += character_xvel;
    character_pos.y += character_yvel;
}    


void GameComponent::clean_up() {
    // clean up the sdl objects
    SDL_DestroyTexture(background);
    //SDL_DestroyTexture(background_parallax); 
    SDL_DestroyTexture(character); 

    SDL_FreeSurface(title_text);
    SDL_DestroyTexture(title_texture);

    // clean up the font 
    TTF_CloseFont(font);
    font = NULL; // to be safe...

}    



//
// keyboard mapping
//
void  GameComponent::key_f1_up() {
}
void  GameComponent::key_f1_down() {
    control->toggle_fullscreen();
}


void  GameComponent::key_0_up() {}
void  GameComponent::key_0_down() {}
void  GameComponent::key_1_up() {}
void  GameComponent::key_1_down() {}
void  GameComponent::key_2_up() {}
void  GameComponent::key_2_down() {}
void  GameComponent::key_3_up() {}
void  GameComponent::key_3_down() {}
void  GameComponent::key_4_up() {}
void  GameComponent::key_4_down() {}
/*  void  GameComponent::key_5_up() {} */
/*  void  GameComponent::key_5_down() {} */
/*  void  GameComponent::key_6_up() {} */
/*  void  GameComponent::key_6_down() {} */
/*  void  GameComponent::key_7_up() {} */
/*  void  GameComponent::key_7_down() {} */
/*  void  GameComponent::key_8_up() {} */
/*  void  GameComponent::key_8_down() {} */
void  GameComponent::key_9_up() {}
void  GameComponent::key_9_down() {}

void GameComponent::key_left_down() { start_moving_character_left(); }
void GameComponent::key_left_up() { stop_moving_character_left(); }

void GameComponent::key_right_down() { start_moving_character_right(); }
void GameComponent::key_right_up() { stop_moving_character_right(); }

void  GameComponent::key_up_down() { start_moving_character_up(); }
void  GameComponent::key_up_up() { stop_moving_character_up(); }

void  GameComponent::key_down_down() { start_moving_character_down(); }
void  GameComponent::key_down_up() { stop_moving_character_down(); }

void  GameComponent::key_a_down() { start_moving_character_left(); }
void  GameComponent::key_a_up() { stop_moving_character_left(); }

void  GameComponent::key_d_down() { start_moving_character_right(); }
void  GameComponent::key_d_up() { stop_moving_character_right(); }

void  GameComponent::key_w_down() { start_moving_character_up(); }
void  GameComponent::key_w_up() { stop_moving_character_up(); }

void  GameComponent::key_x_down() { start_moving_character_down(); }
void  GameComponent::key_x_up() { stop_moving_character_down(); }

void  GameComponent::mouse_button() {}



//
// character movement
//
void GameComponent::start_moving_character_left() { character_xvel = -1; }
void GameComponent::stop_moving_character_left() { if (character_xvel < 0) character_xvel = 0; }

void GameComponent::start_moving_character_right() { character_xvel = +1; }
void GameComponent::stop_moving_character_right() { if (character_xvel > 0) character_xvel = 0; }

void GameComponent::start_moving_character_up() { character_yvel = -1; }
void GameComponent::stop_moving_character_up() { if (character_yvel < 0) character_yvel = 0; }

void GameComponent::start_moving_character_down() { character_yvel = +1; }
void GameComponent::stop_moving_character_down() { if (character_yvel > 0) character_yvel = 0; }
