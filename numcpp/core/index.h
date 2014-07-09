
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
    Index(int index)
        : _tag(INT), _index(index)
    {}
    
    Index(const Slice& slice)
        : _tag(SLICE), _slice(slice)
    {}
    
    Index(const NewAxis& newaxis)
        : _tag(NEWAXIS)
    {}
    
    bool isSingleton() const {return _tag==INT;}
    bool isNewAxis() const {return _tag==NEWAXIS;}
    int index() const {return _index;}
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
