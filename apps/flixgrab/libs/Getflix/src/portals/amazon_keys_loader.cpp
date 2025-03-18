#include "amazon_keys_loader.h"
#include <decrypsis.h>
#include <ctime>


AmazonKeysLoader::~AmazonKeysLoader() {
    DecryptCloseSession(cdm_session_.data(), cdm_session_.size(), (SessionMessageCallback*)this);
}

void AmazonKeysLoader::initCDMSession(std::vector<uint8_t> init_data, QUrl widevine2LicenseQuery, QPointer<WebContent>  sender) {
    
    sender_ = sender;
    QObject::connect(sender_, &WebContent::finished, this, [=]() {
        AmazonKeysLoader::onLoadResponseFromPRS(sender_);
    }, Qt::QueuedConnection);   
    
    widevine2LicenseQuery_ = widevine2LicenseQuery;

    InstallDecrypsis();
    DecryptCreateSessionAndGenerateRequest(init_data.data(), init_data.size(), (SessionMessageCallback*)this, true); // , widevine2LicenseQuery.toString().toUtf8().constData());
    //cdm_initializing_ = true;
}

void AmazonKeysLoader::initCDMSession(AbstractDecryptService* cdmSrv) {
    //DecryptReCreateService(cdmSrv);
}

void AmazonKeysLoader::OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    qDebug() << "OnSessionMessage: " << std::ctime(&end_time);

    sender_->reset();
    
    cdm_session_ = QByteArray(session_id, session_id_size);
    QString post_data = "widevine2Challenge=";
    QByteArray qbChallenge(message, (int)message_size);
    post_data += QString(qbChallenge.toBase64()).replace("/", "%2F").replace("+", "%2B").replace("-", "%2D").replace("=", "%3D");
    post_data += "&includeHdcpTestKeyInLicense=true";
    //post_data = "widevine2Challenge=CAQ=&includeHdcpTestKeyInLicense=true";
    qDebug() << "AmazonKeysLoader: " << post_data.toUtf8() << " >> " << widevine2LicenseQuery_.toString();
    /*auto cookies = networkAccess()->cookieJar()->allcookies();
    QString allCookies;
    for each (const auto& c in cookies)
        allCookies += c.name() + "=" + c.value()+";";
    qDebug() << allCookies.toUtf8();*/

    sender_->post(widevine2LicenseQuery_, post_data.toLocal8Bit()); 
}

void AmazonKeysLoader::onLoadResponseFromPRS(const WebContent* content) {
    try {
        //qDebug() << "licenseResponse for " << metadata_->video_info().video_title << ": " << content->data();
        // {"errorsByResource":{"Widevine2License":{"downstreamReason":"untrusted_device","errorCode":"PRS.Dependency.DRM.Widevine.UnsupportedCdmVersion","message":"Cannot complete request.","type":"PRSWidevine2LicenseDeniedException"}},"returnedTitleRendition":{"asin":"amzn1.dv.gti.14b02470-fb0f-7d69-c65e-b9ab1c235283","audioQuality":"Stereo","contentId":"amzn1.dv.vcid.ab463c37-c27c-4e04-976b-3fc09caf2fe2","selectedEntitlement":{"consumptionExpiration":"1572666812368","entitlementType":"PRIME_SUBSCRIPTION","grantedByCustomerId":"A2GB4MZ7IAX0TJ"},"titleId":"amzn1.dv.gti.14b02470-fb0f-7d69-c65e-b9ab1c235283","videoMaterialType":"Feature","videoQuality":"HD"}}
        // "errorCode":"PRS.Dependency.DRM.Widevine.HdContentNotAllowed.VmpValidationRequired","downstreamReason":"vmp_validation_required"
        auto jsonDoc = QJsonDocument::fromJson(content->data().toUtf8()).object();
        auto lic = jsonDoc.value("widevine2License").toObject();
        auto metadata = lic.value("metadata").toObject();
        auto errorsByResource = jsonDoc.value("errorsByResource").toObject();
        auto license = lic.value("license").toString();

        if (!errorsByResource.isEmpty()) {
            auto Widevine2License = errorsByResource.value("Widevine2License").toObject();
            qDebug() << "errors: " << Widevine2License.value("errorCode").toString() << Widevine2License.value("message").toString() << Widevine2License.value("type").toString();
            //todo: metadata_->invalidate();
            qDebug() << "RELOAD DowngradeToSD ..."; //HD content not allowed.Forcing restart for SD asset
            DecryptCloseSession(cdm_session_.data(), cdm_session_.size(), (SessionMessageCallback*)this);
            emit error(Status(EStatusCode::ForcedReloadPage));
        }
        else {
            qDebug() << "got response from PRS: (" << license.size() << "): " << license.toUtf8();
            auto r = QByteArray::fromBase64(license.toUtf8());
            //qDebug() << r.size() << "): " << r.constData();
            DecryptUpdateSession(cdm_session_.constData(), cdm_session_.size(), r.constData(), r.size());
        }
        auto keyMetadata = metadata.value("keyMetadata").toArray();
        if (!keyMetadata.isEmpty()) {
            bool gotKeys = false;
            for (auto k : keyMetadata) {
                auto id = k.toObject()["id"].toString();
                auto keyType = k.toObject()["keyType"].toString();
                if (keyType == "CONTENT") {
                    qDebug() << "got keys from PRS: " << keyType << " " << id;
                    gotKeys = true;
                }
                else if (keyType == "SERVICE_CERTIFICATE") {
                    qDebug() << "got " << keyType;
                }
            }
            if (gotKeys) {
                //forced_reload_page_ = false;
                //cdm_no_error_ = true;
                emit keysLoaded();
            }
        }
    }
    catch (...) {
        qDebug() << "error loading From PRS.";
    }
}

void AmazonKeysLoader::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size) {
    Q_UNUSED(promise_id);
    Q_UNUSED(session_id);
    Q_UNUSED(session_id_size);
    qDebug() << "interceptor: OnResolveNewSessionPromise ...";
}

void AmazonKeysLoader::OnSessionKeysChange() {
  qDebug() << "got keys.";
}

void AmazonKeysLoader::OnRejectPromise() {
  qDebug() << "License request rejected.";
}
