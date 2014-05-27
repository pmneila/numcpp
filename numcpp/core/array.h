#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include "arraybase.h"
#include "slice.h"

#include <array>

namespace numcpp
{

template<class T>
class Array : public ArrayBase<T, Array<T> >
{
public:
    typedef ArrayBase<T, Array<T> > Super;
    
    Array()
        : Super(ArrayCore())
    {}
    
    Array(const Array& rhs)
        : Super(rhs._core)
    {}
    
    Array(const ArrayCore& core)
        : Super(core)
    {}
};

}

#endif