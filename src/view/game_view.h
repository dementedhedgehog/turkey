#ifndef GAME_VIEW_H
#define GAME_VIEW_H
/*
 * Game view that playes the side scroller game itself.
 *
 */
#include "model/model.h"
#include "view/base_view.h"

class GameView : public BaseView {
 private:

    // some renderer textures FIXME: remove this stuff!
    SDL_Texture * background;
    SDL_Texture * stone; // block of stone  - just hacking this in here for the moment

    // the pause symbol (displayed when the game is paused)
    SDL_Texture * pause_symbol;

    // example text
    // FIXME: add to its own object
    SDL_Surface * title_text;
    SDL_Texture * title_texture;

    // an example of a ttf font
    TTF_Font * font;  // FIXME get the font from a loader object..

    // draw the cell grid?
    bool debug_draw_grid;

    // dimensions of the window 
    int window_width;
    int window_height;

    // how many pixels in a grid cell 
    int cell_width;
    int cell_height;

 public:
    GameView(Model * model, SDL_Window * window, SDL_Renderer * renderer, 
        ImageManager * image_manager, FontManager * font_manager, SoundManager * sound_manager);

    // start displaying this view
    int init();

    // render to the window
    int render();

    // clean up all the resources
    int clean_up();

    // handle mouse events
    void mouse_button();

    //
    // Debugging functions
    //

    // 
    void debug_set_draw_grid(const bool draw_grid) { debug_draw_grid = draw_grid; } 
};

#endif 


