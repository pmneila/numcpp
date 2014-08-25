
#ifndef NUMCPP_INITIALIZATION_H
#define NUMCPP_INITIALIZATION_H

#include <vector>
#include <numeric>

#include "manager.h"
#include "array.h"

namespace numcpp
{

inline std::vector<std::ptrdiff_t> contiguousStrides(const std::vector<size_t>& shape, size_t elemSize)
{
    std::vector<std::ptrdiff_t> strides(shape.size());
    
    auto strides_it = strides.rbegin();
    auto strides_it_end = strides.rend();
    auto shape_it = shape.rbegin();
    int prod = elemSize;
    
    for(; strides_it != strides_it_end; ++shape_it, ++strides_it)
    {
        *strides_it = prod;
        prod *= *shape_it;
    }
    
    return strides;
}

template<typename T>
Array<T> empty(const Shape& shape)
{
    size_t numElements = prod(shape);
    size_t size = numElements * sizeof(T);
    Manager::Ptr manager = SimpleManager::allocate(size);
    auto strides = contiguousStrides(shape, sizeof(T));
    
    ArrayCore core(shape, strides, manager, 0);
    
    return Array<T>(core);
}

template<typename T>
Array<T> zeros(const Shape& shape)
{
    Array<T> res = empty<T>(shape);
    res.deep() = T(0);
    return res;
}

template<typename T>
Array<T> zeros_like(const Array<T>& arr)
{
    return zeros<T>(arr.shape());
}

template<typename T>
Array<T> ones(const Shape& shape)
{
    Array<T> res = empty<T>(shape);
    res.deep() = T(1);
    return res;
}

template<typename Function>
auto fromFunction(Function f, const Shape& shape)
    -> Array<decltype(f(shape))>
{
    typedef decltype(f(shape)) T;
    auto res = empty<T>(shape);
    auto res_it = res.begin();
    auto res_it_end = res.end();
    for(; res_it != res_it_end; ++res_it)
        *res_it = f(res_it.counter());
    
    return res;
}

template<typename T>
Array<T> singleton(const T& value)
{
    auto res = empty<T>({});
    res() = value;
    return res;
}

template<typename T>
Array<T>::Array(const T& value)
    : Array(singleton<T>(value))
{}


template<typename T>
Array<T> copy(const Array<T>& in)
{
    Array<T> res = empty<T>(in.shape());
    res.deep() = in;
    return res;
}

template<typename T, typename Owner>
Array<T> external(T* data, const Shape& shape, const Strides& strides, const Owner& owner)
{
    Manager::Ptr manager = ExternalManager<Owner>::build(reinterpret_cast<unsigned char*>(data), owner);
    
    ArrayCore core(shape, strides, manager, 0);
    
    return Array<T>(core);
}

template<typename T>
Array<T> external(T* data, const Shape& shape, const Strides& strides)
{
    Manager::Ptr manager = ExternalManager<NullOwner>::build(reinterpret_cast<unsigned char*>(data), NullOwner());
    
    ArrayCore core(shape, strides, manager, 0);
    
    return Array<T>(core);
}

template<typename T, typename Owner>
Array<T> external(T* data, const Shape& shape, const Owner& owner)
{
    Manager::Ptr manager = ExternalManager<Owner>::build(reinterpret_cast<unsigned char*>(data), owner);
    
    ArrayCore core(shape, contiguousStrides(shape, sizeof(T)), manager, 0);
    
    return Array<T>(core);
}

template<typename T>
Array<T> external(T* data, const Shape& shape)
{
    Manager::Ptr manager = ExternalManager<NullOwner>::build(reinterpret_cast<unsigned char*>(data), NullOwner());
    
    ArrayCore core(shape, contiguousStrides(shape, sizeof(T)), manager, 0);
    
    return Array<T>(core);
}

// Create a broadcasted array only if possible.
template<typename T>
Array<T> broadcast(const Array<T>& array, const Shape& newShape)
{
    if(array.shape() == newShape)
        return array;
    
    int newndims = newShape.size();
    Strides newStrides(newndims);
    Strides oldStrides(newndims);
    Shape oldShape(newndims, 1);
    
    if(array.ndims() > newndims)
    {
        throw std::invalid_argument("Array with shape <shape> cannot be broadcasted to shape <newShape>");
    }
    
    std::copy(array.shape().rbegin(), array.shape().rend(), oldShape.rbegin());
    std::copy(array.strides().rbegin(), array.strides().rend(), oldStrides.rbegin());
    
    auto it_newshape = newShape.begin();
    auto it_oldshape = oldShape.begin();
    auto it_newstrides = newStrides.begin();
    auto it_oldstrides = oldStrides.begin();
    for(; it_newshape != newShape.end();
            ++it_newshape, ++it_oldshape, ++it_newstrides, ++it_oldstrides)
    {
        if(*it_oldshape == *it_newshape)
            *it_newstrides = *it_oldstrides;
        else if(*it_oldshape==1)
            *it_newstrides = 0;
        else
            throw std::invalid_argument("Array with shape <shape> cannot be broadcasted to shape <newShape>");
    }
    
    return Array<T>(ArrayCore(newShape, newStrides, array.manager(), array.offset()));
}

template<typename T>
Array<T> reshape(const Array<T>& array, const Shape& newShape)
{
    if(array.numElements() != prod(newShape))
        throw std::invalid_argument("total size of the array must be unchanged");
    
    if(!array.isContiguous())
        return reshape(copy(array), newShape);
    
    Strides newStrides = contiguousStrides(newShape, sizeof(T));
    return Array<T>(ArrayCore(newShape, newStrides, array.manager(), array.offset()));
}

}

#endif
