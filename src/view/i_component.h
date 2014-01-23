#ifndef I_COMPONENT_H
#define I_COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "view/utils.h"


/*
 * Interface for Components.
 *
 * Components are parts of the game.. e.g. the help screens, the intro screen, the inventory screen the 
 * game screen etc.
 */
//#include "view/view.h"

class IComponent {
public:

    // Adding a virtual destructor allows you to pass pointer ownership to another party without 
    // exposing the concrete derived class. The destructor doesn't have to do anything, because 
    // the interface doesn't have any concrete members
    virtual ~IComponent() {};

    // increment the count of the frames rendererd (do this each frame).
    //virtual void start(View & view) = 0;
    virtual void init(SDL_Renderer * renderer) = 0;
    virtual void render(SDL_Renderer * renderer, SDL_Window * window) = 0;

    // get the name of the component
    virtual const char * get_name_cstr() = 0;

    // update sprite positions
    virtual void move() = 0;

    // clean up resources
    virtual void clean_up() = 0;

    // keyboard
    virtual void key_f1_down() = 0;
    virtual void key_f1_up() = 0;

    virtual void key_0_down() = 0;
    virtual void key_0_up() = 0;

    virtual void key_1_up() = 0;
    virtual void key_1_down() = 0;
    virtual void key_2_up() = 0;
    virtual void key_2_down() = 0;
    virtual void key_3_up() = 0;
    virtual void key_3_down() = 0;
    virtual void key_4_up() = 0;
    virtual void key_4_down() = 0;
    /* virtual void key_5_up() = 0; */
    /* virtual void key_5_down() = 0; */
    /* virtual void key_6_up() = 0; */
    /* virtual void key_6_down() = 0; */
    /* virtual void key_7_up() = 0; */
    /* virtual void key_7_down() = 0; */
    /* virtual void key_8_up() = 0; */
    /* virtual void key_8_down() = 0; */
    virtual void key_9_up() = 0;
    virtual void key_9_down() = 0;

    virtual void key_left_up() = 0;
    virtual void key_left_down() = 0;
    virtual void key_right_up() = 0;
    virtual void key_right_down() = 0;
    virtual void key_up_up() = 0;
    virtual void key_up_down() = 0;
    virtual void key_down_up() = 0;
    virtual void key_down_down() = 0;

    virtual void key_a_up() = 0;
    virtual void key_a_down() = 0;
    virtual void key_d_up() = 0;
    virtual void key_d_down() = 0;
    virtual void key_w_up() = 0;
    virtual void key_w_down() = 0;
    virtual void key_x_up() = 0;
    virtual void key_x_down() = 0;

    virtual void mouse_button() = 0;
};

#endif 


