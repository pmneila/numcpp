
#ifndef NUMCPP_ARRAYCORE_H
#define NUMCPP_ARRAYCORE_H

#include <vector>

#include "utils.h"
#include "manager.h"

namespace numcpp
{

class ArrayCore
{
public:
    
    ArrayCore(const std::vector<size_t>& shape,
        const std::vector<std::ptrdiff_t> strides,
        const Manager::Ptr manager,
        void* data,
        std::ptrdiff_t offset=0)
        : _shape(shape)
        , _strides(strides)
        , _offset(offset)
        , _manager(manager)
        , _data(data)
    {
        if(_shape.size() != _strides.size())
            throw std::invalid_argument("strides and shape must have the same size");
    }
    
    // ArrayCore(const std::vector<size_t>& shape,
    //     size_t elem_size,
    //     Manager::Ptr manager)
    //     : _shape(shape)
    //     , _offset(0)
    //     , _manager(manager)
    //     , _data(mem.data())
    // {
    //     //if _shape.size() != 
    //     // TODO.
    // }
    
    ArrayCore(const ArrayCore& other)
        : _shape(other._shape)
        , _strides(other._strides)
        , _offset(other._offset)
        , _manager(other._manager)
        , _data(other._data)
    {}
    
    ArrayCore& operator=(const ArrayCore& other)
    {
        _shape = other._shape;
        _strides = other._strides;
        _offset = other._offset;
        _manager = other._manager;
        _data = other._data;
        return *this;
    }
    
    int numElements() const {return prod(_shape);}
    const std::vector<size_t>& shape() const {return _shape;}
    const std::vector<std::ptrdiff_t>& strides() const {return _strides;}
    int ndims() const {return _shape.size();}
    
    Manager::Ptr getManager() const {return _manager;}
    
    void* data() {return _data;}
    
protected:
    std::vector<size_t> _shape;
    std::vector<std::ptrdiff_t> _strides;
    std::ptrdiff_t _offset;
    Manager::Ptr _manager;
    void* _data;
};

}

#endif
