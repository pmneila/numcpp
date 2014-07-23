
#ifndef NUMCPP_ITERATOR_H
#define NUMCPP_ITERATOR_H

//#include "arraybase.h"

namespace numcpp
{

template<typename T>
class Iterator
{
protected:
    template<typename Derived>
    Iterator(const ArrayBase<T, Derived>& array)
        : _core(array.core())
        , _seq_strides(seqStrides(_core.shape(), _core.strides()))
        , _counter(_core.ndims(), 0)
        , _pointer_end(nullptr)
        , _pointer(reinterpret_cast<unsigned char*>(_core.data() + _core.offset()))
    {}
    
    template<typename S, typename Derived>
    friend class ArrayBase;
    
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    
    Iterator(const Iterator& rhs)
        : _core(rhs._core)
        , _seq_strides(rhs._seq_strides)
        , _counter(rhs._counter)
        , _pointer_end(rhs._pointer_end)
        , _pointer(rhs._pointer)
    {}
    
    Iterator& operator=(const Iterator& rhs)
    {
        _core = rhs._core;
        _seq_strides = rhs._seq_strides;
        _counter = rhs._counter;
        _pointer = rhs._pointer;
        _pointer_end = rhs._pointer_end;
    }
    
    Iterator& operator++()
    {
        int i;
        for(i=_core.ndims()-1; i>=0; --i)
        {
            _pointer += _seq_strides[i];
            if(_counter[i] < _core.shape()[i] - 1)
            {
                ++_counter[i];
                break;
            }
            else
                _counter[i] = 0;
        }
        
        if(i==-1)
            _pointer = _pointer_end;
        
        return *this;
    }
    
    Iterator operator++(int)
    {
        Iterator aux(*this);
        
        ++(*this);
        
        return aux;
    }
    
    T& operator*() {return *reinterpret_cast<T*>(_pointer);}
    
    bool operator==(const Iterator& rhs) const
    {
        return _pointer == rhs._pointer;
    }
    
    bool operator!=(const Iterator& rhs) const
    {
        return _pointer != rhs._pointer;
    }
    
    void goToEnd()
    {
        _pointer = _pointer_end;
    }
    
    /// Undefined when the iterator is at end
    const Shape& counter() const {return _counter;}
    
    std::ptrdiff_t offset() const {return _pointer - _core.data();}
    
private:
    const ArrayCore& _core;
    Strides _seq_strides;
    Shape _counter;
    unsigned char* const _pointer_end;
    unsigned char* _pointer;
};

}

#endif
