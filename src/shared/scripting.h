#ifndef SCRIPTING_H
#define SCRIPTING_H


/* /\* */
/*  * Class that wraps a python module. */
/*  *\/ */
/* class Module { */
/*  private: */
/*     PyObject * module_ptr; */

/*  protected: */
/*     Module(std::string module_name); */
/*     ~Module() { Py_DECREF(pModule); }; */
/* };     */


/* class InitalizeScript : public LoadModule { */
/*  public: */
/*     InitializeScript(); */


/* } */


/*
 * Object that provides access to python scripts..
 *
 */
class Scripting {
public:
    Scripting();
    ~Scripting();
    
    void print_time();
    /* void import(); */

    // run the initialization script 
    int initialize_script();

};

#endif
