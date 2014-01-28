
#include <iostream>

#include "view/utils.h"
#include "view/game_component/game_component.h"


// grid
const int N_CELLS_WIDE = 80;
const int N_CELLS_HIGH = 50;
// const int N_CELLS_WIDE = 40;
// const int N_CELLS_HIGH = 25;

// grid size..
const int SCREEN_WIDTH  = 1280;  // FIXME: get this dynamically from the window
const int SCREEN_HEIGHT = 800;
const int GRID_CELL_WIDTH  = 1; // SCREEN_WIDTH / N_CELLS_WIDE;
const int GRID_CELL_HEIGHT = 1; // SCREEN_HEIGHT / N_CELLS_HIGH;
    


GameComponent::GameComponent(Model * model, SDL_Window * window, SDL_Renderer * renderer) :
    Component(model, window, renderer) {
    
    // MOVE TO SCRIPTS
    // display the cell grid.. for development and debugging
    grid_enabled = true;
}

const char * GameComponent::get_name_cstr() {
    return "Game";
}
    

int GameComponent::init(SDL_Renderer * renderer) {

    // FIXME: all this should be done using config files? 
    background = load_texture("./res/background.jpg", renderer);
    if (background == nullptr) {
        log_sdl_error("Load game component background image failed");
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

    // success
    return 0;
}


void GameComponent::render(SDL_Renderer * renderer, SDL_Window * window) {
    int x, y;

    // draw the background
    render_texture(background, renderer, 0, 0);

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


    // draw all the game objects..
    GameState const * game_state = model->get_game_state();
    std::list<GameObj*> game_objs = game_state->get_game_objs();
    GameObj * game_obj;
    std::list<GameObj*>::iterator i;
    for (i = game_objs.begin(); i != game_objs.end(); i++) {         
        game_obj = (*i);
        render_texture(game_obj->texture, renderer, 
                       GRID_CELL_WIDTH * game_obj->x, GRID_CELL_HEIGHT * game_obj->y);
    }

    SDL_Rect src = { 0, 0, title_text->w, title_text->h };
    SDL_Rect dest = { 30, 30, title_text->w, title_text->h};
    SDL_RenderCopy(renderer, title_texture, &src, &dest);

}


void GameComponent::update() {
    model->update();
}    


void GameComponent::clean_up() {
    // clean up the sdl objects
    SDL_DestroyTexture(background);
    //SDL_DestroyTexture(background_parallax); 
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
    toggle_fullscreen(window);
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
void  GameComponent::key_4_down() {

    GameState const * game_state = model->get_game_state();
    std::list<GameObj*> game_objs = game_state->get_game_objs();
    GameObj * game_obj;
    std::list<GameObj*>::iterator i;
    std::cout << "----" << std::endl;
    for (i = game_objs.begin(); i != game_objs.end(); i++) {         
        game_obj = (*i);
        std::cout << "obj: " << game_obj->x << ", " << game_obj->y << "  " << game_obj->texture << std::endl;
    }

}
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

void GameComponent::key_left_down() {     
    model->get_game_state()->start_moving_character_left(); 
}

void GameComponent::key_left_up() { 
    model->get_game_state()->stop_moving_character_left(); 
}

void GameComponent::key_right_down() { 
    model->get_game_state()->start_moving_character_right(); 
}
void GameComponent::key_right_up() { 
    model->get_game_state()->stop_moving_character_right(); 
}

void  GameComponent::key_up_down() { 
    model->get_game_state()->start_moving_character_up(); 
}
void  GameComponent::key_up_up() { 
    model->get_game_state()->stop_moving_character_up(); 
}

void  GameComponent::key_down_down() { 
    model->get_game_state()->start_moving_character_down(); 
}
void  GameComponent::key_down_up() { 
    model->get_game_state()->stop_moving_character_down(); 
}

void  GameComponent::key_a_down() { 
    model->get_game_state()->start_moving_character_left(); 
}
void  GameComponent::key_a_up() { 
    model->get_game_state()->stop_moving_character_left(); 
}

void  GameComponent::key_d_down() { 
    model->get_game_state()->start_moving_character_right(); 
}
void  GameComponent::key_d_up() { 
    model->get_game_state()->stop_moving_character_right(); 
}

void  GameComponent::key_w_down() { 
    model->get_game_state()->start_moving_character_up(); 
}
void  GameComponent::key_w_up() { 
    model->get_game_state()->stop_moving_character_up(); 
}

void  GameComponent::key_x_down() { 
    model->get_game_state()->start_moving_character_down(); 
}
void  GameComponent::key_x_up() { 
    model->get_game_state()->stop_moving_character_down(); 
}

void  GameComponent::mouse_button() {}

