

#include "view/font_manager.h"
#include "view/utils.h"


int FontManager::init() {

    // load font.ttf at size 16 into font
    hobbiton_font = TTF_OpenFont("./res/hobbiton_brush_hand.ttf", 128);
    if (!hobbiton_font) {
        log_ttf_error("TTF_OpenFont: ");
        return 1; // failure
    }

    return 0; // success
}

int FontManager::clean_up() {
    TTF_CloseFont(hobbiton_font);
    hobbiton_font = NULL; // to be safe...
    return 0; // success
};


TTF_Font * FontManager::get_fps_font() {
    return hobbiton_font;
}
