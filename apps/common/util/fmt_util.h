#ifndef UI_COMMON_FMT_UTIL_H
#define UI_COMMON_FMT_UTIL_H

#include <QtCore/qstring.h>
#include <QtCore/qfile.h>

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

namespace fmt
{

inline int strToInt(const QString &strValue, int defaultVal)
{
    bool ok = false;
    int nRet = strValue.toInt(&ok);
    if (ok)
        return nRet;

    return defaultVal;
}

}

#endif // UI_COMMON_FMT_UTIL_H
