// required to be the first line by the python docs
#include <Python.h>

#include <string>
#include <iostream>

#include "shared/scripting.h"
#include "view/utils.h"


/*
 * These functions expose parts of the c++ engine to python.
 */

static int numargs = 0;


/* Returns the number of game objects added (0 or 1) */
static PyObject* turkey_add_game_obj(PyObject *self, PyObject *args)    
{    
    int x, y;
    PyObject * texture_object = nullptr;
    
    if(!PyArg_ParseTuple(args, "ii|O:add_game_obj", &x, &y, &texture_object)) {
        return NULL;
    }
       
    Model * model = (Model*)PyCapsule_Import("turkey._MODEL", 0);
    if (model == NULL) {
        log_msg("Problem extracting the model c api.");
        return Py_BuildValue("i", 0);
    }

    
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, "turkey._TEXTURE");
        if (texture == NULL) {
            log_msg("Problem extracting the texture.");
            return Py_BuildValue("i", 0);
        }
    }

    GameObj * game_obj = new GameObj(x, y, texture);
    GameState * game_state = model->get_game_state();
    game_state->add_game_obj(game_obj);

    return Py_BuildValue("i", 1);
}



/* Returns the number of game objects added (0 or 1) */
static PyObject* turkey_add_character_game_obj(PyObject *self, PyObject *args)    
{    
    int x, y;
    PyObject * texture_object = nullptr;
    
    if(!PyArg_ParseTuple(args, "ii|O:add_game_obj", &x, &y, &texture_object)) {
        return NULL;
    }
       
    Model * model = (Model*)PyCapsule_Import("turkey._MODEL", 0);
    if (model == NULL) {
        log_msg("Problem extracting the model c api.");
        return Py_BuildValue("i", 0);
    }
    
    SDL_Texture * texture = nullptr;
    if (texture_object != nullptr) {
        texture = (SDL_Texture *)PyCapsule_GetPointer(texture_object, "turkey._TEXTURE");
        if (texture == NULL) {
            log_msg("Problem extracting the texture.");
            return Py_BuildValue("i", 0);
        }
    }

    GameObj * game_obj = new GameObj(x, y, texture);
    GameState * game_state = model->get_game_state();
    game_state->add_character_game_obj(game_obj);

    return Py_BuildValue("i", 1);
}



/* Returns the number of game objects added (0 or 1) */
static PyObject* turkey_load_texture(PyObject *self, PyObject *args)
{    
    char * texture_fname;
    if(!PyArg_ParseTuple(args, "s:load_texture", &texture_fname))
        return Py_None;
       
    View * view = (View *)PyCapsule_Import("turkey._VIEW", 0);
    if (view == NULL) {
        log_msg("Problem extracting the view c api.");
        return Py_None;
    }

    SDL_Texture * texture = load_texture(texture_fname, view->get_renderer());

    PyObject * texture_object = PyCapsule_New((void *)texture, "turkey._TEXTURE", NULL);
    if (texture_object == NULL) {
        log_msg("Problem creating texture for the python scripts!");
        return Py_None;
    }

    return Py_BuildValue("O", texture_object);
}

// wrap the methods in a module so that 
static PyMethodDef InitializeTurkeyMethods[] = {

    {"add_game_obj", 
     turkey_add_game_obj, 
     METH_VARARGS, 
     "Add a game object to be displayed."},

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

    // initialize embedded python scripting
    // (no return value to check!)
    Py_Initialize();

    // initialize the turkey python module (the python->c++ interface to the engine).
    PyObject * module;
    numargs = 6;
    module = Py_InitModule("turkey", InitializeTurkeyMethods);
    if (module == NULL) {
        log_msg("Problem initializing scripts!");
        return 1;
    }

    /* Create a capsule containing the model pointer */
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

    /* Create a capsule containing the view pointer */
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
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    //std::string 
    const char * module_name = "turkey_scripts";
    const char * fn_name = "initialize_levels";

    pName = PyString_FromString(module_name);
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, fn_name);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(0);
            // pArgs = PyTuple_New(argc - 3);
            // for (i = 0; i < argc - 3; ++i) {
            //     pValue = PyInt_FromLong(atoi(argv[i + 3]));
            //     if (!pValue) {
            //         Py_DECREF(pArgs);
            //         Py_DECREF(pModule);
            //         fprintf(stderr, "Cannot convert argument\n");
            //         return 1;
            //     }
            //     /* pValue reference stolen here: */
            //     PyTuple_SetItem(pArgs, i, pValue);
            // }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
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
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        std::cout << "Failed to load \"" << module_name << "\"" << std::endl;
        return 1;
    }
    return 0;
}

