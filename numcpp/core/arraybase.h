
#ifndef NUMCPP_ARRAYBASE_H
#define NUMCPP_ARRAYBASE_H

#include "arraycore.h"
#include "abstractexpression.h"

namespace numcpp
{

template <typename T, typename Derived>
class ArrayBase : public AbstractArrayExpression<T, Derived>
{
protected:
    T* _data;
    ArrayCore _core;
    
    ArrayBase(const ArrayBase& rhs)
        : _data(rhs._data)
        , _core(rhs._core)
    {}
    
    ArrayBase(const ArrayCore& core)
        : _data(reinterpret_cast<T*>(core.data()))
        , _core(core)
    {}
    
    const ArrayBase& operator=(const ArrayBase& rhs)
    {
        _data = rhs._data;
        _core = rhs._core;
    }
    
public:
    
    const std::vector<size_t>& shape() const {return _core.shape();}
};

}

#endif
