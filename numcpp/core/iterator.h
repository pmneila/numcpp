
#ifndef NUMCPP_ITERATOR_H
#define NUMCPP_ITERATOR_H

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
        , _contiguous(array.isContiguous())
    {
        if(_contiguous)
            _pointer_end = reinterpret_cast<unsigned char*>(array.data() + array.offset() + array.numElements());
    }
    
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
        , _contiguous(rhs._contiguous)
    {}
    
    Iterator& operator=(const Iterator& rhs)
    {
        _core = rhs._core;
        _seq_strides = rhs._seq_strides;
        _counter = rhs._counter;
        _pointer = rhs._pointer;
        _pointer_end = rhs._pointer_end;
        _contiguous = rhs._contiguous;
    }
    
    Iterator& operator++()
    {
        if(_contiguous)
        {
            _pointer += sizeof(T);
            return *this;
        }
        
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
    unsigned char* _pointer_end;
    unsigned char* _pointer;
    bool _contiguous;
};

template<typename T, typename Derived>
class SliceIterator
{
protected:
    SliceIterator(const ArrayBase<T, Derived>& array, int axis)
        : _array(array), _axis(axis), _index(0)
    {}
    
    template<typename T2, typename Derived2>
    friend class ArrayBase;
    
public:
    
    SliceIterator(const SliceIterator& rhs)
        : _array(rhs._array), _axis(rhs._axis), _index(rhs._index)
    {}
    
    SliceIterator& operator=(const SliceIterator& rhs)
    {
        _array = rhs._array;
        _axis = rhs._axis;
        _index = rhs._index;
    }
    
    SliceIterator& operator++()
    {
        ++_index;
        return *this;
    }
    
    SliceIterator& operator++(int)
    {
        SliceIterator aux(*this);
        ++(*this);
        return aux;
    }
    
    Array<T> operator*()
    {
        std::vector<Index> index(_axis+1, S{});
        index[_axis] = Index(_index);
        
        return _array[index];
    }
    
    bool operator==(const SliceIterator& rhs)
    {
        return _index == rhs._index && _axis == rhs._axis && &_array == &rhs._array;
    }
    
    bool operator!=(const SliceIterator& rhs)
    {
        return _index != rhs._index || _axis != rhs._axis || &_array != &rhs._array;
    }
    
private:
    const ArrayBase<T, Derived>& _array;
    int _axis;
    int _index;
};

}

#endif
