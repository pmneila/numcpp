
#ifndef NUMCPP_ARRAYREF_H
#define NUMCPP_ARRAYREF_H

#include "arraybase.h"
#include "array.h"

namespace numcpp
{

template<class T>
class ArrayRef : public ArrayBase<T, ArrayRef<T> >
{
public:
    typedef ArrayBase<T, ArrayRef<T> > Super;
    
    ArrayRef(const ArrayRef& rhs)
        : Super(rhs._core)
    {}
    
    ArrayRef(const Array<T>& array)
        : Super(array.core())
    {}
    
    ArrayRef(const ArrayCore& core)
        : Super(core)
    {}
    
    ArrayRef<T>& operator=(const Array<T>& rhs)
    {
        Array<T> arr = broadcast(rhs, this->shape());
        
        for(auto elems : zip(*this, arr))
        {
            std::get<0>(elems) = std::get<1>(elems);
        }
        
        return *this;
    }
    
    ArrayRef<T>& operator=(const T& rhs)
    {
        for(T& x : *this)
            x = rhs;
        
        // std::cout << "ASDF" << std::endl;
        return *this;
    }
};

}

#endif
