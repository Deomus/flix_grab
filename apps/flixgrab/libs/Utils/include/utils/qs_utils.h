#ifndef UI_COMMON_STR_QS_UTILS_H
#define UI_COMMON_STR_QS_UTILS_H

#include <QTime>

#include "str_manip.h"
#include <qstring.h>

//CString to QString
#define CS2Q(x) (QString::fromUtf16((const ushort *)(LPCWSTR)(x)))
//wstring to QString
#define SS2Q(x) (QString::fromLatin1((x).c_str()))
#define UTF82Q(x) (QString::fromUtf8((x).c_str()))

#define CS2US(x) ((const ushort *)(LPCWSTR)(x))
#define QS2W(x) ((const wchar_t *)((x).utf16()))

#define WC2Q(x) (QString::fromUtf16((const ushort *)(const wchar_t *)(x)))
#define WS2Q(x) (QString::fromUtf16((const ushort *)(x).c_str()))

#ifndef QS_STR_MACRO_DEFINED
#define QS_STR_MACRO_DEFINED
#define QS2S(s) std::string(((const char *)(s).toUtf8().data()))
#endif

namespace s {

inline QString qsFromString(const std::string str) {
    return str.c_str();
}


inline const char * qs2chars(const QString& str) {
    return (const char *)str.toUtf8().data();
}


inline std::string qs2str(const QString& str) {
    return (const char *)str.toUtf8().data();
}

}


namespace str {

/*!
 * \fn  inline std::string secondsToTime(long long duration)
 * \brief   Seconds to time.
 * \param   duration   The duration.
 * \return  A std::string.
 */
inline std::string secondsToTime(long long duration) {
    int days = (int)duration / (24 * 3600);
    int hoursAndMins = ((int)duration - days * 24 * 3600);
    int hours = hoursAndMins / 3600;
    int minsAndSecs = hoursAndMins - hours * 3600;
    int mins = minsAndSecs / 60;

    int secs = 0;
    if (days == 0 && hours == 0 && mins == 0)
        secs = duration;
    else
        secs = minsAndSecs % 60;

    QTime qt(hours, mins, secs, 0);
    QString qstime = qt.toString();
    std::string result = qstime.toUtf8().data();

    if (days)
        result.insert(0, str::intToString(days) + ":");

    size_t pos = -1;
    do {
        pos = result.find("00:");
        if (pos == 0)
            result.erase(0, 3);
        else
            break;
    } while (true);

    if (result.find("0") == 0)
        result.erase(0, 1);

    if (result.find(":") == std::string::npos)
        result.insert(0, "0:");

    return result;
}

}

#endif // UI_COMMON_STR_QS_UTILS_H
