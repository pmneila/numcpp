
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

template<typename Function, typename... T>
auto array_map(Function&& f, const Array<T>&... arr)
    -> Array<decltype(f(T()...))>
{
    typedef decltype(f(T()...)) TR;
    constexpr int V = sizeof...(T);
    
    auto az = array_zip(arr..., Array<TR>());
    for(auto el : az)
        std::get<V>(el) = detail::call_func(f, el, detail::gen_seq<V>());
    return std::get<V>(az.iterables());
}

template<typename T1, typename T2>
auto operator+(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() + T2())>
{
    typedef decltype(T1() + T2()) TR;
    
    auto az = array_zip(Array<TR>(), arr1, arr2);
    for(auto el : az)
        std::get<0>(el) = std::get<1>(el) + std::get<2>(el);
    return std::get<0>(az.iterables());
}

template<typename T1, typename T2>
auto operator-(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() - T2())>
{
    typedef decltype(T1() - T2()) TR;
    
    auto az = array_zip(Array<TR>(), arr1, arr2);
    for(auto el : az)
        std::get<0>(el) = std::get<1>(el) - std::get<2>(el);
    return std::get<0>(az.iterables());
}

template<typename T1, typename T2>
auto operator*(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() * T2())>
{
    typedef decltype(T1() * T2()) TR;
    
    auto az = array_zip(Array<TR>(), arr1, arr2);
    for(auto el : az)
        std::get<0>(el) = std::get<1>(el) * std::get<2>(el);
    return std::get<0>(az.iterables());
}

template<typename T1, typename T2>
auto operator/(const Array<T1>& arr1, const Array<T2>& arr2)
    -> Array<decltype(T1() / T2())>
{
    typedef decltype(T1() / T2()) TR;
    
    auto az = array_zip(Array<TR>(), arr1, arr2);
    for(auto el : az)
        std::get<0>(el) = std::get<1>(el) / std::get<2>(el);
    return std::get<0>(az.iterables());
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

template<typename Tout, typename Tin>
Array<Tout> cast(const Array<Tin>& in)
{
    return array_map([](const Tin& a){return static_cast<Tout>(a);}, in);
}

#define VECTORIZE2(vectorizedname, name) \
template<typename... T> \
auto vectorizedname(const Array<T>&... arr) \
    -> decltype(array_map(name, arr...)) \
{ \
    return array_map(name, arr...); \
}

VECTORIZE2(sin, ::sin)
VECTORIZE2(cos, ::cos)
VECTORIZE2(tan, ::tan)
VECTORIZE2(asin, ::asin)
VECTORIZE2(acos, ::acos)
VECTORIZE2(atan, ::atan)
VECTORIZE2(atan2, ::atan2)

VECTORIZE2(sinh, ::sinh)
VECTORIZE2(cosh, ::cosh)
VECTORIZE2(tanh, ::tanh)
VECTORIZE2(asinh, ::asinh)
VECTORIZE2(acosh, ::acosh)
VECTORIZE2(atanh, ::atanh)

VECTORIZE2(exp, ::exp)
VECTORIZE2(log, ::log)
VECTORIZE2(log10, ::log10)
VECTORIZE2(exp2, ::exp2)
VECTORIZE2(expm1, ::expm1)
VECTORIZE2(ilogb, ::ilogb)
VECTORIZE2(log2, ::log2)

VECTORIZE2(pow, ::pow)
VECTORIZE2(sqrt, ::sqrt)
VECTORIZE2(cbrt, ::cbrt)
VECTORIZE2(hypot, ::hypot)

VECTORIZE2(ceil, ::ceil)
VECTORIZE2(floor, ::floor)
VECTORIZE2(fmod, ::fmod)
VECTORIZE2(trunc, ::trunc)
VECTORIZE2(round, ::round)
VECTORIZE2(lround, ::lround)

VECTORIZE2(fdim, ::fdim)
VECTORIZE2(fmax, ::fmax)
VECTORIZE2(fmin, ::fmin)

VECTORIZE2(isinf, std::isinf)
VECTORIZE2(isnan, std::isnan)
VECTORIZE2(isfinite, std::isfinite)

}

#endif
