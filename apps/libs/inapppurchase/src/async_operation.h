#pragma once

#include <functional>
#include <windows.foundation.h>
#include <wrl.h>

#include "runtime_utils.h"
#include "sync.h"


using namespace Microsoft::WRL;

template<typename T, typename R, typename S = EventSync>
class AsyncOperation
{
public:
    typedef typename ABI::Windows::Foundation::IAsyncOperation<T>                      TAsyncOperation;
    typedef typename ABI::Windows::Foundation::IAsyncOperationCompletedHandler<T>      TAsyncOperationCompletedHandler;
    typedef typename ABI::Windows::Foundation::IAsyncInfo                               TAsyncInfo;
    typedef typename ABI::Windows::Foundation::AsyncStatus                              AsyncStatus;

    typedef typename std::remove_pointer<R>::type                                       TResultType;

    typedef std::function<HRESULT(R)>                                     FCallback;

public:
    //AsyncOperation(const FCallback &delegate = [](U*, AsyncStatus) {return S_OK; })
    AsyncOperation(const FCallback &delegate = FCallback())
        : delegate_(delegate)
        //, event_(CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET | CREATE_EVENT_INITIAL_SET, WRITE_OWNER | EVENT_ALL_ACCESS))
        , status_(AsyncStatus::Canceled)
        , error_code_(S_OK)
    {
    }

    /* AsyncOperation():AsyncOperation([](TResultPtr, AsyncStatus) {return S_OK; })
    {
    }
    */

    AsyncOperation(AsyncOperation const&) = delete;
    AsyncOperation& operator=(AsyncOperation const&) = delete;

    ~AsyncOperation() {
        if (SUCCEEDED(Cancel()))
            Wait();
    }

    bool            IsProcessing() const {
        return (status_ == Started);
    }

    bool            IsCompleted() const {
        return (status_ == Completed);
    }

    bool            IsCanceled() const {
        return (status_ == Canceled);
        /*AsyncStatus     status;
        return  (info_ &&
        SUCCEEDED(info_->get_Status(&status)) &&
        status == Canceled);*/

    }

    bool            IsOk() const {
        return SUCCEEDED(error_code_);
    }

    HRESULT         Start(TAsyncOperation* async, const FCallback &delegate = FCallback()) {
        HRESULT hr = S_OK;

        if (operation_ &&
            (FAILED(hr = Cancel()) || FAILED(hr = Wait())))
            return hr;

        operation_.Reset();
        result_.Reset();
        //Reset Awaiting event;
        //::ResetEvent(event_.Get());
        sync_.Reset();

        if (async) {
            ComPtr<TAsyncInfo>              info;
            operation_ = async;
            delegate_ = delegate;

            if (FAILED(hr = operation_.As(&info)) ||
                FAILED(hr = info->get_Status(&status_)) ||
                FAILED(hr = info->get_ErrorCode(&error_code_)))
                return hr;

            //auto callback = Callback<IAsyncOperationCompletedHandler<TResultPtr>>(this, &AsyncOperation::OnCompleted);
            auto callback = Callback<TAsyncOperationCompletedHandler>(this, &AsyncOperation::OnCompleted);
            hr = operation_->put_Completed(callback.Get());
        }
        return hr;
    }
    template<typename U>
    HRESULT         GetResults(U* value) {
        HRESULT hr = E_INVALIDARG;

        if (value == nullptr)
            return E_INVALIDARG;

        if (status_ == AsyncStatus::Started &&
            FAILED(hr = Wait()))
            return hr;

        switch (status_) {
        case AsyncStatus::Started:
            hr = ERROR_INTERNAL_ERROR;
            break;

        case AsyncStatus::Completed:
            hr = result_.As(value);
            break;
        case AsyncStatus::Canceled:
            hr = ERROR_CANCELLED;
            break;
        case AsyncStatus::Error:
            hr = error_code_;
            break;
        }
        return hr;
    }

    HRESULT         Cancel() {
        HRESULT hr = E_INVALIDARG;
        Microsoft::WRL::ComPtr<TAsyncInfo>              info;
        if (operation_ &&
            SUCCEEDED(hr = operation_.As(&info))) {
            hr = info->Cancel();
        }
        return hr;
    }

    bool         Wait(DWORD timeout = INFINITE) {
        return sync_.Wait();

    //    HRESULT hr = S_OK;
    ////    if (status_ == AsyncStatus::Started) {

    //        switch (WaitForSingleObject(event_.Get(), timeout)) {
    //        case WAIT_OBJECT_0:
    //            hr = S_OK;
    //            break;
    //        case WAIT_TIMEOUT:
    //            hr = ERROR_TIMEOUT;
    //            break;

    //        default:
    //            hr = GetLastError();
    //        }
    //        //event_loop_.exec();
    //        //hr = S_OK;
    // //   }
    //    return hr;
    }

    HRESULT         Error() const {
        return error_code_;
    }

private:
    HRESULT         OnCompleted(TAsyncOperation* args, AsyncStatus status) {
        HRESULT hr = S_OK;
        ComPtr<TAsyncInfo>              info;

        status_ = status;

        if (SUCCEEDED(hr = operation_.As(&info))) {
            switch (status) {
            case AsyncStatus::Completed:
                hr = args->GetResults(&result_);
                if (SUCCEEDED(hr) && delegate_)
                    error_code_ = delegate_(result_.Get());
                break;
            case AsyncStatus::Error:
                hr = info->get_ErrorCode(&error_code_);
#if defined (Q_OS_WIN)
                qDebug() << "Async operation error" << windowsErrorString(error_code_);
#endif
                break;
            default:
                break;
            }
            if (SUCCEEDED(hr))
                hr = info->Close();
        }
        sync_.Set();
        //::SetEvent(event_.Get());
        //event_loop_.quit();
        //Drop Async Operation;
        operation_.Reset();
        return hr;
    }


private:
    AsyncStatus                                     status_;
    HRESULT                                         error_code_;
    Microsoft::WRL::ComPtr<TAsyncOperation>         operation_;
    Microsoft::WRL::ComPtr<TResultType>             result_;

    //Microsoft::WRL::Wrappers::Event                 event_;

    FCallback                                       delegate_;

    S                                               sync_;
};

