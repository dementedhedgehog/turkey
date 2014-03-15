/*
 *
 * A Simple Side-Scroller Game.
 *
 */
#include <iostream>
#include <string>
#include <sstream> 

#include <SDL2/SDL.h>

#include "model/model.h"
#include "model/keyboard_handler.h"
#include "model/collision_resolution_manager.h"
#include "view/view.h"
#include "view/utils.h"
#include "shared/scripting.h"
#include "shared/actions.h"



class NullCollisionResolver : CollisionResolver {
public:
    NullCollisionResolver() : CollisionResolver("NullCollisionResolver") {};

    void resolve (
        GameObj * moveable_game_obj, 
        GameObj * other_game_obj, 
        CollisionType collision) { };
};




class SolidFloorCollisionResolver : CollisionResolver {
public:
    SolidFloorCollisionResolver() : CollisionResolver("SolidFloorCollisionResolver") {};

    void resolve (
        GameObj * moveable_game_obj, 
        GameObj * other_game_obj, 
        CollisionType collision)
    {
        assert(collision == CollisionType::BOTTOM || collision == CollisionType::TOP);
        assert(!other_game_obj->is_moveable());

        moveable_game_obj->collision_set_y_velocity(0.0f);
        moveable_game_obj->jumping = false;


        assert(moveable_game_obj->y_velocity == 0.0f);
    }
};

// void solid_floor_collision_resolver (
//     GameObj * moveable_game_obj, 
//     GameObj * other_game_obj, 
//     CollisionType collision)
// {
//     assert(collision == CollisionType::BOTTOM || collision == CollisionType::TOP);
//     assert(!other_game_obj->is_moveable());

//     moveable_game_obj->collision_set_y_velocity(0.0f);
//     moveable_game_obj->jumping = false;
// }


class SolidWallCollisionResolver : CollisionResolver {
public:
    SolidWallCollisionResolver() : CollisionResolver("SolidWallCollisionResolver") {};

    void resolve (
        GameObj * moveable_game_obj, 
        GameObj * other_game_obj, 
        CollisionType collision)
    {
        assert(collision == CollisionType::LEFT || collision == CollisionType::RIGHT);
        assert(!other_game_obj->is_moveable());

        moveable_game_obj->collision_set_x_velocity(0.0f);
        moveable_game_obj->jumping = false;
    }

};


// void solid_wall_collision_resolver (
//     GameObj * moveable_game_obj, 
//     GameObj * other_game_obj, 
//     CollisionType collision)
// {
//     assert(collision == CollisionType::LEFT || collision == CollisionType::RIGHT);
//     assert(!other_game_obj->is_moveable());

//     std::cout << "----------------------------_" << std::endl;

//     moveable_game_obj->collision_set_x_velocity(0.0f);
//     moveable_game_obj->jumping = false;
// }



int main(int argc, char **argv){
    int result;

    GameObjType::initialize();

    // log some info about about the run context
    log_debug_info();


    // init sdl
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_sdl_error("SDL_Init");
		return 1;
	}
    
    // init the img reading library
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        log_sdl_error("IMG_Init");
        return 2;
    }

    // init the font library
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init: %s\n" << TTF_GetError() << std::endl;
        return 3;
    }

    // create the game window
    SDL_Window * window = SDL_CreateWindow(GAME_NAME, 
        SCREEN_POS_X, SCREEN_POS_Y, 
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        log_sdl_error("CreateWindow");
        return 2;
    }

    // create the baseview renderer for the window
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        log_sdl_error("CreateRenderer");
        return 3;
    }


    // initialize the font manager
    FontManager * font_manager = new FontManager();
    if ((result = font_manager->init()) != 0) {
        log_msg("init font manager failed!");
        return result;
    };

    // initialize the image manager
    ImageManager * image_manager = new ImageManager(window);
    if ((result = image_manager->init()) != 0) {
        log_msg("init image manager failed!");
        return result;
    };

    // initialize the sound
    SoundManager * sound_manager = new SoundManager();
    if ((result = sound_manager->init()) != 0) {
        log_msg("init sound manager failed!");
        return result;
    };

    // seed the random number generator
    srand(SDL_GetTicks());

    // create the model 
    Model * model = new Model();

    // create the view
    View * view = new View(model, window, renderer, image_manager, font_manager, sound_manager);

    // the view gets change notifications from the model
    model->register_state_listener(view);

    // init the UI libraries or die
    if ((result = view->init()) != 0) {
        log_msg("init view failed!");
        return result;
    }


    CollisionResolver * solid_floor_collision_resolver = 
        (CollisionResolver *) new SolidFloorCollisionResolver();
    CollisionResolver * solid_wall_collision_resolver = 
        (CollisionResolver *) new SolidWallCollisionResolver();
    CollisionResolver * null_collision_resolver = 
        (CollisionResolver *) new NullCollisionResolver();

    CollisionResolutionManager::init();

    // add the resolver
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::BOTTOM, 
        solid_floor_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::TOP, 
        solid_floor_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::LEFT,
        solid_wall_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::RIGHT,
        solid_wall_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::TOP_RIGHT,
        null_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::TOP_LEFT,
        null_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::BOTTOM_RIGHT,
        null_collision_resolver);
    CollisionResolutionManager::add(CHARACTER, BLOCK, CollisionType::BOTTOM_LEFT,
        null_collision_resolver);


    // add the resolver
    // CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::BOTTOM, 
    //     &solid_floor_collision_resolver);
    // CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::TOP, 
    //     &solid_floor_collision_resolver);
    // CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::LEFT,
    //     &solid_wall_collision_resolver);
    // CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::RIGHT,
    //     &solid_wall_collision_resolver);


    // add the resolver
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::BOTTOM, 
        solid_floor_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::TOP, 
        solid_floor_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::LEFT,
        solid_wall_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::RIGHT,
        solid_wall_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::TOP_RIGHT,
        null_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::TOP_LEFT,
        null_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::BOTTOM_RIGHT,
        null_collision_resolver);
    CollisionResolutionManager::add(FIREBALL, BLOCK, CollisionType::BOTTOM_LEFT,
        null_collision_resolver);


    // display the window or die
    if ((result = view->display_window()) != 0) { 
        log_msg("display_window failed!");
        return result; 
    }

    // initialize the embedded python interpreter
    Scripting * scripting = new Scripting(model, view);
    if ((result = scripting->init(argv[0])) != 0) {
        log_msg("Failed to initialize the scripting library!");
        return result;
    };

    // initialize the model using python scripts
    if (scripting->run_initialize_levels_script() != 0) {
        log_msg("Failed to run the init python method!");
        return result;
    };


    //
    // Set up global key binding stuff here.
    // 
    KeyboardHandler::add_global_action(new ToggleFullscreen(window), SDL_SCANCODE_F1);
    KeyboardHandler::add_global_action(new Quit(view), SDL_SCANCODE_ESCAPE);


    // check the binding from objects to collision resolution
    CollisionResolver * resolver;
    GameObjType * g, * g2;    
    CollisionType ct;
    for (int moveable = 0; moveable < GameObjType::get_last_moveable_id(); moveable++) {

        g = GameObjType::get_moveable_type_for_id(moveable);
        std::cout << g->get_name() << std::endl;

        for (int other = 0; other < GameObjType::get_last_id(); other++) {                    
            g2 = GameObjType::get_type_for_id(other);
        
            std::cout << "\t" << g2->get_name() << std::endl;

            for (int c = 0; c < N_VALID_COLLISION_TYPES; c++) {
                ct = ValidCollisionTypes[c];
                resolver = CollisionResolutionManager::get(g, g2, ct);

                std::cout << "\t\t" << g->get_name() << " -> " << g2->get_name() << ": " 
                          << ct << " = ";
                if (resolver == nullptr) {
                    std::cout << "NULL!";
                }
                else {
                    std::cout << resolver->get_name();
                }
                std::cout << std::endl;
            }
        }
    }

    //
    // Main Message Loop!
    //

    // enter the message loop.. move, draw, check ui events, repeat
    // until the game is over
    view->msg_loop();


    //
    // Cleaning up..
    //

    CollisionResolutionManager::clean_up();

    // clean up the model
    model->clean_up();

    // clean up the ui objects
    view->clean_up();

    // clean up the managers
    image_manager->clean_up();
    font_manager->clean_up();
    sound_manager->clean_up();
   
    // clean up
    delete view;
    delete model;
    delete scripting;
    delete image_manager;
    delete font_manager;
    delete sound_manager;

    // clean up the ttf library
    TTF_Quit();
   
    // clean up the img library
    IMG_Quit();

    // clean up the sdl library
	SDL_Quit();

    // done.
	return 0;
}
