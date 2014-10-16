
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
        : _shape(array.shape())
        , _pointer(reinterpret_cast<unsigned char*>(array.data()) + array.offset())
        // , _outerAxes(_core.ndims())
        , _seq_strides(seqStrides(_shape, array.strides()))
        , _counter(array.ndims(), 0)
    {
        std::tie(_outerAxes, _innerStep, _innerSize) = array.getInnerLoopAxisAndStep();
        _innerEnd = _pointer + _innerSize;
        _pointerEnd = nullptr;
        
        if(array.numElements() == 0)
            _pointer = _pointerEnd;
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
        : _shape(rhs._shape)
        , _pointer(rhs._pointer)
        , _pointerEnd(rhs._pointerEnd)
        , _outerAxes(rhs._outerAxes)
        , _seq_strides(rhs._seq_strides)
        , _counter(rhs._counter)
        , _innerStep(rhs._innerStep)
        , _innerSize(rhs._innerSize)
        , _innerEnd(rhs._innerEnd)
    {}
    
    Iterator& operator=(const Iterator& rhs)
    {
        _shape = rhs._shape;
        _pointer = rhs._pointer;
        _pointerEnd = rhs._pointerEnd;
        _outerAxes = rhs._outerAxes;
        _seq_strides = rhs._seq_strides;
        _counter = rhs._counter;
        _innerStep = rhs._innerStep;
        _innerSize = rhs._innerSize;
        _innerEnd = rhs._innerEnd;
        
        return *this;
    }
    
    Iterator& operator++()
    {
        _pointer += _innerStep;
        if(_pointer == _innerEnd)
            outer_step();
        
        return *this;
    }
    
    void outer_step()
    {
        int i;
        for(i=_outerAxes; i>=0; --i)
        {
            _pointer += _seq_strides[i];
            if(_counter[i] < _shape[i] - 1)
            {
                ++_counter[i];
                break;
            }
            else
                _counter[i] = 0;
        }
        
        if(i==-1)
            _pointer = _pointerEnd;
        
        _innerEnd = _pointer + _innerSize;
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
        _pointer = _pointerEnd;
    }
    
    /// Undefined when the iterator is at end
    const Shape& counter() const {return _counter;}
    
    // std::ptrdiff_t offset() const {return _pointer - _core->data();}
    
private:
    // const ArrayCore* _core;
    Shape _shape;
    unsigned char* _pointer;
    unsigned char* _pointerEnd;
    
    // Outer loop variables
    int _outerAxes;
    Strides _seq_strides;
    Shape _counter;
    
    // Inner loop variables
    int _innerStep;
    int _innerSize;
    unsigned char* _innerEnd;
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
