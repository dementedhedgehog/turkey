
#include <iostream>

#include "view/intro_component/intro_component.h"

//void IntroComponent::start(View & view) {
void IntroComponent::init(SDL_Renderer * renderer) {
    std::cout << "foo" << std::endl;

    background = loadTexture("./res/intro_background.jpg", renderer);
    if (background == nullptr) {
        logSDLError(std::cout, "LoadBMPX");
        //return 4;
    }

}


void IntroComponent::render(
        SDL_Renderer * renderer,
        SDL_Window * window) {

    // tile background
    int backgroundWidth, backgroundHeight;
    SDL_QueryTexture(background, NULL, NULL, &backgroundWidth, &backgroundHeight);
    renderTexture(background, renderer, 0, 0);
}

