#include "vending/product.h"

#include <QtNetwork>
#include <QtGui>
#include <QApplication>

#include <purchasing_config.h>

#define PRODUCTS_STORAGE_PATH       "\\Products"
#define UPDATE_STORAGE_PATH         "\\update"
#define EXPIRATION_CHECK_INTERVAL   1000*60*10 //every 10 min check
#define EXPIRATION_PERIOD           60*45 //every 45 min check

//Properties;
#define ProductName                 "product_name"
#define Price                       "price"
#define Currency                    "currency"
#define LicenseDuration             "license_duration"
#define PurchaseUrl                 "purchase_url"
#define ProductId                   "product_id"
#define ProductUrl                  "product_url"
#define DownloadUrl                 "download_url"
#define Version                     "version"
#define ExpirationDate              "expiration_date"

namespace vending {

    class ProductPrivate {

    public:
        ProductPrivate(const QString& product_name, const QString& company_name, const QString& company_settings_path, const QString& company_vending_url, Product* p);

        const QString&  product_name() const;
        const QString&  product_id();
        const QString&  price();
        const QString&  currency();
        const QString&  product_url();
        const QString&  purchase_url();
        const QString&  version();
        const QString&  license_duration();

        void            purchase();
        void            openProductPage(const QString& company_url);
        //Update section
        bool            isUpdateAvailable();
        bool            isUpdateDownloaded();

        void            downloadUpdate();
        void            installUpdate();
        void            downloadProductData();

    private:
        void            initialize();
                
        bool            requestData(bool forced = false);
        void            requestDataWithoutSSL();

    
        const QVariant& productProperty(const QString& name);

        void            loadData();
        void            saveData() const;
        void            saveProductsData() const;
        void            initProductsData(QVariantMap& map);

        void            checkVersion();

        QString         updateDownloadPath();

       

    private:
        QString                 product_name_;
        QString                 product_id_;
        QString                 price_;
        QString                 currency_;
        QString                 product_url_;
        QString                 purchase_url_;
        QString                 version_;
        QString                 license_duration_;

        QString                 company_name_;
        QString                 company_settings_path_;
        QString                 company_vending_url_;

        Status                  status_;

    private:
        Product*                product_ = nullptr;
        QVariantMap             product_data_;
        QVariantMap             products_data_;
        QNetworkAccessManager   nam_;
        //mutable QEventLoop      loop_;

        QTimer                  watchdog_;
        QElapsedTimer           elapsed_;
    };

    //////////////////////////////////////////////////////////////////////////

    inline int      compareVersions(const QString& ver1, const QString& ver2) {
        //Only Major.Minor.Micro compare;
        QStringList ver1_nums = ver1.split('.', Qt::SkipEmptyParts);
        QStringList ver2_nums = ver2.split('.', Qt::SkipEmptyParts);

        int size = qMin(ver1_nums.size(), ver2_nums.size());

        for (int i = 0; i < qMin(size, 3); ++i) {
            int v1_num = ver1_nums[i].toInt();
            int v2_num = ver2_nums[i].toInt();
            if (v1_num < v2_num)
                return -1;
            else if (v1_num > v2_num)
                return 1;
        }

        return 0;
    }

    ProductPrivate::ProductPrivate(const QString& product_name, const QString& company_name, const QString& company_settings_path, const QString& company_vending_url, Product* p) : product_(p)
        , product_name_(product_name)
        , company_name_(company_name)
        , company_settings_path_(company_settings_path)
        , company_vending_url_(company_vending_url) {
        initialize();
    }

    const QString& ProductPrivate::product_name() const {
        return product_name_;
    }

    const QString& ProductPrivate::product_id() {
        return product_id_;
    }

    const QString& ProductPrivate::price() {
        price_ = productProperty(Price).toString();
        return price_;
    }

    const QString& ProductPrivate::currency() {
        currency_ = productProperty(Currency).toString();
        return currency_;
    }

    const QString& ProductPrivate::product_url() {
        product_url_ = productProperty(ProductUrl).toString();
        return product_url_;
    }

    const QString& ProductPrivate::purchase_url() {
        purchase_url_ = productProperty(PurchaseUrl).toString();
        return purchase_url_;
    }

    const QString& ProductPrivate::version() {
        version_ = productProperty(Version).toString();
        return version_;
    }

    const QString& ProductPrivate::license_duration() {
        license_duration_ = productProperty(LicenseDuration).toString();
        return license_duration_;
    }

    void ProductPrivate::purchase() {
        QUrl url(purchase_url());
        if (url.isValid())
            QDesktopServices::openUrl(QUrl(url));
    }

    void ProductPrivate::openProductPage(const QString& company_url) {
        QUrl url(product_url());
        if (url.isValid())
            QDesktopServices::openUrl(QUrl(url));
        else
            QDesktopServices::openUrl(QUrl(company_url));
    }

    void ProductPrivate::initialize() {
        loadData();

        QObject::connect(&watchdog_, &QTimer::timeout, [=]() {
            requestData();
        });
        watchdog_.setInterval(EXPIRATION_CHECK_INTERVAL); // check every 15 min
        watchdog_.start();
        requestData(true);
    }

    void ProductPrivate::initProductsData(QVariantMap& map) {
        for (QVariantMap::const_iterator iter = map.begin(); iter != map.end(); ++iter) {
            QString key = iter.key();
            QMap<QString, QVariant> map = iter.value().toMap();
            auto it = map.find(ProductUrl);
            if (!key.isEmpty() && it != map.end()) {
                products_data_.insert(key, it.value());
            }
        }
    }

    void ProductPrivate::requestDataWithoutSSL() {
        QUrl url = QUrl(QString("%1").arg(company_vending_url_.replace("https:", "http:")));
        QNetworkRequest  request(url);
        qDebug() << "Request product data without SSL url:" << url;
        QNetworkReply* reply = nam_.get(request);

        QObject::connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QVariantMap map = QJsonDocument::fromJson(reply->readAll()).object().toVariantMap();
                initProductsData(map);
                if (!products_data_.isEmpty()) {
                    auto it = map.find(product_name_);
                    if (it != map.end())
                        product_data_ = it.value().toMap();

                    if (!product_data_.isEmpty()) {
                        product_data_[ExpirationDate] = QDateTime::currentDateTime().addSecs(EXPIRATION_PERIOD);  // period every 45 min
                        checkVersion();
                        saveProductsData();
                        saveData();
                    }
                    else {
                        status_.update(Status::NetError, reply->errorString());
                        emit product_->productDataError();
                    }
                }
                else {
                    status_.update(Status::NetError, reply->errorString());
                    emit product_->productDataError();
                }
            }
            else {
                status_.update(Status::NetError, reply->errorString());
                emit product_->productDataNetError();
            }

            reply->deleteLater();
        });
    }
    
    bool ProductPrivate::requestData(bool forced) {
        auto it_exp = product_data_.find(ExpirationDate);
       
        bool is_outdated = it_exp == product_data_.end()
            || QDateTime::currentDateTime() > it_exp->toDateTime();

        bool is_already_requested = it_exp != product_data_.end() 
                                        && it_exp->toDateTime().isNull();

        if ((is_outdated || forced)
            && !is_already_requested) {

            QUrl url = QUrl(QString("%1").arg(company_vending_url_));
            QNetworkRequest  request(url);
            qDebug() << "Request product data url:" << url;
            QNetworkReply* reply = nam_.get(request);

            status_ = Status::Success;
            //Make NULL date to skip reenter to request until finished;
            product_data_[ExpirationDate] = QDateTime();

            QObject::connect(reply, &QNetworkReply::finished, [=]() {
                //QEventLoopLocker locker(&loop_);
                product_data_.clear();
                products_data_.clear();

                if (reply->error() == QNetworkReply::NoError) {
                    QVariantMap map = QJsonDocument::fromJson(reply->readAll()).object().toVariantMap();
                    initProductsData(map);
                    if (!products_data_.isEmpty()) {
                        auto it = map.find(product_name_);
                        if (it != map.end())
                            product_data_ = it.value().toMap();

                        if (!product_data_.isEmpty()) {
                            product_data_[ExpirationDate] = QDateTime::currentDateTime().addSecs(EXPIRATION_PERIOD);  // period every 45 min
                            checkVersion();
                            saveProductsData();
                            saveData();
                        }
                        else {
                            requestDataWithoutSSL();
                        }
                    }
                    else {
                        requestDataWithoutSSL();
                    }
                } else {
                    status_.update(Status::NetError, reply->errorString());
                    emit product_->productDataNetError();
                }

                reply->deleteLater();
            });
        }
        return is_outdated || is_already_requested;
    }

    const QVariant& ProductPrivate::productProperty(const QString& name) {

        //if (requestData())
        //    //Wait until property is ready;
        //    loop_.exec();

        auto it = product_data_.find(name);

        if (it != product_data_.end())
            return *it;

        static QVariant value;
        return value;
    }

    void ProductPrivate::loadData() {
        QSettings       settings(company_settings_path_ + PRODUCTS_STORAGE_PATH, QSettings::NativeFormat);
        product_data_ = settings.value(product_name_).toMap();
    }

    void ProductPrivate::saveData() const {
        QSettings       settings(company_settings_path_ + PRODUCTS_STORAGE_PATH, QSettings::NativeFormat);
        settings.setValue(product_name_, product_data_);
    }

    void ProductPrivate::saveProductsData() const {
        QSettings       settings(company_settings_path_, QSettings::NativeFormat);
        settings.setValue(company_name_, products_data_);
    }

    void ProductPrivate::checkVersion() {
        if (productProperty(ProductName).toString() == product_name_) {
            qDebug() << "Checked remote version: " << productProperty(Version).toString();
            emit product_->updateAvailable(isUpdateAvailable());
        }
    }

    bool ProductPrivate::isUpdateAvailable() {
        if (UPDATE_ENABLED)
            return compareVersions(QApplication::applicationVersion(), productProperty(Version).toString()) < 0;
        else 
            return false;
    }

    bool ProductPrivate::isUpdateDownloaded() {
        bool result = false;

        if (isUpdateAvailable()) {
            QString update_path = updateDownloadPath();
            if (QFileInfo::exists(update_path))
                result = true;
        }
        else {
            QDir dir(QString("%1").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)) + UPDATE_STORAGE_PATH);
            dir.removeRecursively();
        }

        return result;
    }

    QString ProductPrivate::updateDownloadPath() {

        QString  update_dir = QString("%1").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)) + UPDATE_STORAGE_PATH;
        QDir dir(update_dir);

        if (!dir.exists())
            dir.mkpath(update_dir);

        return dir.filePath(QString("%1.exe").arg(productProperty(Version).toString()));
    }

    void ProductPrivate::downloadUpdate() {
        status_ = Status::Success;

        QDir dir(QString("%1").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)) + UPDATE_STORAGE_PATH);
        dir.removeRecursively();

        QString app_path = updateDownloadPath();
        QString temp_path = app_path + ".tmp";

        QFile* app_file = new QFile(temp_path, product_);

        if (app_file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QNetworkRequest request_app(productProperty(DownloadUrl).toString());

            request_app.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
            request_app.setSslConfiguration(QSslConfiguration::defaultConfiguration());
            request_app.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);
            request_app.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

            QNetworkReply* reply = nam_.get(request_app);
            elapsed_.start();
            //is_update_downloading_ = true;
            QObject::connect(reply, &QNetworkReply::finished, [=]() {

                app_file->close();

                if (reply->error() == QNetworkReply::NoError) {
                    if (app_file->rename(app_path)) {
                        emit product_->downloadFinished();
                    } else {
                        qDebug() << "Error in" << app_path << ":" << app_file->errorString();
                        status_.update(Status::IOError, app_file->errorString());
                        emit product_->updateError();
                    }

                } else {
                    qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                    status_.update(Status::NetError, reply->errorString());
                    app_file->remove();
                    emit product_->updateError();
                }
               
                app_file->deleteLater();
                reply->deleteLater();
               
            });

            QObject::connect(reply, &QNetworkReply::readyRead, [=]() {
                app_file->write(reply->readAll());
            });

            QObject::connect(reply, &QNetworkReply::downloadProgress, [=](qint64 bytesRead, qint64 bytesTotal) {
                if (elapsed_.hasExpired(300)) {
                    elapsed_.start();
                    if (bytesTotal > 0) {
                        int progress = (int)(bytesRead * 1000.0 / (double)bytesTotal);

                        float progress_float = progress / 10.f;
                        qDebug() << "Update: " << progress_float << "% : " << bytesRead / 1000000.0 << "/" << bytesTotal / 1000000.0 << " MB";
                        emit product_->downloadProgress(progress_float);
                    } else
                        qDebug() << "Update Downloading: " << bytesRead / 1000000.0 << " MB";
                }

            });
        } else {
            qDebug() << "Error in" << temp_path << ":" << app_file->errorString();
            status_.update(Status::IOError, app_file->errorString());
            emit product_->updateError();
        }
    }

    void ProductPrivate::installUpdate() {
        if (isUpdateDownloaded() &&
            !QProcess::startDetached(QString("\"%1\" /silent").arg(updateDownloadPath()))) {
            qDebug() << "Error run update file " << updateDownloadPath();
            status_.update(Status::IOError, "Can't install update file " + updateDownloadPath());
            emit product_->updateError();
        }
    }

    void ProductPrivate::downloadProductData() {
        requestData(true);
    }

    ////////////////////////////////////////////////////////////////////
    // Purchasing

    //Simple and vaste power way;
    const QString& currencySymbolByIso(const QString& iso_code) {
        static QMap<QString, QString>    symbols;

        if (symbols.empty()) {
            for (int    language = QLocale::Language::AnyLanguage + 1;
                 language < QLocale::Language::LastLanguage; ++language) {
                QLocale locale(static_cast<QLocale::Language>(language));
                QString sym = locale.currencySymbol(QLocale::CurrencySymbol);
                symbols.insert(
                    locale.currencySymbol(QLocale::CurrencyIsoCode),
                    locale.currencySymbol(QLocale::CurrencySymbol));
            }
        }

        return symbols[iso_code];

    }

    Product::Product(const QString& product_name, const QString& company_name, const QString& company_settings_path, const QString& company_vending_url, QObject* parent)
        : QObject(parent) {
        private_.reset(new ProductPrivate(product_name, company_name, company_settings_path, company_vending_url, this));
    }
    Product::~Product()
    { }

    const QString& Product::product_name() const {
        return private_->product_name();
    }

    const QString& Product::product_id() const {
        return private_->product_id();
    }

    const QString& Product::price() const {
        return private_->price();
    }

    const QString& Product::currency() const {
        return private_->currency();
    }

    QString Product::formattedPrice() const {
        QString currency = this->currency();
        if (currency.isEmpty())
            currency = "usd";
        QString currency_symbol = currencySymbolByIso(currency);

        return QString("%1%2").arg(currency_symbol).arg(price());
    }

    const QString& Product::product_url() const {
        return private_->product_url();
    }

    const QString& Product::purchase_url() const {
        return private_->purchase_url();
    }

    void Product::purchase() {
        private_->purchase();
    }

    void Product::openProductPage(const QString& company_url) {
        private_->openProductPage(company_url);
    }

    void Product::downloadUpdate() {
        private_->downloadUpdate();
    }

    void Product::installUpdate() {
        private_->installUpdate();
    }

    void Product::downloadProductData() {
        private_->downloadProductData();
    }

    bool Product::isUpdateDownloaded() const {
        return private_->isUpdateDownloaded();
    }

    bool Product::isUpdateAvailable() const {
        return private_->isUpdateAvailable();
    }

}


