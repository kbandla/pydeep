#include <Python.h>
#include <fuzzy.h>
#include <unistd.h>

#define PYDEEP_VERSION  "0.2"

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *
error_out(PyObject *m) {
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
    return NULL;
}

static PyMethodDef pydeep_methods[] = {
    {"error_out", (PyCFunction)error_out, METH_NOARGS, NULL},
    {NULL, NULL}
};

static PyObject *pydeepError;

static PyObject * pydeep_hash_file(PyObject *self, PyObject *args){
    FILE *inputFile;
    PyObject *ssdeepHash = NULL;
    char *hashResult = NULL;
    char *filename;
    int ret;
    if (!PyArg_ParseTuple(args, "s", &filename)){
        return NULL;
    }

    if( access( filename, F_OK ) == -1 ) {
        // File does not exist
        PyErr_SetString(pydeepError, "File does not exist");
        return NULL;
    }

    inputFile = fopen(filename, "rb");
    if( inputFile <=0 ){
        // We could not open the file
        PyErr_SetString(pydeepError, "Error opening file");
        return NULL;
    }

    // Allocate return buffers for hash
    hashResult = (char *)malloc(FUZZY_MAX_RESULT);
    if (hashResult == NULL){
        PyErr_SetString(pydeepError, "Error allocating malloc buffer");
        return NULL;
    }
    ret = fuzzy_hash_file(inputFile, hashResult);
    if (ret != 0){
        free(hashResult);
        fclose(inputFile);
        PyErr_SetString(pydeepError, "Error in fuzzy_hash!");
        return NULL;
    }
    ssdeepHash = PyBytes_FromString(hashResult);
    free(hashResult);
    fclose(inputFile);
    return ssdeepHash;
}

static PyObject * pydeep_hash_buf(PyObject *self, PyObject *args){
    PyObject *inputStringBuffer = NULL;
    PyObject *ssdeepHash= NULL;
    Py_ssize_t stringSize = 0;
    char *inputBuffer=NULL;
    char *hashResult;
    int ret;

    if (!PyArg_ParseTuple(args, "s#", &inputBuffer, &stringSize)){
        return NULL;
    }

    // Allocate return buffers for hash
    hashResult = (char *)malloc(FUZZY_MAX_RESULT);
    if (hashResult == NULL){
        PyErr_SetString(pydeepError, "Error allocating malloc buffer");
        return NULL;
    }

    inputStringBuffer = PyBytes_FromStringAndSize(inputBuffer, stringSize);

    ret = fuzzy_hash_buf((unsigned char*)inputBuffer, (uint32_t)stringSize, hashResult);
    if (ret !=0 ){
        free(hashResult);
        PyErr_SetString(pydeepError, "Error in fuzzy_hash!");
        return NULL;
    }
    ssdeepHash = PyBytes_FromString( hashResult );
    free(hashResult);
    return ssdeepHash;
}

static PyObject * pydeep_compare(PyObject *self, PyObject *args){
    char *ssdeepHash1= NULL;
    char *ssdeepHash2= NULL;
    int ret;
#if PY_MAJOR_VERSION >= 3
    if (!PyArg_ParseTuple(args, "yy", &ssdeepHash1, &ssdeepHash2)){
#else
    if (!PyArg_ParseTuple(args, "ss", &ssdeepHash1, &ssdeepHash2)){
#endif
        return NULL;
    }
    ret = fuzzy_compare(ssdeepHash1, ssdeepHash2);
    if (ret < 0){
        PyErr_SetString(pydeepError, "Error in fuzzy compare!");
        return NULL;
    }
    return Py_BuildValue("i", ret);
}

// Method definitions
static PyMethodDef pydeepMethods[] = {
    {"hash_file", pydeep_hash_file, METH_VARARGS, "compute the ssdeep fuzzy hash for a file"},
    {"hash_buf", pydeep_hash_buf, METH_VARARGS, "compute the ssdeep fuzzy hash for a buffer"},
    {"hash_bytes", pydeep_hash_buf, METH_VARARGS, "compute the ssdeep fuzzy hash for a buffer"},
    {"compare", pydeep_compare, METH_VARARGS, "compute similarity between two ssdeep hashes"},
    {NULL,  NULL}
};

#if PY_MAJOR_VERSION >= 3

static int pydeep_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int pydeep_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "pydeep",
    NULL,
    sizeof(struct module_state),
    pydeepMethods,
    NULL,
    pydeep_traverse,
    pydeep_clear,
    NULL
};

#define INITERROR return NULL
#else
#define INITERROR return
#endif

#if PY_MAJOR_VERSION >= 3
PyObject *
PyInit_pydeep(void)
#else
void initpydeep(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *pydeep = PyModule_Create(&moduledef);
#else
    PyObject *pydeep = Py_InitModule3("pydeep", pydeepMethods, "C/Python bindings for ssdeep [ssdeep.sourceforge.net]");
#endif
    pydeepError = PyErr_NewException("pydeep.Error", NULL, NULL);
    Py_INCREF(pydeepError);
    PyModule_AddObject(pydeep, "error", pydeepError);
    PyModule_AddStringConstant(pydeep, "__version__", PYDEEP_VERSION);
#if PY_MAJOR_VERSION >= 3
    return pydeep;
#endif
}
