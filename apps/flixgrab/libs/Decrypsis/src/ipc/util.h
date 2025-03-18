#pragma once 

#include <tuple>
#include <type_traits>

namespace util {

    /// A type that represents a parameter pack of zero or more integers.
    template<unsigned... Indices>
    struct index_tuple {
        /// Generate an index_tuple with an additional element.
        template<unsigned N>
        using append = index_tuple<Indices..., N>;
    };

    /// Unary metafunction that generates an index_tuple containing [0, Size)
    template<unsigned Size>
    struct make_index_tuple {
        typedef typename make_index_tuple<Size - 1>::type::template append<Size - 1>
            type;
    };

    // Terminal case of the recursive metafunction.
    template<>
    struct make_index_tuple<0u> {
        typedef index_tuple<> type;
    };

    template<typename... Types>
    using to_index_tuple = typename make_index_tuple<sizeof...(Types)>::type;
    //////////////////////////////////////////////////////////////////////////

    //Append tuple to tuple
    template<class... T> struct list {};
    template<class... L> struct append_impl;

    template<class... L> using append = typename append_impl<L...>::type;

    template<> struct append_impl<> {
        using type = list<>;
    };

    template<template<class...> class L, class... T> struct append_impl<L<T...>> {
        using type = L<T...>;
    };

    template<template<class...> class L1, class... T1,
        template<class...> class L2, class... T2, class... Lr>
    struct append_impl<L1<T1...>, L2<T2...>, Lr...> {
        using type = append<L1<T1..., T2...>, Lr...>;
    };

    //////////////////////////////////////////////////////////////////////////

    template <class T, class Tuple>
    struct get_index;

    template <class T, class... Types>
    struct get_index<T, std::tuple<T, Types...>> {
        static const std::size_t value = 0;
    };

    template <class T, class U, class... Types>
    struct get_index<T, std::tuple<U, Types...>> {
        static const std::size_t value = 1 + get_index<T, std::tuple<Types...>>::value;
    };

    //////////////////////////////////////////////////////////////////////////


    template<std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    for_each(std::tuple<Tp...> &, FuncT) // Unused arguments are given no names.
    {
    }

    template<std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if < I < sizeof...(Tp), void>::type
    for_each(std::tuple<Tp...>& t, FuncT f) {
        f(std::get<I>(t));
        for_each<I + 1, FuncT, Tp...>(t, f);
    }


    //////////////////////////////////////////////////////////////////////////

    template<typename Tuple, template<typename T>class FuncT, std::size_t I = 0>
    inline typename std::enable_if<I == std::tuple_size<Tuple>::value, void>::type
        for_each_type() // Unused arguments are given no names.
    {
    }

    template<typename Tuple, template<typename T>class FuncT, std::size_t I = 0>
    inline typename std::enable_if < I < std::tuple_size<Tuple>::value, void>::type
        for_each_type() {
        FuncT<std::tuple_element<I, Tuple>> f();
        for_each_type<Tuple, FuncT, I + 1>();
    }

    template<typename T> struct argument_type;
    template<typename T, typename U> struct argument_type<T(U)> { typedef U type; };
        
}