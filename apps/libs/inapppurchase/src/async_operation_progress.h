#pragma once

#include <functional>
#include <windows.foundation.h>
#include <wrl.h>
#include "sync.h"

using namespace Microsoft::WRL;

template<typename T, typename R, typename P, typename S = EventSync>
class AsyncOperationProgress
{
public:
    typedef typename ABI::Windows::Foundation::IAsyncOperationWithProgress<T,P>          TAsyncOperationProgress;
    typedef typename ABI::Windows::Foundation::IAsyncOperationWithProgressCompletedHandler<T,P>      TAsyncOperationProgressCompletedHandler;
    typedef typename ABI::Windows::Foundation::IAsyncOperationProgressHandler<T, P>       TAsyncOperationProgressHandler;
    typedef typename ABI::Windows::Foundation::IAsyncInfo                               TAsyncInfo;
    typedef typename ABI::Windows::Foundation::AsyncStatus                              AsyncStatus;

    typedef typename std::remove_pointer<R>::type                                       TResultType;

    typedef std::function<HRESULT(R)>                                     FCallback;
    typedef std::function<HRESULT(P)>                                     FProgress;

public:
    //AsyncOperation(const FCallback &delegate = [](U*, AsyncStatus) {return S_OK; })
    AsyncOperationProgress(const FCallback &delegate = FCallback(), const FProgress& progress = FProgress())
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

    AsyncOperationProgress(AsyncOperationProgress const&) = delete;
    AsyncOperationProgress& operator=(AsyncOperationProgress const&) = delete;

    ~AsyncOperationProgress() {
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

    HRESULT         Start(TAsyncOperationProgress* async, const FCallback &delegate = FCallback(), const FProgress& progress = FProgress()) {
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
            progress_ = progress;

            if (FAILED(hr = operation_.As(&info)) ||
                FAILED(hr = info->get_Status(&status_)) ||
                FAILED(hr = info->get_ErrorCode(&error_code_)))
                return hr;

            //auto callback = Callback<IAsyncOperationCompletedHandler<TResultPtr>>(this, &AsyncOperation::OnCompleted);
            auto callback = Callback<TAsyncOperationProgressCompletedHandler>(this, &AsyncOperationProgress::OnCompleted);
            hr = operation_->put_Completed(callback.Get());

            if (SUCCEEDED(hr)) {
                auto callback = Callback<TAsyncOperationProgressHandler>(this, &AsyncOperationProgress::OnProgress);
                hr = operation_->put_Progress(callback.Get());
            }

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
    HRESULT         OnCompleted(TAsyncOperationProgress* args, AsyncStatus status) {
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

    HRESULT         OnProgress(TAsyncOperationProgress* args, P progress) {
        HRESULT hr = S_OK;
        
        if (progress_)
            hr = progress_(progress);

        return hr;
    }


private:
    AsyncStatus                                     status_;
    HRESULT                                         error_code_;
    Microsoft::WRL::ComPtr<TAsyncOperationProgress> operation_;
    Microsoft::WRL::ComPtr<TResultType>             result_;
       
    FCallback                                       delegate_;
    FProgress                                       progress_;

    S                                               sync_;
};

#pragma once
