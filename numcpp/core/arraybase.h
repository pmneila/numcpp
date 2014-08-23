
#ifndef NUMCPP_ARRAYBASE_H
#define NUMCPP_ARRAYBASE_H

#include "arraycore.h"
#include "index.h"

namespace numcpp
{

template<class T> class Array;
template<class T> class ArrayRef;
template<typename T> class Iterator;

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
    
    DT& operator[](const std::vector<size_t>& index)
    {
        std::ptrdiff_t offset = _core.offset(index);
        return *reinterpret_cast<DT*>(_data + offset);
    }
    
    ArrayRef<DT> operator[](std::vector<Index> index)
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
                int index = index_it->index();
                start = index;
            }
            else
            {
                const Slice& slice = index_it->slice();
                start = slice.start(*shape_it);
                int end = slice.end(*shape_it);
                int step = slice.step();
                
                if(step > 0)
                    newShape.push_back(ceil_div(end - start, step));
                else
                    newShape.push_back(ceil_div(start - end, -step));
                
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
    
    iterator begin();
    iterator end();
    
    int numElements() const {return _core.numElements();}
    const Shape& shape() const {return _core.shape();}
    const Strides& strides() const {return _core.strides();}
    int ndims() const {return _core.ndims();}
    Manager::Ptr manager() const {return _core.manager();}
    bool isEmpty() const {return _core.isEmpty();}
    
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
};

}

#include "iterator.h"

namespace numcpp
{

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::iterator ArrayBase<T, Derived>::begin()
{
    return iterator(*this);
}

template <typename T, typename Derived>
typename ArrayBase<T, Derived>::iterator ArrayBase<T, Derived>::end()
{
    iterator it(*this);
    it.goToEnd();
    return it;
}

}

#endif
