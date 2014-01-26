// required to be the first line by the python docs
#include <Python.h>

#include <string>
#include <iostream>

#include "shared/scripting.h"



static int numargs = 0;


/* Return the number of arguments of the application command line */
static PyObject* turkey_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return Py_BuildValue("i", numargs);
}


static PyMethodDef InitializeTurkeyMethods[] = {
    {"numargs", turkey_numargs, METH_VARARGS,
     "Return the number of arguments received by the process."},
    {NULL, NULL, 0, NULL}
};



Scripting::Scripting() {
    // initialize embedded python scripting
    // FIXME: check it worked!
    Py_Initialize();

    numargs = 6;
    Py_InitModule("turkey", InitializeTurkeyMethods);

    // PyRun_SimpleString("from time import time,ctime\n"
    //                    "print 'Today is',ctime(time())\n");
}


Scripting::~Scripting() {
    // clean up python
    Py_Finalize();
}

void Scripting::print_time() {
    // just a little demo
    PyRun_SimpleString("from time import time,ctime\n"
                       "print 'Today is',ctime(time())\n");
}


// Scripting::import(std::string module_name) {
//     PyObject * python_module, python_name;

//     // load the module with the given name 
//     // (have to convert the string to a python string and release it).
//     python_name = PyString_FromString(module_name.cstr);
//     module_ptr = PyImport_Import(python_name);
//     Py_DECREF(python_name);

//     Module * module;
//     if (module_ptr != NULL) {
//         module = new Module(module_ptr);
//     }
//     else {
//         PyErr_Print();
//         std::cout << "Failed to load \"" << module_name << std::endl;
//         module = nullptr;
//     }
//     return module;
// }





int Scripting::initialize_script() {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    //std::string 
    const char * module_name = "turkey_initialize";
    const char * fn_name = "foo";

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

