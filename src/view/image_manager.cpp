
#include <iostream>
#include "view/image_manager.h"

#include "SDL2/SDL.h"

// an SDL thing..
const Uint32 UNUSED = 0;

ImageManager::ImageManager(SDL_Window * window) {
    this->window = window;
}


/*
 * Manages loading and unloading textures..
 * Attempts to make sure duplicate images are not loaded twice.
 */

int ImageManager::init() {
    return 0; // success
}



/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture * ImageManager::load(const std::string & fname, SDL_Renderer * renderer) {

	// Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;

	// Load the texture
    texture = IMG_LoadTexture(renderer, fname.c_str()); 

    // Make sure converting went ok too
    if (texture == nullptr) {
        log_sdl_error((std::string("CreateTextureFromSurface ") + fname).c_str());
	}

	return texture;
}

std::vector<SDL_Texture*> * ImageManager::load_from_sprite_sheet(
    const std::string & fname, 
    SDL_Renderer * renderer,
    unsigned int width, unsigned int height, // size of a "frame" in pixels
    unsigned int n_frames_wide, unsigned int n_frames_high) { 

    // the result .. a 1D vector of image frames
    std::vector<SDL_Texture *> * frames = new std::vector<SDL_Texture *>(0);

	// Load a surface
    SDL_Surface * sprite_sheet = IMG_Load(fname.c_str()); 
    if (!sprite_sheet) {
        log_msg("Can't load sprite sheet!? " + fname + " " + IMG_GetError());
        delete frames;
        return nullptr;
    }
    
    // sanity checks
    if (width * n_frames_wide > (unsigned int)sprite_sheet->w) {
        log_msg("Sprite sheet not wide enough! " + fname);
        SDL_FreeSurface(sprite_sheet);
        delete frames;
        return nullptr;
    }
    if (height * n_frames_high > (unsigned int)sprite_sheet->h) {
        log_msg("Sprite sheet not high enough! " + fname);
        SDL_FreeSurface(sprite_sheet);
        delete frames;
        return nullptr;
    }

    // create a surface for each texture
    SDL_Surface* frame_surface;
    Uint32 rmask, gmask, bmask, amask;

    // SDL interprets each pixel as a 32-bit number, so our masks must depend
    // on the endianness (byte order) of the machine 
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    // create a surface to store a frame
    frame_surface = SDL_CreateRGBSurface(UNUSED, width, height, 32, rmask, gmask, bmask, amask);
    if (!frame_surface) {
        SDL_FreeSurface(sprite_sheet);
        delete frames;
        return nullptr;
    }
    SDL_SetSurfaceBlendMode(frame_surface, SDL_BLENDMODE_BLEND);

    SDL_Texture * frame;
    SDL_Rect clip;
    clip.w = width;
    clip.h = height;
    for (unsigned int row = 0; row < n_frames_high; row++) {
        for (unsigned int column = 0; column < n_frames_wide; column++) {

            // blit from the sprite sheet to the frame surface
            clip.x = column * width;
            clip.y = row * height;
            SDL_BlitSurface(sprite_sheet, &clip, frame_surface, nullptr);

            // create a texture for the frame on the graphics card
            frame = SDL_CreateTextureFromSurface(renderer, frame_surface);
            if (!frame) {
                log_msg("Can't create frame surface?! " + fname);
                SDL_FreeSurface(sprite_sheet);
                SDL_FreeSurface(frame_surface);
                delete frames;
                return nullptr;
            }

            // clear the fram (otherwise we just get a composite of all frames drawn to the frame_surface).
            SDL_FillRect(frame_surface, NULL, 0x000000);

            // and add it to the list
            frames->push_back(frame);

            // keep a reference because we clean up everything here
            all_textures.push_back(frame);
        }
    }
    
    // clean up 
    SDL_FreeSurface(sprite_sheet);
    SDL_FreeSurface(frame_surface);

    return frames;
}  


int ImageManager::clean_up() {

    // clean up all the textures we've allocated
    for (std::list<SDL_Texture *>::iterator i = all_textures.begin(); i != all_textures.end(); i++) {
        SDL_DestroyTexture(*i);
    }

    return 0; // success
}
