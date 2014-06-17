
#ifndef NUMCPP_ITERTOOLS_H
#define NUMCPP_ITERTOOLS_H

#include <tuple>
#include <type_traits>

namespace numcpp
{

namespace detail
{
    template<int... Is>
    struct seq {};

    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

    template<int... Is>
    struct gen_seq<0, Is...> : seq<Is...> {};
    
    template<typename T, typename F, int... Is>
    auto for_each(T& t, F f, seq<Is...>) -> decltype(std::make_tuple(f(std::get<Is>(t))...))
    {
        auto l = std::make_tuple(f(std::get<Is>(t))...);
        return l;
    }
    
    template<typename... Ts, typename F>
    auto for_each_in_tuple(std::tuple<Ts...>& t, F f) -> decltype(for_each(t, f, gen_seq<sizeof...(Ts)>()))
    {
        return for_each(t, f, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename T, typename F, int... Is>
    auto for_each_ref(T& t, F f, seq<Is...>) -> decltype(std::forward_as_tuple(f(std::get<Is>(t))...))
    {
        auto l = std::forward_as_tuple(f(std::get<Is>(t))...);
        return l;
    }
    
    template<typename... Ts, typename F>
    auto for_each_in_tuple_ref(std::tuple<Ts...>& t, F f) -> decltype(for_each_ref(t, f, gen_seq<sizeof...(Ts)>()))
    {
        return for_each_ref(t, f, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename T, typename F, int... Is>
    std::vector<bool> for_each(T& t1, T& t2, F f, seq<Is...>)
    {
        return std::vector<bool>{f(std::get<Is>(t1), std::get<Is>(t2))...};
    }
    
    template<typename... Ts, typename F>
    std::vector<bool> for_each_in_tuples(const std::tuple<Ts...>& t1, const std::tuple<Ts...>& t2, F f)
    {
        return for_each(t1, t2, f, gen_seq<sizeof...(Ts)>());
    }
    
    struct begin_functor
    {
        template<typename T>
        typename T::iterator operator () (T& t)
        {
            return t.begin();
        }
    };
    
    struct end_functor
    {
        template<typename T>
        typename T::iterator operator () (T& t)
        {
            return t.end();
        }
    };
    
    struct increment_functor
    {
        template<typename T>
        int operator () (T& t)
        {
            ++t;
            return 0;
        }
    };
    
    struct dereference_functor
    {
        template<typename T>
        auto operator () (T& t) -> decltype(*t)
        {
            return *t;
        }
    };
    
    struct equal_functor
    {
        template<typename T>
        bool operator () (T& t1, T& t2)
        {
            return t1 == t2;
        }
    };
    
    struct notequal_functor
    {
        template<typename T>
        bool operator () (T& t1, T& t2)
        {
            return t1 != t2;
        }
    };
}

template<typename... Iterators>
class ZipIterator
{
private:
public:
    typedef std::tuple<Iterators...> iterators_tuple;
    typedef std::tuple<typename Iterators::value_type...> values_tuple;
    
    ZipIterator(const iterators_tuple& t)
        : _iterators(t)
    {}
    
    ZipIterator(const ZipIterator& rhs)
        : _iterators(rhs._iterators)
    {}
    
    ZipIterator& operator++()
    {
        detail::for_each_in_tuple(_iterators, detail::increment_functor());
        return *this;
    }
    
    ZipIterator operator++(int)
    {
        ZipIterator aux(*this);
        detail::for_each_in_tuple(_iterators, detail::increment_functor());
        return aux;
    }
    
    values_tuple operator*()
    {
        return detail::for_each_in_tuple_ref(_iterators, detail::dereference_functor());
    }
    
    bool operator==(const ZipIterator& rhs)
    {
        auto aux = detail::for_each_in_tuples(_iterators, rhs._iterators, detail::equal_functor());
        return std::any_of(aux.begin(), aux.end(), [](bool v) -> bool {return v;});
    }
    
    bool operator!=(const ZipIterator& rhs)
    {
        auto aux = detail::for_each_in_tuples(_iterators, rhs._iterators, detail::notequal_functor());
        return std::all_of(aux.begin(), aux.end(), [](bool v) -> bool {return v;});
    }
    
    const iterators_tuple& iterators() const {return _iterators;}
    
protected:
    iterators_tuple _iterators;
};

template<typename... Iterables>
class Zip
{
public:
    typedef std::tuple<Iterables&...> iterable_tuple;
    static const unsigned int size = sizeof...(Iterables);
    
    Zip(const iterable_tuple& t)
        : _iterables(t)
    {}
    
    ZipIterator<typename Iterables::iterator...> begin()
    {
        auto aux = detail::for_each_in_tuple(_iterables, detail::begin_functor());
        return ZipIterator<typename Iterables::iterator...>(aux);
    }
    
    ZipIterator<typename Iterables::iterator...> end()
    {
        auto aux = detail::for_each_in_tuple(_iterables, detail::end_functor());
        return ZipIterator<typename Iterables::iterator...>(aux);
    }
    
    const iterable_tuple& iterables() const {return _iterables;}
    
protected:
    iterable_tuple _iterables;
};

template<typename... Iterables>
Zip<Iterables...> zip(Iterables&... iterables)
{
    typedef std::tuple<Iterables&...> Tuple;
    Tuple t(iterables...);
    return Zip<Iterables...>(t);
}

}

#endif
