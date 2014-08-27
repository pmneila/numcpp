
#ifndef NUMCPP_ARRAYCORE_H
#define NUMCPP_ARRAYCORE_H

#include <vector>
#include <numeric>

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
    
    unsigned char* data() const {return (bool)_manager ? _manager->data() : 0;}
    
    std::ptrdiff_t offset() const {return _offset;}
    
    std::ptrdiff_t offset(const std::vector<size_t>& index) const
    {
        if(index.size() != _strides.size())
            throw std::invalid_argument("index has an invalid number of dimensions");
        
        #ifndef NDEBUG
        auto shape_it = shape().begin();
        auto shape_it_end = shape().end();
        auto index_it = index.begin();
        for(; shape_it != shape_it_end; ++shape_it, ++index_it)
            assert((*index_it>=0 && *index_it < *shape_it) && "index out of range");
        #endif
        
        return _offset + std::inner_product(index.begin(), index.end(), _strides.begin(), 0);
    }
    
    bool isNull() const
    {
        return !(bool)_manager;
    }
    
protected:
    Shape _shape;
    Strides _strides;
    std::ptrdiff_t _offset;
    Manager::Ptr _manager;
};

}

#endif
