#ifndef INTRO_COMPONENT_H
#define INTRO_COMPONENT_H
/*
 * Game component that plays the intro
 *
 */
#include "view/i_component.h"

class IntroComponent : public IComponent {
 private:
    SDL_Texture * background;
    Control * control;

 public:
    IntroComponent(Control * control);

    // start displaying this component
    void init(SDL_Renderer * renderer);
    void render(SDL_Renderer * renderer, SDL_Window * window);

    const char* get_name_cstr();
    void move(); // RENAME!!!
    void clean_up();


    // keyboard
    void key_f1_down();
    void key_f1_up();

    void key_0_down();
    void key_0_up();

    void key_1_down();
    void key_1_up();

    void key_2_down();
    void key_2_up();

    void key_3_down();
    void key_3_up();

    void key_4_down();
    void key_4_up();

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


