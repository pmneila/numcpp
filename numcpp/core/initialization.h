
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
    *strides_it = elemSize;
    ++strides_it;
    
    int prod = elemSize;
    
    for(auto shape_it = shape.rbegin(); strides_it != strides.rend(); ++shape_it, ++strides_it)
    {
        prod *= *shape_it;
        *strides_it = prod;
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

// Create a broadcasted array only if possible.
template<typename T>
Array<T> broadcast(const Array<T>& array, const Shape& newShape)
{
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

}

#endif
