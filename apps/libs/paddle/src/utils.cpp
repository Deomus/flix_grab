#include "utils.h"

#include <QtNetwork>

namespace paddle{

    namespace utils {

      

        //Finds the MAC address of the NIC with maximum speed.
        static QString getMacAddress();


        QString paddle::utils::getMachineUUID() {
            QSettings setting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Cryptography", QSettings::Registry64Format);
            QString mGuid = setting.value("MachineGuid").toString();
            if (mGuid == "")
                mGuid = getMacAddress();
            return mGuid;
        }

        static QString getMacAddress() {
            foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
                // Return only the first non-loopback MAC Address
                if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
                    return netInterface.hardwareAddress();
            }
            return QString();
        }
               

        //Simple and vaste power way;
        QString getCurrencySymbolByIso(const QString& iso_code) {
            static QMap<QString, QString>    symbols;

            if (symbols.empty()) {
                for (int    language = QLocale::Language::AnyLanguage+1; 
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

    }
}


