
#ifndef NUMCPP_ARRAYREF_H
#define NUMCPP_ARRAYREF_H

#include "arraybase.h"

#include "array.h"

namespace numcpp
{

template<class T>
class ArrayRef : public ArrayBase<T, Array<T> >
{
public:
    typedef ArrayBase<T, Array<T> > Super;
    
    ArrayRef(const ArrayRef& rhs)
        : Super(rhs._core)
    {}
    
    ArrayRef(const Array<T>& core)
        : Super(core)
    {}
    
    ArrayRef(const ArrayCore& core)
        : Super(core)
    {}
    
    ArrayRef<T>& operator=(const Array<T>& rhs)
    {
        
        return *this;
    }
};

}

#endif
