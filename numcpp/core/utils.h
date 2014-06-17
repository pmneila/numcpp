#ifndef NUMCPP_UTILS_H
#define NUMCPP_UTILS_H

#include <iostream>
#include <initializer_list>
#include <vector>
#include <chrono>

namespace numcpp
{

/*!
@file

@addtogroup core
@{
*/

typedef std::vector<size_t> Shape;
typedef std::vector<std::ptrdiff_t> Strides;

template<class Array>
typename Array::value_type prod(Array x)
{
  int prod = 1;
  
  for(size_t i=0; i<x.size(); i++)
    prod *= x[i];
  
  return prod;
}

/*template<class Array>
Array removeOnes(Array x)
{
  x.erase(std::remove(x.begin(), x.end(), 1), x.end());
  return x;
}*/

std::vector<size_t> multiIndex(const size_t& index, const Shape& shape);

size_t flatIndex(const std::vector<size_t>& index, const Strides& strides, size_t offset);

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

template<class T>
void print(const T& x)
{
  std::cout << x << std::endl;
}

// template<class T>
// std::ostream& operator<< (std::ostream& os, const std::vector<T>& x)
// {
//   for(auto& y: x)
//     os << y << " ";
//   return os;
// }

// Ceil of the integer division a/b
template<typename T>
T ceil_div(const T& a, const T& b)
{
    return (a - 1)/b + 1;
}

void copyShapeToSubArray(const std::vector<size_t>& shapeIn, std::vector<size_t>& shapeOut, int axis);

void copyShapeFromSubArray(const std::vector<size_t>& shapeIn, std::vector<size_t>& shapeOut, int axis);


template<class Array>
void println (const Array& x)
{
  for(int i=0; i<x.size(); i++)
    std::cout << x[i] << " ";
  std::cout << std::endl;
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



/*! @} */


}

#endif
