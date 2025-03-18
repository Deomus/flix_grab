#pragma once

#include <string>
#include <qobject.h>
#include <QTranslator>
#include <QStringList>
#include <QtWidgets/QApplication>
#include <QHash>
#include <QVector>

namespace lang {

class CTranslator : public QObject 
{
    Q_OBJECT
    typedef QVector <QTranslator*> TTranslatorCont;
public:
    /*!
     * \fn  CTranslator::CTranslator(const QStringList& qm_list, QObject *pParent = 0) : CTranslator("
     * \brief   Constructor.
     * \param   qm_list        List of QMs.
     * \param [in,out]  pParent (Optional) The parent.
     */
    CTranslator(const QStringList& qm_list, QObject *pParent = 0) :
        CTranslator(":/languages", qm_list, pParent) {}


    /*!
     * \fn  CTranslator::CTranslator(const QString& qsPath, const QStringList& qm_list, QObject *parent = 0)
     * \brief   Constructor.
     * \param   qsPath          Full pathname of the qs file.
     * \param   qm_list        The qs list qm.
     * \param [in,out]  parent  (Optional) The parent.
     */
    CTranslator(const QString & qsPath, const QStringList & qm_list, QObject *parent = 0)
        : QObject(parent)
        , path_(qsPath)
        , en_short_language_("en")
        , base_qm_name_pattern_("")
        , english_full_language_name_("English")
        , qm_list_(qm_list)
        , en_context_("SupportLanguages") {

        Q_ASSERT(qm_list.size() > 0);
        base_qm_name_pattern_ = qm_list[0];
    }

    /*!
     * \fn  CTranslator::~CTranslator()
     * \brief   Destructor.
     */
    ~CTranslator() {}

    /*!
     * \fn  bool CTranslators::switchLang(const std::string& short_lang);
     * \brief   Switch language.
     * \param   short_lang   The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool switchLang(const std::string& short_lang);

    /*!
     * \fn  bool CTranslator::switchLang(const QString& short_lang);
     * \brief   Switch language.
     * \param   short_lang  The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool switchLang(const QString& short_lang);

    /*!
     * \fn  QStringList CTranslator::supportedLangs(void);
     * \brief   Get supported langs.
     * \return  A QStringList.
     */
    QStringList supportedLangs(void) ;

    /*!
     * \fn  QString CTranslator::fullLangName(const QString& lang) const;
     * \brief   Full language name.
     * \param   lang  Short lang name.
     * \return  Full name like 'English'.
     */
    QString fullLangName(const QString& lang) const;

    /*!
     * \fn  QString CTranslators::fullLangName(const std::string& lang) const;
     * \brief   Full language name.
     * \param   lang  Short lang name.
     * \return  Full name like 'English'.
     */
    QString fullLangName(const std::string& lang) const;

private:
    inline bool isInstallTrans() const;
    void installTrans();
    void removeTrans();
    bool loadLang(const QString& lang) const;

    TTranslatorCont        vp_trans_;

private:    
    QString                path_;
    QString                base_qm_name_pattern_;
    const QString          en_short_language_; 
    const QString          english_full_language_name_;
    const QString          en_context_;
    const QStringList      qm_list_;
    QStringList            supported_langs_; 
};


/*!
 * \fn  QString getShortLangByFullName(const QString& full_lang_name);
 * \brief   Get short language by full name.
 * \param   full_lang_name   The full language.
 * \return  The short language by full name.
 */
std::string getShortLangByFullName(const std::string& full_lang_name);

/*!
 * \fn  QString getShortLangByFullLangName(const QString& full_lang_name);
 * \brief   Get short language by full language name.
 * \param   full_lang_name  The qs full language.
 * \return  The short language by full language name.
 */
QString getShortLangByFullLangName(const QString& full_lang_name);


/*!
 * \fn  QString getFullLangByShortName(const QString & qsShortLang);
 * \brief   Get full language by short name.
 * \param   qsShortLang The qs short language.
 * \return  The full language by short name.
 */
QString getFullLangByShortName(const QString& qsShortLang);

/*!
 * \fn  std::string getFullLangByShortName(const std::string & sShortLang);
 * \brief   Get full language by short name.
 * \param   sShortLang  The short language.
 * \return  The full language by short name.
 */
std::string getFullLangByShortName(const std::string& sShortLang);

/*!
 * \fn  QHash <QString, int> getLanguageSortOrger();
 * \brief   Get language sort orger.
 * \return  The language sort orger.
 */
QHash <QString, int> getLanguageSortOrger();


/*!
 * \fn  QString getFGALang();
 * \brief   Get dvdvideosoft language.
 * \return  The dvdvideosoft language.
 */
std::string getFGALang();

} //namespace lang
