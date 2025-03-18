#pragma once 

#include <tuple>
#include <type_traits>

#include "util.h"
#include "value.h"

#include "client.h"
#include "server.h"

namespace ipc {
    namespace detail {
        template<typename Func, typename Tuple, unsigned... I, typename Out>
        void caller_impl(Func func, Tuple&& t, util::index_tuple<I...>, Out& out) {
            out = func(std::get<I>(std::forward<Tuple>(t))...);
        }

        //NOTE: Only const reference args supported at this time;
        template<typename Func, typename Tuple, unsigned... I>
        void caller_impl(Func func, Tuple&& t, util::index_tuple<I...>) {
            func(std::get<I>(std::forward<Tuple>(t))...);
        }
    }

//////////////////////////////////////////////////////////////////////////
    template<class F>
    struct FunctionTraits {};

    //Declare Specialization for various func types;
    template<class R, class... Args>
    struct FunctionTraits<R(*)(Args...)> : public FunctionTraits<R(Args...)>
    {
        typedef  R(*func_type)(Args...);
   
        template<func_type method>
        static R CallWrapper(Args... args) {	// call the function
            return ((*method)(std::forward<Args>(args)...));
        }
    };

    template<class R, class C, class... Args>
    struct FunctionTraits<R(C::*)(Args...)> : public FunctionTraits<R(uintptr_t, Args...)>
    {
        typedef  R(C::*func_type)(Args...);
      
        using class_type = C;

        template<func_type method>
        static R CallWrapper(uintptr_t obj, Args... args) {	
            return ((reinterpret_cast<C*>(obj)->*method)(std::forward<Args>(args)...));
        }
    };

    template<class R, class C, class... Args>
    struct FunctionTraits<R(C::*)(Args...) const> : FunctionTraits<R(Args...)>
    {
        typedef  R(*func_type)(Args...);
        using class_type = C;

        template<func_type method>
        static R CallWrapper(uintptr_t obj, Args... args) {	
            return ((reinterpret_cast<C*>(obj)->*method)(std::forward<Args>(args)...));
        }
    };
    //Specialization for voids;
    template<class R, class... Args>
    struct FunctionTraits<R(Args...)>
    {
        typedef  R(func_type)(Args...);
        using args_pack = ValuePack<Args...>;
        using ret_pack = ValuePack<R>;
        using return_type = R;
         
    protected:
        template<typename F>
        static void Call(F func, const args_pack&& args, ret_pack&& ret) {
            detail::caller_impl(func, std::forward<const args_pack>(args), args_pack::index_type(), ret.value<0>());
        }

    };

    template<class... Args>
    struct FunctionTraits<void(Args...)>
    {
        typedef  void(func_type)(Args...);
        using return_type = void;
        using args_pack = ValuePack<Args...>;
        using ret_pack = ValuePack<void>;

    protected:
        //Only Const Args Supported At this time;
        template<typename F>
        static void Call(F func, const args_pack&& args, ret_pack&&) {
            detail::caller_impl(func, std::forward<const args_pack>(args), args_pack::index_type());
        }
    };

    template<>
    struct FunctionTraits<void(void)>
    {
        typedef  void(func_type)();
        using return_type = void;
        using args_pack = ValuePack<void>;
        using ret_pack = ValuePack<void>;
              
    protected:
        //Only Const Args Supported At this time;
        template<typename F>
        static void Call(F func, const args_pack&&, ret_pack&&) { func(); }
    };

    //////////////////////////////////////////////////////////////////////////
    //Server

    template<typename T, typename FunctionTraits<T>::func_type func>
    struct FunctionDecl : public FunctionTraits<T>// <FunctionTraits<T>, func>
    {
        using func_traits = FunctionTraits<T>;
        using args_pack = typename func_traits::args_pack;
        using ret_pack = typename func_traits::ret_pack;

        template < typename Rb, typename Wb>
        static bool CallLocal(Rb& reader, Wb& writer){
            args_pack       args;
            ret_pack        ret;

            if (args.Deserialize(reader)) {
                Call(&FunctionTraits<T>::CallWrapper<func>, std::forward<const args_pack>(args), std::forward<ret_pack>(ret));
                return ret.Serialize(writer);
            }
            return false;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    //Client

    template<typename T>
    struct FunctionStorage : public Storage
    {
        using func_traits = FunctionTraits<T>;
        using args_pack = typename func_traits::args_pack;
        using ret_pack = typename func_traits::ret_pack;
        using return_type = typename func_traits::return_type;

        template <typename ...Args>
        FunctionStorage(Args... args) : args_(args...) {}

        virtual bool    Serialize(BufferWriter& b) override {
            return args_.Serialize(b);
        }
        virtual bool    Deserialize(BufferReader& b) override {
            return ret_.Deserialize(b);
        }

        return_type ret() const { return ret_.value<0>(); }

    private:
        args_pack       args_;
        ret_pack        ret_;
    };

    //Invoke Function On Server;
    template <typename T, typename ...Args>
    inline typename T::storage_type::return_type InvokeRemote(Client* client, Args... args) {
        using storage_type = typename T::storage_type;
        static_assert(storage_type::args_pack::arity == sizeof...(Args), "Count of Args MUST be equal");

        storage_type storage(args...);

        client->Invoke(T::id, &storage);
        return storage.ret();
    }

}

