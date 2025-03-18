#pragma once

#include <wrl.h>

class EventSync {
public:
    EventSync()
        : event_(CreateEventEx(nullptr, nullptr,
            CREATE_EVENT_MANUAL_RESET | CREATE_EVENT_INITIAL_SET,
            WRITE_OWNER | EVENT_ALL_ACCESS))
    {}

    void        Reset() {
        //Reset Awaiting event;
        ::ResetEvent(event_.Get());
    }

    void        Set() {
        ::SetEvent(event_.Get());
    }

    bool        Wait(int timeout = INFINITE) {
        
        DWORD result = WaitForSingleObject(event_.Get(), timeout);
        return (result == WAIT_OBJECT_0);
/*

        switch (WaitForSingleObject(event_.Get(), INFINITY)) {
        case WAIT_OBJECT_0:
            hr = S_OK;
            break;
        case WAIT_TIMEOUT:
            hr = ERROR_TIMEOUT;
            break;

        default:
            hr = GetLastError();
        }
        return hr;*/
    }
private:
    Microsoft::WRL::Wrappers::Event           event_;
};