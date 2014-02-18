// required to be the first line by the python docs
#include <Python.h>

#include <string>
#include <iostream>

#include "shared/scripting.h"
#include "model/particle_system.h"
#include "view/utils.h"

#define __STR(X) #X
#define STR(X) __STR(X)

// capsule names
#define MODULE "turkey"
#define DOT "."
#define VIEW "_VIEW"
#define IMG_MGR "_IMG_MGR"
#define MODEL "_MODEL"
#define GAME_STATE "_GAME_STATE"
#define RENDERER "_RENDERER"
#define TEXTURE "_TEXTURE"
#define SOUND_MGR "_SOUND_MGR"
#define MUSIC "_MUSIC"

// python exceptions we throw from c++
static PyObject * FailedToLoadTextureError;
static PyObject * FailedToLoadMusicError;
static PyObject * FailedToPlayMusicError;

// embedded python treats bools like ints.. 
// we use these consts to make our intention a little clearer
const int FALSE = 0;
const int TRUE = 0;


// helper function..
static int add_capsule(PyObject * module, void * c_obj, 
    const char * qualified_name, const char * name) {
    PyObject * c_api_object;

    // Create a capsule containing the view pointer 
    c_api_object = PyCapsule_New((void *)c_obj, qualified_name, NULL);
    if (c_api_object == NULL) {
        log_msg("Problem creating view object for the python scripts!");
        return 1;
    }

    // and add it to the module
    if (PyModule_AddObject(module, name, c_api_object) != 0) {
        log_msg("Problem adding object for the python scripts!");
        return 2;
    }

    return 0; // success
}



/*
 *
 * These functions expose parts of the c++ engine to python.
 *
 */



/* loads a piece of music from a file */
static PyObject* turkey_load_music(PyObject *self, PyObject *args)    
{    
    char * music_fname;
    if(!PyArg_ParseTuple(args, "s:load_music", &music_fname)) {
        log_msg("Problem parsing the python arguments");
        Py_INCREF(Py_None);
        return Py_None;
    }

    // extract the sound manager
    SoundManager * sound_manager = (SoundManager*)PyCapsule_Import(MODULE DOT SOUND_MGR, 0);
    if (sound_manager == NULL) {
        log_msg("Problem extracting the c++ sound_manager object.");
        return Py_BuildValue("i", 0);
    }

    // load the music object
    Mix_Music * music = sound_manager->load_music(music_fname);
    if (music == nullptr) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadMusicError,
            (std::string("Problem extracting the music at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }
    
    // wrap the music object in a python capsule
    PyObject * music_object = PyCapsule_New((void *)music, MUSIC, NULL);
    if (music_object == NULL) {
        log_msg("Problem creating music capsule for the python scripts!");
        Py_INCREF(Py_None);
        return Py_None;
    }
    return Py_BuildValue("O", music_object);
}



/* plays a piece of music in a loop with a 2 second fade in */
static PyObject* turkey_play_music(PyObject *self, PyObject *args)    
{        
    // get the music object from the arguments
    PyObject * music_object = nullptr;
    if (!PyArg_ParseTuple(args, "O:play_music", &music_object)) {
        return NULL;
    }

    if (music_object == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToPlayMusicError,
            (std::string("Problem extracting the music at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }


    // extract the sound manager
    SoundManager * sound_manager = (SoundManager*)PyCapsule_Import(MODULE DOT SOUND_MGR, 0);
    if (sound_manager == NULL) {
        log_msg("Problem extracting the c++ sound_manager object.");
        return Py_BuildValue("i", 0);
    }


    // extract the music from the python capsule
    Mix_Music * music = nullptr;
    music = (Mix_Music *)PyCapsule_GetPointer(music_object, MUSIC);
    if (music == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureError,
            (std::string("Problem extracting the texture at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }

    // play it again sam..
    sound_manager->play_music(music);

    return Py_BuildValue("i", 1);
}


/* Adds a new scenery object to the game */
static PyObject* turkey_add_scenery(PyObject *self, PyObject *args)    
{    
    float x, y, scroll_rate;
    PyObject * texture_object = nullptr;
    
    if (!PyArg_ParseTuple(args, "fffO:add_scenery", 
            &scroll_rate, &x, &y, &texture_object)) {
        return NULL;
    }

    View * view = (View*)PyCapsule_Import(MODULE DOT VIEW, 0);
    if (view == NULL) {
        log_msg("Problem extracting the c++ view object.");
        return Py_BuildValue("i", 0);
    }
        
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, TEXTURE);
    }

    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureError,
            (std::string("Problem extracting the texture at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }

    view->add_scenery(scroll_rate, x, y, texture);

    return Py_BuildValue("i", 1);
}


/*
 * Quit the game!!!
 */
static PyObject* turkey_quit(PyObject *self, PyObject *args)    
{        
    if (!PyArg_ParseTuple(args, ":quit")) {
        return NULL;
    }
       
    View * view = (View*)PyCapsule_Import(MODULE DOT VIEW, 0);
    if (view == NULL) {
        log_msg("Problem extracting the c++ view object.");
        return Py_BuildValue("i", 0);
    }
    
    // turn on the cell grid 
    view->quit();

    Py_INCREF(Py_None);
    return Py_None;
}


/*
 * Set the default gravity for new game objects
 */
static PyObject* turkey_set_default_gravity(PyObject *self, PyObject *args)    
{    
    float gravity; // pixels per second per second
    
    if (!PyArg_ParseTuple(args, "f:set_default_gravity", &gravity)) {
        return NULL;
    }
    
    // turn on the cell grid 
    GameObj::default_gravity = gravity;

    Py_INCREF(Py_None);
    return Py_None;
}


/*
 * Set the default jump velocity for new game objects
 */
static PyObject* turkey_set_default_jump_velocity(PyObject *self, PyObject *args)    
{    
    float jump_velocity; // pixels per second 
    
    if (!PyArg_ParseTuple(args, "f:set_default_jump_velocity", &jump_velocity)) {
        return NULL;
    }
    
    // set the default jump velocity
    GameObj::default_jump_velocity = jump_velocity;

    Py_INCREF(Py_None);
    return Py_None;
}




/*
 * Set the default maximum x velocity for new game objects
 */
static PyObject* turkey_set_default_max_x_velocity(PyObject *self, PyObject *args)    
{    
    float max_velocity; // pixels per second 
    
    if (!PyArg_ParseTuple(args, "f:set_default_max_x_velocity", &max_velocity)) {
        return NULL;
    }
    
    // set the default maximum x velocity
    GameObj::default_x_max_velocity = max_velocity;

    Py_INCREF(Py_None);
    return Py_None;
}


/*
 * Set the default maximum y velocity for new game objects
 */
static PyObject* turkey_set_default_max_y_velocity(PyObject *self, PyObject *args)    
{    
    float max_velocity; // pixels per second 
    
    if (!PyArg_ParseTuple(args, "f:set_default_max_y_velocity", &max_velocity)) {
        return NULL;
    }
    
    // set the default maximum y velocity
    GameObj::default_y_max_velocity = max_velocity;

    Py_INCREF(Py_None);
    return Py_None;
}



/*
 * Set the default x acceleration for new game objects (what happens when you go left or right)
 */
static PyObject* turkey_set_default_x_acceleration(PyObject *self, PyObject *args)    
{    
    float max_acceleration; // pixels per second per second
    
    if (!PyArg_ParseTuple(args, "f:set_default_max_x_acceleration", &max_acceleration)) {
        return NULL;
    }
    
    // set the default max x acceleration
    GameObj::default_x_acceleration = max_acceleration;

    Py_INCREF(Py_None);
    return Py_None;
}


/*
 * Set the default default x deceleration for new game objects
 */
static PyObject* turkey_set_default_x_deceleration(PyObject *self, PyObject *args)    
{    
    float x_deceleration; // pixels per second per second
    
    if (!PyArg_ParseTuple(args, "f:set_default_x_deceleration", &x_deceleration)) {
        return NULL;
    }
    
    // set the default max y acceleration
    GameObj::default_x_deceleration = x_deceleration;

    Py_INCREF(Py_None);
    return Py_None;
}



/*
 * Turns the cell grid on/off in the game view.
 */
static PyObject* turkey_debug_set_draw_grid(PyObject *self, PyObject *args)    
{    
    int draw_grid;
    
    if (!PyArg_ParseTuple(args, "i:debug_set_draw_grid", &draw_grid)) {
        return NULL;
    }
       
    View * view = (View*)PyCapsule_Import(MODULE DOT VIEW, 0);
    if (view == NULL) {
        log_msg("Problem extracting the c++ view object.");
        return Py_BuildValue("i", 0);
    }
    
    // turn on the cell grid 
    view->debug_set_draw_grid((bool)draw_grid);

    Py_INCREF(Py_None);
    return Py_None;
}

/*
 * Turns the frames per second display on/off
 */
static PyObject* turkey_debug_set_draw_fps(PyObject *self, PyObject *args)    
{    
    int draw_fps;
    
    if (!PyArg_ParseTuple(args, "i:debug_set_draw_fps", &draw_fps)) {
        return NULL;
    }
       
    View * view = (View*)PyCapsule_Import(MODULE DOT VIEW, 0);
    if (view == NULL) {
        log_msg("Problem extracting the c++ view object.");
        return Py_BuildValue("i", 0);
    }
    
    // turn on the cell grid 
    view->debug_set_draw_fps((bool)draw_fps);

    Py_INCREF(Py_None);
    return Py_None;
}



/* Adds a new game object to the game */
static PyObject* turkey_add_game_obj(PyObject *self, PyObject *args)    
{    
    float x, y;
    int movable = FALSE;
    PyObject * texture_object = nullptr;

    if (!PyArg_ParseTuple(args, "ff|Oi:add_game_obj", &x, &y, &texture_object, &movable)) {
        return NULL;
    }
       
    Model * model = (Model*)PyCapsule_Import(MODULE DOT MODEL, 0);
    if (model == NULL) {
        log_msg("Problem extracting the c++ model object.");
        return Py_BuildValue("i", 0);
    }
        
    if (texture_object == nullptr) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureError,
            (std::string(
                "Problem extracting the texture at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }    

    SDL_Texture * texture = nullptr;
    texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, TEXTURE);

    GameObj * game_obj = new GameObj(x, y, texture, (bool)movable);
    GameState * game_state = model->get_game_state();
    game_state->add_game_obj(game_obj);

    return Py_BuildValue("i", 1);
}



/* Adds a particle system to the game */
static PyObject* turkey_add_particle_system(PyObject *self, PyObject *args)    
{    
    PyObject * texture_object = nullptr;
    
    if (!PyArg_ParseTuple(args, "O:add_particle_system", &texture_object)) {
        return NULL;
    }
       
    Model * model = (Model*)PyCapsule_Import(MODULE DOT MODEL, 0);
    if (model == NULL) {
        log_msg("Problem extracting the c++ model object.");
        return Py_BuildValue("i", 0);
    }
        
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, TEXTURE);
    }

    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureError,
            (std::string(
                "Problem extracting the texture at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }

    ParticleSystem * particle_system = new ParticleSystem(texture);
    GameState * game_state = model->get_game_state();
    game_state->add_particle_system(particle_system);

    return Py_BuildValue("i", 1);
}



/* 
 * Add a game object to the game.
 */
static PyObject* turkey_add_character_game_obj(PyObject *self, PyObject *args)    
{    
    float x, y;
    PyObject * texture_object = nullptr;
    
    if(!PyArg_ParseTuple(args, "ff|O:add_character_game_obj", &x, &y, &texture_object)) {
        return NULL;
    }
       
    Model * model = (Model*)PyCapsule_Import(MODULE DOT MODEL, 0);
    if (model == NULL) {
        log_msg("Problem extracting the c++ model object.");
        return Py_BuildValue("i", 0);
    }
    
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, TEXTURE);
    }

    // check we got a texture
    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureError,
            (std::string(
                "Problem extracting the texture at ") + 
                __FILE__ + "," + STR(__LINE__) + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }

    const bool movable = true;
    GameObj * game_obj = new GameObj(x, y, texture, movable);
    GameState * game_state = model->get_game_state();
    game_state->add_character_game_obj(game_obj);

    return Py_BuildValue("i", 1);
}



/*
 * Loads a texture from an image file.
 */
static PyObject* turkey_load_texture(PyObject *self, PyObject *args)
{    

    char * texture_fname;
    if(!PyArg_ParseTuple(args, "s:load_texture", &texture_fname)) {
        log_msg("Problem parsing the python arguments");
        Py_INCREF(Py_None);
        return Py_None;
    }
    
    /* FIXME: clean this shit up */
    View * view = (View *)PyCapsule_Import(MODULE DOT VIEW, 0);
    if (view == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureError,
            "Problem extracting the c++ view object. " 
            __FILE__ ":" STR(__LINE__) "\n");
        return NULL;
    }

    // FIXME: this is ugly..
    SDL_Texture * texture = load_texture(texture_fname, view->get_renderer());
    if (texture == NULL) {
        log_msg("Problem extracting the c++ texture object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject * texture_object = PyCapsule_New((void *)texture, TEXTURE, NULL);
    if (texture_object == NULL) {
        log_msg("Problem creating texture for the python scripts!");
        Py_INCREF(Py_None);
        return Py_None;
    }

    return Py_BuildValue("O", texture_object);
}



/*
 * Loads a list of equispaced textures from an spritesheet image file.
 */
static PyObject* turkey_load_textures_using_grid(PyObject *self, PyObject *args)
{    
    // parse the arguments
    int x, y, w, h;
    char * texture_fname;
    if(!PyArg_ParseTuple(args, "siiii:load_texture", &texture_fname, &x, &y, &w, &h)) {
        log_msg("Problem parsing the python arguments");
        Py_INCREF(Py_None);
        return Py_None;
    }
       
    // extract the image manager 
    ImageManager * image_manager = (ImageManager *)PyCapsule_Import(MODULE DOT IMG_MGR, 0);
    if (image_manager == NULL) {
        log_msg("Problem extracting the image manager from the c++ view object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    // extract the renderer
    SDL_Renderer * renderer = (SDL_Renderer *)PyCapsule_Import(MODULE DOT RENDERER, 0);
    if (renderer == NULL) {
        log_msg("Problem extracting the renderer from the c++ view object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    // load the textures from the sprite sheet
    std::vector<SDL_Texture*> * textures = 
        image_manager->load_textures_from_sprite_sheet_using_grid(
            texture_fname, 
            renderer, 
            x, y, w, h);


    SDL_Texture * texture;
    PyObject * py_textures = PyList_New((uint32_t)textures->size());
    std::vector<SDL_Texture*>::iterator i;
    Py_ssize_t index = 0;
    for (i = textures->begin(); i != textures->end(); i++) {

        // get the SDL texture
        texture = *i;
        if (texture == NULL) {
            log_msg("Problem extracting the c++ texture object.");
            Py_INCREF(Py_None);
            return Py_None;
        }

        // wrap it in a capsule
        PyObject * texture_object = PyCapsule_New((void *)texture, TEXTURE, NULL);
        if (texture_object == NULL) {
            log_msg("Problem creating texture for the python scripts!");
            Py_INCREF(Py_None);
            return Py_None;
        }

        // add it to the list
        if (PySequence_SetItem(py_textures, index, texture_object) == -1) {
            log_msg("Problem adding texture to the list for the python scripts!");
            Py_INCREF(Py_None);
            return Py_None;
        }
        index += 1;
    }

    return Py_BuildValue("O", py_textures);
}




/*
 * Loads a list of textures from an spritesheet image file given a list of rects.
 */
static PyObject* turkey_load_textures_using_rects(PyObject *self, PyObject *args)
{    
    // parse the arguments
    char * texture_fname;
    PyObject * py_rects_maybe;
    if(!PyArg_ParseTuple(args, "sO:load_texture", &texture_fname, &py_rects_maybe)) {
        log_msg("Problem parsing the python arguments");
        Py_INCREF(Py_None);
        return Py_None;
    }
       
    // extract the image manager 
    ImageManager * image_manager = (ImageManager *)PyCapsule_Import(MODULE DOT IMG_MGR, 0);
    if (image_manager == NULL) {
        log_msg("Problem extracting the image manager from the c++ view object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    // extract the renderer
    SDL_Renderer * renderer = (SDL_Renderer *)PyCapsule_Import(
        MODULE DOT RENDERER, 0);
    if (renderer == NULL) {
        log_msg("Problem extracting the renderer from the c++ view object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    // convert from a list of int 4-tuples to a c++ vector of SDL_Rect
    std::vector<SDL_Rect*> rects;
    SDL_Rect * rect;
    PyObject * py_rects;
    int i, len;
    std::vector<SDL_Rect*>::iterator j;
    py_rects = PySequence_Fast(py_rects_maybe, "expected a sequence");
    len = PySequence_Size(py_rects_maybe);
    PyObject* py_tuple;
    for (i = 0; i < len; i++) {
        py_tuple = PySequence_Fast_GET_ITEM(py_rects, i);

        rect = new SDL_Rect();
        if (!PyArg_ParseTuple(py_tuple, "ffff:load_texture", 
                &rect->x, &rect->y, &rect->w, &rect->h)) {
            Py_DECREF(py_tuple);
            log_msg("Problem parsing tuple python arguments");
            Py_INCREF(Py_None);
            return Py_None;
        }

        rects.push_back(rect);
    }
    Py_DECREF(py_tuple);
    

    // load the textures from the sprite sheet
    std::vector<SDL_Texture*> * textures = 
        image_manager->load_textures_from_sprite_sheet_using_rects(
            texture_fname, 
            renderer, 
            rects);
    
    // free the rects
    for (j = rects.begin(); j != rects.end(); j++) {
        rect = *j;
        free(rect);
    }
    

    SDL_Texture * texture;
    PyObject * py_textures = PyList_New((uint32_t)textures->size());
    Py_ssize_t index = 0;
    std::vector<SDL_Texture*>::iterator k;
    for (k = textures->begin(); k != textures->end(); k++) {

        // get the SDL texture
        texture = *k;
        if (texture == NULL) {
            log_msg("Problem extracting the c++ texture object.");
            Py_INCREF(Py_None);
            return Py_None;
        }

        // wrap it in a capsule
        PyObject * texture_object = PyCapsule_New((void *)texture, TEXTURE, NULL);
        if (texture_object == NULL) {
            log_msg("Problem creating texture for the python scripts!");
            Py_INCREF(Py_None);
            return Py_None;
        }

        // add it to the list
        if (PySequence_SetItem(py_textures, index, texture_object) == -1) {
            log_msg("Problem adding texture to the list for the python scripts!");
            Py_INCREF(Py_None);
            return Py_None;
        }
        index += 1;
    }

    return Py_BuildValue("O", py_textures);
}





// wrap the methods in a module so that 
static PyMethodDef InitializeTurkeyMethods[] = {

    {"quit", 
     turkey_quit, 
     METH_VARARGS, 
     "Quit the game!!!"},

    {"load_music", 
     turkey_load_music, 
     METH_VARARGS, 
     "Load music from a file."},

    {"play_music", 
     turkey_play_music, 
     METH_VARARGS, 
     "Play a piece of music in a loop with a 2 second fade in.."},

    {"add_scenery", 
     turkey_add_scenery, 
     METH_VARARGS, 
     "Add a new piece of scenery to the game."},

    {"debug_set_draw_grid", 
     turkey_debug_set_draw_grid, 
     METH_VARARGS, 
     "DEBUG METHOD: Set whether to display the cell grid or not."},

    {"debug_set_draw_fps", 
     turkey_debug_set_draw_fps, 
     METH_VARARGS, 
     "DEBUG METHOD: Set whether to display the frames per second or not."},

    {"set_default_gravity", 
     turkey_set_default_gravity, 
     METH_VARARGS, 
     "Set the default gravity."},

    {"set_default_jump_velocity", 
     turkey_set_default_jump_velocity, 
     METH_VARARGS, 
     "Set the default jump velocity (effects or affects the jump height)."},

    {"set_default_max_x_velocity", 
     turkey_set_default_max_x_velocity, 
     METH_VARARGS, 
     "Set the default max velocity in the x axis."},

    {"set_default_max_y_velocity", 
     turkey_set_default_max_y_velocity, 
     METH_VARARGS, 
     "Set the default max velocity in the y axis."},

    {"set_default_x_acceleration", 
     turkey_set_default_x_acceleration, 
     METH_VARARGS, 
     "Set the default x acceleration (your acceleration when moving left or right)."},

    {"set_default_x_deceleration", 
     turkey_set_default_x_deceleration, 
     METH_VARARGS, 
     "Set the default x deceleration (your deceleration when you stop moving left or right)."},

    {"add_game_obj", 
     turkey_add_game_obj, 
     METH_VARARGS, 
     "Add a game object to be displayed."},

    {"add_particle_system", 
     turkey_add_particle_system, 
     METH_VARARGS, 
     "Add a particle system to the game."},

    {"add_character_game_obj", 
     turkey_add_character_game_obj, 
     METH_VARARGS, 
     "Add a character game object to be displayed."},

    {"load_texture", 
     turkey_load_texture, 
     METH_VARARGS, 
     "Load a texture to be displayed."},

    {"load_textures_using_grid", 
     turkey_load_textures_using_grid, 
     METH_VARARGS, 
     "Load a list of textures from a sprite sheet using a grid."},

    {"load_textures_using_rects", 
     turkey_load_textures_using_rects, 
     METH_VARARGS, 
     "Load a list of textures from a sprite sheet using a list of rectangles."},

    {NULL, NULL, 0, NULL} // sentinel.
};



/*
 * Initialize the embedded python engine.
 */
Scripting::Scripting(Model * model, View * view) {
    this->model = model;
    this->view = view;

    this->renderer = view->get_renderer();
    //this->game_view = view->get_game_view();
    this->game_state = model->get_game_state();
    this->image_manager = view->get_image_manager();
    this->sound_manager = view->get_sound_manager();
}




int Scripting::init(char * program_fname) {

    // add the directory containing the executable to the python path
    // (no return value to check!)
    Py_SetProgramName(program_fname);

    // Disable importing site.py
    Py_NoSiteFlag = 1;  

    // initialize embedded python scripting
    // (no return value to check!)
    Py_Initialize();

    // initialize the turkey python module (the python->c++ interface to the engine).
    PyObject * module;
    module = Py_InitModule("turkey", InitializeTurkeyMethods);
    if (module == NULL) {
        log_msg("Problem initializing scripts!");
        return 1;
    }

    // Create capsules containing pointers to things we want to expose to the python api.
    int result;
    result = add_capsule(module, (void *)model, MODULE DOT MODEL, MODEL);
    if (result != 0) {
        return result;
    }

    result = add_capsule(module, (void *)game_state, MODULE DOT GAME_STATE, GAME_STATE);
    if (result != 0) {
        return result;
    }

    result = add_capsule(module, (void *)view, MODULE DOT VIEW, VIEW);
    if (result != 0) {
        return result;
    }

    result = add_capsule(module, (void *)renderer, MODULE DOT RENDERER, RENDERER);
    if (result != 0) {
        return result;
    }

    result = add_capsule(module, (void *)image_manager, MODULE DOT IMG_MGR, IMG_MGR);
    if (result != 0) {
        return result;
    }

    result = add_capsule(module, (void *)sound_manager, MODULE DOT SOUND_MGR, SOUND_MGR);
    if (result != 0) {
        return result;
    }
        
    return 0;
}





Scripting::~Scripting() {
    // clean up python
    // (this function can be called without calling init - it's safe)
    Py_Finalize();
}


/*
 * Execute the initialize levels python script from c++
 *
 */
int Scripting::run_initialize_levels_script() {
    PyObject *py_name, *py_module, *py_func;
    PyObject *py_args, *py_value;

    //std::string 
    const char * module_name = "scripts";
    const char * fn_name = "initialize_levels";

    py_name = PyString_FromString(module_name);
    py_module = PyImport_Import(py_name);
    Py_DECREF(py_name);

    if (py_module != NULL) {

        // create a global exception class
        FailedToLoadTextureError = PyErr_NewException(
            (char*)"turkey.FailedToLoadTextureError", NULL, NULL);
        Py_INCREF(FailedToLoadTextureError);
        PyModule_AddObject(py_module, "error", FailedToLoadTextureError);
 

        // call the init function
        py_func = PyObject_GetAttrString(py_module, fn_name);

        if (py_func && PyCallable_Check(py_func)) {
            py_args = PyTuple_New(0);
            py_value = PyObject_CallObject(py_func, py_args);
            Py_DECREF(py_args);
            if (py_value != NULL) {
                log_msg("Result of call: %ld\n" + PyInt_AsLong(py_value));
                Py_DECREF(py_value);
            }
            else {
                Py_DECREF(py_func);
                Py_DECREF(py_module);
                PyErr_Print();
                log_msg("Call to initialize levels failed \n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            std::cout << "Cannot find function \"" << fn_name << "\"" << std::endl;
        }
        Py_XDECREF(py_func);
        Py_DECREF(py_module);
    }
    else {
        PyErr_Print();
        std::cout << "Failed to load \"" << module_name << "\"" << std::endl;
        return 1;
    }
    return 0; // success
}

