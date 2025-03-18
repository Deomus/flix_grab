//
// Copyright (c) 2014 FreeGrabApp. All rights reserved.
//

#include "precomp.h"

#include <list>
#include <string>

#ifdef _WIN32
#include "wmihelper.h"
#endif

#include "licensing.h"
#include "uniconv.h"
#include "except.h"
#include "hwid.h"

class KeyHelperImpl {

friend class KeyHelperT<char>;
friend class KeyHelperT<wchar_t>;

private:
    static const char_t * GetCurrentHardwareId()
    {
        return HardwareId::GetCurrentHardwareId();
    }

    static bool MatchCurrentHardwareId(const char_t * hwid)
    {
        return HardwareId::MatchCurrentHardwareId(hwid);
    }

    static bool DetectClockManipulation(int year, int month, int day)
    {
#ifdef _WIN32
        FILETIME thresholdTime;
        SYSTEMTIME st;
        TCHAR path[MAX_PATH];

        ZeroMemory(&st, sizeof(st));

        st.wYear = year;
        st.wMonth = month;
        st.wDay = day;
        st.wHour = 23;
        st.wMinute = 59;
        st.wSecond = 59;

        if (!SystemTimeToFileTime(&st, &thresholdTime))
            return false;

        if (GetTempPath(MAX_PATH, path) > 0)
        {
            _tcsncat_s<MAX_PATH>(path, _T("*.*"), _TRUNCATE);

            WIN32_FIND_DATA findData;
            
            HANDLE findHandle = FindFirstFile(path, &findData);
            if (findHandle != INVALID_HANDLE_VALUE)
            {
                while (FindNextFile(findHandle, &findData))
                {
                    if (CompareFileTime(&findData.ftLastWriteTime, &thresholdTime) > 0)
                        return true;
                }

                FindClose(findHandle);
            }
        }

        WmiHelper wmi;
        list<string> propList;

        char dmtfTime[100];
        char query[MAX_PATH];

        _snprintf_s(query, MAX_PATH, _TRUNCATE, "SELECT EventIdentifier FROM Win32_NTLogEvent WHERE LogFile='system' AND TimeGenerated > '%04d%02d%02d000000.000000-000'", st.wYear, st.wMonth, st.wDay);

        wmi.GetPropertyList(query, "EventIdentifier", &propList);

        if (propList.size() > 0)
            return true;
#endif

        return false;
    }
};

namespace FreeGrabApp
{
    namespace Licensing
    {
        template<>
        const char * KeyHelperT<char>::GetCurrentHardwareId()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string hwid = W2A(KeyHelperImpl::GetCurrentHardwareId());
            return hwid.c_str();
        #else
            return KeyHelperImpl::GetCurrentHardwareId();
        #endif
        }

        template<>
        const wchar_t * KeyHelperT<wchar_t>::GetCurrentHardwareId()
        {
        #ifdef _UNICODE
            return KeyHelperImpl::GetCurrentHardwareId();
        #else
            USES_CONVERSION;
            static std::wstring hwid = A2W(KeyHelperImpl::GetCurrentHardwareId());
            return hwid.c_str();
        #endif
        }

        template<>
        bool KeyHelperT<char>::MatchCurrentHardwareId(const char * hwid)
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            return KeyHelperImpl::MatchCurrentHardwareId(A2W(hwid));
        #else
            return KeyHelperImpl::MatchCurrentHardwareId(hwid);
        #endif
        }

        template<>
        bool KeyHelperT<wchar_t>::MatchCurrentHardwareId(const wchar_t * hwid)
        {
        #ifdef _UNICODE
            return KeyHelperImpl::MatchCurrentHardwareId(hwid);
        #else
            USES_CONVERSION;
            return KeyHelperImpl::MatchCurrentHardwareId(W2A(hwid));
        #endif
        }

        template<>
        bool KeyHelperT<char>::DetectClockManipulation(int thresholdYear, int thresholdMonth, int thresholdDay)
        {
            return KeyHelperImpl::DetectClockManipulation(thresholdYear, thresholdMonth, thresholdDay);
        }

        template<>
        bool KeyHelperT<wchar_t>::DetectClockManipulation(int thresholdYear, int thresholdMonth, int thresholdDay)
        {
            return KeyHelperImpl::DetectClockManipulation(thresholdYear, thresholdMonth, thresholdDay);
        }
    };
};
