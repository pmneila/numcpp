#include "slice.h"

namespace numcpp
{

Slice::Slice()
    : start(0)
    , end(-1)
    , step(1)
{
}


Slice::Slice(ptrdiff_t start, ptrdiff_t end, ptrdiff_t step)
    : start(start)
    , end(end)
    , step(step)
{
}

Slice::Slice(int index)
    : start(index)
    , end(index+1)
    , step(0)
{
    
}

Slice::Slice(ptrdiff_t index)
    : start(index)
    , end(index+1)
    , step(0)
{

}

Slice::Slice(size_t index)
    : start(index)
    , end(index+1)
    , step(0)
{

}


Slice::Slice(const Slice& slice)
    : start(slice.start)
    , end(slice.end)
    , step(slice.step)
{

}

template<class T>
Slice::Slice(std::initializer_list<T> list)
{
    auto it = list.begin();

    if(list.size() == 1)
    {
        start = (ptrdiff_t)*it;
        end = start+1;
        step = 1;
    } else if(list.size() == 2)
    {
        start = (ptrdiff_t)*it++;
        end = (ptrdiff_t)*it;
        step = 1;
    } else
    {
        start = (ptrdiff_t)*it++;
        end = (ptrdiff_t)*it++;
        step = (ptrdiff_t)*it;
    }
}
template Slice::Slice(std::initializer_list<int> list);
template Slice::Slice(std::initializer_list<long> list);
template Slice::Slice(std::initializer_list<unsigned int> list);
template Slice::Slice(std::initializer_list<unsigned long> list);

std::vector<Slice> convertToSliceArray(std::initializer_list<Slice> slices)
{
  return std::vector<Slice>(slices);
}

}
