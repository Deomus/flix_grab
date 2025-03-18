#pragma once


#include "web_portal.h"


static AudioProfile netflixAudioProfile(const QString& profile_str) {
    const QList<QString>
        profile_strs = { "heaac-2-piff", "heaac-2-dash", "playready-heaac-2-dash", "heaac-2-elem", "heaac-2hq-dash", "heaac-2-dash-alt",
                            "ddplus-2.0-dash",
                            "heaac-5.1-dash",
                            "dd-5.1-elem", "ddplus-5.1-dash", "ddplus-5.1-piff", "ddplus-5.1hq-dash",
                            "ddplus-atmos-dash" };
    const AudioProfile
        profile_ids[] = { AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC, AudioProfile::AAC,
                            AudioProfile::DolbyDigital20,
                            AudioProfile::AAC51,
                            AudioProfile::DolbyDigital, AudioProfile::DolbyDigitalPlus, AudioProfile::DolbyDigitalPlus, AudioProfile::DolbyDigitalPlus,
                            AudioProfile::DolbyDigitalPlusAtmos };
    int index = profile_strs.indexOf(profile_str);
    return index >= 0 ? profile_ids[index] : AudioProfile::UnknownProfile;
}

static SubtitleTrackInfo::SubtitleProfile subtitleProfile(const QString& profile_str) {
    const QList<QString>
        profile_strs = { "simplesdh", "dfxp-ls-sdh", "nflx-cmisc" };
    const SubtitleTrackInfo::SubtitleProfile
        profile_ids[] = { SubtitleTrackInfo::SimpleSdh, SubtitleTrackInfo::LessSimpleSdh, SubtitleTrackInfo::NflxCmisc };
    int index = profile_strs.indexOf(profile_str);
    return index >= 0 ? profile_ids[index] : SubtitleTrackInfo::Unknown;
}

class NetflixAkiraScript;

class NetflixInterceptor : public WebPortal {
    Q_OBJECT

public:
    NetflixInterceptor(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* p = nullptr);
    ~NetflixInterceptor();

private:
    void initialize() override;
    void startPage(bool forced_reload_page) override;
    void finishPage(bool ok) override;
    void interceptRequest(QWebEngineUrlRequestInfo& info) override;
    bool parserExists() override { return true; }

private:
    enum EInterceptFlags {
        kTracksReady = 0,
        kVideoInfoReady_ = 0x1,
    };

    bool checkInterceptFlag(int value, int flag) {
        return (value == flag);
    }

    void setFlag(EInterceptFlags flag);

    bool is_tracks_ready_ = false;
    bool is_video_info_ready_ = false;

private:
    Q_INVOKABLE void interceptConfig(const QString& jsonStr, const QString& confName) override;
    Q_INVOKABLE void interceptPlayerScript(const QUrl& url);
    Q_INVOKABLE void interceptBootstrap(const QUrl& url);
    Q_INVOKABLE void interceptAkiraClient(const QUrl& url);
    Q_INVOKABLE void interceptLoginController(const QUrl& url);
    Q_INVOKABLE void interceptHTML(const QUrl& url);
    Q_INVOKABLE void interceptMetadata(const QUrl& url);

private:
    void parseMetadata(const QString& metadata);
    void parseManifest(const QJsonObject& manifest);
    void parseVideoTracks(const QJsonArray& tracks);
    void parseAudioTracks(const QJsonArray& tracks);
    void parseTextTracks(const QJsonArray& tracks);
    void clearTracksInfo() {
        tracks_info_.audio_tracks.clear();
        tracks_info_.all_audio_tracks.clear();
        tracks_info_.audio_languages.clear();
        tracks_info_.all_audio_languages.clear();
        tracks_info_.video_tracks.clear();
        tracks_info_.image_tracks.clear();
        tracks_info_.subtitle_tracks.clear();
    }

private:
    QPointer<WebContent>            html_;
    QPointer<WebContent>            metadata_;
    QPointer<WebContent>            player_script_;
    QPointer<WebContent>            bootstrap_script_;
    QPointer<NetflixAkiraScript>    akira_script_;
    QPointer<WebContent>            loginController_script_;
    QWebEnginePage*                 page_ = nullptr;
    QString                         base_url_;
};
