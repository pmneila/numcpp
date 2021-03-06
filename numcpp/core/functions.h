
#ifndef NUMCPP_FUNCTION_H
#define NUMCPP_FUNCTION_H

#include <functional>
#include <cmath>

namespace numcpp
{

namespace detail
{
    template<typename Function, typename Tuple, int... Is>
    auto call_func(Function&& f, const Tuple& tuple, seq<Is...>)
        -> decltype(f(std::get<Is>(tuple)...))
    {
        return f(std::get<Is>(tuple)...);
    }
}

// Maps
template<typename Function, typename... T>
auto array_map(Function&& f, const Array<T>&... arr)
    -> Array<decltype(f(T()...))>
{
    typedef decltype(f(T()...)) TR;
    constexpr int V = sizeof...(T);
    
    auto all_ndims = std::vector<int>({arr.ndims()...});
    int max_ndims = *std::max_element(all_ndims.begin(), all_ndims.end());
    
    switch(max_ndims)
    {
    case 1:
    {
        auto az = array_zip<1>(arr..., Array<TR>());
        for(auto el : az)
            std::get<V>(el) = detail::call_func(f, el, detail::gen_seq<V>());
        return std::get<V>(az.iterables());
    }
    case 2:
    {
        auto az = array_zip<2>(arr..., Array<TR>());
        for(auto el : az)
            std::get<V>(el) = detail::call_func(f, el, detail::gen_seq<V>());
        return std::get<V>(az.iterables());
    }
    case 3:
    {
        auto az = array_zip<3>(arr..., Array<TR>());
        for(auto el : az)
            std::get<V>(el) = detail::call_func(f, el, detail::gen_seq<V>());
        return std::get<V>(az.iterables());
    }
    case 4:
    {
        auto az = array_zip<4>(arr..., Array<TR>());
        for(auto el : az)
            std::get<V>(el) = detail::call_func(f, el, detail::gen_seq<V>());
        return std::get<V>(az.iterables());
    }
    default:
    {
        auto az = array_zip<-1>(arr..., Array<TR>());
        for(auto el : az)
            std::get<V>(el) = detail::call_func(f, el, detail::gen_seq<V>());
        return std::get<V>(az.iterables());
    }
    }
}

template<typename T1, typename T2>
auto operator+(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() + T2())>
{
    return array_map([](const T1& a, const T2& b){return a+b;}, arr1, arr2);
}

template<typename T1, typename T2>
auto operator-(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() - T2())>
{
    return array_map([](const T1& a, const T2& b){return a-b;}, arr1, arr2);
}

template<typename T1, typename T2>
auto operator*(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() * T2())>
{
    return array_map([](const T1& a, const T2& b){return a*b;}, arr1, arr2);
}

template<typename T1, typename T2>
auto operator/(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() / T2())>
{
    return array_map([](const T1& a, const T2& b){return a/b;}, arr1, arr2);
}

template<typename T1, typename T2>
Array<bool> operator<(const Array<T1>& arr1, const Array<T2>& arr2)
{
    return array_map([](const T1& a, const T2& b){return a<b;}, arr1, arr2);
}

template<typename T1, typename T2>
Array<bool> operator<=(const Array<T1>& arr1, const Array<T2>& arr2)
{
    return array_map([](const T1& a, const T2& b){return a<=b;}, arr1, arr2);
}

template<typename T1, typename T2>
Array<bool> operator>(const Array<T1>& arr1, const Array<T2>& arr2)
{
    return array_map([](const T1& a, const T2& b){return a>b;}, arr1, arr2);
}

template<typename T1, typename T2>
Array<bool> operator>=(const Array<T1>& arr1, const Array<T2>& arr2)
{
    return array_map([](const T1& a, const T2& b){return a>=b;}, arr1, arr2);
}

template<typename T1, typename T2>
Array<bool> operator==(const Array<T1>& arr1, const Array<T2>& arr2)
{
    return array_map([](const T1& a, const T2& b){return a==b;}, arr1, arr2);
}

template<typename T1, typename T2>
Array<bool> operator!=(const Array<T1>& arr1, const Array<T2>& arr2)
{
    return array_map([](const T1& a, const T2& b){return a!=b;}, arr1, arr2);
}

namespace detail
{
    template<typename Tout, typename Tin>
    Array<Tout> cast(const Array<Tin>& in, std::false_type)
    {
        return array_map([](const Tin& a){return static_cast<Tout>(a);}, in);
    }

    template<typename T>
    Array<T> cast(const Array<T>& in, std::true_type)
    {
        return in;
    }
}

template<typename Tout, typename Tin>
Array<Tout> cast(const Array<Tin>& in)
{
    return detail::cast<Tout>(in, std::is_same<Tout,Tin>());
}

#define VECTORIZE(vectorizedname, name) \
template<typename... T> \
auto vectorizedname(const Array<T>&... arr) \
    -> decltype(array_map(name, arr...)) \
{ \
    return array_map(name, arr...); \
}

VECTORIZE(sin, ::sin)
VECTORIZE(cos, ::cos)
VECTORIZE(tan, ::tan)
VECTORIZE(asin, ::asin)
VECTORIZE(acos, ::acos)
VECTORIZE(atan, ::atan)
VECTORIZE(atan2, ::atan2)

VECTORIZE(sinh, ::sinh)
VECTORIZE(cosh, ::cosh)
VECTORIZE(tanh, ::tanh)
VECTORIZE(asinh, ::asinh)
VECTORIZE(acosh, ::acosh)
VECTORIZE(atanh, ::atanh)

VECTORIZE(exp, ::exp)
VECTORIZE(log, ::log)
VECTORIZE(log10, ::log10)
VECTORIZE(exp2, ::exp2)
VECTORIZE(expm1, ::expm1)
VECTORIZE(ilogb, ::ilogb)
VECTORIZE(log2, ::log2)

VECTORIZE(pow, ::pow)
VECTORIZE(sqrt, ::sqrt)
VECTORIZE(cbrt, ::cbrt)
VECTORIZE(hypot, ::hypot)

VECTORIZE(ceil, ::ceil)
VECTORIZE(floor, ::floor)
VECTORIZE(fmod, ::fmod)
VECTORIZE(trunc, ::trunc)
VECTORIZE(round, ::round)
VECTORIZE(lround, ::lround)

VECTORIZE(fdim, ::fdim)
VECTORIZE(fmax, ::fmax)
VECTORIZE(fmin, ::fmin)

VECTORIZE(isinf, std::isinf)
VECTORIZE(isnan, std::isnan)
VECTORIZE(isfinite, std::isfinite)

// Reductions
template<typename T>
T sum(const Array<T>& arr)
{
    return std::accumulate(arr.begin(), arr.end(), T(0));
}

template<typename T>
Array<T> sum(const Array<T>& arr, int axis)
{
    if(axis < 0)
        axis = arr.ndims() + axis;
    
    assert(axis >= 0 && axis < arr.ndims());
    
    Shape newShape(arr.shape());
    newShape.erase(newShape.begin() + axis);
    
    auto res = zeros<T>(newShape);
    
    auto arr_it = arr.slice_begin(axis);
    auto arr_it_end = arr.slice_end(axis);
    for(; arr_it != arr_it_end; ++arr_it)
        res.deep() += *arr_it;
    
    return res;
}

template<typename T>
double mean(const Array<T>& arr)
{
    return std::accumulate(arr.begin(), arr.end(), 0) / static_cast<double>(arr.numElements());
}

template<typename T>
Array<double> mean(const Array<T>& arr, int axis)
{
    return sum(arr, axis) / singleton(static_cast<double>(arr.shape()[axis]));
}

}

#endif
