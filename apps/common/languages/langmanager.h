#ifndef __LANG_MANAGER_H
#define __LANG_MANAGER_H

#include <QString>
#include <QSettings>
#include <QStringList>
#include <QDateTime>

#include "../app/app_defs.h"


namespace lang {

    static const char ss_KeyTimeModifyLang[]    = "TimeModifyCurrentCulture";


 /*!
 * \enum    EStorageLangType
 * \brief   Values that represent storage language types.
 */
enum EStorageLangType
{
    eStorageLangTypeAuto = 0,
    eStorageLangTypeGlobal ,
    eStorageLangTypeLocal ,
};

class CLangManager
{
public:
     /*!
     * \fn  CLangManager::CLangManager(const QString & sRootLocal, const QStringList & vSupportedLangs);
     * \brief   Constructor.
     * \param   sRootLocal      The root local.
     * \param   vSupportedLangs The supported langs.
     */
    CLangManager(const QString & sRootLocal, const QStringList & vSupportedLangs);


     /*!
     * \fn  virtual CLangManager::~CLangManager();
     * \brief   Destructor.
     */
    virtual ~CLangManager();
public:
     /*!
     * \fn  bool CLangManager::getLang(QString &sLang, EStorageLangType & nLangStorage);
     * \brief   Get a language.
     * \param [in,out]  sLang           The language.
     * \param [in,out]  nLangStorage    The language storage.
     * \return  true if it succeeds, false if it fails.
     */
    bool getLang(QString &sLang, EStorageLangType & nLangStorage);


     /*!
     * \fn  bool CLangManager::setLang(const QString &sLang, EStorageLangType nLangStorage = eStorageLangTypeAuto);
     * \brief   Set a language.
     * \param   sLang           The language.
     * \param   nLangStorage    The language storage.
     * \return  true if it succeeds, false if it fails.
     */
    bool setLang(const QString &sLang, EStorageLangType nLangStorage = eStorageLangTypeAuto);
private:
     /*!
     * \fn  bool CLangManager::isCommonsLang(const QString & sLang);
     * \brief   Query if 'sLang' is commons language.
     * \param   sLang   The language.
     * \return  true if commons language, false if not.
     */
    bool isCommonsLang(const QString & sLang);


     /*!
     * \fn  bool CLangManager::isSupportedLang(const QString & sLang);
     * \brief   Query if 'sLang' is supported language.
     * \param   sLang   The language.
     * \return  true if supported language, false if not.
     */
    bool isSupportedLang(const QString & sLang);
    //QString getRegistryPath(const QString& sPath , const QString& sKey);


     /*!
     * \fn  bool CLangManager::getValue(const QString& sPath , const QString& sKey , QVariant & value);
     * \brief   Get a value.
     * \param   sPath           Full pathname of the file.
     * \param   sKey            The key.
     * \param [in,out]  value   The value.
     * \return  true if it succeeds, false if it fails.
     */
    bool    getValue(const QString& sPath , const QString& sKey , QVariant & value);


     /*!
     * \fn  bool CLangManager::setValue(const QString& sPath , const QString& sKey , const QVariant & value);
     * \brief   Set a value.
     * \param   sPath   Full pathname of the file.
     * \param   sKey    The key.
     * \param   value   The value.
     * \return  true if it succeeds, false if it fails.
     */
    bool    setValue(const QString& sPath , const QString& sKey , const QVariant & value);


     /*!
     * \fn  bool CLangManager::getLocalLang (QString &sLang);
     * \brief   Get local language.
     * \param [in,out]  sLang   The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool    getLocalLang (QString &sLang);


     /*!
     * \fn  bool CLangManager::getGlobalLang(QString &sLang);
     * \brief   Get global language.
     * \param [in,out]  sLang   The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool    getGlobalLang(QString &sLang);


     /*!
     * \fn  bool CLangManager::setLocalLang (const QString &sLang);
     * \brief   Set local language.
     * \param   sLang   The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool    setLocalLang (const QString &sLang);


     /*!
     * \fn  bool CLangManager::setGlobalLang(const QString &sLang);
     * \brief   Set global language.
     * \param   sLang   The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool    setGlobalLang(const QString &sLang);


     /*!
     * \fn  bool CLangManager::getTimeModifyGlobalLang(qint64 &nTime);
     * \brief   Get time modify global language.
     * \param [in,out]  nTime   The time.
     * \return  true if it succeeds, false if it fails.
     */
    bool    getTimeModifyGlobalLang(qint64 &nTime);


     /*!
     * \fn  bool CLangManager::getTimeModifyLocalLang (qint64 &nTime);
     * \brief   Get time modify local language.
     * \param [in,out]  nTime   The time.
     * \return  true if it succeeds, false if it fails.
     */
    bool    getTimeModifyLocalLang (qint64 &nTime);


     /*!
     * \fn  bool CLangManager::setTimeModifyGlobalLang(qint64 &nTime);
     * \brief   Set time modify global language.
     * \param [in,out]  nTime   The time.
     * \return  true if it succeeds, false if it fails.
     */
    bool    setTimeModifyGlobalLang(qint64 &nTime);


     /*!
     * \fn  bool CLangManager::setTimeModifyLocalLang (qint64 &nTime);
     * \brief   Set time modify local language.
     * \param [in,out]  nTime   The time.
     * \return  true if it succeeds, false if it fails.
     */
    bool    setTimeModifyLocalLang (qint64 &nTime);

private:
    QString     m_sRootLocal;
    QStringList m_vSupportedLangs;
    const  QString m_sRootGlobal;
    const  QString m_sLangRegValueName;
    const  QString m_sKeyTimeModifyLang;

};

} //namespace lang 
#endif // LANG_MANAGER_H