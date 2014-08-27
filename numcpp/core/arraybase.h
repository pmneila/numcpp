
#ifndef NUMCPP_ARRAYBASE_H
#define NUMCPP_ARRAYBASE_H

#include "arraycore.h"
#include "index.h"

namespace numcpp
{

template<class T> class Array;
template<class T> class ArrayRef;
template<typename T> class Iterator;
template<typename T, typename Derived> class SliceIterator;

template <typename DT, typename Derived>
class ArrayBase
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
    
public:
    typedef DT value_type;
    typedef DT& reference;
    typedef const DT& const_reference;
    typedef DT* pointer;
    typedef const DT* const_pointer;
    typedef Iterator<DT> iterator;
    typedef Iterator<const DT> const_iterator;
    typedef SliceIterator<DT, Derived> slice_iterator;
    typedef DT* contiguous_iterator;
    
    DT& operator()() const
    {
        std::ptrdiff_t offset = _core.offset();
        return *reinterpret_cast<DT*>(_data + offset);
    }
    
    DT& operator()(const std::vector<size_t>& index) const
    {
        std::ptrdiff_t offset = _core.offset(index);
        return *reinterpret_cast<DT*>(_data + offset);
    }
    
    ArrayRef<DT> operator[](int index) const
    {
        return operator[](std::vector<Index>{index});
    }
    
    ArrayRef<DT> operator[](const std::vector<Index>& index) const
    {
        std::ptrdiff_t newOffset = _core.offset();
        
        Strides newStrides;
        Shape newShape;
        
        auto shape_it = shape().begin();
        auto strides_it = strides().begin();
        for(auto index_it = index.begin(); index_it != index.end();
            ++index_it)
        {
            int start;
            
            if(index_it->isNewAxis())
            {
                newShape.push_back(1);
                newStrides.push_back(0);
                continue;
            }
            
            if(shape_it == shape().end())
                throw std::invalid_argument("the index has more elements than array dimensions");
            
            if(index_it->isSingleton())
            {
                int index = index_it->index(*shape_it);
                start = index;
                
                assert((start>=0 && start < *shape_it) && "index out of range");
            }
            else
            {
                const Slice& slice = index_it->slice();
                start = slice.start(*shape_it);
                int end = slice.end(*shape_it);
                int step = slice.step();
                
                if(step > 0)
                    newShape.push_back(std::max(0, ceil_div(end - start, step)));
                else
                    newShape.push_back(std::max(0, ceil_div(start - end, -step)));
                
                newStrides.push_back(step * *strides_it);
            }
            
            newOffset += start * *strides_it;
            
            ++shape_it;
            ++strides_it;
        }
        
        for(; shape_it != shape().end(); ++shape_it, ++strides_it)
        {
            newShape.push_back(*shape_it);
            newStrides.push_back(*strides_it);
        }
        
        return ArrayRef<DT>(ArrayCore(newShape, newStrides, manager(), newOffset));
    }
    
    Array<DT> T()
    {
        Shape newShape(shape());
        Strides newStrides(strides());
        std::reverse(newShape.begin(), newShape.end());
        std::reverse(newStrides.begin(), newStrides.end());
        
        return Array<DT>(ArrayCore(newShape, newStrides, manager(), offset()));
    }
    
    iterator begin() const;
    iterator end() const;
    
    slice_iterator slice_begin(int axis) const;
    slice_iterator slice_end(int axis) const;
    
    contiguous_iterator cont_begin() const
    {
        if(!isContiguous())
            throw std::invalid_argument("cannot create a contiguous_iterator; array is not contiguous");
        
        return data() + offset();
    }
    
    contiguous_iterator cont_end() const
    {
        if(!isContiguous())
            throw std::invalid_argument("cannot create a contiguous_iterator; array is not contiguous");
        
        return data() + offset() + numElements();
    }
    
    int numElements() const {return _core.numElements();}
    const Shape& shape() const {return _core.shape();}
    const Strides& strides() const {return _core.strides();}
    int ndims() const {return _core.ndims();}
    Manager::Ptr manager() const {return _core.manager();}
    bool isNull() const {return _core.isNull();}
    
    DT* data() const {return reinterpret_cast<DT*>(_data);}
    std::ptrdiff_t offset() const {return _core.offset();}
    
    ArrayCore& core() {return _core;}
    const ArrayCore& core() const {return _core;}
    
    bool isContiguous() const
    {
        // return contiguousStrides(shape(), sizeof(DT)) == strides();
        size_t value = sizeof(DT);
        
        auto strides_it = strides().rbegin();
        auto strides_it_end = strides().rend();
        auto shape_it = shape().rbegin();
        for(; strides_it != strides_it_end; ++strides_it, ++shape_it)
        {
            if(*strides_it != value)
                return false;
            
            value *= *shape_it;
        }
        
        return true;
    }
    
    std::tuple<int,int,int> getInnerLoopAxisAndStep() const
    {
        int axis = ndims();
        
        if(axis == 0)
            return std::make_tuple(0, 0, 0);
        
        auto strides_it = strides().rbegin();
        size_t value = *strides_it;
        const auto strides_it_end = strides().rend();
        auto shape_it = shape().rbegin();
        
        const int step = value;
        if(step == 0)
            return std::make_tuple(axis, 0, 0);
        
        for(; strides_it != strides_it_end; ++strides_it, ++shape_it, --axis)
        {
            if(*strides_it != value)
                return std::make_tuple(axis, step, value);
            
            value *= *shape_it;
        }
        
        return std::make_tuple(0, step, value);
    }
};

}

#include "iterator.h"

namespace numcpp
{

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::iterator ArrayBase<T, Derived>::begin() const
{
    return iterator(*this);
}

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::iterator ArrayBase<T, Derived>::end() const
{
    iterator it(*this);
    it.goToEnd();
    return it;
}

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::slice_iterator ArrayBase<T, Derived>::slice_begin(int axis) const
{
    if(axis < 0)
        axis = ndims() + axis;
    
    assert(axis >= 0 && axis < ndims());
    return slice_iterator(*this, axis);
}

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::slice_iterator ArrayBase<T, Derived>::slice_end(int axis) const
{
    if(axis < 0)
        axis = ndims() + axis;
    
    assert(axis >= 0 && axis < ndims());
    slice_iterator it(*this, axis);
    it._index = shape()[axis];
    return it;
}

}

#endif
