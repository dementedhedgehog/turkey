// required to be the first line by the python docs
#include <Python.h>

#include <string>
#include <iostream>

#include "shared/scripting.h"
#include "model/particle_system.h"
#include "view/utils.h"


// python exceptions we throw from c++
static PyObject * FailedToLoadTextureErr;

// embedded python treats bools like ints.. 
// we use these consts to make our intention a little clearer
const int FALSE = 0;
const int TRUE = 0;


/*
 *
 * These functions expose parts of the c++ engine to python.
 *
 */


/* Adds a new scenery object to the game */
static PyObject* turkey_add_scenery(PyObject *self, PyObject *args)    
{    
    float x, y, scroll_rate;
    PyObject * texture_object = nullptr;
    
    if (!PyArg_ParseTuple(args, "fffO:add_scenery", 
            &scroll_rate, &x, &y, &texture_object)) {
        return NULL;
    }

    View * view = (View*)PyCapsule_Import("turkey._VIEW", 0);
    if (view == NULL) {
        log_msg("Problem extracting the c++ view object.");
        return Py_BuildValue("i", 0);
    }
        
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, "turkey._TEXTURE");
    }

    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureErr,
            (std::string(
                "Problem extracting the texture at ") + 
                //__FILE__ + "," + (char*)__LINE__ + ": " +
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
    if (!PyArg_ParseTuple(args, ":debug_set_draw_grid")) {
        return NULL;
    }
       
    View * view = (View*)PyCapsule_Import("turkey._VIEW", 0);
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
 * Turns the cell grid on/off in the game view.
 */
static PyObject* turkey_debug_set_draw_grid(PyObject *self, PyObject *args)    
{    
    int draw_grid;
    
    if (!PyArg_ParseTuple(args, "i:debug_set_draw_grid", &draw_grid)) {
        return NULL;
    }
       
    View * view = (View*)PyCapsule_Import("turkey._VIEW", 0);
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
       
    View * view = (View*)PyCapsule_Import("turkey._VIEW", 0);
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
       
    Model * model = (Model*)PyCapsule_Import("turkey._MODEL", 0);
    if (model == NULL) {
        log_msg("Problem extracting the c++ model object.");
        return Py_BuildValue("i", 0);
    }
        
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, "turkey._TEXTURE");
    }

    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureErr,
            (std::string(
                "Problem extracting the texture at ") + 
                //__FILE__ + "," + (char*)__LINE__ + ": " +
                SDL_GetError()
             ).c_str());
        return NULL;
    }

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
       
    Model * model = (Model*)PyCapsule_Import("turkey._MODEL", 0);
    if (model == NULL) {
        log_msg("Problem extracting the c++ model object.");
        return Py_BuildValue("i", 0);
    }
        
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, "turkey._TEXTURE");
    }

    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureErr,
            (std::string(
                "Problem extracting the texture at ") + 
                //__FILE__ + "," + (char*)__LINE__ + ": " +
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
       
    Model * model = (Model*)PyCapsule_Import("turkey._MODEL", 0);
    if (model == NULL) {
        log_msg("Problem extracting the c++ model object.");
        return Py_BuildValue("i", 0);
    }
    
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, "turkey._TEXTURE");
    }

    // check we got a texture
    if (texture == NULL) {
        // throw an exception!
        PyErr_SetString(
            FailedToLoadTextureErr,
            (std::string(
                "Problem extracting the texture at ") + 
                __FILE__ + "," + (char*)__LINE__ + ": " +
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
       
    View * view = (View *)PyCapsule_Import("turkey._VIEW", 0);
    if (view == NULL) {
        log_msg("Problem extracting the c++ view object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    // FIXME: this is ugly..
    SDL_Texture * texture = load_texture(texture_fname, view->get_renderer());
    //SDL_Texture * texture = view->pyapi_load_texture(texture_fname);
    if (texture == NULL) {
        log_msg("Problem extracting the c++ texture object.");
        Py_INCREF(Py_None);
        return Py_None;
    }

    PyObject * texture_object = PyCapsule_New((void *)texture, "turkey._TEXTURE", NULL);
    if (texture_object == NULL) {
        log_msg("Problem creating texture for the python scripts!");
        Py_INCREF(Py_None);
        return Py_None;
    }

    return Py_BuildValue("O", texture_object);
}

// wrap the methods in a module so that 
static PyMethodDef InitializeTurkeyMethods[] = {

    {"quit", 
     turkey_quit, 
     METH_VARARGS, 
     "Quit the game!!!"},

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

    {NULL, NULL, 0, NULL} // sentinel.
};



/*
 * Initialize the embedded python engine.
 */
Scripting::Scripting(Model * model, View * view) {
    this->model = model;
    this->view = view;
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

    // Create a capsule containing the model pointer
    PyObject * c_api_object;
    c_api_object = PyCapsule_New((void *)model, "turkey._MODEL", NULL);
    if (c_api_object == NULL) {
        log_msg("Problem creating model object for the python scripts!");
        return 2;
    }

    // and add it to the module
    if (PyModule_AddObject(module, "_MODEL", c_api_object) != 0) {
        log_msg("Problem adding model for the python scripts!");
        return 3;
    }

    // Create a capsule containing the view pointer 
    c_api_object = PyCapsule_New((void *)view, "turkey._VIEW", NULL);
    if (c_api_object == NULL) {
        log_msg("Problem creating view object for the python scripts!");
        return 4;
    }

    // and add it to the module
    if (PyModule_AddObject(module, "_VIEW", c_api_object) != 0) {
        log_msg("Problem adding view for the python scripts!");
        return 5;
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
        FailedToLoadTextureErr = PyErr_NewException(
            (char*)"turkey.FailedToLoadTextureErr", NULL, NULL);
        Py_INCREF(FailedToLoadTextureErr);
        PyModule_AddObject(py_module, "error", FailedToLoadTextureErr);
 

        // call the init function
        py_func = PyObject_GetAttrString(py_module, fn_name);

        if (py_func && PyCallable_Check(py_func)) {
            py_args = PyTuple_New(0);
            py_value = PyObject_CallObject(py_func, py_args);
            Py_DECREF(py_args);
            if (py_value != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(py_value));
                Py_DECREF(py_value);
            }
            else {
                Py_DECREF(py_func);
                Py_DECREF(py_module);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
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
    return 0;
}

