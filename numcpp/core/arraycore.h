
#ifndef NUMCPP_ARRAYCORE_H
#define NUMCPP_ARRAYCORE_H

#include <vector>
#include <numeric>

#include "utils.h"
#include "manager.h"

namespace numcpp
{

class ArrayCore
{
public:
    
    ArrayCore()
    {}
    
    ArrayCore(const Shape& shape,
        const Strides& strides,
        const Manager::Ptr manager,
        std::ptrdiff_t offset=0)
        : _shape(shape)
        , _strides(strides)
        , _offset(offset)
        , _manager(manager)
    {
        if(_shape.size() != _strides.size())
            throw std::invalid_argument("strides and shape must have the same size");
    }
    
    ArrayCore(const ArrayCore& other)
        : _shape(other._shape)
        , _strides(other._strides)
        , _offset(other._offset)
        , _manager(other._manager)
    {}
    
    ArrayCore& operator=(const ArrayCore& other)
    {
        _shape = other._shape;
        _strides = other._strides;
        _offset = other._offset;
        _manager = other._manager;
        return *this;
    }
    
    int numElements() const {return prod(_shape);}
    const Shape& shape() const {return _shape;}
    const Strides& strides() const {return _strides;}
    int ndims() const {return _shape.size();}
    
    Manager::Ptr manager() const {return _manager;}
    
    unsigned char* data() const {return _manager->data();}
    
    std::ptrdiff_t offset() const {return _offset;}
    
    std::ptrdiff_t offset(const std::vector<size_t>& index) const
    {
        if(index.size() != _strides.size())
            throw std::invalid_argument("index has an invalid number of dimensions");
        
        return _offset + std::inner_product(index.begin(), index.end(), _strides.begin(), 0);
    }
    
    bool isEmpty() const
    {
        return (bool)_manager;
    }
    
protected:
    Shape _shape;
    Strides _strides;
    std::ptrdiff_t _offset;
    Manager::Ptr _manager;
};

}

#endif
