
#ifndef NUMCPP_ARRAYCORE_H
#define NUMCPP_ARRAYCORE_H

#include <vector>
#include "memoryblock.h"
#include "utils.h"

namespace numcpp
{

class ArrayCore
{
public:
    
    ArrayCore(const std::vector<size_t>& shape,
         const std::vector<std::ptrdiff_t> strides,
         const MemoryBlock& mem,
         std::ptrdiff_t offset=0)
        : _shape(shape)
        , _strides(strides)
        , _offset(offset)
        , _mem(mem)
        , _data(mem.data())
    {
        if(_shape.size() != _strides.size())
            throw std::invalid_argument("strides and shape must have the same size");
    }
    
    ArrayCore(const std::vector<size_t>& shape,
        size_t elem_size,
        const MemoryBlock& mem)
        : _shape(shape)
        , _offset(0)
        , _mem(mem)
        , _data(mem.data())
    {
        //if _shape.size() != 
        // TODO.
    }
    
    ArrayCore(const ArrayCore& other);
    
    int size() const {return prod(_shape);}
    const std::vector<size_t>& shape() const {return _shape;}
    const std::vector<std::ptrdiff_t>& strides() const {return _strides;}
    int ndims() const {return _shape.size();}
    
    void* data() {return _data;}
    
protected:
    std::vector<size_t> _shape;
    std::vector<std::ptrdiff_t> _strides;
    std::ptrdiff_t _offset;
    MemoryBlock _mem;
    void* _data;
};

}

#endif
