#pragma once

#include <QtCore>
#include "status.h"

#include <openssl/aes.h>
#include <openssl/evp.h>

namespace vending {
    
class License : public QObject {
    friend class LicensePrivate;

    Q_OBJECT

public:
    ~License();

public:
    void                activate(const QString& email, const QString& license_key);
    void                verify(bool forced_activate = false);
    void                deactivate();
    void                firstTryEnded();
    bool                isFirstTry() const;
    bool                isValid() const;

    QString             registeredEmail() const;
    QString             registeredKey() const;
    QDateTime           expirationDate() const;
    QString             productId() const;

    Status              status() const;

/*  public:
    void                startTrial(qint64 seconds);
    void                stopTrial();
    bool                isTrial() const;

public:
    QString             generateKey(const QString& email, int product_id, const QString& private_key) const;
*/

signals:
    void                changed(bool forced = false);
    void                error();
    void                finished();
    //void                trialChanged(bool is_active);
        
public:
    explicit License(const QString& product_id, const QString& public_key, const QString& company_settings_path, QObject* parent = nullptr);

private:
    Q_DISABLE_COPY(License);
    QScopedPointer<LicensePrivate>           private_;
};

}
