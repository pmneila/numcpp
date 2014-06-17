
#ifndef NUMCPP_INDEX_H
#define NUMCPP_INDEX_H

#include "slice.h"

namespace numcpp
{

struct Index
{
public:
    Index(int index)
        : _tag(INT), _index(index)
    {}
    
    Index(const Slice& slice)
        : _tag(SLICE), _slice(slice)
    {}
    
    bool isSingleton() const {return _tag==INT;}
    int index() const {return _index;}
    const Slice& slice() const {return _slice;}
    
protected:
    enum {INT, SLICE} _tag;
    //union
    //{
        int _index;
        Slice _slice;
    //};
};

}

#endif
