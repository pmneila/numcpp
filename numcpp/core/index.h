
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
    
    int start() const {return _tag == INT ? _index : _slice.start;}
    int end() const {return _tag == INT ? _index+1 : _slice.end;}
    int step() const {return _tag == INT ? 1 : _slice.step;}
    
    int offsetBegin(int shape) const
    {
        int raw_start = start();
        if(raw_start < 0)
            raw_start = shape + raw_start;
        
        if(_tag == INT)
        {
            if(raw_start < 0 || raw_start >= shape)
                throw std::invalid_argument("index out of bounds");
            
            return raw_start;
        }
        
        if(_tag == SLICE)
        {
            int step = this->step();
            
            if(raw_start < 0)
                return step > 0 ? 0 : -1;
            
            if(raw_start >= shape)
                return step > 0 ? shape : shape - 1;
            
            return raw_start;
        }
        
        throw std::runtime_error("invalid index tag");
    }
    
    int offsetEnd(int shape) const
    {
        int raw_end = end();
        if(raw_end < 0)
            raw_end = shape + raw_end;
        
        if(_tag == INT)
        {
            if(raw_end < 1 || raw_end > shape)
                throw std::invalid_argument("index out of bounds");
            
            return raw_end;
        }
        
        if(_tag == SLICE)
        {
            int step = this->step();
            
            if(raw_end < 0)
                return step > 0 ? 0 : -1;
            
            if(raw_end >= shape)
                return step > 0 ? shape : shape - 1;
            
            return raw_end;
        }
        
        throw std::runtime_error("invalid index tag");
    }
    
    int newShape(int oldShape)
    {
        int s = offsetBegin(oldShape);
        int t = offsetEnd(oldShape);
        int step = this->step();
        
        if(step > 0)
            return (step + std::max(t - s, 0) - 1) / step;
        if(step < 0)
            return (-step + std::max(s - t, 0) - 1) / -step;
    }
    
    // int end(int shape) const {return _tag == INT ? _index+1 : _slice.end;}
    
protected:
    enum {INT, SLICE} _tag;
    union
    {
        int _index;
        Slice _slice;
    };
};

}

#endif
