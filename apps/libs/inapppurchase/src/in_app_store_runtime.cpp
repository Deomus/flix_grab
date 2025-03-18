#include "in_app_store_runtime.h"

#include "async_operation.h"
#include "async_operation_progress.h"

#include <qfunctions_winrt.h>
#include <functional>
#include <map>

#include <Windows.Services.Store.h>
#include <windows.foundation.h>
#include <shobjidl.h>
#include <wrl.h>
#include "collection.h"
#include "runtime_utils.h"
#include "in_app_product_runtime.h"
#include "in_app_license_runtime.h"

#pragma comment(lib,"windowsapp.lib")

using namespace ABI::Windows::Services::Store;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;


using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;

const static QString  application_identifier = "Application";

class InAppStoreRuntimePrivate {
public:
    InAppStoreRuntimePrivate(InAppStoreRuntime* p)
        : q_ptr(p)
        , roinit_(RO_INIT_MULTITHREADED)

    {
        Q_ASSERT_SUCCEEDED(roinit_);
        qRegisterMetaType<InAppStatus>();
    }

    ~InAppStoreRuntimePrivate() {
    }
    
    bool         initialize() {
        
        HRESULT hr = roinit_;
        ComPtr<IStoreContextStatics> store_statics;
        // if this blows, you have initialized COM in an incompatible appartment mode
        // so better create a separate temp thread just for the winRT stuff here

        RETURN_FALSE_IF_FAILED("COM initialized in an incompatible appartment mode.");
        hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Services_Store_StoreContext).Get(), &store_statics);
        RETURN_FALSE_IF_FAILED("Can't activate RuntimeClass_Windows_Services_Store_StoreContext.");
        hr = store_statics->GetDefault(&context_);
        RETURN_FALSE_IF_FAILED("Can't get Default a StoreContext.");

        // this is a requirement for desktop converted apps, dunno why
        // https://msdn.microsoft.com/windows/uwp/monetize/in-app-purchases-and-trials#desktop
        //ComPtr<IInitializeWithWindow> init_window;
        //hr = context_.As(&init_window);
        ////hr = context_->QueryInterface(IID_PPV_ARGS(&init_window));
        //RETURN_FALSE_IF_FAILED("Can't QueryInterface IInitializeWithWindow.");
        //hr = init_window->Initialize(hwdlg);


        //RETURN_FALSE_IF_FAILED_WITH_ARGS("Can't Initialize StoreContext with HWND ", hwdlg);
        hr = addLicensesChangedEvent();

        RETURN_FALSE_IF_FAILED("Registration license event error.");

//        hr = getLicensesAndProducts();

        hr = requestLicense();
        RETURN_FALSE_IF_FAILED("Request GetAppLicenseAsync error.");
        hr = requestProduct();
        RETURN_FALSE_IF_FAILED("Request GetStoreProductForCurrentAppAsync error.");

        hr = requestAddonProducts();
        RETURN_FALSE_IF_FAILED("Request StoreProduct error.");

        hr = requestAppUpdates();
        RETURN_FALSE_IF_FAILED("Request AppUpdates error.");
       
        return SUCCEEDED(hr);
    }

    bool         initializeUi(HWND hwdlg) {

        HRESULT hr;
     
        // this is a requirement for desktop converted apps, dunno why
        // https://msdn.microsoft.com/windows/uwp/monetize/in-app-purchases-and-trials#desktop
        ComPtr<IInitializeWithWindow> init_window;
        hr = context_.As(&init_window);
        //hr = context_->QueryInterface(IID_PPV_ARGS(&init_window));
        RETURN_FALSE_IF_FAILED("Can't QueryInterface IInitializeWithWindow.");
        hr = init_window->Initialize(hwdlg);

        return SUCCEEDED(hr);
    }
       

    void            deinitialize() {
        HRESULT hr = removeLicensesChangedEvent();
        RETURN_VOID_IF_FAILED("Remove license event error.");
        product_.Reset();
        license_.Reset();
        context_.Reset();
    }

    InAppStatus        purchaseSync(IStoreProduct* product) {

        if (product == nullptr)
            return InAppStatus(InAppStatus::InternalError);

        HRESULT hr;
        ComPtr<IAsyncOperation<StorePurchaseResult*>> purchase_op;
        AsyncOperation<StorePurchaseResult*, IStorePurchaseResult*>   async;

        InAppStatus status;
        StorePurchaseStatus purchase_status;

        if (SUCCEEDED(hr = product->RequestPurchaseAsync(&purchase_op))) {
            auto callback = [&](IStorePurchaseResult* result) {

                HRESULT extended = S_OK;
                HRESULT hr = result->get_ExtendedError(&extended);

                if (SUCCEEDED(hr))
                    hr = result->get_Status(&purchase_status);

                return SUCCEEDED(hr) ? extended : hr;
            };

            if (SUCCEEDED(hr = async.Start(*&purchase_op, callback)))
                hr = async.Wait();
        }

        if (FAILED(hr))
            status = makeStatus(hr);
        else if (FAILED(async.Error()))
            status = makeStatus(async.Error());
        else
            status = makeStatus(purchase_status);

        qDebug() << "Purchase status: " << status.isOk() << " message:" << status.message();

        //emit q_ptr->purchasingStatus(status);
        return status;
    }
   
    HRESULT            requestReview() {
        
        ComPtr<IStoreRequestHelperStatics> store_request;
        
        HString response_string;
        
        ComPtr<IAsyncOperation<StoreSendRequestResult*>> review_op;
        AsyncOperation<StoreSendRequestResult*, IStoreSendRequestResult*>   async;
        HRESULT hr = GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Services_Store_StoreRequestHelper).Get(), &store_request);
        
        response_string.Set(L"");
        
        if (SUCCEEDED(hr) &&
            SUCCEEDED(hr = store_request->SendRequestAsync(*&context_, 16, response_string.Get(), &review_op))) {
            auto callback = [&](IStoreSendRequestResult *result) {

                InAppStatus status;
                HRESULT extended;
                HRESULT hr;
                if (SUCCEEDED(hr = result->get_ExtendedError(&extended)) &&
                    SUCCEEDED(hr = result->get_Response(response_string.GetAddressOf())))
                    hr = extended;

                if (FAILED(hr))
                    status = makeStatus(hr);
                else
                    status = parseJsonReviewResponse(response_string);

                emit q_ptr->ratingReviewResponse(status);
                                                                
                return hr;
            };
                     
            if (SUCCEEDED(hr = async.Start(*&review_op, callback)))
                hr = async.Wait();
        }

        if (FAILED(hr))
            emit q_ptr->ratingReviewResponse(makeStatus(hr));
        
        
        return hr;
            
    }

   
    

    void        purchaseProduct(const QString& id) {
        //Wait until operation completed;
        HRESULT hr;
        InAppStatus status;
        
        qDebug() << "Purchasing Store Product" << id;
                
        if ( addon_products_async_.Wait() && SUCCEEDED(addon_products_async_.Error())) {
           
            for (auto& v : addon_products_) 
                if (v.second->identifier() == id) {
                    status = purchaseSync(v.first.Get());
                    emit v.second->purchasingStatus(status);
                }
        }
        else if (product_async_.Wait() && SUCCEEDED(product_async_.Error()) &&
            app_product_->identifier() == id) {
            status = purchaseSync(product_.Get());
            emit app_product_->purchasingStatus(status);
        }
       
    }


    InAppProductRuntime*    product() {
        if (product_async_.Wait())
            return app_product_.get();
        return nullptr;
    }


    InAppProductRuntime*    addon_product(const QString& id) {
       
        if (addon_products_async_.Wait()) {

            for (auto& v : addon_products_) {
                if (v.second->identifier() == id)
                    return v.second.get();
            }
                        
        }
        return nullptr;
    }

    QList<InAppProduct*>    addon_products() {
        QList<InAppProduct*>    list;

        HRESULT hr = addon_products_async_.Wait();
        if (SUCCEEDED(hr)) {
            for (auto& v : addon_products_) 
                list.push_back(v.second.get());
        }
        return list;
    }

    InAppLicenseRuntime*    license() {
        if (license_async_.Wait())
            return app_license_.get();
        return nullptr;
    }


    InAppLicenseRuntime*    addon_license(const QString& id) {
        
        if (license_async_.Wait()) {

            for (auto& v : addon_licenses_) {
                if (v.second->identifier() == id)
                    return v.second.get();
            }

        }
        return nullptr;
    }

    QList<InAppLicense*>    addon_licenses() {
        QList<InAppLicense*>    list;
              
        if (license_async_.Wait()) {
            for (auto& v : addon_licenses_)
                list.push_back(v.second.get());
        }
        return list;
    }

    
   
    InAppProductRuntime*    makeProduct(IStoreProduct* product) {
       
       
        HString         value;
        HRESULT         hr;

        ComPtr<IStorePrice>         price_store;
        ComPtr<IUriRuntimeClass>    uri_store;

        QString         price, currency, title, description, id, uri;

        if (SUCCEEDED(hr = product->get_Title(value.GetAddressOf())))
            title = hStringToQString(value);

        if (SUCCEEDED(hr = product->get_Description(value.GetAddressOf())))
            description = hStringToQString(value);

        if (SUCCEEDED(hr = product->get_InAppOfferToken(value.GetAddressOf())))
            id = hStringToQString(value);

        if (id.isEmpty())
            id = application_identifier;

        if (SUCCEEDED(hr = product->get_Price(&price_store))) {
            if (SUCCEEDED(hr = price_store->get_FormattedPrice(value.GetAddressOf())))
                price = hStringToQString(value);
            if (SUCCEEDED(hr = price_store->get_CurrencyCode(value.GetAddressOf())))
                currency = hStringToQString(value);
        }

        if (SUCCEEDED(hr = product->get_LinkUri(&uri_store))) {
            if (SUCCEEDED(hr = uri_store->get_AbsoluteUri(value.GetAddressOf())))
                uri = hStringToQString(value);
        }

        return new InAppProductRuntime(q_ptr, price, currency, title, description, uri, id );
    }
        
    InAppLicenseRuntime*  makeLicense(IStoreLicense* license) {
       
        HString             value;
        QString             id;
        bool                is_active = false;
        bool                is_trial = false;
        QDateTime           expiration_date;

        HRESULT hr;

        if (SUCCEEDED(hr = license->get_InAppOfferToken(value.GetAddressOf())))
            id = hStringToQString(value);
        
        DateTime expiration;
        if (SUCCEEDED(hr = license->get_ExpirationDate(&expiration))) 
            expiration_date = convertDateTime(expiration);

        boolean boolean_value;
        if (SUCCEEDED(hr = license->get_IsActive(&boolean_value)))
            is_active = boolean_value;

        return new InAppLicenseRuntime(q_ptr, expiration_date, is_active, is_trial, id /*q_ptr*/);
    }

    InAppLicenseRuntime*  makeLicense(IStoreAppLicense* license) {

        HString             value;
        QString             id = application_identifier;
        bool                is_active = false;
        bool                is_trial = false;
        QDateTime           expiration_date;

        HRESULT hr;

        DateTime expiration;
        if (SUCCEEDED(hr = license->get_ExpirationDate(&expiration)))
            expiration_date = convertDateTime(expiration);

        boolean boolean_value;
        if (SUCCEEDED(hr = license->get_IsActive(&boolean_value)))
            is_active = boolean_value;

        if (SUCCEEDED(hr = license->get_IsTrial(&boolean_value)))
            is_trial = boolean_value;

        return new InAppLicenseRuntime(q_ptr, expiration_date, is_active, is_trial, id /*q_ptr*/);
    }


private:

    HRESULT     addLicensesChangedEvent() {
        if (event_token_.value)
            return S_OK;

        auto licenses_changed_handler = Callback<ITypedEventHandler<StoreContext*, IInspectable*>>
            ([&](IStoreContext* context, IInspectable* object)->HRESULT {
            Q_Q(InAppStoreRuntime);
            qDebug() << "License changed event received.";
            HRESULT hr = requestLicense();

            emit q->licenseChanged();
            return hr;
        });
        return context_->add_OfflineLicensesChanged(licenses_changed_handler.Get(), &event_token_);

    }

    HRESULT     removeLicensesChangedEvent() {
        HRESULT hr = context_->remove_OfflineLicensesChanged(event_token_);
        event_token_ = { 0 };
        return hr;

    }

 
    HRESULT             requestLicense() {
        addon_licenses_.clear();
        if (!context_)
            return E_INVALIDARG;

        qDebug() << "Requesting Store License";
              
        ComPtr<IAsyncOperation<StoreAppLicense*>> license_op;
        HRESULT hr = context_->GetAppLicenseAsync(&license_op);

        if (SUCCEEDED(hr)) {
            //Callback of async
            auto callback = [=](IStoreAppLicense *result) {
                license_ = result;

                app_license_.reset(makeLicense(result));

               /* HString json_debug;
                if (SUCCEEDED(license_->get_ExtendedJsonData(json_debug.GetAddressOf())))
                    qDebug() << "License Json: " << hStringToQString(json_debug);*/

                return addAddonLicenses();
            };

            //Run waiting;
            hr = license_async_.Start(*&license_op, callback);
        }
                
        return hr;
    }

   

    //Sync request license;
    HRESULT             addAddonLicenses() {

        if (!license_)
            return E_INVALIDARG;

        ComPtr<IMapView<HSTRING, StoreLicense*>> addons;
        ComPtr<IIterable< IKeyValuePair<HSTRING, StoreLicense*> *>> iterable;
        ComPtr<IIterator< IKeyValuePair<HSTRING, StoreLicense*> *>> it;

        HRESULT hr = license_->get_AddOnLicenses(&addons);

        RETURN_HR_IF_FAILED("Can't get_AddOnLicenses.");
        hr = addons.As(&iterable);
        RETURN_HR_IF_FAILED("Can't get IIterable< IKeyValuePair<HSTRING, StoreLicense*> *> interface.");
        hr = iterable->First(&it);
        RETURN_HR_IF_FAILED("Can't get First() IIterator< IKeyValuePair<HSTRING, StoreLicense*> *> interface.");

        boolean has_current = false;
        for (hr = it->get_HasCurrent(&has_current); SUCCEEDED(hr) && has_current; hr = it->MoveNext(&has_current)) {

            ComPtr<IKeyValuePair<HSTRING, StoreLicense*>> pair;
            HString  key;
            ComPtr<IStoreLicense> license;
            if (SUCCEEDED(hr = it->get_Current(&pair)) &&
                SUCCEEDED(hr = pair->get_Key(key.GetAddressOf())) &&
                SUCCEEDED(hr = pair->get_Value(&license)) &&
                SUCCEEDED(hr = license->get_InAppOfferToken(key.GetAddressOf()))) {

                addon_licenses_.emplace_back(license, makeLicense(license.Get()));
                //addon_licenses_.emplace(hStringToQString(key), std::pair<ComPtr<IStoreLicense>, QScopedPointer<InAppLicenseRuntime>>(license, makeLicense(license.Get())));
                //addon_licenses_.emplace(hStringToQString(key), license, makeLicense(license.Get()));
            }
        }
        return hr;
    }
        

    HRESULT             requestProduct() {
        if (!context_)
            return E_INVALIDARG;

        ComPtr<IAsyncOperation<StoreProductResult*>> product_op;
        HRESULT hr = context_->GetStoreProductForCurrentAppAsync(&product_op);

        if (SUCCEEDED(hr)) {
            //Callback of async
            auto callback = [=](IStoreProductResult *result) {

                HRESULT extended = S_OK;
                HRESULT hr = result->get_ExtendedError(&extended);
                if (SUCCEEDED(hr) && SUCCEEDED(extended) &&
                    SUCCEEDED(hr = result->get_Product(&product_))) {
                                        
                    app_product_.reset( makeProduct(product_.Get()));

                }

                return SUCCEEDED(hr) ? extended : hr;
            };

            //Run waiting;
            hr = product_async_.Start(*&product_op, callback);

        }

        return hr;
    }

    

    HRESULT             requestAddonProducts() {
        addon_products_.clear();
        if (!context_)
            return E_INVALIDARG;
        

        ComPtr<Vector<HSTRING>> product_kinds = Make<Vector<HSTRING>>();
        HString product_kind;
        product_kind.Set(L"Durable");
        product_kinds->Append(product_kind.Get());
        product_kind.Set(L"Consumable");
        product_kinds->Append(product_kind.Get());

        qDebug() << "Requesting addon products list.";

        ComPtr<IAsyncOperation<StoreProductQueryResult*>> product_query_op;
        HRESULT hr = context_->GetAssociatedStoreProductsAsync(product_kinds.Get(), &product_query_op);

        if (SUCCEEDED(hr)) {
            //Callback of async
            auto callback = [=](IStoreProductQueryResult* results) {
                HRESULT extended = S_OK;
                
                ComPtr<IMapView<HSTRING, StoreProduct*>> products;
                HRESULT hr = results->get_ExtendedError(&extended);

                if (SUCCEEDED(hr) && SUCCEEDED(extended) &&
                    SUCCEEDED(hr = results->get_Products(&products))) 
                    hr = addAddonProducts(products.Get());
                
                return SUCCEEDED(hr) ? extended : hr;
            };

            //Run;
            hr = addon_products_async_.Start(product_query_op.Get(), callback);

        }

        return hr;

    }

    HRESULT             addAddonProducts(IMapView<HSTRING, StoreProduct*>* products) {
        ComPtr<IIterable< IKeyValuePair<HSTRING, StoreProduct*> *>> iterable;
        ComPtr<IIterator< IKeyValuePair<HSTRING, StoreProduct*> *>> it;
        ComPtr<IMapView<HSTRING, StoreProduct*>> products_view(products);
               
        HRESULT hr = products_view.As(&iterable);
        RETURN_HR_IF_FAILED("Can't get IIterable< IKeyValuePair<HSTRING, StoreLicense*> *> interface.");
        hr = iterable->First(&it);
        RETURN_HR_IF_FAILED("Can't get First() IIterator< IKeyValuePair<HSTRING, StoreLicense*> *> interface.");

        boolean has_current = false;
                
        for (hr = it->get_HasCurrent(&has_current); SUCCEEDED(hr) && has_current; hr = it->MoveNext(&has_current)) {

            ComPtr<IKeyValuePair<HSTRING, StoreProduct*>> pair;
            HString  key;
            ComPtr<IStoreProduct> product;
            if (SUCCEEDED(hr = it->get_Current(&pair)) &&
                SUCCEEDED(hr = pair->get_Key(key.GetAddressOf())) &&
                SUCCEEDED(hr = pair->get_Value(&product)) &&
                SUCCEEDED(hr = product->get_InAppOfferToken(key.ReleaseAndGetAddressOf()))) {

                addon_products_.emplace_back(product, makeProduct(product.Get()));
            }
        }
        return hr;
    }


    HRESULT             requestAppUpdates() {
        addon_products_.clear();
        if (!context_)
            return E_INVALIDARG;
               
        ComPtr<IAsyncOperation<IVectorView<StorePackageUpdate*>*>> update_op;
        HRESULT hr = context_->GetAppAndOptionalStorePackageUpdatesAsync(&update_op);
                
        if (SUCCEEDED(hr)) {
            //Callback of async
            auto callback = [=](IVectorView<StorePackageUpdate*>* results) {
                unsigned int size = 0;
                HRESULT hr = results->get_Size(&size);

                for (int i = 0; i < size && SUCCEEDED(hr); ++i) {
                    ComPtr<IStorePackageUpdate>     update;
                    boolean mandatory = false;
                    if (SUCCEEDED(hr = results->GetAt(i, &update))) {
                        hr = update->get_Mandatory(&mandatory);
                        qDebug() << "Found Update package. Mandatory " << mandatory;
                    }
                }
                updates_ = results;
                if (size > 0)
                    emit q_ptr->updatesAvailable();
                return hr;
            };

            //Run;
            hr = update_async_.Start(update_op.Get(), callback);

        }

        return hr;

    }

    bool                    isUpdatesAvailable() {
        if (update_async_.Wait() && updates_) {
            unsigned int size = 0;
            return SUCCEEDED(updates_->get_Size(&size)) && size > 0;
        }
        return false;
    }

    InAppStatus            downloadUpdates() {
                
        
        HRESULT hr ;
        ComPtr<IAsyncOperationWithProgress<StorePackageUpdateResult*, StorePackageUpdateStatus>>  updates_op;
        AsyncOperationProgress<StorePackageUpdateResult*, IStorePackageUpdateResult*, StorePackageUpdateStatus>   async;

        ComPtr<IIterable<StorePackageUpdate*>>       packages;
        StorePackageUpdateState overall_state;
        InAppStatus status;
        
               

        if (update_async_.Wait() && updates_ && 
            SUCCEEDED(hr = updates_.As(&packages)) &&
            SUCCEEDED(hr = context_->RequestDownloadStorePackageUpdatesAsync(*&packages, &updates_op))) {


            auto callback = [&](IStorePackageUpdateResult* result) {
                                                
                HRESULT hr = result->get_OverallState(&overall_state);
                              
                return hr;
            };

            auto progress = [&](StorePackageUpdateStatus status) {
                qDebug() << "Update downloading progress: " << status.TotalDownloadProgress << " downloaded_size: " << status.PackageBytesDownloaded;
                emit q_ptr->updatesProgressChanged(status.TotalDownloadProgress);
                
                return S_OK;
            };

            if (SUCCEEDED(hr = async.Start(*&updates_op, callback, progress)))
                async.Wait();
        }

        if (FAILED(hr))
            status = makeStatus(hr);
        else if (FAILED(async.Error()))
            status = makeStatus(async.Error());
        else
            status = makeStatus(overall_state);

        qDebug() << "Download state: " << status.isOk() << " message:" << status.message();

        emit q_ptr->updatesDownloadFinished(status);
        return status;

    }

    InAppStatus            installUpdates() {


        HRESULT hr;
        ComPtr<IAsyncOperationWithProgress<StorePackageUpdateResult*, StorePackageUpdateStatus>>  updates_op;
        AsyncOperationProgress<StorePackageUpdateResult*, IStorePackageUpdateResult*, StorePackageUpdateStatus>   async;

        ComPtr<IIterable<StorePackageUpdate*>>       packages;
        StorePackageUpdateState overall_state;
        InAppStatus status;



        if (update_async_.Wait() && updates_ &&
            SUCCEEDED(hr = updates_.As(&packages)) &&
            SUCCEEDED(hr = context_->RequestDownloadAndInstallStorePackageUpdatesAsync(*&packages, &updates_op))) {


            auto callback = [&](IStorePackageUpdateResult* result) {

                HRESULT hr = result->get_OverallState(&overall_state);

                return hr;
            };

            auto progress = [&](StorePackageUpdateStatus status) {
                qDebug() << "Update downloading and install progress: " << status.TotalDownloadProgress << " downloaded_size: " << status.PackageBytesDownloaded;
                emit q_ptr->updatesProgressChanged(status.TotalDownloadProgress);

                return S_OK;
            };

            if (SUCCEEDED(hr = async.Start(*&updates_op, callback, progress)))
                async.Wait();
        }

        if (FAILED(hr))
            status = makeStatus(hr);
        else if (FAILED(async.Error()))
            status = makeStatus(async.Error());
        else
            status = makeStatus(overall_state);

        qDebug() << "Download and Install state: " << status.isOk() << " message:" << status.message();

        emit q_ptr->updatesInstallFinished(status);
        return status;

    }


private:
    ComPtr<IStoreContext>       context_;
    RoInitializeWrapper         roinit_;
    EventRegistrationToken      event_token_ = { 0 };

private:
    struct UniqueObjectDeleter {
        void operator()(QObject* o) { if (o) o->deleteLater(); }
    };


    typedef std::unique_ptr<InAppProductRuntime, UniqueObjectDeleter> ProductRuntimePtr;
    typedef std::unique_ptr<InAppLicenseRuntime, UniqueObjectDeleter> LicenseRuntimePtr;
      
private:
    //App license;
    ComPtr<IStoreAppLicense>                    license_;
    LicenseRuntimePtr                           app_license_;

    //App product;
    ComPtr<IStoreProduct>                       product_;
    ProductRuntimePtr                           app_product_;
      
private:
    
    AsyncOperation<StoreProductQueryResult*, IStoreProductQueryResult*>         addon_products_async_;
    AsyncOperation<StoreAppLicense*, IStoreAppLicense*>                         license_async_;
    AsyncOperation<StoreProductResult*, IStoreProductResult*>                   product_async_;

    //Update
    AsyncOperation<IVectorView<StorePackageUpdate*>*, 
        IVectorView<StorePackageUpdate*>*>                                      update_async_;

private:
    
    ComPtr<IVectorView<StorePackageUpdate*>>                                    updates_;

private:
    std::vector<std::pair<ComPtr<IStoreProduct>, ProductRuntimePtr>>            addon_products_;

    std::vector<std::pair<ComPtr<IStoreLicense>, LicenseRuntimePtr>>            addon_licenses_;

 

private:
    InAppStoreRuntime*          q_ptr;
    
    Q_DECLARE_PUBLIC(InAppStoreRuntime)
};



//////////////////////////////////////////////////////////////////////////
// CLASS: InAppPrivate

InAppStoreRuntime::InAppStoreRuntime(InAppStore* p) : q_ptr(p) {

}

InAppStoreRuntime::~InAppStoreRuntime() {
   
    qDebug() << "~InAppStoreRuntime";
}


bool InAppStoreRuntime::initialize() {
    private_.reset(new InAppStoreRuntimePrivate(this));
//    RETURN_VOID_IF_FAILED("Could not initialize purchase backend");
    
    is_initialized_ = private_->initialize();
    return is_initialized_;
}

Q_INVOKABLE bool InAppStoreRuntime::initializeUi(WId hwdlg) {
    Q_ASSERT(private_);
    return private_->initializeUi((HWND)hwdlg);
}

void InAppStoreRuntime::deinitialize() {
    Q_ASSERT(private_);
    private_->deinitialize();
    is_initialized_ = false;
    private_.reset();
}


Q_INVOKABLE InAppProduct* InAppStoreRuntime::addon_product(const QString& id) {
    Q_ASSERT(private_);
    return private_->addon_product(id);
}

Q_INVOKABLE InAppProduct* InAppStoreRuntime::product() {
    Q_ASSERT(private_);
    return private_->product();
}

Q_INVOKABLE QList<InAppProduct*> InAppStoreRuntime::addon_products() {
    Q_ASSERT(private_);
    return private_->addon_products();
}

Q_INVOKABLE InAppLicense * InAppStoreRuntime::license() {
    Q_ASSERT(private_);
    return private_->license();
}

Q_INVOKABLE QList<InAppLicense*> InAppStoreRuntime::addon_licenses() {
    Q_ASSERT(private_);
    return private_->addon_licenses();
}

Q_INVOKABLE InAppLicense * InAppStoreRuntime::addon_license(const QString & id) {
    Q_ASSERT(private_);
    return private_->addon_license(id);
}


Q_INVOKABLE bool InAppStoreRuntime::isUpdatesAvailable() {
    Q_ASSERT(private_);
    return private_->isUpdatesAvailable();
}

Q_INVOKABLE void InAppStoreRuntime::downloadUpdates() {
    Q_ASSERT(private_);
    private_->downloadUpdates();
}

Q_INVOKABLE void InAppStoreRuntime::installUpdates() {
    Q_ASSERT(private_);
    private_->installUpdates();
}

Q_INVOKABLE void InAppStoreRuntime::purchaseProduct(const QString& id) {
    Q_ASSERT(private_);
    private_->purchaseProduct(id);
    //emit purchasingStatus(status);
}

Q_INVOKABLE InAppProduct * InAppStoreRuntime::product(const QString & id) {
    Q_ASSERT(private_);
    if (id == application_identifier)
        return private_->product();
    else
        return private_->addon_product(id);
}


Q_INVOKABLE void InAppStoreRuntime::requestReview() {
    Q_ASSERT(private_);

    private_->requestReview();
}
