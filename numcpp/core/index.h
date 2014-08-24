
#ifndef NUMCPP_INDEX_H
#define NUMCPP_INDEX_H

#include "slice.h"

namespace numcpp
{

struct NewAxis{};

NewAxis newaxis;

struct Index
{
public:
    template<typename T>
    Index(std::initializer_list<T> list)
        : _tag(SLICE), _slice(list)
    {}
    
    Index(int index)
        : _tag(INT), _index(index)
    {}
    
    Index(const Slice& slice)
        : _tag(SLICE), _slice(slice)
    {}
    
    Index(const NewAxis& newaxis)
        : _tag(NEWAXIS)
    {}
    
    int tag() const {return _tag;}
    bool isSingleton() const {return _tag==INT;}
    bool isSlice() const {return _tag==SLICE;}
    bool isNewAxis() const {return _tag==NEWAXIS;}
    int index() const {return _index;}
    int index(int shape) const {return _index >= 0 ? _index : shape + _index;}
    const Slice& slice() const {return _slice;}
    
protected:
    enum {INT, SLICE, NEWAXIS} _tag;
    //union
    //{
        int _index;
        Slice _slice;
    //};
};

}

#endif
