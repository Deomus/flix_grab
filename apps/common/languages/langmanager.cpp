#include "langmanager.h"

#include "translators.h"
#include "lang_util.h"

namespace lang {

CLangManager::CLangManager(const QString & sRootLocal, const QStringList & vSupportedLangs)
    : m_sRootGlobal(fga::kLangRegKey)
    , m_sLangRegValueName(fga::kLangRegValueName)
    , m_sKeyTimeModifyLang(ss_KeyTimeModifyLang)
{
    m_sRootLocal      = sRootLocal;
    m_vSupportedLangs = vSupportedLangs;
}


CLangManager::~CLangManager()
{
}


bool CLangManager::isSupportedLang(const QString & sLang)
{
    QString sShortNameLang = getShortLangByFullLangName(sLang);
    return (m_vSupportedLangs.contains(sShortNameLang));
}


bool CLangManager::isCommonsLang(const QString& sLang)
{
    return lang::isSupportedCulture(sLang.toUtf8().data());
}


bool CLangManager::getValue(const QString& sPath , const QString& sKey , QVariant & value)
{
    QSettings settings(sPath, QSettings::NativeFormat);
    value = settings.value(sKey);
    return  !value.isNull();
}


bool CLangManager::setValue(const QString & sPath , const QString & sKey , const QVariant & value)
{
    QSettings settings(sPath, QSettings::NativeFormat);
    settings.setValue(sKey, value);
    return true;
}


bool CLangManager::getLocalLang(QString &sLang)
{
    bool bRet = false;
    QVariant value;
    if (getValue(m_sRootLocal,m_sLangRegValueName,value))
    {
        sLang = value.toString();
        bRet = true;
    }
    return bRet;
}


bool CLangManager::getGlobalLang(QString &sLang)
{
    bool bRet = false;
    QVariant value;
    if (getValue(m_sRootGlobal,m_sLangRegValueName,value))
    {
        sLang = value.toString();
        bRet = true;
    }
    return bRet;
}


bool   CLangManager::setLocalLang (const QString &sLang)
{
    QVariant value(sLang);
    return setValue(m_sRootLocal,m_sLangRegValueName,value);
}


bool   CLangManager::setGlobalLang(const QString &sLang)
{
    QVariant value(sLang);
    return setValue(m_sRootGlobal,m_sLangRegValueName,value);
}


bool  CLangManager::getTimeModifyGlobalLang(qint64 &nTime)
{
    bool bRet = false;
    QVariant value;
    if (getValue(m_sRootGlobal,m_sKeyTimeModifyLang,value))
    {
        if (value.canConvert<qint64>())
        {
            nTime = value.value<qint64>();
            bRet = true;
        }
    }
    return bRet;
}


bool  CLangManager::getTimeModifyLocalLang (qint64 &nTime)
{
    bool bRet = false;
    QVariant value;
    if (getValue(m_sRootLocal,m_sKeyTimeModifyLang,value))
    {
        if (value.canConvert<qint64>())
        {
            nTime = value.value<qint64>();
            bRet = true;
        }
    }
    return bRet;
}


bool  CLangManager::setTimeModifyGlobalLang(qint64 &nTime)
{
    QVariant value(nTime);
    return setValue(m_sRootGlobal,m_sKeyTimeModifyLang,value);
}


bool  CLangManager::setTimeModifyLocalLang (qint64 &nTime)
{
    QVariant value(nTime);
    return setValue(m_sRootLocal,m_sKeyTimeModifyLang,value);
}


bool CLangManager::getLang(QString &sLang, EStorageLangType & eLangType)
{
    bool bRet = true;
    if (eLangType == eStorageLangTypeGlobal)
    {
        bRet = getGlobalLang(sLang);
    }
    else if (eLangType == eStorageLangTypeLocal)
    {
        bRet = getLocalLang(sLang);
    }
    else if (eLangType == eStorageLangTypeAuto)
    {
        qint64 nTimeModifyGlobalLang = 0;
        qint64 nTimeModifyLocalLang  = 0;
        QString sGlobalLang;
        QString sLocalLang;

        bool bGlTime   = getTimeModifyGlobalLang(nTimeModifyGlobalLang);
        bool bLocTime  = getTimeModifyLocalLang(nTimeModifyLocalLang);
        bool bGlLang   = getGlobalLang(sGlobalLang);
        bool bLocLang  = getLocalLang(sLocalLang);

        if (bGlTime && bLocTime && bGlLang && bLocLang)
        {
            if (nTimeModifyGlobalLang > nTimeModifyLocalLang)
            {
                sLang = sGlobalLang;
                eLangType = eStorageLangTypeGlobal;
            }
            else
            {
                sLang = sLocalLang;
                eLangType = eStorageLangTypeLocal;
            }
        }
        else if (bGlLang)
        {
            sLang = sGlobalLang;
            eLangType = eStorageLangTypeGlobal;
        }
        else if (bLocLang)
        {
            sLang = sLocalLang;
            eLangType = eStorageLangTypeLocal;
        }
        else
        {
            bRet = false;
        }

    }
    return bRet;
}


bool CLangManager::setLang(const QString &sLang , EStorageLangType eLangType )
{
    bool bRet = true;

    if (eLangType == eStorageLangTypeGlobal)
    {
        bRet = setGlobalLang(sLang);
    }
    else if (eLangType == eStorageLangTypeLocal)
    {
        bRet = setLocalLang(sLang);
    }
    else if (eLangType == eStorageLangTypeAuto)
    {
        qint64 nNow = QDateTime::currentDateTime().toMSecsSinceEpoch();

        if (isCommonsLang(sLang))
        {
            bRet = setGlobalLang(sLang) && setTimeModifyGlobalLang(nNow);
        }
        else
        {
            bRet = setLocalLang(sLang) && setTimeModifyLocalLang(nNow);
        }
    }
    return bRet;
}

} //namespace lang 
