#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <vector>
#include <list>
#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "view/utils.h"


class ImageManager {
 private:
    SDL_Window * window;

    // keep a list of all textures used
    std::list<SDL_Texture*> all_textures;

 public:
    
    // constructor
    ImageManager(SDL_Window * window);

    // initialize the image manager
    int init();

    // load the images
    SDL_Texture * load(const std::string & fname, SDL_Renderer * renderer); 

    // load a list of textures from an image
    std::vector<SDL_Texture*> * load_from_sprite_sheet(
        const std::string & fname, 
        SDL_Renderer * renderer,
        unsigned int width, // of frame in pixels
        unsigned int height, // of frame in pixels
        unsigned int n_frames_wide = 0,
        unsigned int n_frames_high = 0);


    // clean up all image resources
    int clean_up();
};

#endif
