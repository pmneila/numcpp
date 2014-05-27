
#ifndef NUMCPP_INITIALIZATION_H
#define NUMCPP_INITIALIZATION_H

#include <vector>

#include "manager.h"
#include "array.h"

namespace numcpp
{

inline std::vector<std::ptrdiff_t> computeStrides(std::vector<size_t> shape, size_t elemSize)
{
    std::vector<std::ptrdiff_t> strides(shape.size());
    
    
    
    return strides;
}

template<typename T>
Array<T> allocate(std::vector<size_t> shape)
{
    size_t numElements = prod(shape);
    size_t size = numElements * sizeof(T);
    Manager::Ptr manager = SimpleManager::allocate(size);
    auto strides = computeStrides(shape, sizeof(T));
    
    ArrayCore core(shape, strides, manager, 0);
    
    return Array<T>(core);
}

}

#endif
