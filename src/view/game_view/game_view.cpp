
#include <iostream>

#include "view/utils.h"
#include "view/game_view/game_view.h"


// grid size
const int N_CELLS_WIDE = 80;
const int N_CELLS_HIGH = 50;

// void render_bounding_box(SDL_Renderer *renderer, GameObj * game_obj);
void render_bounding_box(SDL_Renderer * renderer, GameObj * game_obj) {
    SDL_RenderDrawLine(renderer, game_obj->ax, game_obj->ay, game_obj->bx, game_obj->ay);
    SDL_RenderDrawLine(renderer, game_obj->bx, game_obj->ay, game_obj->bx, game_obj->by);
    SDL_RenderDrawLine(renderer, game_obj->bx, game_obj->by, game_obj->ax, game_obj->by);
    SDL_RenderDrawLine(renderer, game_obj->ax, game_obj->by, game_obj->ax, game_obj->ay);
}


GameView::GameView(Model * model, SDL_Window * window, SDL_Renderer * renderer) :
    SubView(model, window, renderer) {
    
    // display the cell grid.. for development and debugging
    debug_draw_grid = true;
}
    

int GameView::init() {

    // FIXME: all this should be done using config files? 
    background = load_texture("./res/background.jpg", renderer);
    if (background == nullptr) {
        log_sdl_error("Load game view background image failed");
        return 1;
    }

    // the pause indicator
    pause_symbol = load_texture("./res/pause.png", renderer);
    if (pause_symbol == nullptr) {
        log_sdl_error("Load game view pause image failed");
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
    title_text = TTF_RenderText_Blended(font, "Game", fg);
    if (!title_text) {
        std::cout << "TTF_RenderText: " << TTF_GetError() << std::endl;
        return 3;
    }

    title_texture = SDL_CreateTextureFromSurface(renderer, title_text);
    if (!title_texture) {
        std::cout << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return 4;
    }        

    // get the window dimensions and size
    SDL_GetWindowSize(window, &window_width, &window_height);
    cell_width = window_width / N_CELLS_WIDE;
    cell_height = window_height / N_CELLS_HIGH;

    // success
    return 0;
}


void GameView::render() {

    // draw the background
    render_texture(background, renderer, 0, 0);

    // draw the cell lines
    if (debug_draw_grid) {
                
        for (int x = 0; x < window_width; x += cell_width) {
            SDL_RenderDrawLine(renderer, x, 0, x , window_height);
        }

        for (int y = 0; y < window_height; y += cell_height) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
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
            game_obj->x - game_obj->half_width, game_obj->y - game_obj->half_height);

        render_bounding_box(renderer, game_obj);
        // render_texture(game_obj->texture, renderer, 
        //                cell_width * game_obj->x, cell_height * game_obj->y);
    }    

    // draw the title
    SDL_Rect src = { 0, 0, title_text->w, title_text->h };
    SDL_Rect dest = { 30, 30, title_text->w, title_text->h};
    SDL_RenderCopy(renderer, title_texture, &src, &dest);    

    // draw the pause button if we're paused
    if (game_state->is_paused()) {
        render_texture(pause_symbol, renderer, window_width - 90, 30);
    }
}


void GameView::clean_up() {
    // clean up the sdl objects
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(pause_symbol);
    SDL_FreeSurface(title_text);
    SDL_DestroyTexture(title_texture);

    // clean up the font 
    TTF_CloseFont(font);
    font = NULL; // to be safe...
}    

void  GameView::mouse_button() { }

