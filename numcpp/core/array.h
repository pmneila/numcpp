
#ifndef NUMCPP_ARRAY_H
#define NUMCPP_ARRAY_H

#include "arraybase.h"

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
    
    Array& operator=(const Array& rhs)
    {
        this->_data = rhs._data;
        this->_core = rhs._core;
        
        return *this;
    }
};

}

#endif
