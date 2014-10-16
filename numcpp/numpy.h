
#include "numcpp.h"

#define NO_IMPORT_ARRAY
#include "numpy/arrayobject.h"

#include <complex>

namespace numcpp
{

template<class T>
struct numpy_typemap;

#define define_numpy_type(ctype, dtype) \
    template<> \
    struct numpy_typemap<ctype> \
    {static constexpr auto numpy_type = dtype;};

define_numpy_type(bool, NPY_BOOL);
define_numpy_type(char, NPY_BYTE);
define_numpy_type(short, NPY_SHORT);
define_numpy_type(int, NPY_INT);
define_numpy_type(long, NPY_LONG);
define_numpy_type(long long, NPY_LONGLONG);
define_numpy_type(unsigned char, NPY_UBYTE);
define_numpy_type(unsigned short, NPY_USHORT);
define_numpy_type(unsigned int, NPY_UINT);
define_numpy_type(unsigned long, NPY_ULONG);
define_numpy_type(unsigned long long, NPY_ULONGLONG);
define_numpy_type(float, NPY_FLOAT);
define_numpy_type(double, NPY_DOUBLE);
define_numpy_type(long double, NPY_LONGDOUBLE);
define_numpy_type(std::complex<float>, NPY_CFLOAT);
define_numpy_type(std::complex<double>, NPY_CDOUBLE);
define_numpy_type(std::complex<long double>, NPY_CLONGDOUBLE);

template<class T>
Array<T> from_numpy(PyArrayObject* pyarray)
{
    int ndims = PyArray_NDIM(pyarray);
    
    npy_intp* pyshape = PyArray_SHAPE(pyarray);
    Shape shape(pyshape, pyshape+ndims);
    
    npy_intp* pystrides = PyArray_STRIDES(pyarray);
    Strides strides(pystrides, pystrides+ndims);
    
    T* data = reinterpret_cast<T*>(PyArray_DATA(pyarray));
    
    if(numpy_typemap<T>::numpy_type == PyArray_TYPE(pyarray))
        return copy<T>(external<T>(data, shape, strides));
    else
        throw std::runtime_error("from_numpy: trying to cast array to a different type");
}

template<class T>
Array<T> cast_from_numpy(PyArrayObject* pyarray)
{
    if(numpy_typemap<T>::numpy_type == PyArray_TYPE(pyarray))
        return from_numpy<T>(pyarray);
    
    PyArrayObject* aux = (PyArrayObject*)PyArray_FromArray(pyarray,
            PyArray_DescrFromType(numpy_typemap<T>::numpy_type),
            NPY_DEFAULT|NPY_ARRAY_FORCECAST);
    if(!aux)
        throw std::runtime_error("cannot cast from numpy array to given type");
    Array<T> res = from_numpy<T>(aux);
    
    Py_XDECREF(aux);
    
    return res;
}

template<class T>
PyArrayObject* to_numpy(const Array<T>& array)
{
    int ndims = array.ndims();
    npy_intp pyshape[ndims];
    std::copy(array.shape().begin(), array.shape().end(), pyshape);
    
    // Create output array.
    PyArrayObject* res = (PyArrayObject*)PyArray_SimpleNew(ndims, pyshape, numpy_typemap<T>::numpy_type);
    
    // Copy data into new NumPy array.
    npy_intp* pystrides = PyArray_STRIDES(res);
    Strides strides(pystrides, pystrides+ndims);
    T* data = reinterpret_cast<T*>(PyArray_DATA(res));    
    Array<T> aux = external<T>(data, array.shape(), strides);
    aux.deep() = array;
    
    return res;
}

}
