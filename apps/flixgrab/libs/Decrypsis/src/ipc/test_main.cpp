#include "ipc.h"
#include <tuple>
#include <iostream>



//Tests;

void   foo0(void) {
    std::cout << "foo0 " << std::endl;

}

void   foo1(uint32_t a) {
    std::cout << "foo1 " << a << std::endl;
    a = 0;
}

int   foo2(void* a) {
    std::cout << "foo2 " << a << std::endl;
    return (int)a;
}
int   foo3(int a, float b) {
    std::cout << "foo3 " << a << " : " << b << std::endl;
    return (int)(a + b);
}

bool   foo4(int a, float b, const std::string& data) {
    std::cout << "foo4 " << a << " : " << b << " - " << data << std::endl;
    return (a > 0);
}

std::string  foo5(const std::string& data) {
    std::cout << "foo5 " << data << std::endl;
    return "World " + data;
}

struct struct1 {
    std::string  foo1(const std::string& data) {
        std::cout << "struct1::foo1 " << data << std::endl;
        return "struct1 " + data;
    }

    void  foo2(const std::string& data) {
        std::cout << "struct1::foo2 " << data << std::endl;

    }

    void  foo3() {
        std::cout << "struct1::foo3 " << std::endl;

    }

    virtual void  foo4() {
        std::cout << "struct1::foo4 " << std::endl;

    }

    static void  foo5(const std::vector<int>& a) {
        std::cout << "struct1::foo5 " << std::endl;

    }
};

struct Buffer {

    template<typename T>
    bool Pod(T& v) {
        return true;
    }

    bool Pod(uintptr_t& v) {
        v = reinterpret_cast<uintptr_t>(&test);
        return true;
    }

    bool String(std::string& str) {
        return true;
    }

    bool String(const std::string& str) {
        return true;
    }

    template<typename T>
    bool Vector(std::vector<T>& str) {
        return true;
    }

    template<typename T>
    bool Vector(const std::vector<T>& str) {
        return true;
    }

    struct1  test;
};



//////////////////////////////////////////////////////////////////////////

namespace gggg {
    class ty : public ipc::FunctionDecl<decltype(&foo0), &foo0> {};
    class tz : public ipc::FunctionStorage<decltype(&foo0)> {};
}

#define INVOKE_TABLE(TABLE_FUNC)          \
    TABLE_FUNC(Inv0, &foo0)\
    TABLE_FUNC(Inv1, &foo1)\
    TABLE_FUNC(Inv2, &foo2)\
    TABLE_FUNC(Inv3, &foo3)\
    TABLE_FUNC(Inv4, &struct1::foo1)


DECLARE_INVOKE_TABLE(first, INVOKE_TABLE)



int main() {

    ipc::ValuePack<void> aaa;
    ipc::FunctionTraits<decltype(&foo0)>::args_pack     args;



    struct1 sss;
    Buffer bw;

    ipc::FunctionDecl<decltype(&struct1::foo1), &struct1::foo1>::CallLocal(bw, bw);
    ipc::FunctionDecl<decltype(&struct1::foo2), &struct1::foo2>::CallLocal(bw, bw);
    ipc::FunctionDecl<decltype(&foo2), &foo2>::CallLocal(bw, bw);
    ipc::FunctionDecl<decltype(&foo0), &foo0>::CallLocal(bw, bw);

    ipc::FunctionDecl<decltype(&struct1::foo4), &struct1::foo4>::CallLocal(bw, bw);
    ipc::FunctionDecl<decltype(&struct1::foo5), &struct1::foo5>::CallLocal(bw, bw);


    ipc::Server server("test", SERVER_TABLE(first));


    ipc::Client client("test");
    ipc::Client client2("test");


    INVOKE_REMOTE(&client, first, Inv1, 100);
    ipc::InvokeRemote<first::Inv1>(&client, 17);
    int tz = ipc::InvokeRemote<first::Inv3>(&client, 17, 19.f);
    ipc::InvokeRemote<first::Inv0>(&client);

    auto sd = ipc::InvokeRemote<first::Inv4>(&client, (uintptr_t)&sss, "aaa");



    server.Cancel();




    return 0;
}