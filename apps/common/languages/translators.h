
#ifndef __TRANSLATORS_H
#define __TRANSLATORS_H
#include <qobject.h>

#include <QTranslator>
#include <QStringList>
#include <QtWidgets/QApplication>
#include <QHash>
#include <QVector>

namespace lang {

class CTranslators : public QObject 
{
    Q_OBJECT
    typedef QVector <QTranslator*> TTranslatorCont;
public:
     /*!
     * \fn  CTranslators::CTranslators(const QStringList & qsListQm, QObject *pParent = 0) : CTranslators("
     * \brief   Constructor.
     * \param   qsListQm        The qs list qm.
     * \param [in,out]  pParent (Optional) The parent.
     */
    CTranslators(const QStringList & qsListQm, QObject *pParent = 0) :
        CTranslators(":/languages", qsListQm, pParent)
    {}


     /*!
     * \fn  CTranslators::CTranslators(const QString & qsPath, const QStringList & qsListQm, QObject *parent = 0)
     * \brief   Constructor.
     * \param   qsPath          Full pathname of the qs file.
     * \param   qsListQm        The qs list qm.
     * \param [in,out]  parent  (Optional) The parent.
     */
    CTranslators(const QString & qsPath, const QStringList & qsListQm, QObject *parent = 0) : QObject(parent),
        m_qsPath(qsPath),
        m_qsEnLangPrefix("en"),
        m_qsPatternNameBaseQMFile(""),
        m_qsEnWord("English"),
        m_qslQmList(qsListQm),
        m_qsEnContext("SupportLanguages")
    {
        Q_ASSERT(qsListQm.size() > 0);
        m_qsPatternNameBaseQMFile = qsListQm[0];
    }


     /*!
     * \fn  CTranslators::~CTranslators()
     * \brief   Destructor.
     */
    ~CTranslators()
    {
    }


     /*!
     * \fn  bool CTranslators::switchLang(const std::string & sLang);
     * \brief   Switch language.
     * \param   sLang   The language.
     * \return  true if it succeeds, false if it fails.
     */
    bool switchLang(const std::string & sLang);


     /*!
     * \fn  bool CTranslators::switchLang(const QString & qsLang);
     * \brief   Switch language.
     * \param   qsLang  The qs language.
     * \return  true if it succeeds, false if it fails.
     */
    bool switchLang(const QString & qsLang);


     /*!
     * \fn  QStringList CTranslators::supportedLangs(void);
     * \brief   Get supported langs.
     * \return  A QStringList.
     */
    QStringList supportedLangs(void) ;


     /*!
     * \fn  QString CTranslators::fullLangName(const QString & qsLang) const;
     * \brief   Full language name.
     * \param   qsLang  The qs language.
     * \return  A QString.
     */
    QString fullLangName(const QString & qsLang) const;

private:
    inline bool isInstallTrans() const;
    inline bool isEnLang(const QString & qsLang) const;
    void installTrans();
    void removeTrans();
    bool loadLang(const QString & qsLang) const;

    TTranslatorCont        m_vpTrans;

private:    
    QString                m_qsPath;
    QString                m_qsPatternNameBaseQMFile;
    const QString          m_qsEnLangPrefix; 
    const QString          m_qsEnWord;
    const QString          m_qsEnContext;
    const QStringList      m_qslQmList;
    QStringList            m_qslSupportLangs; 
};


     /*!
     * \fn  std::string getShortLangByFullName(const std::string & sFullLang);
     * \brief   Get short language by full name.
     * \param   sFullLang   The full language.
     * \return  The short language by full name.
     */
    std::string getShortLangByFullName(const std::string & sFullLang);


     /*!
     * \fn  QString getShortLangByFullLangName(const QString& qsFullLang);
     * \brief   Get short language by full language name.
     * \param   qsFullLang  The qs full language.
     * \return  The short language by full language name.
     */
    QString getShortLangByFullLangName(const QString& qsFullLang);


     /*!
     * \fn  QString getFullLangByShortName(const QString & qsShortLang);
     * \brief   Get full language by short name.
     * \param   qsShortLang The qs short language.
     * \return  The full language by short name.
     */
    QString getFullLangByShortName(const QString & qsShortLang);


     /*!
     * \fn  std::string getFullLangByShortName(const std::string & sShortLang);
     * \brief   Get full language by short name.
     * \param   sShortLang  The short language.
     * \return  The full language by short name.
     */
    std::string getFullLangByShortName(const std::string & sShortLang);


     /*!
     * \fn  QHash <QString, int> getLanguageSortOrger();
     * \brief   Get language sort orger.
     * \return  The language sort orger.
     */
    QHash <QString, int> getLanguageSortOrger();


     /*!
     * \fn  std::string getDvdvideosoftLang();
     * \brief   Get dvdvideosoft language.
     * \return  The dvdvideosoft language.
     */
    std::string getDvdvideosoftLang();

} //namespace lang

#endif // TRANSLATORS_H
