#ifndef NUMCPP_SLICE_H
#define NUMCPP_SLICE_H

// #include <cstdlib>
#include <initializer_list>
#include <vector>

namespace numcpp
{

namespace detail
{

template<typename T, typename Result>
Result getWithDefault(const std::initializer_list<T> list,
    int index, const Result& _default)
{
    if(list.size() > index)
    {
        auto it = list.begin();
        for(int i = 0; i < index; ++i, ++it);
        return *it;
    }
    
    return _default;
}

}

class SliceElem
{
public:
    SliceElem()
        : _unit(true)
        , _index(0)
    {}
    
    SliceElem(int index)
        : _unit(false)
        , _index(index)
    {}
    
    bool isNone() const {return _unit;}
    bool isUnit() const {return _unit;}
    int value() const {return _index;}
    
private:
    bool _unit;
    int _index;
};

class Slice
{
public:
    
    Slice()
        : _start()
        , _end()
        , _step(1)
    {}
    
    Slice(const SliceElem& start, const SliceElem& end, int step = 1)
        : _start(start)
        , _end(end)
        , _step(step)
    {}
    
    explicit Slice(const SliceElem& start)
        : _start(start)
        , _end(start.value() + 1)
        , _step(1)
    {}
    
    Slice(const Slice& slice)
        : _start(slice._start)
        , _end(slice._end)
        , _step(slice._step)
    {}
    
    template<typename T>
    Slice(std::initializer_list<T> list)
        : _start(detail::getWithDefault(list, 0, SliceElem()))
        , _end(detail::getWithDefault(list, 1, SliceElem()))
        , _step(detail::getWithDefault(list, 2, 1))
    {
    }
    
    const SliceElem& start() const {return _start;}
    const SliceElem& end() const {return _end;}
    int step() const {return _step;}
    
    std::ptrdiff_t start(int shape) const
    {
        if(_start.isUnit())
        {
            if(_step >= 0)
                return 0;
            
            return shape - 1;
        }
        
        int v = _start.value();
        v = v >= 0 ? v : shape + v;
        return clamp(v, 0, shape);
    }
    
    std::ptrdiff_t end(int shape) const
    {
        if(_end.isUnit())
        {
            if(_step >= 0)
                return shape;
            
            return -1;
        }
        
        int v = _end.value();
        v = v >= 0 ? v : shape + v;
        return clamp(v, 0, shape);
    }
    
private:
    SliceElem _start;
    SliceElem _end;
    int _step;
};

const Slice full;

using slice = Slice;
using S = Slice;

// std::vector<Slice> convertToSliceArray(std::initializer_list<Slice> slices);

}

#endif // RANGE_H
