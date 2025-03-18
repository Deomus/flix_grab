#pragma once

#include <QtCore>

#include <product_config.h>
#include <product_info.h>

#include "video_item_controller.h"


namespace settings {

    static const char    kLanguage[]                = "Language";
    static const char    kPostDownloadAction[]      = "PostDownloadAction";
    static const char    kOutputFolder[]            = "OutputFolder";
    static const char    kDownloadThreads[]         = "DownloadThreads";
    static const char    kEncodingSpeed[]           = "EncodingSpeed";
    static const char    kSaveAs[]                  = "SaveAs";
    static const char    kAudioLanguage[]           = "AudioLanguage";
    static const char    kAudioType[]               = "AudioType";
    static const char    kSubtitleLanguage[]        = "SubtitleLanguage";
    static const char    kSaveSubtitlesAs[]         = "SaveSubtitlesAs";
    
    static const char    kVideoStream[]             = "VideoStream";
    static const char    kAudioStream[]             = "AudioStream";
    static const char    kImageStream[]             = "ImageStream";
    static const char    kListPath[]                = "ListPath";

    static const char    kProxyType[]               = "ProxyType";
    static const char    kProxyAddress[]            = "ProxyAddress";
    static const char    kProxyPort[]               = "ProxyPort";
    static const char    kProxyCred[]               = "ProxyCred";
    static const char    kProxyRetryLoadInterval[]  = "ProxyRetryLoadInterval";

    static const char    kLeftPanelVisible[]        = "LeftPanelVisible";
    static const char    kDownloadsPanelVisible[]   = "DownloadsPanelVisible";
    static const char    kFiltersPanelVisible[]     = "FiltersPanelVisible";
    static const char    kSortPanelVisible[]        = "SortPanelVisible";
    static const char    kProxyPanelVisible[]       = "ProxyPanelVisible";
    static const char    kTutorialVisible[]         = "TutorialVisible";
    static const char    kDeleteAccounts[]          = "DeleteAccounts";

    static const char    kLanguageDefault[]         = "en-US";
    static const char    kAudioLanguageDefault[]    = "en-us";
    static const char    kSubtitleLanguageDefault[] = "none";


    static  void        createDefaults() {
        QSettings           storage;
        qRegisterMetaType<QList<QString>>("strs");
        qRegisterMetaType<QList<int>>("ints");
#ifdef QT5
		qRegisterMetaTypeStreamOperators<QList<QString>>("strs");
		qRegisterMetaTypeStreamOperators<QList<int>>("ints");
#endif

        //user menu settings
        if (!storage.contains(kLanguage)
            || storage.value(kLanguage).toString().isEmpty())
            storage.setValue(kLanguage, kLanguageDefault);

        //menu settings

        if (!storage.contains(kPostDownloadAction)) {
            QList<int> selected_postdownloadaction_items({ kDoNothing });
            storage.setValue(kPostDownloadAction, QVariant::fromValue(selected_postdownloadaction_items));
        }

        if (!storage.contains(kOutputFolder)
            || storage.value(kOutputFolder).toString().isEmpty()) {

            if (DEFAULTPRESET < kPreset_MP3Original)
                storage.setValue(kOutputFolder, QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
            else
                storage.setValue(kOutputFolder, QStandardPaths::writableLocation(QStandardPaths::MusicLocation));

            if (QString(PRODUCT_NAME) == "FreeInstagramDownload")
                storage.setValue(kOutputFolder, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
        }

        if (THREADSSELECTORENABLED) {
            int kDownloadThreadsMax = 7;
            if (!storage.contains(kDownloadThreads))
                storage.setValue(kDownloadThreads, 3);
            else if (storage.value(kDownloadThreads).toInt() > kDownloadThreadsMax)
                storage.setValue(kDownloadThreads, kDownloadThreadsMax);
        }

        if (ENCODINGSELECTORENABLED) {
            if (!storage.contains(kEncodingSpeed))
                storage.setValue(kEncodingSpeed, kEncodingSelector_Superfast);
        }

        if (VIDEOSELECTORENABLED) {
            if (!storage.contains(kVideoStream))
                storage.setValue(kVideoStream, kVideoSelector_HighQuality);
        }

        if (AUDIOSELECTORENABLED) {
            if (!storage.contains(kAudioStream))
                storage.setValue(kAudioStream, kAudioSelector_HighQuality);
        }

        if (IMAGESELECTORENABLED) {
            if (!storage.contains(kImageStream))
                storage.setValue(kImageStream, kImageSelector_HighQuality);
        }

        if (SAVEASSELECTORENABLED) {
            if (!storage.contains(kSaveAs))
                storage.setValue(kSaveAs, DEFAULTPRESET);
        }

        if (AUDIOTYPESELECTORENABLED) {
            if (!storage.contains(kAudioType))
                storage.setValue(kAudioType, kAudioType_Original);
        }

        if (AUDIOLANGSELECTORENABLED) {
            if (!storage.contains(kAudioLanguage)) {
                QList<QString> selected_audio_languages({ QString(settings::kAudioLanguageDefault) });
                storage.setValue(kAudioLanguage, QVariant::fromValue(selected_audio_languages));
            }
        }

        if (SUBTITLESELECTORENABLED) {
            if (!storage.contains(kSubtitleLanguage)) {
                QList<QString> selected_subtitle_languages({ QString(settings::kSubtitleLanguageDefault) });
                storage.setValue(kSubtitleLanguage, QVariant::fromValue(selected_subtitle_languages));
            }

            if (!storage.contains(kSaveSubtitlesAs)) {
                QList<int> selected_savesubtitlesas_items({ kSaveSubtitlesAs_Original });
                storage.setValue(kSaveSubtitlesAs, QVariant::fromValue(selected_savesubtitlesas_items));
            }
        }

        //common settings
        if (!storage.contains(kListPath)
            || storage.value(kListPath).toString().isEmpty())
            storage.setValue(kListPath, QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QDir::separator() + PRODUCT_NAME +".lst"));

        //visible settings
        if (!storage.contains(kLeftPanelVisible))
            storage.setValue(kLeftPanelVisible, 0);

        if (!storage.contains(kDownloadsPanelVisible))
            storage.setValue(kDownloadsPanelVisible, 0);

        if (!storage.contains(kFiltersPanelVisible))
            storage.setValue(kFiltersPanelVisible, 0);

        if (!storage.contains(kSortPanelVisible))
            storage.setValue(kSortPanelVisible, 0);

        if (!storage.contains(kProxyPanelVisible))
            storage.setValue(kProxyPanelVisible, 0);
        
        if (!storage.contains(kTutorialVisible))
            storage.setValue(kTutorialVisible, 1);

        if (!storage.contains(kDeleteAccounts))
            storage.setValue(kDeleteAccounts, 0);
        
        if (!storage.contains(kProxyType))
            storage.setValue(kProxyType, 0);

        if (!storage.contains(kProxyAddress))
            storage.setValue(kProxyAddress, "");

        if (!storage.contains(kProxyPort))
            storage.setValue(kProxyPort, "");

        if (!storage.contains(kProxyCred))
            storage.setValue(kProxyCred, "");

        if (!storage.contains(kProxyRetryLoadInterval))
            storage.setValue(kProxyRetryLoadInterval, 0);
    }
}