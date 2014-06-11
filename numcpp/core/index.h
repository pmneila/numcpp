
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
    
    // int start() const {return _tag == INT ? _index : _slice.start;}
    // int end() const {return _tag == INT ? _index+1 : _slice.end;}
    // int step() const {return _tag == INT ? 1 : _slice.step;}
    
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
