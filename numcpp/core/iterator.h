
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
        , _pointer(reinterpret_cast<unsigned char*>(_core.data() + _core.offset()))
    {}
    
    template<typename S, typename Derived>
    friend class ArrayBase;
    
public:
    Iterator(const Iterator& rhs)
        : _core(rhs._core)
        , _counter(rhs._counter)
        , _pointer(rhs._pointer)
    {}
    
    Iterator& operator=(const Iterator& rhs)
    {
        _core = rhs._core;
        _counter = rhs._counter;
        _pointer = rhs._pointer;
    }
    
    Iterator& operator++()
    {
        for(int i=_core.ndims()-1; i>=0; --i)
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
        
        return *this;
    }
    
    Iterator operator++(int)
    {
        Iterator aux(*this);
        
        ++(*this);
        
        return aux;
    }
    
    T& operator*() {return *reinterpret_cast<T*>(_pointer);}
    
    bool operator==(const Iterator& rhs)
    {
        return _pointer == rhs._pointer;
    }
    
    bool operator!=(const Iterator& rhs)
    {
        return _pointer != rhs._pointer;
    }
    
    void goToEnd()
    {
        _pointer = _core.data() + _core.offset(_core.shape());
    }
    
    const Shape& counter() const {return _counter;}
    
    std::ptrdiff_t offset() const {return _pointer - _core.data();}
    
private:
    const ArrayCore& _core;
    Strides _seq_strides;
    Shape _counter;
    unsigned char* _pointer;
};

}

#endif
