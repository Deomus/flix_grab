#pragma once

#include <QtCore>

namespace paddle {

    namespace utils {

        // Gets an ID we can use to identify this machine for our licenses later on.
        QString getMachineUUID();
        
        //Gets the currency used by the given country (that Paddle supports).
        QString getCurrencyForCountry();

        QString getCurrencySymbolByIso(const QString& iso_code);

    }

}
