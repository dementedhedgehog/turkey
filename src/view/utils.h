/**
 * SDL utility fns
 *
 */
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <SDL2/SDL.h>

/**
 * Log an SDL error with some error message
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void log_sdl_error(const std::string &msg);
void log_ttf_error(const std::string &msg);
void log_mix_error(const std::string &msg);

/**
 * Log an error message 
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void log_msg(const std::string &msg);


/**
 * Log execution context information
 */
void log_debug_info();


/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture * load_texture(const std::string & fname, SDL_Renderer * renderer);


/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param renderer The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void render_texture(SDL_Texture *tex, SDL_Renderer *renderer, int x, int y);



/**
 * Toggle the window to full size.
 */
void toggle_fullscreen(SDL_Window * window);



#endif
