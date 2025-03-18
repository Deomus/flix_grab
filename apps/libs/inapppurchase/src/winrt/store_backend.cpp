
#include <QLoggingCategory>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QCoreApplication>

#include <qfunctions_winrt.h>
#include <functional>

#include <Windows.Services.Store.h>

#include <Windows.ApplicationModel.store.h>
#include <Windows.Applicationmodel.Activation.h>

#include <windows.foundation.h>
#include <windows.system.threading.h>
#include <windows.ui.core.h>
#include <windows.applicationmodel.core.h>
#include <shobjidl.h>

#include <wrl.h>

using namespace ABI::Windows::Services::Store;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

using namespace ABI::Windows::System::Threading;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Windows::ApplicationModel::Core;

//typedef IAsyncOperationCompletedHandler<ListingInformation *> ListingInformationHandler;



class StoreBridge {
public:
    StoreBridge();


    HRESULT     activate(HWND hwdlg);
    HRESULT     add_LicensesChangedEvent(ComPtr<ITypedEventHandler<StoreContext*, IInspectable*>> &delegate, EventRegistrationToken& token) {
        HRESULT hr;
        /*auto licenses_changed_handler = Callback<ITypedEventHandler<StoreContext*, IInspectable*>>
            ([&](IStoreContext* context, IInspectable* object)->HRESULT
        {
            return S_OK;
        });*/
        hr = context_->add_OfflineLicensesChanged(delegate.Get(), &token);
        return hr;
    }

    HRESULT     remove_LicensesChangedEvent(const EventRegistrationToken& token) {
        return context_->remove_OfflineLicensesChanged(token);
    }


   /* HRESULT LoadListingInformationAsync(ComPtr<IAsyncOperation<ListingInformation *>> &op);
    HRESULT GetAppReceiptAsync(ComPtr<IAsyncOperation<HSTRING>> &op);
    HRESULT GetProductReceiptAsync(HSTRING product, ComPtr<IAsyncOperation<HSTRING>> &op);
    HRESULT RequestAppPurchaseAsync(bool receipt, ComPtr<IAsyncOperation<HSTRING>> &op);
    HRESULT RequestProductPurchaseAsync(HSTRING productId, bool receipt, ComPtr<IAsyncOperation<HSTRING>> &op);
    HRESULT RequestProductPurchaseWithResultsAsync(HSTRING productId, ComPtr<IAsyncOperation<PurchaseResults *>> &purchaseOp);
    HRESULT ReportConsumableFulfillmentAsync(HSTRING productId, GUID transactionId, ComPtr<IAsyncOperation<FulfillmentResult>> &op);
    HRESULT get_LicenseInformation(ComPtr<ILicenseInformation> &licenseInfo);*/
private:
        
    
    ComPtr<IStoreContext>       context_;

    RoInitializeWrapper         roinit_;

    EventRegistrationToken      event_token_;
    
};

StoreBridge::StoreBridge()
    :roinit_(RO_INIT_MULTITHREADED)
{
    Q_ASSERT_SUCCEEDED(roinit_);
}

HRESULT StoreBridge::activate(HWND hwdlg)
{    
    //Q_ASSERT(::IsWindow(hwdlg)); // e.g the window showing "checking license..."
       
    HRESULT hr = roinit_;
    // if this blows, you have initialized COM in an incompatible appartment mode
    // so better create a separate temp thread just for the winRT stuff here
    if (!SUCCEEDED(hr))
        return hr;
        
    ComPtr<IStoreContextStatics> storeContextStatics;
    //hr = GetActivationFactory(HStringReference(L"Windows.Services.Store.StoreContext").Get(), &storeContextStatics);
    
    hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Services_Store_StoreContext).Get(), &storeContextStatics);
    Q_ASSERT_SUCCEEDED(hr);
    hr = storeContextStatics->GetDefault(&context_);

    Q_ASSERT_SUCCEEDED(hr);
    // this is a requirement for desktop converted apps, dunno why
    // https://msdn.microsoft.com/windows/uwp/monetize/in-app-purchases-and-trials#desktop
    ComPtr<IInitializeWithWindow> init_window;
    hr = context_->QueryInterface(IID_PPV_ARGS(&init_window));
    Q_ASSERT_SUCCEEDED(hr);
    hr = init_window->Initialize(hwdlg);
    Q_ASSERT_SUCCEEDED(hr);

    //////////////////////////////////////////////////////////////////////////

    /*auto licenses_changed_handler = Callback<ITypedEventHandler<StoreContext*, IInspectable*>>
        ([&](IStoreContext* context, IInspectable* object)->HRESULT
    {
        return S_OK;
    });
    hr = context_->add_OfflineLicensesChanged(licenses_changed_handler.Get(), &this->event_token_);*/



    return hr;
}
//
//HRESULT QWinRTAppBridge::LoadListingInformationAsync(ComPtr<IAsyncOperation<ListingInformation *> > &op)
//{
//    HRESULT hr;
//    if (m_simulate)
//        hr = m_simulator->LoadListingInformationAsync(&op);
//    else
//        hr = m_app->LoadListingInformationAsync(&op);
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::GetAppReceiptAsync(ComPtr<IAsyncOperation<HSTRING> > &op)
//{
//    HRESULT hr;
//    if (m_simulate)
//        hr = m_simulator->GetAppReceiptAsync(&op);
//    else
//        hr = m_app->GetAppReceiptAsync(&op);
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::GetProductReceiptAsync(HSTRING product, ComPtr<IAsyncOperation<HSTRING> > &op)
//{
//    HRESULT hr;
//    if (m_simulate)
//        hr = m_simulator->GetProductReceiptAsync(product, &op);
//    else
//        hr = m_app->GetProductReceiptAsync(product, &op);
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::RequestAppPurchaseAsync(bool receipt, ComPtr<IAsyncOperation<HSTRING> > &op)
//{
//    HRESULT hr;
//    if (m_simulate)
//        hr = m_simulator->RequestAppPurchaseAsync(receipt, op.GetAddressOf());
//    else
//        hr = m_app->RequestAppPurchaseAsync(receipt, op.GetAddressOf());
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::RequestProductPurchaseAsync(HSTRING productId, bool receipt, ComPtr<IAsyncOperation<HSTRING> > &op)
//{
//    HRESULT hr;
//    if (m_simulate)
//        hr = m_simulator->RequestProductPurchaseAsync(productId, receipt, op.GetAddressOf());
//    else
//        hr = m_app->RequestProductPurchaseAsync(productId, receipt, op.GetAddressOf());
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::RequestProductPurchaseWithResultsAsync(HSTRING productId, ComPtr<IAsyncOperation<PurchaseResults *> > &purchaseOp)
//{
//    HRESULT hr;
//    if (m_simulate) {
//        ComPtr<ICurrentAppSimulatorWithConsumables> consumApp;
//        hr = m_simulator.As(&consumApp);
//        Q_ASSERT_SUCCEEDED(hr);
//        hr = consumApp->RequestProductPurchaseWithResultsAsync(productId, purchaseOp.GetAddressOf());
//    }
//    else {
//        ComPtr<ICurrentAppWithConsumables> consumApp;
//        hr = m_app.As(&consumApp);
//        Q_ASSERT_SUCCEEDED(hr);
//        hr = consumApp->RequestProductPurchaseWithResultsAsync(productId, purchaseOp.GetAddressOf());
//    }
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::ReportConsumableFulfillmentAsync(HSTRING productId, GUID transactionId, ComPtr<IAsyncOperation<FulfillmentResult> > &op)
//{
//    HRESULT hr;
//    if (m_simulate) {
//        ComPtr<ICurrentAppSimulatorWithConsumables> consumApp;
//        hr = m_simulator.As(&consumApp);
//        Q_ASSERT_SUCCEEDED(hr);
//        hr = consumApp->ReportConsumableFulfillmentAsync(productId, transactionId, op.GetAddressOf());
//    }
//    else {
//        ComPtr<ICurrentAppWithConsumables> consumApp;
//        hr = m_app.As(&consumApp);
//        Q_ASSERT_SUCCEEDED(hr);
//        hr = consumApp->ReportConsumableFulfillmentAsync(productId, transactionId, op.GetAddressOf());
//    }
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::get_LicenseInformation(ComPtr<ILicenseInformation> &licenseInfo)
//{
//    HRESULT hr;
//    if (m_simulate) {
//        hr = m_simulator->get_LicenseInformation(&licenseInfo);
//    }
//    else {
//        hr = m_app->get_LicenseInformation(&licenseInfo);
//    }
//    return hr;
//}
//
//HRESULT QWinRTAppBridge::qt_winrt_load_simulator_config(const QString &fileName, ComPtr<ICurrentAppSimulator> &simulator)
//{
//    qCDebug(lcPurchasingBackend) << __FUNCTION__ << fileName;
//
//    HRESULT hr;
//    const QString nativeFilename = QDir::toNativeSeparators(fileName);
//    HString nativeName;
//    hr = nativeName.Set(reinterpret_cast<PCWSTR>(nativeFilename.utf16()), nativeFilename.size());
//    Q_ASSERT_SUCCEEDED(hr);
//
//    ComPtr<IStorageFileStatics> fileStatics;
//    hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Storage_StorageFile).Get(), &fileStatics);
//    Q_ASSERT_SUCCEEDED(hr);
//
//    ComPtr<IAsyncOperation<StorageFile*>> op;
//    hr = fileStatics->GetFileFromPathAsync(nativeName.Get(), &op);
//    Q_ASSERT_SUCCEEDED(hr);
//
//    ComPtr<IStorageFile> storeFile;
//    hr = QWinRTFunctions::await(op, storeFile.GetAddressOf());
//    RETURN_HR_IF_FAILED("Could not find purchasing simulator xml description.");
//
//    ComPtr<ABI::Windows::Foundation::IAsyncAction> reloadAction;
//    hr = simulator->ReloadSimulatorAsync(storeFile.Get(), &reloadAction);
//    Q_ASSERT_SUCCEEDED(hr);
//
//    hr = QWinRTFunctions::await(reloadAction, QWinRTFunctions::YieldThread);
//    RETURN_HR_IF_FAILED("Failed to load purchasing description.");
//
//    return hr;
//}