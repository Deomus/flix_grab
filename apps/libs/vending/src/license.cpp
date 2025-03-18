#include "vending/license.h"
#include "license_template.h"

#include <company_info.h>

#include <licensing.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <exception>
#include <tchar.h>
#include <QtConcurrent>

using namespace FreeGrabApp::Licensing;

#pragma comment(lib, "winhttp.lib") 
#pragma comment(lib, "Ws2_32.lib") 
#pragma comment(lib, "licensing.lib")

#define LicenseEmail                        "email"
#define LicenseKey                          "license_key"
#define ActivationKey                       "activation_key"
#define Valid                               "valid"
#define ProductId                           "product_id"
#define HardwareId                          "hardware_id"
#define VerificationDate                    "verification_date"
#define ExpirationDate                      "expiration_date"
#define FirstTry                            "first_try"

#define CHECK_VERIFICATION_PERIOD           1000 * 60 * 30  //30 min in ms, every half of hour check if verification is needed
#define REACTIVATE_LICENSE_PERIOD           60 * 60 * 2  //2 hours


namespace vending {

#ifdef _UNICODE
    typedef std::wstring string_t;
#else
    typedef std::string string_t;
#endif

class ExceptionPrivate : public Exception {
public:
    ExceptionPrivate(int code, const char_t * message = NULL) {
        SetCode(code);
        if (message)
            SetMessage(message);
    }

    virtual ~ExceptionPrivate() {}
    virtual int GetCode() { return code_; }
    virtual const char_t * GetExceptionMessage() { return message_.c_str(); }
    virtual void Destroy() { delete this; }
    void SetCode(int code) { code_ = code; }
    void SetMessage(const char_t * message) { message_ = message; }

protected:
    int                 code_;
    string_t            message_;
};


class LicensePrivate {

public:
    //C-tor, D-tor;
    LicensePrivate(License* d);
    LicensePrivate(const QString& product_id, const QString& public_key, const QString& company_settings_path, License* d);
    ~LicensePrivate();

public:
    void                activate(const QString& email, const QString& license_key, bool forced_activate = false);
    void                deactivate(bool forced = false);//Clear license and stuff;
    void                verify(bool forced_activate = false);
    void                firstTryEnded();
    __forceinline bool  isFirstTry();
    __forceinline bool  isValid();

    //License data;
    const QDateTime&    expirationDate();
    const QString&      registeredEmail();
    const QString&      registeredKey();
    int                 productId() const;
    //Current status of license;
    const Status&       status() const;

    //QString             generateKey(const QString& email, int product_id, const QString& private_key);

private:
    void                initialize(const QString& product_id = QString(), const QString& public_key = QString());
    //Check verification period sometimes;
    void                check();

private:
    //License storage;
    void                loadData(const QString& product_id);
    void                saveData() const;

    const QVariant&     licenseProperty(const QString& name, const QVariant &default_val = QVariant()) const {
        if (!request_watcher_.isRunning()) {

            auto it = license_data_.find(name);
            if (it != license_data_.end())
                return *it;
        }

        return default_val;
    }

private://Licensing stuff;
    void                setupKeys(const QString& email, const QString& license_key,
        const QString& hardware_id = QString(), const QString& activation_key = QString());

    Status              activateInternal(const QString& email, const QString& license_key);
    //Paddle case
    //Status              generateAndActivateInternal(const QString& email, int product_id, const QString& private_key);
    
private://openssl aes256 encryption
    void                initCrypto();
    int                 aesInit(unsigned char* key_data, int key_data_len, unsigned char* salt, EVP_CIPHER_CTX* e_ctx, EVP_CIPHER_CTX* d_ctx);
    unsigned char*      aesEncrypt(EVP_CIPHER_CTX* e, unsigned char* plaintext, int* len, bool encrypt_final = false);
    unsigned char*      aesDecrypt(EVP_CIPHER_CTX* e, unsigned char* ciphertext, int* len, bool decrypt_final = false);
    
private://openssl aes256 encryption
    EVP_CIPHER_CTX*     encryptor_ = nullptr;
    EVP_CIPHER_CTX*     decryptor_ = nullptr;
    const QByteArray    enc_data_enable_  =         "DfvNJxyjYtCnjbnGsnfnmcz";
    const QByteArray    enc_data_disable_ =         "JNsdvjnIOJfkl2hjjbnddsf";
    const QByteArray    enc_data_enable_file_ =     "pfrfxrf2Ljgecrftncz0Ljgecrftncz0pfrfxrf2";
    const QByteArray    enc_data_disable_file_ =    "Pfrfxrf1Pfghtoftncz9Pfghtoftncz9Pfrfxrf1";
    QString             config_path_;

private:
    //public interface;
    License*                license_;
    int                     product_id_ = -1;

    //stuff;
    KeyTemplate             template_;
    KeyValidator            validator_;
    LicensingClient         client_;

    //Storage, used in other threads;
    QVariantMap             license_data_;

    QFutureWatcher<Status>  request_watcher_;
    QTimer                  watch_dog_;
    bool                    required_deactivate_ = false;

private:
    //Main Thread variables;
    mutable QString         email_;
    mutable QString         license_key_;

    //Date Time In UTC;
    mutable QDateTime       expiration_date_;
    mutable Status          status_;

    mutable QAtomicInt      valid_ = 0;
    mutable bool            first_try_ = false;
    mutable bool            finished_ = false;
    mutable bool            forced_activate_ = false;

private:
    QString                 company_settings_path_;
};


//Fix Wrong VC realization of inline;
inline std::wstring toWString(const QString& s) {
    std::wstring str;
    str.resize(s.length());
    if (s.length() > 0)
        str.resize(s.toWCharArray(&(*str.begin())));
    return std::move(str);
}

LicensePrivate::LicensePrivate(License* license)
    : license_(license) {
    initialize();
}

LicensePrivate::LicensePrivate(const QString& product_id, const QString& public_key,
    const QString& company_settings_path, License* license)
    : license_(license)
    , company_settings_path_(company_settings_path) {
    initialize(product_id, public_key);
}

LicensePrivate::~LicensePrivate() {
    saveData();
}

int LicensePrivate::aesInit(unsigned char* key_data, int key_data_len, unsigned char* salt,
    EVP_CIPHER_CTX* e_ctx, EVP_CIPHER_CTX* d_ctx) {
    int i, nrounds = 5;
    unsigned char key[32], iv[32];

    i = EVP_BytesToKey(EVP_aes_256_gcm(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
    if (i != 32) {
        printf("Key size is %d bits - should be 256 bits\n", i);
        exit(-1);
    }

    e_ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_gcm(), nullptr, key, iv);
    d_ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_gcm(), nullptr, key, iv);

    return 0;
}

unsigned char* LicensePrivate::aesEncrypt(EVP_CIPHER_CTX* e, unsigned char* plaintext, int* len, bool encrypt_final) {
    int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
    unsigned char* ciphertext = reinterpret_cast<unsigned char*>(malloc(static_cast<size_t>(c_len)));

    EVP_EncryptInit_ex(e, nullptr, nullptr, nullptr, nullptr);
    EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);
    if (encrypt_final)
        EVP_EncryptFinal_ex(e, ciphertext + c_len, &f_len);

    *len = c_len + f_len;

    return ciphertext;
}

unsigned char* LicensePrivate::aesDecrypt(EVP_CIPHER_CTX* e, unsigned char* ciphertext, int* len, bool decrypt_final) {
    int p_len = *len, f_len = 0;
    unsigned char* plaintext = reinterpret_cast<unsigned char*>(malloc(static_cast<size_t>(p_len + AES_BLOCK_SIZE)));

    EVP_DecryptInit_ex(e, nullptr, nullptr, nullptr, nullptr);
    EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
    if (decrypt_final)
        EVP_DecryptFinal_ex(e, plaintext + p_len, &f_len);

    *len = p_len + f_len;

    return plaintext;
}

void LicensePrivate::initCrypto() {
    bool result = true;
        
    encryptor_ = EVP_CIPHER_CTX_new();
    decryptor_ = EVP_CIPHER_CTX_new();

    int hardware_id = QString::fromWCharArray(client_.GetHardwareId()).toInt();
    int id = 1292020;
    unsigned int salt[] = { hardware_id, id }; //hardware id
    unsigned char key_data[] = "NRXUSA-AAX32G-YRWVND-P2XZYA-KAJ465";
    int key_data_len = static_cast<int>(strlen(reinterpret_cast<char*>(key_data)));

    // gen key and iv. init the cipher ctx object
    if (aesInit(key_data, key_data_len, reinterpret_cast<unsigned char*>(&salt), encryptor_, decryptor_)) {
        qDebug() << "Couldn't initialize AES cipher";
        encryptor_ = nullptr;
        decryptor_ = nullptr;
    }
}

void LicensePrivate::initialize(const QString& product_id, const QString& public_key) {
    loadData(product_id);

    //openssl aes256 encryption
    //initCrypto();
        
    QString configDataDir = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
    QDir dir(configDataDir);
    if (!dir.exists())
        dir.mkpath(configDataDir);

    config_path_ = configDataDir + "/" + product_id + ".bin";

    if (encryptor_
        && licenseProperty(FirstTry).toString().isEmpty()
        && !QFileInfo::exists(config_path_)) {
        first_try_ = true;
    }
    else if (decryptor_
        && !licenseProperty(FirstTry).toString().isEmpty()
        && QFileInfo::exists(config_path_)) {
            
        QByteArray first_try_ciphered_text = QByteArray::fromHex(license_data_[FirstTry].toByteArray());
        int len = first_try_ciphered_text.length();
        const char* ciphered_text = first_try_ciphered_text;
        unsigned char* unciphered_text = aesDecrypt(decryptor_,
            reinterpret_cast<unsigned char*>(const_cast<char*>(ciphered_text)), &len);
        QByteArray unciphered_array(reinterpret_cast<char*>(unciphered_text), len);
        //qDebug() << "unciphered array = " << static_cast<const char*>(unciphered_array);

        QFile   file(config_path_);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray first_try_ciphered_text_file = QByteArray::fromHex(file.readAll());
            int len = first_try_ciphered_text_file.length();
            const char* ciphered_text_file = first_try_ciphered_text_file;
            unsigned char* unciphered_text_file = aesDecrypt(decryptor_,
                reinterpret_cast<unsigned char*>(const_cast<char*>(ciphered_text_file)), &len, true);
            QByteArray unciphered_array_file(reinterpret_cast<char*>(unciphered_text_file), len);
            //qDebug() << "unciphered array file = " << static_cast<const char*>(unciphered_array_file);

            first_try_ = (enc_data_enable_ == QString(unciphered_array)
                && enc_data_enable_file_ == QString(unciphered_array_file));
        }
    }

    if (encryptor_) {
        if (licenseProperty(FirstTry).toString().isEmpty()) {
            int len = enc_data_enable_.length() + 1;
            auto first_try_unciphered_text = reinterpret_cast<unsigned char*>(const_cast<char*>(static_cast<const char*>(enc_data_enable_)));
            unsigned char* ciphered_text = aesEncrypt(encryptor_, first_try_unciphered_text, &len);
            QByteArray ciphered_array(reinterpret_cast<char*>(ciphered_text), len);
            license_data_[FirstTry] = ciphered_array.toHex();
            //qDebug() << "ciphered array enable = " << static_cast<const char*>(ciphered_array.toHex());
            saveData();
        }

        QFile file(config_path_);
        if (!QFileInfo::exists(config_path_)) {
            int len = enc_data_enable_file_.length() + 1;
            auto first_try_unciphered_text_file = reinterpret_cast<unsigned char*>(const_cast<char*>(static_cast<const char*>(enc_data_enable_file_)));
            unsigned char* ciphered_text_file = aesEncrypt(encryptor_, first_try_unciphered_text_file, &len);
            QByteArray ciphered_array_file(reinterpret_cast<char*>(ciphered_text_file), len);

            QFile   file(config_path_);
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(ciphered_array_file.toHex());
                file.close();
            }

            //qDebug() << "ciphered array enable file = " << static_cast<const char*>(ciphered_array_file.toHex());
        }
    }

    QObject::connect(&request_watcher_, &QFutureWatcher<Status>::finished, license_, [this]() {
        bool forced_activate = forced_activate_;
        forced_activate_ = false;

        if (!status().isOk()) {
            if (product_id_ != -1) // -1 = not activated
                if (status().code() != Status::NetError) {
                    if (!license_->isValid())
                        deactivate();
                }

            emit license_->error();//ActivationDialog
        }
        else {
            saveData();
            watch_dog_.start(CHECK_VERIFICATION_PERIOD);
            if (std::exchange(finished_, false))
                emit license_->finished();//ActivationDialog
        }

        if (email_ != licenseProperty(LicenseEmail).toString()
            || license_key_ != licenseProperty(LicenseKey).toString()
            || (valid_ != 0) != licenseProperty(Valid).toBool()
            || forced_activate) {

            emit license_->changed(forced_activate);//AlertDialog
        }
    });

    QObject::connect(&watch_dog_, &QTimer::timeout, [this]() {
        check();
    });

    try {
        // Entering the purchased developer key is the first step in using the SDK. 
        // The key below is a demo key which only allows certain public/private key pairs to be generated and used for license key generation/validation purposes.
        // You if you purchased the SDK, replace this key with your purchased key.
        //SDKRegistration::SetLicenseKey(_T(SOFTACTIVATE_LICENSE_KEY));
        template_.LoadXml(kLicenseTemplate);
        template_.SetPublicKeyCertificate(toWString(public_key).data());

        validator_.SetKeyTemplate(&template_);

        email_ = licenseProperty(LicenseEmail).toString();
        license_key_ = licenseProperty(LicenseKey).toString();

        valid_ = licenseProperty(Valid).toBool();

        //Email can be empty!;
        if (!license_key_.isEmpty()) {
            setupKeys(email_, license_key_,
                licenseProperty(HardwareId).toString(), licenseProperty(ActivationKey).toString());
            //hid, licenseProperty(ActivationKey).toString());
        }

        client_.SetLicensingServiceUrl(template_.GetSigningServiceUrl());
        //new in version 3.x : connects to a timeserver and retrieves the time, instead of retrieving the (possibly altered) local time 
        client_.SetTimeValidationMethod(PREFER_INTERNET_TIME); //alternative: USE_INTERNET_TIME  or USE_LOCAL_TIME

        expiration_date_ = licenseProperty(ExpirationDate).toDateTime();
        
        qDebug() << "initialize valid_: " << valid_ << " expiration_date_: " << expiration_date_ << " product_id_: " << product_id_;
        
        if (valid_
            || product_id_ != -1)
            watch_dog_.start(CHECK_VERIFICATION_PERIOD);
    }
    catch (Exception * ex) {
        status_.updateInternal(ex->GetCode(), QString::fromWCharArray(ex->GetExceptionMessage()));
        ex->Destroy();
        emit license_->error();
    }
}

void LicensePrivate::setupKeys(const QString& email, const QString& license_key, const QString& hardware_id /*= QString()*/, const QString& activation_key /*= QString()*/) {
    validator_.SetKey(toWString(license_key).data());
    validator_.SetValidationData(_T(LICENSE_REGISTRATION_EMAIL), toWString(email).data());

    client_.SetLicenseTemplate(template_);
    client_.SetLicenseKey(toWString(license_key).data());
    client_.SetHardwareId(toWString(hardware_id).data());
    client_.SetActivationKey(toWString(activation_key).data());

    if (validator_.IsKeyValid()) {
        int validation_data_len = 256;
        unsigned char validation_data[256];
        validator_.QueryValidationData(NULL, validation_data, &validation_data_len);
        client_.SetLicenseKeyValidationData(validation_data, validation_data_len);  //SetLicenseKeyValidationData needs to be called before SetActivationKeyTemplate !
        product_id_ = validator_.QueryIntKeyData(_T(LICENSE_PRODUCT_ID));           //set product id from license data;
        qDebug() << "License product id: " << product_id_;
        client_.SetProductId(product_id_);
    }
    else {
        product_id_ = -1;
        valid_ = 0;
        qDebug() << "Can't validate email and license key";
        throw new ExceptionPrivate(STATUS_INVALID_LICENSE_KEY, _T("Invalid license key and/or email"));
    }

}

void LicensePrivate::activate(const QString& email, const QString& license_key, bool forced_activate) {
    forced_activate_ = forced_activate;
    request_watcher_.waitForFinished();
    if (!request_watcher_.isRunning()) {
        auto future = QtConcurrent::run(this, &LicensePrivate::activateInternal, email, license_key);
        request_watcher_.setFuture(future);
        finished_ = true;
    }
}

//QString LicensePrivate::generateKey(const QString & email, int product_id, const QString & private_key) {
//    try {
//        KeyGenerator generator(&template_);
//
//        template_.SetPrivateKey(toWString(private_key).data());
//        generator.SetKeyData(_T(LICENSE_PRODUCT_ID), product_id);
//        generator.SetValidationData(_T(LICENSE_REGISTRATION_EMAIL), toWString(email).data());
//        return QString::fromWCharArray(generator.GenerateKey());
//    }
//    catch (Exception * ex) {
//        status_.updateInternal(ex->GetCode(), QString::fromWCharArray(ex->GetExceptionMessage()));
//        ex->Destroy();
//    }
//
//    return QString();
//}

void LicensePrivate::verify(bool forced_activate) {
    qDebug() << "verify license";

    request_watcher_.waitForFinished();
    if (!request_watcher_.isRunning()) {
        qDebug() << "verify: LicenseEmail: " << license_data_[LicenseEmail] << " LicenseKey: " << license_data_[LicenseKey] << " forced_activate: " << forced_activate;
        if (license_data_.contains(LicenseKey)
            && license_data_.contains(LicenseEmail)) {
            activate(license_data_[LicenseEmail].toString(), license_data_[LicenseKey].toString(), forced_activate);
        }
        else {
            deactivate();
        }
    }
}

void LicensePrivate::check() {
    if (!request_watcher_.isRunning()) {
        qDebug() << "check start";

        if (required_deactivate_)
            deactivate();

        QDateTime verification_date = licenseProperty(VerificationDate).toDateTime();

        if (!verification_date.isNull()
            && QDateTime::currentDateTimeUtc() > verification_date
            && valid_ != 0
            && !licenseProperty(HardwareId).toString().isEmpty()) {
            verify();
        }
        else {//Check activation status;
            request_watcher_.setFuture(QtConcurrent::run([this]()->Status {
                Status status;

                license_data_[Valid] = client_.IsLicenseValid();

                qDebug() << "check: license_data_[Valid]: " << license_data_[Valid] << "license_data_[VerificationDate]: " << license_data_[VerificationDate];
                if (license_data_[Valid].toBool()) {
                    if (!license_data_.contains(VerificationDate))
                        license_data_[VerificationDate] = QDateTime::currentDateTimeUtc().addSecs(REACTIVATE_LICENSE_PERIOD);
                    else if (license_data_[VerificationDate].toDateTime().toSecsSinceEpoch() > QDateTime::currentSecsSinceEpoch() + REACTIVATE_LICENSE_PERIOD)
                        license_data_[VerificationDate] = QDateTime::currentDateTimeUtc().addSecs(REACTIVATE_LICENSE_PERIOD);

                    int year, month, day;
                    // the license expiration date returned by GetLicenseExpirationDate() is only valid if IsLicenseValid() returns > 0, 
                    // or if IsLicenseValid() returns 0 and GetLicenseActivationStatus() returns STATUS_LICENSE_EXPIRED
                    client_.GetLicenseExpirationDate(&year, &month, &day);
                    license_data_[ExpirationDate] = QDateTime(QDate(year, month, day), QTime());

                    qDebug() << "License Expiration date " << expiration_date_.toString();
                }

                status.updateInternal(client_.GetLicenseActivationStatus());

                return status;
            }));
        }
    }
}

__forceinline bool LicensePrivate::isFirstTry() {
    return first_try_;
}

__forceinline bool LicensePrivate::isValid() {
    if (!request_watcher_.isRunning()) {
        valid_ = licenseProperty(Valid).toInt();
        if (valid_ != 0) {
            if (licenseProperty(ActivationKey).toString().isEmpty()
                || licenseProperty(HardwareId).toString().isEmpty()
                || licenseProperty(VerificationDate).toDateTime().isNull()
                || licenseProperty(ExpirationDate).toDateTime().isNull())
                valid_ = 0;
        }
    }

    return valid_ != 0;
}

const QDateTime& LicensePrivate::expirationDate() {
    if (!request_watcher_.isRunning())
        expiration_date_ = licenseProperty(ExpirationDate).toDateTime();

    return expiration_date_;
}

const QString& LicensePrivate::registeredEmail() {
    if (!request_watcher_.isRunning())
        email_ = licenseProperty(LicenseEmail).toString();

    return email_;
}

const QString& LicensePrivate::registeredKey() {
    if (!request_watcher_.isRunning())
        license_key_ = licenseProperty(LicenseKey).toString();

    return license_key_;
}

int LicensePrivate::productId() const {
    return product_id_;
}

const Status& LicensePrivate::status() const {
    if (request_watcher_.isFinished()
        && request_watcher_.future().resultCount())
        status_ = request_watcher_.result();

    return status_;
}

void LicensePrivate::deactivate(bool forced) {
    qDebug() << "deactivate start";
    request_watcher_.waitForFinished();
    if (!request_watcher_.isRunning()) {
        qDebug() << "deactivate license";
        required_deactivate_ = false;
        watch_dog_.stop();
        email_.clear();
        license_key_.clear();
        valid_ = 0;
        license_data_[Valid] = false;
        first_try_ = false;
        product_id_ = -1;
        license_data_.clear();
        client_.SetLicenseKey(_T(""));
        client_.SetHardwareId(_T(""));
        client_.SetActivationKey(_T(""));
        saveData();
        emit license_->changed(forced);
    }
}

void LicensePrivate::firstTryEnded() {
    if (encryptor_) {
        int len = enc_data_disable_.length() + 1;
        auto first_try_unciphered_text = reinterpret_cast<unsigned char*>(const_cast<char*>(static_cast<const char*>(enc_data_disable_)));
        unsigned char* ciphered_text = aesEncrypt(encryptor_, first_try_unciphered_text, &len);
        QByteArray ciphered_array(reinterpret_cast<char*>(ciphered_text), len);
        //qDebug() << "ciphered array disable = " << static_cast<const char*>(ciphered_array.toHex());
        license_data_[FirstTry] = ciphered_array.toHex();
        saveData();

        int len_file = enc_data_enable_file_.length() + 1;
        auto first_try_unciphered_text_file = reinterpret_cast<unsigned char*>(const_cast<char*>(static_cast<const char*>(enc_data_disable_file_)));
        unsigned char* ciphered_text_file = aesEncrypt(encryptor_, first_try_unciphered_text_file, &len_file, true);
        QByteArray ciphered_array_file(reinterpret_cast<char*>(ciphered_text_file), len_file);
        //qDebug() << "ciphered array disable file = " << static_cast<const char*>(ciphered_array_file.toHex());

        QFile   file(config_path_);
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(ciphered_array_file.toHex());
            file.close();
        }
    }

    first_try_ = false;
}

Status LicensePrivate::activateInternal(const QString & email, const QString & license_key) {
    Status status;
    QElapsedTimer timer;
    timer.start();
    QString email_in = email.toLower();

    try {
        setupKeys(email_in, license_key);
            
        client_.AcquireLicense();                   //The license activation consists in obtaining an "activation key" from the activation server, linked to this particular computer's hardware id. 
        license_data_[LicenseEmail] = email_in;     //Store email and key even if license not valid for registeredKey call;
        license_data_[LicenseKey] = license_key;
    }
    catch (Exception * ex) {
        status.updateInternal(ex->GetCode(), QString::fromWCharArray(ex->GetExceptionMessage()));

        //Server connection problem, so verificate later;
        if (license_data_.contains(ActivationKey)
            && (status.code() == Status::NetError || license_data_[LicenseKey] != license_key)) { //Reset old keys;
            try { 
                setupKeys(license_data_[LicenseEmail].toString(), license_data_[LicenseKey].toString(), license_data_[HardwareId].toString(), license_data_[ActivationKey].toString());
            }
            catch (Exception* ex) {
                qDebug() << "Setup old keys exception: " << QString::fromWCharArray(ex->GetExceptionMessage()) << " code: " << ex->GetCode();
                ex->Destroy();
            }
        }
        else if (status.code() != Status::NetError) {
            required_deactivate_ = true;
        }

        ex->Destroy();
    }
    catch (...) {
        status.updateInternal(STATUS_GENERIC_ERROR, "Unknown exception during activation attempt !");
    }

    license_data_[Valid] = client_.IsLicenseValid();
    //Stop Verifying;
    license_data_.remove(ActivationKey);
    license_data_.remove(VerificationDate);
    license_data_.remove(HardwareId);
    license_data_.remove(ExpirationDate);

    if (license_data_[Valid].toBool()) {
        //You should now save the hardware id and the activation key along with the product (license) key. 
        //Note that these keys do not have to be hidden or obfuscated in any way, because they are useless on another computer (IsLicenseActivated() will return false).
        license_data_[ActivationKey] = QString::fromWCharArray(client_.GetActivationKey());
        license_data_[HardwareId] = QString::fromWCharArray(client_.GetHardwareId());
        int year, month, day;
        //The license expiration date returned by GetLicenseExpirationDate() is only valid if IsLicenseValid() returns > 0, 
        //or if IsLicenseValid() returns 0 and GetLicenseActivationStatus() returns STATUS_LICENSE_EXPIRED
        client_.GetLicenseExpirationDate(&year, &month, &day);
        license_data_[ExpirationDate] = QDateTime(QDate(year, month, day), QTime());
        license_data_[VerificationDate] = QDateTime::currentDateTimeUtc().addSecs(REACTIVATE_LICENSE_PERIOD);
    }
    else {
        status.updateInternal(client_.GetLicenseActivationStatus());
    }

    if (status.isOk())
        required_deactivate_ = false;
    
    qDebug() << "Activation: " << (status.isOk() ? "Successful" : status.message()) << ", time elapsed:" << timer.elapsed() << "ms.";

    return status;
}

//Paddle case;
//Status LicensePrivate::generateAndActivateInternal(const QString & email, int product_id, const QString& private_key) {
//    Status status;
//    try {
//        KeyGenerator generator(&template_);
//
//        template_.SetPrivateKey(toWString(private_key).data());
//        generator.SetKeyData(_T(LICENSE_PRODUCT_ID), product_id);
//        generator.SetValidationData(_T(LICENSE_REGISTRATION_EMAIL), toWString(email).data());
//        status = activateInternal(email, QString::fromWCharArray(generator.GenerateKey()));
//    }
//    catch (Exception * ex) {
//        status.updateInternal(ex->GetCode(), QString::fromWCharArray(ex->GetExceptionMessage()));
//        ex->Destroy();
//    }
//    return status;
//}

void LicensePrivate::loadData(const QString& product_id) {
    QSettings       settings(LICENSES_STORAGE_PATH, QSettings::NativeFormat);
    license_data_ = settings.value(QCoreApplication::applicationName()).toMap();
}

void LicensePrivate::saveData() const {
    QSettings       settings(LICENSES_STORAGE_PATH, QSettings::NativeFormat);
    settings.setValue(QCoreApplication::applicationName(), license_data_);
}

//QString LicensePrivate::generateKey(const QString & email, int product_id, const QString & private_key) {

//    try {
//        KeyGenerator generator(&template_);

//        template_.SetPrivateKey(toWString(private_key).data());
//        generator.SetKeyData(_T("ProductId"), product_id);
//        generator.SetValidationData(_T("RegistrationEmail"), toWString(email).data());
//        return QString::fromWCharArray(generator.GenerateKey());
//    }
//    catch (Exception * ex) {
//        status_.updateInternal(ex->GetCode(), QString::fromWCharArray(ex->GetExceptionMessage()));
//        ex->Destroy();
//    }

//    return QString();
//}

//////////////////////////////////////////////////////////////////////////

License::License(const QString& product_id, const QString& public_key, const QString& company_settings_path, QObject* parent /*= nullptr*/)
    : QObject(parent) {
    private_.reset(new LicensePrivate(product_id, public_key, company_settings_path, this));
}

License::~License() {
}

void License::activate(const QString& email, const QString & license_key) {
    qDebug() << "activate license - user";
    private_->activate(email, license_key);
}

void License::deactivate() {
    qDebug() << "deactivate license - user";
    private_->deactivate(true);
}

void License::verify(bool forced_activate) {
    private_->verify(forced_activate);
}

bool License::isValid() const {
    return private_->isValid();
}

bool License::isFirstTry() const {
    return private_->isFirstTry();
}

void License::firstTryEnded() {
    private_->firstTryEnded();
}

QString License::registeredEmail() const {
    return private_->registeredEmail();
}

QString License::registeredKey() const {
    return private_->registeredKey();
}

QDateTime License::expirationDate() const {
    return private_->expirationDate();
}

QString License::productId() const {
    return QString::number(private_->productId());
}

Status License::status() const {
    return private_->status();
}

//QString License::generateKey(const QString& email, int product_id, const QString& private_key) const {
//    return private_->generateKey(email, product_id, private_key);
//}
}
