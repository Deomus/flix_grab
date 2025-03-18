#include "translators.h"

#include <QDir>

#include "app/app_defs.h"

#ifdef Q_OS_WIN
#include "util/sys/platform_helpers.h"
#endif

namespace lang {

    void CTranslators::installTrans()
    {
        if (!m_vpTrans.empty())
        {
            Q_ASSERT(m_vpTrans.size() == m_qslQmList.size());
            return;
        }
        for (int i = 0; i < m_qslQmList.size(); ++i)
        {
            QTranslator* pQtTranslator = new QTranslator(this);
            qApp->installTranslator(pQtTranslator);
            m_vpTrans.push_back(pQtTranslator);
        }
    }


    void CTranslators::removeTrans()
    {
        std::for_each(m_vpTrans.begin(), m_vpTrans.end(), [](QTranslator* pTrans)
        {
            qApp->removeTranslator(pTrans);
        });
        m_vpTrans.clear();
    }


    bool CTranslators::loadLang(const QString & qsLang) const
    {
        Q_ASSERT(m_vpTrans.size() == m_qslQmList.size());
        bool b_ret = true;
        size_t i = 0;
        std::for_each(m_vpTrans.begin(), m_vpTrans.end(), [=, &b_ret, &i](QTranslator* pTrans)
        {
            if (!pTrans->load(QString(m_qslQmList[i]).arg(qsLang), m_qsPath))
            {
                b_ret = (b_ret && false);
            }
            i++;
        });
        return b_ret;
    }


    bool CTranslators::isInstallTrans() const
    {
        return !m_vpTrans.empty();
    }


    bool CTranslators::isEnLang(const QString & qsLang) const
    {
        if (qsLang == m_qsEnLangPrefix)
            return true;
        return false;
    }


    bool CTranslators::switchLang(const std::string & sLang)
    {
        return switchLang(QString::fromStdString(sLang));
    }


    bool CTranslators::switchLang(const QString & qsLang)
    {
        // If the language is EN, remove the translators
        if (isEnLang(qsLang))
        {
            if (isInstallTrans())
                removeTrans();
            return true;
        }

        if (!isInstallTrans())
            installTrans();

        return loadLang(qsLang);
    }


    QStringList CTranslators::supportedLangs(void)
    {
        if (m_qslSupportLangs.size() == 0)
        {
            QHash <QString, int> sortOrder = getLanguageSortOrger();
            for (int n = 0; n < sortOrder.count(); n++)
            {
                m_qslSupportLangs.append(sortOrder.key(n));
            }
        }
        return m_qslSupportLangs;
    }


    QString CTranslators::fullLangName(const QString& qsLang) const
    {
        if (isEnLang(qsLang))
            return m_qsEnWord;
        QTranslator qTranslator;
        QString languageLabel;
        if (!qTranslator.load(m_qsPatternNameBaseQMFile.arg(qsLang), m_qsPath))
        {
            return languageLabel;
        }
        languageLabel = qTranslator.translate(m_qsEnContext.toUtf8().data(), m_qsEnWord.toUtf8().data());
        return languageLabel;
    }


    std::string getShortLangByFullName(const std::string & sFullLang)
    {
        if (sFullLang == "de-DE")
            return "de";
        //if (sFullLang == "ja-JP")
        //    return "ja";
        //if (sFullLang == "fr-FR")
        //    return "fr";
        //if (sFullLang == "es-ES")
        //    return "es";
        //if (sFullLang == "pt-PT")
        //    return "pt";
        //if (sFullLang == "pt-BR")
        //    return "br";
        if (sFullLang == "ru-RU")
            return "ru";
        //if (sFullLang == "zh-CN")
        //    return "cn";
        //if (sFullLang == "zh-TW")
        //    return "tw";
        //if (sFullLang == "it-IT")
        //    return "it";
        //if (sFullLang == "nl-NL")
        //    return "nl";
        //if (sFullLang == "pl-PL")
        //    return "pl";
        //if (sFullLang == "el-GR")
        //    return "gr";
        //if (sFullLang == "tr-TR")
        //    return "tr";
        //if (sFullLang == "hu-HU")
        //    return "hu";
        //if (sFullLang == "da-DK")
        //    return "dk";
        //if (sFullLang == "sv-SE")
        //    return "sv";
        //if (sFullLang == "sl-SI")
        //    return "si";
        //if (sFullLang == "fi-FI")
        //    return "fi";
        //if (sFullLang == "sk-SK")
        //    return "sk";
        //if (sFullLang == "mn-MN")
        //    return "mn";
        //if (sFullLang == "vi-VN")
        //    return "vn";
        //if (sFullLang == "th-TH")
        //    return "th";
        return "en";
    }


    QString getShortLangByFullLangName(const QString& qsFullLang)
    {
        return QString::fromStdString(getShortLangByFullName(qsFullLang.toLatin1().data()));
    }


    std::string getFullLangByShortName(const std::string & sShortLang)
    {
        if (sShortLang == "de")
            return "de-DE";
 /*       else if (sShortLang == "ja")
            return "ja-JP";
        else if (sShortLang == "fr")
            return "fr-FR";
        else if (sShortLang == "es")
            return "es-ES";
        else if (sShortLang == "pt")
            return "pt-PT";
        else if (sShortLang == "br")
            return "pt-BR";*/
        else if (sShortLang == "ru")
            return "ru-RU";
        //else if (sShortLang == "cn")
        //    return "zh-CN";
        //else if (sShortLang == "tw")
        //    return "zh-TW";
        //else if (sShortLang == "it")
        //    return "it-IT";
        //else if (sShortLang == "nl")
        //    return "nl-NL";
        //else if (sShortLang == "pl")
        //    return "pl-PL";
        //else if (sShortLang == "gr")
        //    return "el-GR";
        //else if (sShortLang == "tr")
        //    return "tr-TR";
        //else if (sShortLang == "hu")
        //    return "hu-HU";
        //else if (sShortLang == "dk")
        //    return "da-DK";
        //else if (sShortLang == "sv")
        //    return "sv-SE";
        //else if (sShortLang == "si")
        //    return "sl-SI";
        //else if (sShortLang == "fi")
        //    return "fi-FI";
        //else if (sShortLang == "sk")
        //    return "sk-SK";
        //else if (sShortLang == "mn")
        //    return "mn-MN";
        //else if (sShortLang == "vn")
        //    return "vi-VN";
        //else if (sShortLang == "th")
        //    return "th-TH";
        //else if (sShortLang == "ru")
        //    return "ru-RU";

        return "en-US";
    }


    QString getFullLangByShortName(const QString& qsShortLang)
    {
        return getFullLangByShortName(std::string(qsShortLang.toLatin1().data())).c_str();
    }


    QHash <QString, int> getLanguageSortOrger()
    {
        QHash <QString, int>  sortOrder;

        sortOrder.insert("en", 1);  //English
        sortOrder.insert("de", 2);  //Deutsch
        //sortOrder.insert("ru", 3); //Russian

        //sortOrder.insert("dk", 2);  //Dansk

        //sortOrder.insert("gr", 4);  //Greek
        //sortOrder.insert("es", 5);  //Espan
        //sortOrder.insert("fr", 6);  //French
        //sortOrder.insert("it", 7);  //Italian
        //sortOrder.insert("hu", 8);  //Hungarian
        //sortOrder.insert("nl", 9);  //Netherlands
        //sortOrder.insert("pl", 10); //Polish
        //sortOrder.insert("pt", 11); //Portuguese
        //sortOrder.insert("br", 12); //Portuguese brasilian

        //sortOrder.insert("sk", 14); //Slovak
        //sortOrder.insert("si", 15); //Sloven
        //sortOrder.insert("fi", 16);  //Finnish
        //sortOrder.insert("sv", 17); //Svenska - sweden
        //sortOrder.insert("vn", 18); //Vietnamese
        //sortOrder.insert("tr", 19); //Turkish
        //sortOrder.insert("th", 20);  //Thai
        //sortOrder.insert("ja", 21); //Japanese
        //sortOrder.insert("cn", 22); //Chinese simplified
        //sortOrder.insert("tw", 23); //Chinese traditional
        return sortOrder;
    }


    std::string getDvdvideosoftLang()
    {
        std::string sLang = "en";
#ifdef Q_OS_WIN
        std::string sCulture = sys::readRegString(fga::kLangRegKey, fga::kLangRegValueName);
        if (sCulture.size() > 0)
            sLang = lang::getShortLangByFullName(sCulture);//?
#endif

        return sLang;
    }


} //namespace lang

