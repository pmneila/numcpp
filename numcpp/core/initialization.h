
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
    std::cout << size << std::endl;
    Manager::Ptr manager = SimpleManager::allocate(size);
    auto strides = contiguousStrides(shape, sizeof(T));
    
    ArrayCore core(shape, strides, manager, 0);
    
    return Array<T>(core);
}

}

#endif
