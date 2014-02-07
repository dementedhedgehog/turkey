/*
 * Intro screen implementation.
 *
 */

#include <iostream>

#include "model/model.h"
#include "view/utils.h"
#include "view/intro_view.h"

IntroView::IntroView(Model * model, SDL_Window * window, SDL_Renderer * renderer) :
    SubView(model, window, renderer) {}


int IntroView::init() {
    std::cout << "intro init" << std::endl;

    background = load_texture("./res/intro_background.jpg", renderer);
    if (background == nullptr) {
        log_sdl_error("Load intro background texture failed");
        return 2;
    }

    // load font.ttf at size 16 into font
    font = TTF_OpenFont("./res/hobbiton_brush_hand.ttf", 128);
    if (!font) {
        std::cout << "TTF_OpenFont: " << TTF_GetError() << std::endl;
        return 3;
    }
       
    SDL_Color fg = { 255, 255, 0, 255 };
    //title_text = TTF_RenderText_Blended(font, get_name_cstr(), fg);
    title_text = TTF_RenderText_Blended(font, "Intro", fg);
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



void IntroView::render() {

    // tile background
    int backgroundWidth, backgroundHeight;
    SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
    render_texture(background, renderer, 0, 0);

    SDL_Rect src = { 0, 0, title_text->w, title_text->h };
    SDL_Rect dest = { 30, 30, title_text->w, title_text->h};
    SDL_RenderCopy(renderer, title_texture, &src, &dest);
}

void IntroView::clean_up() {
    // clean up the sdl objects
    SDL_DestroyTexture(background);

    SDL_FreeSurface(title_text);
    SDL_DestroyTexture(title_texture);

    // clean up the font 
    TTF_CloseFont(font);
    font = NULL; // to be safe...
}

void  IntroView::mouse_button() {};
