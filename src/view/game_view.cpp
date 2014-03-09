
#include <iostream>

#include "view/utils.h"
#include "view/game_view.h"


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


GameView::GameView(Model * model, 
    SDL_Window * window, SDL_Renderer * renderer, 
    ImageManager * image_manager, FontManager * font_manager, SoundManager * sound_manager) :
    BaseView(model, window, renderer, image_manager, font_manager, sound_manager) {
    
    // display the cell grid.. for development and debugging
    debug_draw_grid = true;
}
    

int GameView::init() {

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


int GameView::render() {

    // get the game state to render
    GameState const * game_state = model->get_game_state();
    float camera_x, camera_y;
    game_state->get_camera_position(&camera_x, &camera_y);

    // draw the background
    //render_texture(background, renderer, 0 - camera_x, 0-camera_y);

    //std::cout << camera_x << ", " << camera_y << std::endl;
    scenery_manager.render_background(renderer, camera_x, camera_y);
    //scenery_manager.render_background(renderer, 0, 0);

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
    std::list<GameObj*> game_objs = game_state->get_game_objs();
    GameObj * game_obj;
    
    // for each game object
    std::list<GameObj*>::iterator i;
    for (i = game_objs.begin(); i != game_objs.end(); i++) {         
        game_obj = *i;
        
        // draw the game object
        render_texture(game_obj->texture, renderer, 
            game_obj->x - game_obj->half_width, game_obj->y - game_obj->half_height);

        // draw a bounding box around the game object
        render_bounding_box(renderer, game_obj);
    }    

    // draw the foregrond scenery
    scenery_manager.render_foreground(renderer, camera_x, camera_y);

    // draw the title
    SDL_Rect src = { 0, 0, title_text->w, title_text->h };
    SDL_Rect dest = { window_width - title_text->w - 30, 30, title_text->w, title_text->h};
    SDL_RenderCopy(renderer, title_texture, &src, &dest);    

    // draw the pause symbol if we're paused
    if (game_state->is_paused()) {
        render_texture(pause_symbol, renderer, window_width - 90, 30);
    }

    return 0; // success
}


int GameView::clean_up() {
    // clean up the sdl objects
    SDL_DestroyTexture(pause_symbol);
    SDL_FreeSurface(title_text);
    SDL_DestroyTexture(title_texture);

    // clean up the font 
    TTF_CloseFont(font);
    font = NULL; // to be safe...

    return 0; // success
}    

void  GameView::mouse_button() { }


// expose this to python
int GameView::add_scenery(float scroll_rate, float x, float y, SDL_Texture * texture) 
{
    return scenery_manager.add_scenery(scroll_rate, x, y, texture);
}

