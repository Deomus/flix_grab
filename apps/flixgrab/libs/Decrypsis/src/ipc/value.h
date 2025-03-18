#pragma once 

#include <type_traits>
#include <string>
#include <vector>

namespace ipc {
    

    namespace detail {
        
        template<typename Tuple, size_t I = std::tuple_size<Tuple>::value>
        struct indexed_helper {
            //Serialization is backwarded;
            static const size_t next_index = I - 1;
            using element_type = typename std::tuple_element<next_index, Tuple>::type;

            template<typename B>
            static bool serialize_impl(B& b, const Tuple&& t) {

                return indexed_helper<Tuple, next_index>::serialize_impl(b, std::forward<const Tuple>(t))
                    && WriteValue(b, std::forward<const element_type>(std::get<next_index>(t)));
            }

            template<typename B>
            static bool deserialize_impl(B& b, Tuple&& t) {

                return indexed_helper<Tuple, next_index>::deserialize_impl(b, std::forward<Tuple>(t))
                    && ReadValue(b, std::forward<element_type>(std::get<next_index>(t)));
            }
        };

        template<typename Tuple>
        struct indexed_helper<Tuple, 0u> {

            template<typename B>
            static bool serialize_impl(B&, const Tuple&&) {
                return true;
            }

            template<typename B>
            static bool deserialize_impl(B&, Tuple&&) {
                return true;
            }

        };
    }

    //////////////////////////////////////////////////////////////////////////
    //Param traits definition;
    template<typename... Args>
    struct ValuePack : public std::tuple<std::decay_t<Args>...>{
        using tuple_type = std::tuple<std::decay_t<Args>...>;

        using index_type = util::to_index_tuple<Args...>;

        static const std::size_t arity = sizeof...(Args);


        //param_traits(Args... args, unsigned overload_guard = 0) : tuple_type(args...) {}
        ValuePack(Args... args) : tuple_type(args...) {}
        ValuePack()  {}

        template<typename B>
        bool    Serialize(B& b) const {
            return detail::indexed_helper<const tuple_type>::serialize_impl(b, std::forward<const tuple_type>(*this));
        }

        template<typename B>
        bool    Deserialize(B& b) {
            return detail::indexed_helper<tuple_type>::deserialize_impl(b, std::forward<tuple_type>(*this));
        }

        template<int N>
        typename std::tuple_element<N, tuple_type>::type& value() { return std::get<N>(*this); }
        template<int N>
        const typename std::tuple_element<N, tuple_type>::type& value() const { return std::get<N>(*this); }


    };

    //Void Pack;
    template<>
    struct ValuePack<void> {

        static const std::size_t arity = 0;
        template<typename B>
        bool    Serialize(B&) const { return true; }

        template<typename B>
        bool    Deserialize(B&) { return true; }

        //Needs For return void type;
        template<int N>
        void value() const {  }


    };


    //////////////////////////////////////////////////////////////////////////
    //Serializer;
    template<typename T, typename Enabled = void>
    struct ValueTraits {
        static_assert(true,
            "Type Not defined");
    };

    template<typename T>
    struct ValueTraits<T, typename std::enable_if<std::is_pod<T>::value>::type > {

        template<typename B>
        static bool Read(B& b, T&& value) {
            return b.Pod(value);
        }

        template<typename B>
        static bool Write(B& b, const T&& value) {
            return b.Pod(value);
        }
    };

    //String Specialization;
    template<>
    struct ValueTraits<std::string, void > {

        template<typename B>
        static bool Read(B& b, std::string&& value) {
            return b.String(value);
        }

        template<typename B>
        static bool Write(B& b, const std::string&& value) {
            return b.String(value);
        }
    };

    //String Specialization;
    template<>
    struct ValueTraits<std::wstring, void > {

        template<typename B>
        static bool Read(B& b, std::wstring&& value) {
            return b.WString(value);
        }

        template<typename B>
        static bool Write(B& b, const std::wstring&& value) {
            return b.WString(value);
        }
    };

    //Vector Specialization
    template<typename U>
    struct ValueTraits<std::vector<U>, void > {

        template<typename B>
        static bool Read(B& b, std::vector<U>&& value) {
            return b.Vector(value);
        }

        template<typename B>
        static bool Write(B& b, const std::vector<U>&& value) {
            return b.Vector(std::forward<std::vector<U>>(value));
        }
    };

    template<typename T, typename B>
    bool WriteValue(B& b, const T&& value) {
        return ValueTraits<T>::Write(b, std::forward<const T>(value));
    }

    template<typename T, typename B>
    bool ReadValue(B& b, T&& value) {
        return ValueTraits<T>::Read(b, std::forward<T>(value));
    }

    //////////////////////////////////////////////////////////////////////////



}

