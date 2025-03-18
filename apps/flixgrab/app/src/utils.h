#pragma once

#include <QtCore>
#include <QtGui>
#include <QtMultimedia>


inline void    showInGraphicalShell(const QString &pathIn) {
    const QString explorer = QLatin1String("explorer.exe");
    /*if (explorer.isEmpty()) {
        QMessageBox::warning(parent,
            tr("Launching Windows Explorer failed"),
            tr("Could not find explorer.exe in path to launch Windows Explorer."));
        return;
    }*/
    QString param;
    QFileInfo fileInfo(pathIn);
    if (QFileInfo(pathIn).exists()
        && fileInfo.isFile()) {
        param = QLatin1String("/select, ");
        param += QString("\"%1\"").arg(QDir::toNativeSeparators(pathIn).replace("/", "\\"));
    }
    else {
        param = QString("\"%1\"").arg(fileInfo.absolutePath().replace("/","\\"));
    }

    QString command = explorer + " " + param;
    QProcess::startDetached(command);
}

inline void    shutdownPC() {
    QProcess::startDetached("shutdown -s -f -t 00");
}

inline void playSound(const QUrl& path) {
#ifdef QT6
	QAudioOutput* audioOutput = new QAudioOutput;
	audioOutput->setVolume(100);

	QMediaPlayer* player = new QMediaPlayer;
	player->setSource(path);
	player->setAudioOutput(audioOutput);
	player->play();
#else // QT5
	QMediaPlayer* player = new QMediaPlayer;
	player->setMedia(path);
	player->setVolume(100);
	player->play();
#endif
}

inline QString getISOCode(QString lang) {
    lang = lang.toLower();
    if (lang == "afrikaans") {
        return "af";
    }
    else if (lang == "albanian") {
        return "sq";
    }
    else if (lang == "arabic") {
        return "ar";
    }
    else if (lang == "armenian") {
        return "hy";
    }
    else if (lang == "azerbaijani") {
        return "az";
    }
    else if (lang == "basque") {
        return "eu";
    }
    else if (lang == "belarusian") {
        return "be";
    }
    else if (lang == "bengali") {
        return "bn";
    }
    else if (lang == "bosnian") {
        return "bs";
    }
    else if (lang == "bulgarian") {
        return "bg";
    }
    else if (lang == "catalan") {
        return "ca";
    }
    else if (lang == "cebuano") {
        return "ceb";
    }
    else if (lang == "chinese") {
        return "zh-CN";
    }
    else if (lang == "croatian") {
        return "hr";
    }
    else if (lang == "czech") {
        return "cs";
    }
    else if (lang == "danish") {
        return "da";
    }
    else if (lang == "dutch") {
        return "nl";
    }
    else if (lang == "english") {
        return "en";
    }
    else if (lang == "esperanto") {
        return "eo";
    }
    else if (lang == "estonian") {
        return "et";
    }
    else if (lang == "filipino") {
        return "tl";
    }
    else if (lang == "finnish") {
        return "fi";
    }
    else if (lang == "french") {
        return "fr";
    }
    else if (lang == "galician") {
        return "gl";
    }
    else if (lang == "georgian") {
        return "ka";
    }
    else if (lang == "german") {
        return "de";
    }
    else if (lang == "greek") {
        return "el";
    }
    else if (lang == "gujarati") {
        return "gu";
    }
    else if (lang == "haitian creole") {
        return "ht";
    }
    else if (lang == "hausa") {
        return "ha";
    }
    else if (lang == "hebrew") {
        return "iw";
    }
    else if (lang == "hindi") {
        return "hi";
    }
    else if (lang == "hmong") {
        return "hmn";
    }
    else if (lang == "hungarian") {
        return "hu";
    }
    else if (lang == "icelandic") {
        return "is";
    }
    else if (lang == "igbo") {
        return "ig";
    }
    else if (lang == "indonesian") {
        return "id";
    }
    else if (lang == "irish") {
        return "ga";
    }
    else if (lang == "italian") {
        return "it";
    }
    else if (lang == "japanese") {
        return "ja";
    }
    else if (lang == "javanese") {
        return "jw";
    }
    else if (lang == "kannada") {
        return "kn";
    }
    else if (lang == "khmer") {
        return "km";
    }
    else if (lang == "korean") {
        return "ko";
    }
    else if (lang == "lao") {
        return "lo";
    }
    else if (lang == "latin") {
        return "la";
    }
    else if (lang == "latvian") {
        return "lv";
    }
    else if (lang == "lithuanian") {
        return "lt";
    }
    else if (lang == "macedonian") {
        return "mk";
    }
    else if (lang == "malay") {
        return "ms";
    }
    else if (lang == "maltese") {
        return "mt";
    }
    else if (lang == "maori") {
        return "mi";
    }
    else if (lang == "marathi") {
        return "mr";
    }
    else if (lang == "mongolian") {
        return "mn";
    }
    else if (lang == "nepali") {
        return "ne";
    }
    else if (lang == "norwegian") {
        return "no";
    }
    else if (lang == "persian") {
        return "fa";
    }
    else if (lang == "polish") {
        return "pl";
    }
    else if (lang == "portuguese") {
        return "pt";
    }
    else if (lang == "punjabi") {
        return "pa";
    }
    else if (lang == "romanian") {
        return "ro";
    }
    else if (lang == "russian") {
        return "ru";
    }
    else if (lang == "serbian") {
        return "sr";
    }
    else if (lang == "slovak") {
        return "sk";
    }
    else if (lang == "slovenian") {
        return "sl";
    }
    else if (lang == "somali") {
        return "so";
    }
    else if (lang == "spanish") {
        return "es";
    }
    else if (lang == "swahili") {
        return "sw";
    }
    else if (lang == "swedish") {
        return "sv";
    }
    else if (lang == "tamil") {
        return "ta";
    }
    else if (lang == "telugu") {
        return "te";
    }
    else if (lang == "thai") {
        return "th";
    }
    else if (lang == "turkish") {
        return "tr";
    }
    else if (lang == "ukrainian") {
        return "uk";
    }
    else if (lang == "urdu") {
        return "ur";
    }
    else if (lang == "vietnamese") {
        return "vi";
    }
    else if (lang == "welsh") {
        return "cy";
    }
    else if (lang == "yiddish") {
        return "yi";
    }
    else if (lang == "yoruba") {
        return "yo";
    }
    else if (lang == "zulu") {
        return "zu";
    }
    else
        return "";
}


static QString makeFilename(const QString& name) {
    const char forbidden[] = { '<', '>', ':', '\"', '/', '\\', '|', '?', '*', '\t', '\r', '\n' };
    const char* replace[] = { "(", ")", " -", "\'", "+", "+", "-", "", "+", " ", " ", " " };

    QString file_name = name;
    for (int i = 0; i < sizeof(forbidden) / sizeof(char); ++i) {
        file_name.replace(forbidden[i], replace[i]);
    }
    return file_name;
}


template<typename T>
inline T*   getHiddenChild(QWidget* parent) {

    T* widget = nullptr;
    if (parent) {
        auto it = std::find_if(parent->children().begin(), parent->children().end(), [](const QObject* obj) {
            if (auto target = qobject_cast<T const*>(obj)) {
                return target->isHidden();
            }
            return false;
        });

        if (it != parent->children().end()) {
            widget = qobject_cast<T*>(const_cast<QObject*>(*it));
        }
    }

    return widget;
}

static QAction*        createAction(const QString& text, const QVariant& data, QObject* parent = nullptr) {
    QAction* action = new QAction(text, parent);
    action->setData(data);
    return action;
}

