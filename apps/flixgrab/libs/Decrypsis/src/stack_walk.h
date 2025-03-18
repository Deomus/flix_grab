#pragma once
//StackWalk.h
//Credits: MSDN

#ifndef __STACKWALK_H__
#define __STACKWALK_H__

#include <windows.h>
#include <tchar.h>
#include <list>
#include <tlhelp32.h>
#include <mutex>

class StackWalk
{
private:
    class Module
    {
    public:
        DWORD adress;
        DWORD size;
        TCHAR name[256];

        bool IsIn(DWORD address)
        {
            return address >= this->adress && address <= this->adress + size;
        }
    };
    //---------------------------------------------------------------------------
    //void* stack[63];
    //int stackSize;
    std::list<Module> moduleList;
    std::mutex		lock_;

    //---------------------------------------------------------------------------
public:
    void GetModuleList()
    {
        MODULEENTRY32 module32;
        module32.dwSize = sizeof(MODULEENTRY32);

        HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
        if (snapShot != INVALID_HANDLE_VALUE)
        {
            if (Module32First(snapShot, &module32))
            {
                std::unique_lock<std::mutex> lock(lock_);
                moduleList.clear();
                do
                {
                    Module module;
                    module.adress = (DWORD)module32.modBaseAddr;
                    module.size = module32.modBaseSize;
                    _tcscpy_s(module.name, module32.szModule);

                    moduleList.push_back(module);

                } while (Module32Next(snapShot, &module32));
            }
            CloseHandle(snapShot);
        }
    }
    //---------------------------------------------------------------------------
public:
    class StackFrame
    {
    private:
        DWORD address;
        DWORD offset;
        std::wstring moduleName;

    public:
        StackFrame(DWORD address, DWORD offset, TCHAR* moduleName):
            moduleName(moduleName)
        {
            this->address = address;
            this->offset = offset;
            //_tcscpy(this->moduleName, moduleName);
        }
        //---------------------------------------------------------------------------
        DWORD GetAddress() const
        {
            return address;
        }
        //---------------------------------------------------------------------------
        DWORD GetOffset() const
        {
            return offset;
        }
        //---------------------------------------------------------------------------
        const std::wstring GetModuleName() const
        {
            return moduleName;
        }
        //---------------------------------------------------------------------------
    };
    //---------------------------------------------------------------------------

    std::list<StackFrame> GetCallStack(void* stack[63], int stackSize)
    {
        std::list<StackFrame> frames;

        //stackSize = RtlCaptureStackBackTrace(1, 63, stack, 0);

        for (int i = 0; i < max(stackSize, 20); ++i)
        {
            DWORD address = (DWORD)stack[i];
            for (int j = 0; j < 2; j++) {
                Module module;
                {
                    std::unique_lock<std::mutex> lock(lock_);

                    for (std::list<Module>::iterator it = moduleList.begin(); it != moduleList.end(); it++)
                    {
                        if (it->IsIn(address))
                        {
                            module = *it;
                            //frames.push_back(StackFrame(address - 5, address - module.adress - 5, module.name));
                            break;
                        }
                    }
                }
                if (module.IsIn(address))
                {
                    frames.push_back(StackFrame(address - 5, address - module.adress - 5, module.name));
                    break;
                }
                GetModuleList();
                // load modules and do second attempt
            }
        }
        return frames;
    }
    //---------------------------------------------------------------------------
};
//---------------------------------------------------------------------------

#endif