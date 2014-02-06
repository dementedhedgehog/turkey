/**
 * SDL utility fns
 *
 */
#include <string>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "utils.h"

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void log_sdl_error(const std::string &msg) {
    std::cerr << msg << " error: " << SDL_GetError() << std::endl;
}

void log_ttf_error(const std::string &msg) {
    std::cerr << msg << " error: " << TTF_GetError() << std::endl;
}

void log_msg(const std::string &msg) {
    std::cerr << msg << std::endl;
}



/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* load_texture(const std::string & fname, SDL_Renderer * renderer) {

	// Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;

	// Load the texture
    texture = IMG_LoadTexture(renderer, fname.c_str()); 

    // Make sure converting went ok too
    if (texture == nullptr) {
        log_msg("XXXX");
        log_sdl_error((std::string("CreateTextureFromSurface ") + fname).c_str());
	}

	return texture;
}




/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param renderer The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void render_texture(SDL_Texture * tex, SDL_Renderer * renderer, int x, int y) {

	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	// Query the texture to get its width and height to use	
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, tex, NULL, &dst);
}


/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param rend The renderer we want to draw too
* @param dst The destination rectangle to render the texture too
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *renderer, SDL_Rect destination,
	SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(renderer, tex, clip, &destination);
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of
 *	the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer * renderer, int x, int y,
	SDL_Rect *clip = nullptr) {
	SDL_Rect destination;

	destination.x = x;
	destination.y = y;
	if (clip != nullptr) {
		destination.w = clip->w;
		destination.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &destination.w, &destination.h);
    }

	renderTexture(tex, renderer, destination, clip);
}


// FIXME: need to move this somewhere!! or do it better .. we've got state lying around here.
// should go through the control layer and get the value from there?
// method on control!
bool fullscreen = false;
void toggle_fullscreen(SDL_Window * window) {
    if (!fullscreen) {

        if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) == 0) {
            // success
            fullscreen = true;
        }
        else {
            // failure
            std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        }
    }
    else {
        if (SDL_SetWindowFullscreen(window, 0) == 0) {
            // success
            fullscreen = false;
        }
        else {
            // failure
            std::cerr << "Video initialization failed: " << SDL_GetError() << std::endl;
        }
    }
}


