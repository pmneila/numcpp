#include "slice.h"

namespace numcpp
{

Slice::Slice(SliceElem index)
    : start(index)
    , end(index+1)
    , step(1)
{}

// Slice::Slice(ptrdiff_t index)
//     : start(index)
//     , end(index+1)
//     , step(0)
// {

// }

// Slice::Slice(size_t index)
//     : start(index)
//     , end(index+1)
//     , step(0)
// {

// }


Slice::Slice(const Slice& slice)
    : start(slice.start)
    , end(slice.end)
    , step(slice.step)
{
}

template<class T>
static SliceElem startFromList(const std::initializer_list<T>& list)
{
    if(list.size() >= 1)
        return SliceElem(*list.begin());
    
    return SliceElem();
}

template<class T>
static SliceElem endFromList(const std::initializer_list<T>& list)
{
    if(list.size() >= 2)
    {
        auto it = list.begin();
        return SliceElem(++it);
    }
    
    return SliceElem();
}

template<class T>
static T& stepFromList(const std::initializer_list<T>& list)
{
    if(list.size() >= 3)
    {
        auto it = list.begin();
        ++it;
        return SliceElem(++it);
    }
    
    return 1;
}

template<class T>
Slice::Slice(std::initializer_list<T> list)
    : start(startFromList(list))
    , end(endFromList(list))
    , step(stepFromList(list))
{
}

template Slice::Slice(std::initializer_list<int> list);
template Slice::Slice(std::initializer_list<long> list);
template Slice::Slice(std::initializer_list<unsigned int> list);
template Slice::Slice(std::initializer_list<unsigned long> list);

// std::vector<Slice> convertToSliceArray(std::initializer_list<Slice> slices)
// {
//   return std::vector<Slice>(slices);
// }

}
