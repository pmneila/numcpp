
#ifndef NUMCPP_UTILS_H
#define NUMCPP_UTILS_H

#include <iostream>
#include <initializer_list>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cassert>
#include <numeric>

namespace numcpp
{

typedef std::vector<size_t> Shape;
typedef std::vector<std::ptrdiff_t> Strides;

template<typename T>
T clamp(T v, T lower, T upper)
{
    return std::max(lower, std::min(v, upper));
}

template<class Array>
typename Array::value_type prod(Array x)
{
  int prod = 1;
  
  for(size_t i=0; i<x.size(); i++)
    prod *= x[i];
  
  return prod;
}

std::vector<size_t> multiIndex(const size_t& index, const Shape& shape);

size_t flatIndex(const std::vector<size_t>& index, const Strides& strides, size_t offset);

Shape broadcastedShape(const Shape& shape1, const Shape& shape2)
{
    int ndim = std::max(shape1.size(), shape2.size());
    
    Shape res(ndim);
    Shape extshape1(ndim, 1);
    Shape extshape2(ndim, 1);
    
    std::copy(shape1.rbegin(), shape1.rend(), extshape1.rbegin());
    std::copy(shape2.rbegin(), shape2.rend(), extshape2.rbegin());
    
    auto it1 = extshape1.begin();
    auto it2 = extshape2.begin();
    auto itres = res.begin();
    for(; it1 != extshape1.end(); ++it1, ++it2, ++itres)
    {
        if(*it1 == *it2)
            *itres = *it1;
        else if(*it1==1)
            *itres = *it2;
        else if(*it2==1)
            *itres = *it1;
        else
            throw std::invalid_argument("Shapes <shape1> and <shape2> are not broadcastable");
    }
    
    return res;
}

Shape broadcastShapes(const std::vector<Shape>& shapes)
{
    return std::accumulate(shapes.begin(), shapes.end(), Shape(),
        broadcastedShape);
}

// Sequential strides.
Strides seqStrides(const Shape& shape, const Strides& strides)
{
    Strides prod(strides.size(), 0);
    Strides res(strides);
    // std::transform(shape.begin(), shape.end(), strides.begin(), prod.begin(),
    //     [](const Shape::value_type& v1, const Strides::value_type& v2) -> Strides::value_type {return v1*v2;});
    std::transform(shape.begin(), shape.end(), strides.begin(), prod.begin(),
        std::multiplies<Strides::value_type>());
    
    std::transform(res.begin(), res.end()-1, prod.begin()+1, res.begin(),
        std::minus<Strides::value_type>());
    
    return res;
}

// Ceil of the integer division a/b
template<typename T>
T ceil_div(const T& a, const T& b)
{
    return (a - 1)/b + 1;
}

/// Clock type that can hold the current time point.
typedef std::chrono::high_resolution_clock clock;

/*!
Start a timer and return the current clock time.
Example:
@code
auto t = tic();
// do some calculations
toc(t);
@endcode
\sa toc()
*/
clock::time_point tic();

/*!
Returns the time that is ellapsed since the time point \a t and now.
If the parameter \a print is true, the function prints the ellapsed time
in a pretty format to the console.
@code
auto t = tic();
// do some calculations
toc(t);
@endcode
\sa tic()
*/
size_t toc(clock::time_point t, bool print=true);

}

#endif
