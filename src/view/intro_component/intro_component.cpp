
#include <iostream>

#include "control/control.h"
#include "view/intro_component/intro_component.h"

IntroComponent::IntroComponent(Control * control) {
    this->control = control;
}


int IntroComponent::init(SDL_Renderer * renderer) {
    std::cout << "intro init" << std::endl;

    background = loadTexture("./res/intro_background.jpg", renderer);
    if (background == nullptr) {
        logSDLError(std::cout, "LoadBMPX");
        return 2;
    }

    // load font.ttf at size 16 into font
    font = TTF_OpenFont("./res/hobbiton_brush_hand.ttf", 128);
    if (!font) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        return 3;
    }
       
    SDL_Color fg = { 255, 255, 0, 255 };
    title_text = TTF_RenderText_Blended(font, get_name_cstr(), fg);
    if (!title_text) {
        std::cout << "TTF_RenderText: " << TTF_GetError() << std::endl;
        return 4;
    }

    title_texture = SDL_CreateTextureFromSurface(renderer, title_text);
    if (!title_texture) {
        std::cout << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
        return 5;
    }        

    // success
    return 0;
}



void IntroComponent::render(
        SDL_Renderer * renderer,
        SDL_Window * window) {

    // tile background
    int backgroundWidth, backgroundHeight;
    SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
    renderTexture(background, renderer, 0, 0);

    SDL_Rect src = { 0, 0, title_text->w, title_text->h };
    SDL_Rect dest = { 30, 30, title_text->w, title_text->h};
    SDL_RenderCopy(renderer, title_texture, &src, &dest);
}

const char* IntroComponent::get_name_cstr() {
    return "Intro";
}

void IntroComponent::move() {
    // no-op for the moment..
}

void IntroComponent::clean_up() {
    // clean up the sdl objects
    SDL_DestroyTexture(background);

    SDL_FreeSurface(title_text);
    SDL_DestroyTexture(title_texture);

    // clean up the font 
    TTF_CloseFont(font);
    font = NULL; // to be safe...
}


// keyboard
void IntroComponent::key_f1_up() {
}

void IntroComponent::key_f1_down() {
    control->toggle_fullscreen();
}

void  IntroComponent::key_0_up() {};
void  IntroComponent::key_0_down() {
};
void  IntroComponent::key_1_up() {};
void  IntroComponent::key_1_down() {};
void  IntroComponent::key_2_up() {};
void  IntroComponent::key_2_down() {};
void  IntroComponent::key_3_up() {};
void  IntroComponent::key_3_down() {};
void  IntroComponent::key_4_up() {};
void  IntroComponent::key_4_down() {};
/*  void  IntroComponent::key_5_up() {}; */
/*  void  IntroComponent::key_5_down() {}; */
/*  void  IntroComponent::key_6_up() {}; */
/*  void  IntroComponent::key_6_down() {}; */
/*  void  IntroComponent::key_7_up() {}; */
/*  void  IntroComponent::key_7_down() {}; */
/*  void  IntroComponent::key_8_up() {}; */
/*  void  IntroComponent::key_8_down() {}; */
void  IntroComponent::key_9_up() {};
void  IntroComponent::key_9_down() {};

void  IntroComponent::key_left_up() {};
void  IntroComponent::key_left_down() {};
void  IntroComponent::key_right_up() {};
void  IntroComponent::key_right_down() {};
void  IntroComponent::key_up_up() {};
void  IntroComponent::key_up_down() {};
void  IntroComponent::key_down_up() {};
void  IntroComponent::key_down_down() {};

void  IntroComponent::key_a_up() {};
void  IntroComponent::key_a_down() {
    std::cout << "AAAAX" << std::endl;                            
    control->change_state(State::GAME);
};
void  IntroComponent::key_d_up() {};
void  IntroComponent::key_d_down() {};
void  IntroComponent::key_w_up() {};
void  IntroComponent::key_w_down() {};
void  IntroComponent::key_x_up() {};
void  IntroComponent::key_x_down() {};

void  IntroComponent::mouse_button() {};
