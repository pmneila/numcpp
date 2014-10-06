
#ifndef NUMCPP_ITERTOOLS_H
#define NUMCPP_ITERTOOLS_H

#include <tuple>
#include <type_traits>

#include "ostream.h"

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
    auto seq_map(const T& t, F f, seq<Is...>) -> decltype(std::make_tuple(f(std::get<Is>(t))...))
    {
        auto l = std::make_tuple(f(std::get<Is>(t))...);
        return l;
    }
    
    template<typename T, typename F, int... Is>
    auto seq_map(const T& t1, const T& t2, F f, seq<Is...>)
        -> decltype(std::make_tuple(f(std::get<Is>(t1), std::get<Is>(t2))...))
    {
        return std::make_tuple(f(std::get<Is>(t1), std::get<Is>(t2))...);
    }
    
    template<typename T, typename F, int... Is>
    auto seq_map_ref(T& t, F f, seq<Is...>) -> decltype(std::forward_as_tuple(f(std::get<Is>(t))...))
    {
        auto l = std::forward_as_tuple(f(std::get<Is>(t))...);
        return l;
    }
    
    template<typename... Ts, typename F>
    auto tuple_map(const std::tuple<Ts...>& t, F f) -> decltype(seq_map(t, f, gen_seq<sizeof...(Ts)>()))
    {
        return seq_map(t, f, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename... Ts, typename F>
    auto tuple_map(const std::tuple<Ts...>& t1, const std::tuple<Ts...>& t2, F f)
        -> decltype(seq_map(t1, t2, f, gen_seq<sizeof...(Ts)>()))
    {
        return seq_map(t1, t2, f, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename... Ts, typename F>
    auto tuple_map_ref(std::tuple<Ts...>& t, F f) -> decltype(seq_map_ref(t, f, gen_seq<sizeof...(Ts)>()))
    {
        return seq_map_ref(t, f, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename T, typename F, int I, int... Is>
    void seq_for_each(T& t, F&& f, seq<I, Is...>)
    {
        f(std::get<I>(t));
        seq_for_each(t, f, seq<Is...>());
    }
    
    template<typename T, typename F>
    void seq_for_each(T& t, F&& f, seq<>)
    {}
    
    template<typename... Ts, typename F>
    
    void tuple_for_each(std::tuple<Ts...>& t, F&& f)
    {
        seq_for_each(t, f, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename T, int I, int... Is>
    bool seq_any(const T& t, seq<I, Is...>)
    {
        if(std::get<I>(t))
            return true;
        
        return seq_any(t, seq<Is...>());
    }
    
    template<typename T>
    bool seq_any(const T& t, seq<>)
    {
        return false;
    }
    
    template<typename... Ts>
    bool tuple_any(const std::tuple<Ts...>& t)
    {
        return seq_any(t, gen_seq<sizeof...(Ts)>());
    }
    
    template<typename T, int I, int... Is>
    bool seq_all(const T& t, seq<I, Is...>)
    {
        if(!std::get<I>(t))
            return false;
        
        return seq_all(t, seq<Is...>());
    }
    
    template<typename T>
    bool seq_all(const T& t, seq<>)
    {
        return true;
    }
    
    template<typename... Ts>
    bool tuple_all(const std::tuple<Ts...>& t)
    {
        return seq_all(t, gen_seq<sizeof...(Ts)>());
    }
    
    struct begin_functor
    {
        template<typename T>
        auto operator () (T& t) -> decltype(std::begin(t))
        {
            return std::begin(t);
        }
    };
    
    struct end_functor
    {
        template<typename T>
        auto operator () (T& t) -> decltype(std::end(t))
        {
            return std::end(t);
        }
    };
    
    struct cont_begin_functor
    {
        template<typename T>
        auto operator () (T& t) -> decltype(t.cont_begin())
        {
            return t.cont_begin();
        }
    };
    
    struct cont_end_functor
    {
        template<typename T>
        auto operator () (T& t) -> decltype(t.cont_end())
        {
            return t.cont_end();
        }
    };
    
    struct increment_functor
    {
        template<typename T>
        void operator () (T& t)
        {
            ++t;
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
        auto operator () (T& t1, T& t2) -> decltype(t1 == t2)
        {
            return t1 == t2;
        }
    };
    
    struct notequal_functor
    {
        template<typename T>
        auto operator () (T& t1, T& t2) -> decltype(t1 != t2)
        {
            return t1 != t2;
        }
    };
    
}

template<typename iterators_tuple>
class ZipIterator
{
protected:
    iterators_tuple _iterators;

public:
    ZipIterator(const iterators_tuple& t)
        : _iterators(t)
    {}
    
    ZipIterator(const ZipIterator& rhs)
        : _iterators(rhs._iterators)
    {}
    
    ZipIterator& operator++()
    {
        detail::tuple_for_each(_iterators, detail::increment_functor());
        return *this;
    }
    
    ZipIterator operator++(int)
    {
        ZipIterator aux(*this);
        detail::tuple_for_each(_iterators, detail::increment_functor());
        return aux;
    }
    
    auto operator*() -> decltype(detail::tuple_map_ref(this->_iterators, detail::dereference_functor()))
    {
        return detail::tuple_map_ref(_iterators, detail::dereference_functor());
    }
    
    bool operator==(const ZipIterator& rhs)
    {
        return std::get<0>(_iterators) == std::get<0>(rhs._iterators);
        // auto aux = detail::tuple_map(_iterators, rhs._iterators, detail::equal_functor());
        // return detail::tuple_all(aux);
    }
    
    bool operator!=(const ZipIterator& rhs)
    {
        return std::get<0>(_iterators) != std::get<0>(rhs._iterators);
        // auto aux = detail::tuple_map(_iterators, rhs._iterators, detail::notequal_functor());
        // return detail::tuple_any(aux);
    }
    
    const iterators_tuple& iterators() const {return _iterators;}
};

template<typename... Iterables>
class Zip
{
public:
    typedef std::tuple<Iterables&...> iterable_tuple;
    static const unsigned int size = sizeof...(Iterables);
    
protected:
    iterable_tuple _iterables;
    
public:
    Zip(const iterable_tuple& t)
        : _iterables(t)
    {}
    
    auto begin() -> ZipIterator<decltype(detail::tuple_map(this->_iterables, detail::begin_functor()))>
    {
        auto aux = detail::tuple_map(_iterables, detail::begin_functor());
        return ZipIterator<decltype(aux)>(aux);
    }
    
    auto end() -> ZipIterator<decltype(detail::tuple_map(this->_iterables, detail::end_functor()))>
    {
        auto aux = detail::tuple_map(_iterables, detail::end_functor());
        return ZipIterator<decltype(aux)>(aux);
    }
    
    const iterable_tuple& iterables() const {return _iterables;}
    
};

template<typename... Iterables>
Zip<Iterables...> zip(Iterables&... iterables)
{
    typedef std::tuple<Iterables&...> Tuple;
    Tuple t(iterables...);
    return Zip<Iterables...>(t);
}

namespace detail
{
    struct init_shape
    {
        Shape _shape;
        
        init_shape(const Shape& shape)
            : _shape(shape)
        {}
        
        template<typename T>
        void operator () (Array<T>& t)
        {
            if(t.isNull())
                t = empty<T>(_shape);
            
            if(t.shape() != _shape)
                throw std::invalid_argument("arrays in array_zip do not have the same shape");
        }
    };
    
    template<size_t NDims, int... Is>
    struct IndexArrayWithStdArray
    {
        const std::array<size_t, NDims> _index;
        
        IndexArrayWithStdArray(const std::array<size_t, NDims>& index)
            : _index(index)
        {}
        
        template<typename T>
        T& operator()(const Array<T>& arr)
        {
            return arr(_index[Is]...);
        }
    };
    
    template<size_t NDims, int N, int... Is>
    struct gen_IndexArrayWithStdArray : public gen_IndexArrayWithStdArray<NDims, N-1, N-1, Is...>
    {
        using gen_IndexArrayWithStdArray<NDims, N-1, N-1, Is...>::gen_IndexArrayWithStdArray;
    };
    
    template<size_t NDims, int... Is>
    struct gen_IndexArrayWithStdArray<NDims, 0, Is...> : public IndexArrayWithStdArray<NDims, Is...>
    {
        using IndexArrayWithStdArray<NDims, Is...>::IndexArrayWithStdArray;
    };
};

template<size_t NDims, typename... T>
class ArrayZipIterator
{
public:
    typedef std::tuple<const Array<T>&...> array_tuple;
    
private:
    array_tuple _arrays;
    std::array<size_t, NDims> _shape;
    std::array<size_t, NDims> _index;
    
public:
    ArrayZipIterator(const array_tuple& arrays)
        : _arrays(arrays)
    {
        const Shape& sh = std::get<0>(_arrays).shape();
        for(int i = 0; i < NDims; ++i)
        {
            _shape[i] = sh[i];
            _index[i] = 0;
        }
    }
    
    ArrayZipIterator(const ArrayZipIterator& rhs)
        : _arrays(rhs._arrays)
        , _shape(rhs._shape)
        , _index(rhs._index)
    {}
    
    ArrayZipIterator& operator=(const ArrayZipIterator& rhs)
    {
        _arrays = rhs._arrays;
        _shape = rhs._shape;
        _index = rhs._index;
        return *this;
    }
    
    ArrayZipIterator& operator++()
    {
        int i = NDims - 1;
        for(; i > 0; --i)
        {
            ++_index[i];
            if(_index[i] < _shape[i])
                break;
            
            _index[i] = 0;
        }
        
        if(i==0)
            ++_index[0];
        
        return *this;
    }
    
    ArrayZipIterator& operator++(int)
    {
        ArrayZipIterator<T...> aux(*this);
        ++(*this);
        return *this;
    }
    
    std::tuple<T&...> operator*()
    {
        return tuple_map_ref(_arrays, detail::gen_IndexArrayWithStdArray<NDims, NDims>(_index));
    }
    
    bool operator==(const ArrayZipIterator& rhs)
    {
        for(int i = 0; i < NDims; ++i)
        {
            if(_index[i] != rhs._index[i])
                return false;
        }
        return true;
    }

    bool operator!=(const ArrayZipIterator& rhs)
    {
        for(int i = 0; i < NDims; ++i)
        {
            if(_index[i] != rhs._index[i])
                return true;
        }
        return false;
    }
    
    void goToEnd()
    {
        _index = {0};
        _index[0] = _shape[0];
    }
};

// General ArrayZipIterator for any number of dimensions
template<typename... T>
class ArrayZipIterator<-1, T...> : public ZipIterator<std::tuple<typename Array<T>::iterator...>>
{
public:
    typedef ZipIterator<std::tuple<typename Array<T>::iterator...>> base;
    typedef std::tuple<const Array<T>&...> array_tuple;
    
protected:
    array_tuple _arrays;
    
public:
    
    ArrayZipIterator(const array_tuple& arrays)
        : base(detail::tuple_map(arrays, detail::begin_functor()))
        , _arrays(arrays)
    {}
    
    void goToEnd()
    {
        this->_iterators = detail::tuple_map(_arrays, detail::end_functor());
    }
};

// Similar to Zip, but ArrayZip holds the iterables, not just references.
template<size_t NDims, typename... T>
class ArrayZip
{
public:
    typedef std::tuple<Array<T>...> array_tuple;
    
protected:
    array_tuple _arrays;
    
public:
    ArrayZip(const Array<T>&... arrays)
        : _arrays(arrays...)
    {}
    
    ArrayZip(const std::tuple<Array<T>...>& arrays)
        : _arrays(arrays)
    {}
    
    ArrayZipIterator<NDims, T...> begin()
    {
       return ArrayZipIterator<NDims, T...>(_arrays);
    }
    
    ArrayZipIterator<NDims, T...> end()
    {
        ArrayZipIterator<NDims, T...> it(_arrays);
        it.goToEnd();
        return it;
    }
    
    const array_tuple& iterables() const {return _arrays;}
    const array_tuple& arrays() const {return _arrays;}
};

template<size_t NDims=-1, typename... T>
ArrayZip<NDims, T...> array_zip(const Array<T>&... arr)
{
    Shape bshape = broadcastShapes({arr.shape()...});
    auto arrays = std::make_tuple(broadcast(arr, bshape)...);
    detail::tuple_for_each(arrays, detail::init_shape(bshape));
    
    return ArrayZip<NDims, T...>(arrays);
}

}

#endif
