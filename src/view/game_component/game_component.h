#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H
/*
 * Game component that playes the side scroller game itself.
 *
 */
//#include "control/control.h"
#include "model/model.h"
#include "view/component.h"

class GameComponent : public Component {
 private:
    //Control * control;
    //Model * model;

    // some renderer textures
    SDL_Texture * character;
    SDL_Texture * background;
    SDL_Texture * stone; // block of stone  - just hacking this in here for the moment
    //SDL_Texture * background_parallax;

    // FIXME: move these things to a sprite_image object.. sprites then contain 
    // a bunch of sprite_images
    // a grid cell posn, positive down and left (like sdl coords) 
    // velocity?, scripts etc.

    // example text
    // FIXME: add to its own object
    SDL_Surface * title_text;
    SDL_Texture * title_texture;

    // an example of a ttf font
    TTF_Font * font;  // FIXME get the font from a loader object..

    // FIXME: move to view
    bool grid_enabled;

    // character position
    // (assumes we go with the cell based positioning platformer model).
    // FIXME: move this info into a sprite!
    SDL_Point character_pos; 
    uint character_width;
    uint character_height; 

    // flags for handling character movement
    int character_xvel;
    int character_yvel;

    /* void start_moving_character_left(); */
    /* void stop_moving_character_left(); */

    /* void start_moving_character_right(); */
    /* void stop_moving_character_right(); */

    /* void start_moving_character_up(); */
    /* void stop_moving_character_up(); */

    /* void start_moving_character_down(); */
    /* void stop_moving_character_down(); */



 public:
    //GameComponent(Control * control);
    GameComponent(Model * model, SDL_Window * window, SDL_Renderer * renderer);

    // start displaying this component
    int init(SDL_Renderer * renderer);
    void render(SDL_Renderer * renderer, SDL_Window * window);

    const char* get_name_cstr();

    // do any animation, update positions and displayed values etc.
    void update(); 
    void clean_up();


    // keyboard
    void key_f1_down();
    void key_f1_up();

    void key_0_up();
    void key_0_down();
    void key_1_up();
    void key_1_down();
    void key_2_up();
    void key_2_down();
    void key_3_up();
    void key_3_down();
    void key_4_up();
    void key_4_down();
    /*  void key_5_up(); */
    /*  void key_5_down(); */
    /*  void key_6_up(); */
    /*  void key_6_down(); */
    /*  void key_7_up(); */
    /*  void key_7_down(); */
    /*  void key_8_up(); */
    /*  void key_8_down(); */
    void key_9_up();
    void key_9_down();

    void key_left_up();
    void key_left_down();
    void key_right_up();
    void key_right_down();
    void key_up_up();
    void key_up_down();
    void key_down_up();
    void key_down_down();

    void key_a_up();
    void key_a_down();
    void key_d_up();
    void key_d_down();
    void key_w_up();
    void key_w_down();
    void key_x_up();
    void key_x_down();

    void mouse_button();
};

#endif 


