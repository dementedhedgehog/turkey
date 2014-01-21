// required to be the first line by the python docs
#include <Python.h>

#include "shared/module.h"


Module::Module(std::string module_name) {
}


Module::~Module() {
    // clean up python
    Py_Finalize();
}

void Module::print_time() {
    // just a little demo
    PyRun_SimpleString("from time import time,ctime\n"
                       "print 'Today is',ctime(time())\n");
}

// Module::import() {
//     pModule = PyImport_Import(pName);
//     Py_DECREF(pName);

//     if (pModule != NULL) {
//         pFunc = PyObject_GetAttrString(pModule, argv[2]);
//         /* pFunc is a new reference */

//         if (pFunc && PyCallable_Check(pFunc)) {
//             pArgs = PyTuple_New(argc - 3);
//             for (i = 0; i < argc - 3; ++i) {
//                 pValue = PyInt_FromLong(atoi(argv[i + 3]));
//                 if (!pValue) {
//                     Py_DECREF(pArgs);
//                     Py_DECREF(pModule);
//                     fprintf(stderr, "Cannot convert argument\n");
//                     return 1;
//                 }
//                 /* pValue reference stolen here: */
//                 PyTuple_SetItem(pArgs, i, pValue);
//             }
//             pValue = PyObject_CallObject(pFunc, pArgs);
//             Py_DECREF(pArgs);
//             if (pValue != NULL) {
//                 printf("Result of call: %ld\n", PyInt_AsLong(pValue));
//                 Py_DECREF(pValue);
//             }
//             else {
//                 Py_DECREF(pFunc);
//                 Py_DECREF(pModule);
//                 PyErr_Print();
//                 fprintf(stderr,"Call failed\n");
//                 return 1;
//             }
//         }
//         else {
//             if (PyErr_Occurred())
//                 PyErr_Print();
//             fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
//         }
//         Py_XDECREF(pFunc);
//         Py_DECREF(pModule);
//     }
//     else {
//         PyErr_Print();
//         fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
//         return 1;
//     }
// }
