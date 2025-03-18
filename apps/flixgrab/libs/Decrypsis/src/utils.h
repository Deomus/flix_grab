#pragma once

#include <windows.h>

template<class C, typename... Args>
typename C::ReturnType CallOriginal(C& handler, Args... args)
{
    return handler.CallOriginal(std::forward<Args>(args)...);
}



static HMODULE GetCurrentModule()
{ // NB: XP+ solution!
    HMODULE hModule = NULL;
    ::GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCTSTR)GetCurrentModule,
        &hModule);

    return hModule;
}

#define STRINGIFY(x)   #x
