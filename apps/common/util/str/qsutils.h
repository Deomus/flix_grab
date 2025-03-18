#ifndef UI_COMMON_STR_UTILS_H
#define UI_COMMON_STR_UTILS_H

#include "str_manip.h"

//CString to QString
#define CS2Q(x) (QString::fromUtf16((const ushort *)(LPCWSTR)(x)))
//wstring to QString
#define WS2Q(x) (QString::fromUtf16((const ushort *)(x).c_str()))
#define WC2Q(x) (QString::fromUtf16((const ushort *)(const wchar_t *)(x)))
#define SS2Q(x) (QString::fromLatin1((x).c_str()))
#define UTF82Q(x) (QString::fromUtf8((x).c_str()))

#define CS2US(x) ((const ushort *)(LPCWSTR)(x))
#define QS2W(x) ((const wchar_t *)((x).utf16()))
#define QS2C(x) ((const char *)(x).toUtf8().data())

#ifndef QS_STR_MACRO_DEFINED
#define QS_STR_MACRO_DEFINED
#   ifdef WIN32
#       define QS2TSTR(s) (s).toStdWString()
#       define QS_FROM_TSTR(s) QString::fromStdWString(s)
#   else
#       define QS2TSTR(s) (s).toStdString()
#       define QS_FROM_TSTR(s) QString::fromStdString(s)
#   endif
#endif

#include <QTime>

namespace str
{

inline std::string secondsToTime(long long nDuration)
{
    int days = (int)nDuration / (24 * 3600);
    int hoursAndMins = ((int)nDuration - days * 24 * 3600);
    int hours = hoursAndMins / 3600;
    int minsAndSecs = hoursAndMins - hours * 3600;
    int mins = minsAndSecs / 60;

    int secs = 0;
    if (days == 0 && hours == 0 && mins == 0)
        secs = nDuration;
    else
        secs = minsAndSecs % 60;

    QTime qt(hours, mins, secs, 0);
    QString qstime = qt.toString();
    std::string sRes = qstime.toUtf8().data();

    if (days)
        sRes.insert(0, str::intToString(days) + ":");

    size_t pos = -1;
    do
    {
        pos = sRes.find("00:");
        if (pos == 0)
            sRes.erase(0, 3);
        else
            break;
    } while (true);

    if (sRes.find("0") == 0)
        sRes.erase(0, 1);

    if (sRes.find(":") == NPP)
        sRes.insert(0, "0:");

    return sRes;
}

}

#endif // UI_COMMON_STR_UTILS_H
