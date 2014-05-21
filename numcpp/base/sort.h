#ifndef NUMCPP_SORT_H
#define NUMCPP_SORT_H

#include "../core/array.h"
#include <algorithm>

namespace numcpp
{

template<class T>
Array<T>& sort_(Array<T>& x);

DynTypeArray& sort_(DynTypeArray& x);

template<class T>
Array<size_t> argsort(const Array<T>& x);

Array<size_t> argsort(const DynTypeArray& x);

template<class T>
Array<T>& sort_(Array<T>& x)
{
  std::sort(x.data(), x.data()+x.size());
  return x;
}

template<class T>
Array<size_t> argsort(const Array<T>& x)
{
  Array<size_t> idx = range(0, x.size());

  std::sort(idx.data(), idx.data()+idx.size(),
    [&x](size_t i1, size_t i2) {return x[i1] < x[i2];});

  return idx;
}

}

#endif
