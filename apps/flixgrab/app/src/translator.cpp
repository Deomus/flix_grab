#include "translator.h"

#include <QDir>

//#include <fs/file_utils.h>
//#include <appfx/appfx_utils.h>
#include "utils/qs_utils.h"

//#ifdef Q_OS_WIN
//#include "../../../common/sys/win32/platform_helpers.h"
//#endif


namespace lang {

void CTranslator::installTrans() {
    if (!vp_trans_.empty()) {
        Q_ASSERT(vp_trans_.size() == qm_list_.size());
        return;
    }

    for (int i = 0; i < qm_list_.size(); ++i) {
        QTranslator* pQtTranslator = new QTranslator(this);
        qApp->installTranslator(pQtTranslator);
        vp_trans_.push_back(pQtTranslator);
    }
}

void CTranslator::removeTrans() {
    std::for_each(vp_trans_.begin(), vp_trans_.end(), [](QTranslator* pTrans) { qApp->removeTranslator(pTrans); });
    vp_trans_.clear();
}


bool CTranslator::loadLang(const QString & lang) const {
    Q_ASSERT(vp_trans_.size() == qm_list_.size());
    bool b_ret = true;
    size_t i = 0;
    std::for_each(vp_trans_.begin(), vp_trans_.end(), [=, &b_ret, &i](QTranslator* pTrans) {
        if (!pTrans->load(QString(qm_list_[i]).arg(lang), path_))
            b_ret = (b_ret && false);

        i++;
    });
    return b_ret;
}

bool CTranslator::isInstallTrans() const {
    return !vp_trans_.empty();
}

bool CTranslator::switchLang(const std::string& sLang) {
    return switchLang(s::qsFromString(sLang));
}

bool CTranslator::switchLang(const QString& lang) {
    // If the language is EN, remove the translators    
    if (lang == en_short_language_) {
        if (isInstallTrans())
            removeTrans();

        return true;
    }

    if (!isInstallTrans())
        installTrans();

    return loadLang(lang);
}

QStringList CTranslator::supportedLangs(void) {
    if (supported_langs_.isEmpty()) {
        QHash<QString, int> sortOrder = getLanguageSortOrger();

        for (int n = 0; n < sortOrder.count(); n++)
            supported_langs_.append(sortOrder.key(n));
    }

    return supported_langs_;
}

QString CTranslator::fullLangName(const std::string& lang) const {
    if (en_short_language_.compare(lang.c_str()) == 0)
        return english_full_language_name_;

    QTranslator qTranslator;
    QString languageLabel;
    if (!qTranslator.load(base_qm_name_pattern_.arg(lang.c_str()), path_))
        return languageLabel;

    languageLabel = qTranslator.translate(en_context_.toUtf8().data(), english_full_language_name_.toUtf8().data());
    return languageLabel;
}

QString CTranslator::fullLangName(const QString& lang) const {
    if (en_short_language_ == lang)
        return english_full_language_name_;

    QTranslator qTranslator;
    QString languageLabel;
    if (!qTranslator.load(base_qm_name_pattern_.arg(lang), path_))
        return languageLabel;

    languageLabel = qTranslator.translate(en_context_.toUtf8().data(), english_full_language_name_.toUtf8().data());

    return languageLabel;
}

std::string getShortLangByFullName(const std::string& full_lang_name) {
    if (full_lang_name == "de-DE")
        return "de";
    if (full_lang_name == "da-DK")
        return "da";
    if (full_lang_name == "ja-JP")
        return "ja";
    if (full_lang_name == "fr-FR")
        return "fr";
    if (full_lang_name == "es-ES")
        return "es";
    if (full_lang_name == "pt-PT")
        return "pt";
    if (full_lang_name == "pt-BR")
        return "br";
    if (full_lang_name == "ru-RU")
        return "ru";
    if (full_lang_name == "zh-CN")
        return "cn";
    if (full_lang_name == "zh-TW")
        return "tw";
    if (full_lang_name == "it-IT")
        return "it";
    if (full_lang_name == "nl-NL")
        return "nl";
    if (full_lang_name == "pl-PL")
        return "pl";
    if (full_lang_name == "el-GR")
        return "gr";
    if (full_lang_name == "tr-TR")
        return "tr";
    if (full_lang_name == "hu-HU")
        return "hu";
    if (full_lang_name == "sv-SE")
        return "sv";
    if (full_lang_name == "sl-SI")
        return "si";
    if (full_lang_name == "fi-FI")
        return "fi";
    if (full_lang_name == "sk-SK")
        return "sk";
    if (full_lang_name == "mn-MN")
        return "mn";
    if (full_lang_name == "vi-VN")
        return "vn";
    if (full_lang_name == "th-TH")
        return "th";
    if (full_lang_name == "hi-IN")
        return "hi";
    if (full_lang_name == "ko-KR")
        return "ko";
    if (full_lang_name == "cs-CZ")
        return "cs";
    
    return "en";
}

QString getShortLangByFullLangName(const QString& full_lang_name) {
    return s::qsFromString(getShortLangByFullName(full_lang_name.toLatin1().data()));
}

std::string getFullLangByShortName(const std::string& sShortLang) {
    if (sShortLang == "de")
        return "de-DE";
    else if (sShortLang == "da")
        return "da-DK";
    else if (sShortLang == "ja")
        return "ja-JP";
    else if (sShortLang == "fr")
        return "fr-FR";
    else if (sShortLang == "es")
        return "es-ES";
    else if (sShortLang == "pt")
        return "pt-PT";
    else if (sShortLang == "br")
        return "pt-BR";
    else if (sShortLang == "ru")
        return "ru-RU";
    else if (sShortLang == "cn")
        return "zh-CN";
    else if (sShortLang == "tw")
        return "zh-TW";
    else if (sShortLang == "it")
        return "it-IT";
    else if (sShortLang == "nl")
        return "nl-NL";
    else if (sShortLang == "pl")
        return "pl-PL";
    else if (sShortLang == "gr")
        return "el-GR";
    else if (sShortLang == "tr")
        return "tr-TR";
    else if (sShortLang == "hu")
        return "hu-HU";
    else if (sShortLang == "dk")
        return "da-DK";
    else if (sShortLang == "sv")
        return "sv-SE";
    else if (sShortLang == "si")
        return "sl-SI";
    else if (sShortLang == "fi")
        return "fi-FI";
    else if (sShortLang == "sk")
        return "sk-SK";
    else if (sShortLang == "mn")
        return "mn-MN";
    else if (sShortLang == "vn")
        return "vi-VN";
    else if (sShortLang == "th")
        return "th-TH";
    else if (sShortLang == "ru")
        return "ru-RU";
    else if (sShortLang == "hi")
        return "hi-IN";
    else if (sShortLang == "ko")
        return "ko-KR";
    else if (sShortLang == "cs")
        return "cs-CZ";

    return "en-US";
}

QString getFullLangByShortName(const QString& qsShortLang) {
    return getFullLangByShortName(std::string(qsShortLang.toLatin1().data())).c_str();
}

QHash <QString, int> getLanguageSortOrger() {
    QHash <QString, int>  sortOrder;

    sortOrder.insert("en", 0);  //English
    sortOrder.insert("cs", 1);  //Czech
    sortOrder.insert("da", 2);  //Dansk
    sortOrder.insert("de", 3);  //Deutsch
    sortOrder.insert("es", 4);  //Espan
    sortOrder.insert("fr", 5);  //French
    sortOrder.insert("it", 6);  //Italian
    sortOrder.insert("nl", 7);  //Netherlands
    sortOrder.insert("pl", 8); //Polish
    sortOrder.insert("pt", 9); //Portuguese
    sortOrder.insert("ru", 10); //Russian
    sortOrder.insert("sv", 11); //Swedish
    sortOrder.insert("tr", 12); //Turkish
    sortOrder.insert("cn", 13); //Chinese simplified
    sortOrder.insert("tw", 14); //Chinese traditional
    sortOrder.insert("hi", 15); //Hindi
    sortOrder.insert("ko", 16); //Korean
    sortOrder.insert("ja", 17); //Japanese
    //sortOrder.insert("gr", 4);  //Greek
    //sortOrder.insert("hu", 8);  //Hungarian

    //sortOrder.insert("br", 12); //Portuguese brasilian
    //sortOrder.insert("sk", 14); //Slovak
    //sortOrder.insert("si", 15); //Sloven
    //sortOrder.insert("fi", 16);  //Finnish
    //sortOrder.insert("vn", 18); //Vietnamese
    //sortOrder.insert("th", 20);  //Thai

    return sortOrder;
}

} //namespace lang
