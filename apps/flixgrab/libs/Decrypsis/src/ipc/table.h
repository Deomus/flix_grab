#pragma once

#include "function.h"
#include <vector>

#define NAME_IMPL(name)       name##_impl
//#define INVOKE_TYPE_IMPL(name, t) struct NAME_IMPL(name) {using type = argument_type<void(t)>::type;}; 
#define INVOKE_TYPE_IMPL(name, t) struct NAME_IMPL(name) : public ipc::FunctionDecl<decltype(t), t> {}; 
#define TUPLE_ARG(name, t) detail::NAME_IMPL(name),
#define SERVER_FUNC(name, t) &detail::NAME_IMPL(name)::CallLocal,

#define TABLE_ELEMENT_DECL(name, t) \
    struct name { \
        using storage_type = ipc::FunctionStorage<decltype(t)>; \
        static const int id = util::get_index<detail::NAME_IMPL(name), table_tuple>::value; }; 

#define DECLARE_INVOKE_TABLE(name, tbl) \
    namespace name {\
    namespace detail { tbl(INVOKE_TYPE_IMPL) };\
        using table_tuple = std::tuple<tbl(TUPLE_ARG)void>; \
        tbl(TABLE_ELEMENT_DECL)\
        static const std::vector<ipc::ServerFunc> server_funcs = {tbl(SERVER_FUNC)nullptr}; }; 


#define INVOKE_REMOTE(client, table_name, func_name, ...)  ipc::InvokeRemote<table_name::func_name>(client,  __VA_ARGS__)
#define SERVER_TABLE(table_name)        table_name::server_funcs


/*

MAKING TABLE NOTES:

1) Define Table as:
#define MY_TABLE(item, sep)          \
    item(Inv1, &foo1) sep\
    ...
    item(InvN, &fooN)

2) Declare Table:

DECLARE_INVOKE_TABLE(MyName, MY_TABLE)

3) Register Server;
Server my_server("NAME", SERVER_TABLE(MyName));


*/