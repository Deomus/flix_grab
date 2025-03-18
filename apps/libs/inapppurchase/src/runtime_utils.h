#pragma once

#include <QtCore>

//#include <qfunctions_winrt.h>
#include <functional>

#include <Windows.Services.Store.h>
#include <windows.foundation.h>
#include <shobjidl.h>
#include <wrl.h>
#include "collection.h"
#include "in_app_status.h"
//#include "collection/collection_winrt.h"

using namespace ABI::Windows::Services::Store;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;


using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;



inline QString hStringToQString(const HString &h)
{
    unsigned int length;
    const wchar_t* raw = h.GetRawBuffer(&length);
    return QString::fromWCharArray(raw, length);
}

template<typename T, typename U>
HRESULT         GetAsyncResults(IAsyncOperation<T*> *op, HRESULT* extended, U** results) {
    ComPtr<IAsyncInfo> info;
    ComPtr<IAsyncOperation<T*>> async_op(op);
    AsyncStatus     status;
    HRESULT hr = async_op.As(&info);

    if (SUCCEEDED(hr) &&
        SUCCEEDED(hr = info->get_ErrorCode(extended)) &&
        SUCCEEDED(hr = info->get_Status(&status))) {

        switch (status) {
        case AsyncStatus::Completed:
            hr = async_op->GetResults(results);
            break;
        case AsyncStatus::Error:
            //Nothing to do;
            break;
        case AsyncStatus::Canceled:
            *extended = ERROR_CANCELLED;
            break;
        default:
        case AsyncStatus::Started:
            *extended = E_UNEXPECTED;
            break;
        }
    }
    return hr;
}

static QString windowsErrorString(HRESULT errorCode)
{
    QString ret;
    wchar_t errorString[1024] = {0};
    DWORD sz = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&errorString,
        sizeof(errorString) / sizeof(wchar_t),
        NULL);
    ret = QString::fromWCharArray(errorString);

    if (ret.isEmpty() && errorCode == ERROR_MOD_NOT_FOUND)
        ret = QString::fromLatin1("The specified module could not be found.");
    return ret;
}

static  InAppStatus     makeStatus(ABI::Windows::Services::Store::StorePurchaseStatus status) {
    InAppStatus     ret;

    switch (status) {
    case StorePurchaseStatus_Succeeded:
        ret = InAppStatus(InAppStatus::Succeeded,
            "The purchase request succeeded.");
        break;
    case StorePurchaseStatus_AlreadyPurchased:
        ret = InAppStatus(InAppStatus::AlreadyPurchased,
            "The product has already purchased.");
        break;
    case StorePurchaseStatus_NotPurchased:
        ret = InAppStatus(InAppStatus::NotPurchased,
            "The purchase request did not succeed.");
        break;
    case StorePurchaseStatus_NetworkError:
        ret = InAppStatus(InAppStatus::NetworkError,
            "The purchase request did not succeed because of a network connectivity error.");
        break;
    case StorePurchaseStatus_ServerError:
        ret = InAppStatus(InAppStatus::ServerError,
            "The purchase request did not succeed because of a server error returned by the Microsoft Store.");
        break;

    }
    return ret;
}

static  InAppStatus     makeStatus(ABI::Windows::Services::Store::StorePackageUpdateState update_state) {
    InAppStatus     ret;
    /*StorePackageUpdateState_Pending = 0,
        StorePackageUpdateState_Downloading = 1,
        StorePackageUpdateState_Deploying = 2,
        StorePackageUpdateState_Completed = 3,
        StorePackageUpdateState_Canceled = 4,
        StorePackageUpdateState_OtherError = 5,
        StorePackageUpdateState_ErrorLowBattery = 6,
        StorePackageUpdateState_ErrorWiFiRecommended = 7,
        StorePackageUpdateState_ErrorWiFiRequired = 8,*/
    switch (update_state) {
    case StorePackageUpdateState_Completed:
        ret = InAppStatus(InAppStatus::Succeeded,
            "The package updates have finished downloading or installing.");
        break;
    case StorePackageUpdateState_Downloading:
        ret = InAppStatus(InAppStatus::InternalError,
            "The package updates are being downloaded.");
        break;
    case StorePackageUpdateState_Deploying:
        ret = InAppStatus(InAppStatus::InternalError,
            "The package updates are being deployed to the device.");
        break;
    case StorePackageUpdateState_Canceled:
        ret = InAppStatus(InAppStatus::Aborted,
            "The download or installation of the package updates was canceled.");
        break;
    case StorePackageUpdateState_OtherError:
        ret = InAppStatus(InAppStatus::OtherError,
            "An unknown error occurred.");
        break;
    case StorePackageUpdateState_ErrorLowBattery:
        ret = InAppStatus(InAppStatus::OtherError,
            "The download or installation of the package updates did not succeed because the device does not have enough battery power.");
        break;
    case StorePackageUpdateState_ErrorWiFiRecommended:
        ret = InAppStatus(InAppStatus::OtherError,
            "The download did not succeed because a Wi-Fi connection is recommended to download the package updates.");
        break;
    case StorePackageUpdateState_ErrorWiFiRequired:
        ret = InAppStatus(InAppStatus::OtherError,
            "The download did not succeed because a Wi-Fi connection is required to download the package updates.");
        break;
    case StorePackageUpdateState_Pending:
        ret = InAppStatus(InAppStatus::OtherError,
            "The download of the package updates has not started.");
        break;

    }
    return ret;
}


static  InAppStatus     makeStatus(HRESULT hr) {
    InAppStatus     ret;

    if (FAILED(hr))
        ret = InAppStatus(InAppStatus::InternalError, windowsErrorString(hr));

    return ret;
}


static  InAppStatus     makeStatus(HRESULT hr, const QString& msg) {
    InAppStatus     ret;

    if (FAILED(hr))
        ret = InAppStatus(InAppStatus::InternalError, msg);

    return ret;
}



inline  InAppStatus     parseJsonReviewResponse(const HString& response_string) {

    InAppStatus review_response;
    QString response = hStringToQString(response_string);

    QJsonDocument response_json = QJsonDocument::fromJson(response.toLocal8Bit());
    QJsonObject response_obj = response_json.object();
    QString response_status = response_obj.value("status").toString();

    if (response_status == "aborted")
        review_response = InAppStatus(InAppStatus::Aborted,
            response_obj.value("errorDetails").toString());
    else if (response_obj.contains("errorDetails"))
        review_response = InAppStatus(InAppStatus::Succeeded,
            response_obj.value("errorDetails").toString());

    return review_response;

}


QDateTime   convertDateTime(DateTime dt) {
    ULARGE_INTEGER  ul;
    FILETIME filetime;
    SYSTEMTIME systemtime;

    ul.QuadPart = dt.UniversalTime;
    filetime.dwHighDateTime = ul.HighPart;
    filetime.dwLowDateTime = ul.LowPart;

    if (FileTimeToSystemTime(&filetime, &systemtime)) {
        QDate date(systemtime.wYear, systemtime.wMonth, systemtime.wDay);
        QTime time(systemtime.wHour, systemtime.wMinute, systemtime.wSecond);
        
        return QDateTime(date, time);
    }
    return QDateTime();
}


//
//static InAppStatus        purchaseSync(IStoreProduct* product) {
//
//    if (product == nullptr)
//        return InAppStatus(InAppStatus::InternalError);
//
//    HRESULT hr;
//    ComPtr<IAsyncOperation<StorePurchaseResult*>> purchase_op;
//    AsyncOperation<StorePurchaseResult, IStorePurchaseResult>   async;
//
//    InAppStatus status;
//    StorePurchaseStatus purchase_status;
//
//    if (SUCCEEDED(hr = product->RequestPurchaseAsync(&purchase_op))) {
//        auto callback = [&](IStorePurchaseResult* result) {
//
//            HRESULT extended = S_OK;
//            HRESULT hr = result->get_ExtendedError(&extended);
//
//            if (SUCCEEDED(hr))
//                hr = result->get_Status(&purchase_status);
//
//            return SUCCEEDED(hr) ? extended : hr;
//        };
//
//        if (SUCCEEDED(hr = async.Start(*&purchase_op, callback)))
//            hr = async.Wait();
//    }
//
//    if (FAILED(hr))
//        status = makeStatus(hr);
//    else if (FAILED(async.Error()))
//        status = makeStatus(async.Error());
//    else
//        status = makeStatus(purchase_status);
//
//    qDebug() << "Purchase status: " << status.isOk() << " message:" << status.message();
//
//    //emit q_ptr->purchasingStatus(status);
//    return status;
//}

