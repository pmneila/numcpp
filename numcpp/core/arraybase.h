
#ifndef NUMCPP_ARRAYBASE_H
#define NUMCPP_ARRAYBASE_H

#include <algorithm>

#include "arraycore.h"
#include "abstractexpression.h"
#include "slice.h"

namespace numcpp
{

template <typename T, typename Derived>
class ArrayBase : public AbstractArrayExpression<T, Derived>
{
protected:
    unsigned char* _data;
    ArrayCore _core;
    
    ArrayBase(const ArrayBase& rhs)
        : _data(rhs._data)
        , _core(rhs._core)
    {}
    
    ArrayBase(const ArrayCore& core)
        : _data(reinterpret_cast<unsigned char*>(core.data()))
        , _core(core)
    {}
    
    const ArrayBase& operator=(const ArrayBase& rhs)
    {
        _data = rhs._data;
        _core = rhs._core;
    }
    
    T& operator[](const std::vector<size_t>& index)
    {
        std::ptrdiff_t offset = _core.offset(index);
        return *reinterpret_cast<T*>(data[offset]);
    }
    
    Array<T> operator[](std::vector<Slice>& slices)
    {
        std::ptrdiff_t newOffset;
        
        int ndims = this->ndims();
        
        Strides newStrides(_core.strides().size());
        Shape newShape(_core.shape().size());
        
        if(slices.size() > ndims)
            throw std::invalid_argument("more slices than array dimensions");
        
        if(slices.size() < ndims)
            slices.resize(ndims, full);
        
        std::vector<std::ptrdiff_t> starts(slices.size());
        //std::vector<std::ptrdiff_t> ends(slices.size());
        
        // std::transform(slices.begin(), slices.end(), starts.begin(),
        //    [&ndims](const Slice& v) {return v.start >= 0 ? v.start : ndims + v.start;});
        
        auto starts_it = starts.begin();
        auto newStrides_it = newStrides.begin();
        
        for(auto slices_it = slices.begin(); slices_it != slices.end();
            ++slices_it)
        {
            
        }
    }
    
public:
    int numElements() const {return _core.numElements();}
    const Shape& shape() const {return _core.shape();}
    const Strides& strides() const {return _core.strides();}
    int ndims() const {return _core.ndims();}
    T* data() const {return _data;}
};

}

#endif
