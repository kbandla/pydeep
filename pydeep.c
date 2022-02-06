#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <fuzzy.h>
#include <unistd.h>
#include <bytesobject.h>

#define PYDEEP_VERSION  "0.4"

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

    ret = fuzzy_hash_buf((unsigned char*)inputBuffer, (uint32_t)stringSize, hashResult);
    if (ret !=0 ){
        free(hashResult);
        PyErr_SetString(pydeepError, "Error in fuzzy_hash!");
        return NULL;
    }
    ssdeepHash = PyBytes_FromString(hashResult);
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
        PyErr_SetString(pydeepError, "Error in fuzzy compare");
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

#if PY_MAJOR_VERSION >=3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "pydeep",       // m_name
    NULL,           // m_doc
    -1,             // m_size
    pydeepMethods,  // m_methods
    NULL,           // m_slots
    NULL,           // m_traverse
    NULL,           // m_clear
    NULL            // m_free
};
#endif

#if PY_MAJOR_VERSION >= 3
PyObject *PyInit_pydeep(void)
#else
void initpydeep(void)
#endif
{
    PyObject *pydeep;
#if PY_MAJOR_VERSION >= 3
    // Python 3
    pydeep = PyModule_Create(&moduledef);
    if (pydeep == NULL)
        return NULL;
#else
    // Python 2
    pydeep = Py_InitModule3("pydeep", pydeepMethods, "C/Python bindings for ssdeep [ssdeep.sourceforge.net]");
#endif
    pydeepError = PyErr_NewException("pydeep.Error", NULL, NULL);
    Py_INCREF(pydeepError);
    PyModule_AddObject(pydeep, "error", pydeepError);
    PyModule_AddStringConstant(pydeep, "__version__", PYDEEP_VERSION);
#if PY_MAJOR_VERSION >= 3
    return pydeep;
#endif
}
