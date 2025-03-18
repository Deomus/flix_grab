#include "netflix_interceptor.h"

#include "utils/qs_utils.h"

NetflixInterceptor::NetflixInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p) 
    : WebPortal(page, nam, p)
    , page_(page) {

    Q_INIT_RESOURCE(scripts);
}

NetflixInterceptor::~NetflixInterceptor() {
    qDebug() << "Interceptor: ~NetflixInterceptor";
}

class NetflixAkiraScript : public WebContent {
public:
    NetflixAkiraScript(QObject* p = nullptr) : WebContent(p) {}

protected:
    Status  modifyContent() override {
        qDebug() << "Interceptor: akira injection...";

#ifdef _DEBUG
        QFile   file("akira.js");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(mutableData().toLocal8Bit());
            file.close();
        }
#endif
        auto size_bkup = mutableData().size();

        QRegularExpression rx_recap("if\\(\\w\\.type===\\w\\.RECAP_PREPLAY\\|\\|\\w\\.type===\\w\\.TITLE_GROUP_RECAP\\)");
        QRegularExpressionMatch match_recap = rx_recap.match(this->data());
        int pos_recap = -1;
        if (match_recap.hasMatch())
            pos_recap = match_recap.capturedStart();

        QRegularExpression rx_tutorial("if\\(\\w\\.type===\\w\\.TUTORIAL_PREPLAY\\)");
        QRegularExpressionMatch match_tutorial = rx_tutorial.match(this->data());
        int pos_tutorial = -1;
        if (match_tutorial.hasMatch())
            pos_tutorial = match_tutorial.capturedStart();

        int pos_preplay = -1;

        if (pos_recap >= 0
            && pos_tutorial >= 0
            && pos_tutorial > pos_recap) {

            mutableData().insert(pos_recap, "/*");
            mutableData().insert(pos_tutorial + 1, "*/");

            QRegularExpression rx_preplay("if\\(\\w\\.type===\\w\\.prePlayTypes.RECAP_PREPLAY\\|\\|\\w\\.type===\\w\\.prePlayTypes.TITLE_GROUP_RECAP\\)");
            QRegularExpressionMatch match_preplay = rx_preplay.match(this->data());

            if (match_preplay.hasMatch())
                pos_preplay = match_preplay.capturedStart();

            if (pos_preplay >= 0) {
                mutableData().insert(pos_preplay, "/*");
                QString keyword = "if";
                pos_preplay = pos_preplay + 3;
                pos_preplay = mutableData().indexOf(keyword, pos_preplay);
                mutableData().insert(mutableData().indexOf(keyword, pos_preplay), "*/");
            }
        }

        auto ok = size_bkup != mutableData().size();
        qDebug() << "Injected  akira.js. Newsize: " << mutableData().size() << (!ok ? "Failed to inject" : ". OK") << " in position " << pos_recap << " in pos_tutorial " << pos_tutorial << " in pos_preplay " << pos_preplay;

#ifdef _DEBUG
        QFile   jf("akira_injected.js");
        if (jf.open(QIODevice::ReadWrite)) {
            jf.resize(0);
            QTextStream(&jf) << mutableData() << Qt::endl;
            jf.close();
        }
#endif
        return EStatusCode::Success;
    }

    bool    isModifyAsync() const override {
        return true;
    }
};

void NetflixInterceptor::initialize() {
    auto initContent = [this](WebContent* content) {
        content->setNetworkAccess(networkAccess());
        QObject::connect(content, &WebContent::finished, this, [=]() {
            if (!content->status())
                emit error(content->status());
        });
    };

    html_ = new WebContent(this);
    initContent(html_);

    metadata_ = new WebContent(this);
    initContent(metadata_);

    //Main core script;
    //player_script_ = new NetflixPlayerScript(this);
    player_script_ = new WebContent(this);
    initContent(player_script_);

    //Bootstrap ui;
    bootstrap_script_ = new WebContent(this);
    initContent(bootstrap_script_);

    //Akira UI;
    akira_script_ = new NetflixAkiraScript(this);
    //akira_script_ = new WebContent(this);
    initContent(akira_script_);

    loginController_script_ = new WebContent(this);
    initContent(loginController_script_);

    QObject::connect(metadata_, &WebContent::finished, this, [=]() {
        parseMetadata(metadata_->data().toLocal8Bit());
    }, Qt::QueuedConnection);

//#ifdef _DEBUG
    //QObject::connect(html_, &WebContent::finished, this, [=]() {
    //    QFile   file("main.html");
    //    if (file.open(QIODevice::ReadWrite)) {
    //        file.resize(0);
    //        file.write(loginController_script_->data().toLocal8Bit());
    //        file.close();
    //    }
    //}, Qt::QueuedConnection);
    //QObject::connect(loginController_script_, &WebContent::finished, this, [=]() {
    //    QFile   file("loginController_script.js");
    //    if (file.open(QIODevice::ReadWrite)) {
    //        file.resize(0);
    //        file.write(loginController_script_->data().toLocal8Bit());
    //        file.close();
    //    }
    //}, Qt::QueuedConnection);

    //QObject::connect(player_script_, &WebContent::finished, this, [=]() {
    //    QFile   file("playcore_new.js");
    //    if (file.open(QIODevice::ReadWrite)) {
    //        file.resize(0);
    //        file.write(player_script_->data().toLocal8Bit());
    //        file.close();
    //    }
    //}, Qt::QueuedConnection);

    //QObject::connect(bootstrap_script_, &WebContent::finished, this, [=]() {
    //    QFile   file("bootstrap_new.js");
    //    if (file.open(QIODevice::ReadWrite)) {
    //        file.resize(0);
    //        file.write(bootstrap_script_->data().toLocal8Bit());
    //        file.close();
    //    }
    //}, Qt::QueuedConnection);
//#endif
    qDebug() << "NetflixInterceptor: initialized.";
}

void NetflixInterceptor::startPage(bool forced_reload_page) {
    qDebug() << "Interceptor: startPage";
    Q_UNUSED(forced_reload_page);

    invalidate();
    html_->reset();
    metadata_->reset();
    player_script_->reset();
    bootstrap_script_->reset();
    akira_script_->reset();
    loginController_script_->reset();

    base_url_.clear();
    is_tracks_ready_ = false;
    is_video_info_ready_ = false;
}

void NetflixInterceptor::finishPage(bool ok) {
    if (ok)
        scripts_.run(page_, true);
}

Q_INVOKABLE void NetflixInterceptor::interceptHTML(const QUrl& url) {
    html_->fetch(url);
}

Q_INVOKABLE void NetflixInterceptor::interceptMetadata(const QUrl& url) {
    metadata_->fetch(url);
}

void NetflixInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
    
    QUrl url = info.requestUrl();
    QString qstr = url.toEncoded();
    auto method = QString::fromLatin1(info.requestMethod());
    qDebug() << "####### Request url: " << url.fileName() << "   " << method << "   " << qstr << "   " << info.resourceType() << "   signed_in_: " << signed_in_;
    
    info.setHttpHeader("User-Agent", USER_AGENT);
    
    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
        base_url_ = qstr;
        //QMetaObject::invokeMethod(this, "interceptHTML", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        //info.block(true);
    }

    if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript) {
        qDebug() << "###### " << QString::fromLatin1(info.requestMethod()) << ": " << qstr;
        if (qstr.contains("playercore", Qt::CaseInsensitive) && !url.isLocalFile()) {
            qDebug() << "Interceptor: Core Script " << QFileInfo(qstr).fileName();

            QMetaObject::invokeMethod(this, "interceptPlayerScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            //Block loading of original script;
            info.block(true);
        }
        else if (qstr.contains("akiraClient.js", Qt::CaseInsensitive)) {
            qDebug() << "Interceptor: Akira: " << qstr;
            QMetaObject::invokeMethod(this, "interceptAkiraClient", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }
        //else if (qstr.contains("bootstrap.js", Qt::CaseInsensitive)) {
        //    qDebug() << "Bootstrap: " << qstr;
        //    QMetaObject::invokeMethod(this, "interceptBootstrap", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        //    info.block(true);
        //}
        //else if (qstr.contains("loginControllerClient.js", Qt::CaseInsensitive)) {
        //    //return;
        //    qDebug() << "Interceptor: loginControllerClient: " << qstr;
        //    QMetaObject::invokeMethod(this, "interceptLoginController", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
        //    info.block(true);
        //}
        else if (qstr.contains("nflxext.com", Qt::CaseInsensitive)) {
            qDebug() << "Interceptor: Script intercepted: " << qstr;

            QMetaObject::invokeMethod(this, "interceptDeferScript", Qt::QueuedConnection, Q_ARG(const QUrl&, url));
            info.block(true);
        }
        else if (qstr.contains("gstatic.com", Qt::CaseInsensitive)
            || qstr.contains("recaptcha", Qt::CaseInsensitive)
            || qstr.contains("bootstrap.js", Qt::CaseInsensitive)) {
            qDebug() << "Interceptor: Script not blocked: " << qstr;
            // don't block;
        }
        else {
            qDebug() << "Interceptor: Script blocked: " << qstr;
            info.block(true);
        }
    }
    else if (qstr.contains("ProfilesGate")) { // https://www.netflix.com/ProfilesGate
        emit showWebEngine(ERequestType::profile);
    }

    /* if (qstr.contains("ichnaea/log", Qt::CaseInsensitive) ) {
        qDebug() << "Check self state '" << qstr << "'";
        emit initialFinished();
    }*/

    if (qstr.contains("metadata", Qt::CaseInsensitive)
        && video_info_.video_title.isEmpty()) {
        //Found Video Description Url;
        QUrlQuery query(url);
        QString id = query.queryItemValue("movieid");
        if (page_) {
            QString base_url = page_->url().toEncoded();
            QString base_id = base_url.split("/").last();
            qDebug() << "Interceptor: skip intercept metadata id: " << id << " base_id: " << base_id << " new_url: " << url;
        }

        QString metadata_url = qstr.replace("imageFormat=webp", "imageFormat=jpg");
        QMetaObject::invokeMethod(this, "interceptMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, metadata_url));
    }

    //Block all logging activity, to hide downloading process;
    if (qstr.contains("track/debug", Qt::CaseInsensitive) ||
        qstr.contains("cadmium/logblob", Qt::CaseInsensitive) ||
        qstr.contains("ichnaea", Qt::CaseInsensitive)) {
        info.block(true);
        qDebug() << "Request blocked: " << qstr;
    }

    if (qstr.contains("manifest", Qt::CaseInsensitive)) {
        qDebug() << "Interceptor: Manifest has been detected.";
        emit hideWebEngine(ERequestType::profile);
    }
}

//////////////////////////////////////////////////////////////////////////
//class NetflixPlayerScript : public WebContent {
//public:
//    NetflixPlayerScript(QObject* p = nullptr) : WebContent(p) {}
//
//protected:
//    Status  modifyContent() override {
//        qDebug() << "Interceptor: Player injection";
//        return Status::Success;// parser::injectPlayer(mutableData());
//    }
//
//    bool    isModifyAsync() const override {
//        return true;
//    }
//};
//
//

Q_INVOKABLE void NetflixInterceptor::interceptPlayerScript(const QUrl& url) {
    Q_UNUSED(url);
    //player_script_->fetch(url);
#if _DEBUG
    player_script_->fetch(QUrl("qrc:///scripts/netflix/playcore_debug.js"));
#else
    player_script_->fetch(QUrl("qrc:///scripts/netflix/playcore.js"));
#endif

    scripts_.addAsync(player_script_);
    scripts_.run(page_);
}

Q_INVOKABLE void NetflixInterceptor::interceptBootstrap(const QUrl& url) {
    Q_UNUSED(url);
    //bootstrap_script_->fetch(url);
    
    bootstrap_script_->fetch(QUrl("qrc:///scripts/netflix/bootstrap.js"));
    scripts_.addDefer(bootstrap_script_);
}

Q_INVOKABLE void NetflixInterceptor::interceptAkiraClient(const QUrl& url) {
    //Q_UNUSED(url);
    akira_script_->fetch(url);
    //if (url.toEncoded().contains("v3561408"))
    //    akira_script_->fetch(QUrl("qrc:///scripts/netflix/akira-js-v3561408c.js"));
    //else
    //    akira_script_->fetch(QUrl("qrc:///scripts/netflix/akira-js-v82208092.js"));

    scripts_.addDefer(akira_script_);
}

Q_INVOKABLE void NetflixInterceptor::interceptLoginController(const QUrl& url) {
    Q_UNUSED(url);
    //loginController_script_->fetch(url);
    loginController_script_->fetch(QUrl("qrc:///scripts/netflix/loginControllerClient.js"));
    scripts_.addDefer(loginController_script_);
}

Q_INVOKABLE void NetflixInterceptor::interceptConfig(const QString& jsonStr, const QString& confName) {
    qDebug() << "interceptConfig: jsonStr.length()=" << jsonStr.length();

#ifdef _DEBUG
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   manifest_file("netflix_manifest_" + QString::number(ms) + ".json");
    if (manifest_file.open(QIODevice::ReadWrite)) {
        manifest_file.resize(0);
        QTextStream(&manifest_file) << jsonStr << Qt::endl;
    }
#endif

    QTextDocument text;
    text.setHtml(jsonStr);
    QString plain_json = text.toPlainText();

    QJsonDocument hook_json = QJsonDocument::fromJson(plain_json.toUtf8());
    QJsonObject hook_obj = hook_json.object();
    QString type = hook_obj.value("type").toString();
    QJsonObject value_obj = hook_obj.value("value").toObject();

    qDebug() << "interceptConfig type: " << type;

    if (type == "manifest") {
        clearTracksInfo();
        QtConcurrent::run([this, value_obj]() {
            parseManifest(value_obj);
        });
    }
    else if (type == "error") {
        qDebug() << "Page: service error: " << value_obj.value("title").toString();
        emit error(Status(EStatusCode::ParseError, value_obj.value("text").toString()));
    }
    else {
        emit error(Status(EStatusCode::ParseError, "Unknown hook message type " + type));
    }

    return;
}

void NetflixInterceptor::parseMetadata(const QString& metadata) {
#ifdef _DEBUG
    QTextDocument text;
    text.setHtml(metadata);
    QString plain_json = text.toPlainText();
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    QFile   manifest_file("netflix_metadata_" + QString::number(ms) + ".json");
    if (manifest_file.open(QIODevice::ReadWrite)) {
        manifest_file.resize(0);
        QTextStream(&manifest_file) << plain_json << Qt::endl;
    }
#endif
    //TODO: rewrite
    QJsonDocument metadata_json = QJsonDocument::fromJson(metadata.toLocal8Bit());

    if (!metadata_json.isNull()) {
        QJsonObject video = metadata_json.object()["video"].toObject();

        video_info_.url_type = video["type"].toString();
        if (video_info_.url_type == "supplemental")
            video_info_.url_type = "more";

        QString title;
        if (video.contains("currentEpisode") &&
            video.contains("seasons")) {

            QString episode_id = video["currentEpisode"].toVariant().toString();
            
            QString base_ep_url = base_url_.mid(0, base_url_.lastIndexOf("/") + 1);
            if (base_ep_url.isEmpty())
                base_ep_url = "https://www.netflix.com/watch/";

            for (const auto& season_ref : video["seasons"].toArray()) {
                QList<QPair<QString, QString>> episodes;
                QList<QPair<int, QPair<QString, QString>>> episodes_to_sort;

                QJsonObject season = season_ref.toObject();

                QRegularExpression rx("(\\d+)");
                QString season_number;
                int pos = 0;
                QString longName = season["longName"].toString();
                if (!longName.isEmpty())
                    season_number = longName.split(" ").last();

                if (season_number.isEmpty())
                    season_number = QString::number(season["seq"].toVariant().toInt());

                int season_number_int = season_number.toInt();

                if (season_number_int < 10)
                    season_number = "0" + season_number;

                for (const auto& episode_ref : season["episodes"].toArray()) {
                    QJsonObject episode = episode_ref.toObject();

                    int episodeNumber = episode["seq"].toInt();

                    QString ep_title = normalizeTitle(episode["title"].toString(), 40);
                    int duration = episode["runtime"].toInt();
                    QString ep_duration = QDateTime::fromSecsSinceEpoch(duration).toUTC().toString(duration < 3600 ? "mm:ss" : "hh:mm:ss");
                    QString ep_data = ep_title + " " + ep_duration;

                    QString ep_url = base_ep_url;
                    ep_url.append(episode["episodeId"].toVariant().toString());

                    QPair<QString, QString> ep = qMakePair(ep_data, ep_url);
                    episodes_to_sort.push_back(qMakePair(episodeNumber, ep));

                    if (episode["episodeId"].toVariant().toString() == episode_id) {

                        int episode_num = episode["seq"].toVariant().toInt();
                        QString episode_number = QString::number(episode_num);
                        if (episode_num < 10)
                            episode_number = "0" + episode_number;

                        //Episode detected;
                        QString movie_name = episode["title"].toString();
                        QString album_name = video["title"].toString();

                        video_info_.metadata_title = normalizeTitle(movie_name);
                        video_info_.metadata_album = normalizeTitle(album_name);
                        //video_info_.metadata_track = "S" + QString::number(season_num) + ".E" + QString::number(episode_num);
                        video_info_.metadata_track = QString::number(episode_num) + "/" + QString::number(season_number_int);

                        video_info_.duration = episode["runtime"].toInt();
                        video_info_.metadata_comment = episode["synopsis"].toString();
                        title = "[S" + season_number + ".E" + episode_number + "] " + album_name + " - " + movie_name;
                        video_info_.metadata_year = season["year"].toString();

                        QJsonObject still = episode["stills"].toArray()[0].toObject();
                        video_info_.artwork_url = still["url"].toString();
                    }
                }

                if (!episodes_to_sort.isEmpty()) {
                    std::sort(episodes_to_sort.begin(), episodes_to_sort.end(), [](const QPair<int, QPair<QString, QString>>& t1, const QPair<int, QPair<QString, QString>>& t2)->bool {
                        return (t1.first < t2.first);
                    });

                    for (auto& ep : episodes_to_sort)
                        episodes.push_back(ep.second);

                    video_info_.playlist.push_back(qMakePair(season_number_int, episodes));
                }
            }

            std::sort(video_info_.playlist.begin(), video_info_.playlist.end(), [](const QPair<int, QList<QPair<QString, QString>>>& t1, const QPair<int, QList<QPair<QString, QString>>>& t2)->bool {
                return (t1.first < t2.first);
            });
        }
        else {
            //qint64 id = video["id"].toVariant().toLongLong();
            //Movie or supplemential detected;
            video_info_.duration = video["runtime"].toInt();
            video_info_.metadata_comment = video["synopsis"].toString();
            title = video["title"].toString();
            video_info_.metadata_title = normalizeTitle(title);
            video_info_.metadata_year = video["year"].toString();

            QJsonObject artwork = video["artwork"].toArray()[0].toObject();
            video_info_.artwork_url = artwork["url"].toString();
        }

        video_info_.video_title = normalizeTitle(title);

        if (!video_info_.video_title.isEmpty()
            /* && !video_info_.video_title.contains("recap", Qt::CaseInsensitive)*/) {
            qDebug() << "Parsed title: " << video_info_.url_type << ": '" << video_info_.video_title << "' Year: " << video_info_.metadata_year << " Duration: " << video_info_.duration;
            setFlag(kVideoInfoReady_);
        }
        //else if (video_info_.video_title.contains("recap", Qt::CaseInsensitive)) {
        //    qDebug() << "Parsed title recap";
        //    emit showWebEngine(ERequestType::recap);
        //}
        else {
            qDebug() << "Parsed Title Error ";
        }
    }
}

void NetflixInterceptor::parseVideoTracks(const QJsonArray& tracks) {
    for (const auto& track_ref : tracks) {
        QJsonObject track = track_ref.toObject();
        VideoTrackInfo video_track;
        for (const auto& download_ref : track["downloadables"].toArray()) {
            QJsonObject download = download_ref.toObject();

            video_track.bitrate = download["bitrate"].toInt() * 1000;
            video_track.height = download["height"].toInt();
            video_track.width = download["width"].toInt();
            video_track.is_encrypted = download["isEncrypted"].toBool();
            video_track.size = download["size"].toVariant().toLongLong();

            video_track.contains_audio = false;

            video_track.urls.clear();

            for (const auto& url_ref : download["urls"].toObject()) {
                qDebug() << "video trask url: " << url_ref.toString();
                video_track.urls.append(url_ref.toString());
            }

            if (!video_track.urls.isEmpty()) {
                normalizeTrack(video_track);
                normalizeVideoTrack(video_track);
                tracks_info_.video_tracks.append(video_track);
            }
        }
    }
}

void NetflixInterceptor::parseAudioTracks(const QJsonArray& tracks) {
    for (const auto& track_ref : tracks) {
        QJsonObject track = track_ref.toObject();

        AudioTrackInfo audio_track;

        audio_track.channels = track["channelsCount"].toInt();
        QString tag = track["bcp47"].toString().toLower();

        if (tag == "es")
            tag = SS2Q(getLangTagByLangName("Spanish Latin"));

        audio_track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));

        if (track["trackType"].toString() == "ASSISTIVE")
            audio_track.is_assistive = true;

        for (const auto& download_ref : track["downloadables"].toArray()) {
            QJsonObject download = download_ref.toObject();

            audio_track.bitrate = download["bitrate"].toInt() * 1000;
            audio_track.profile = netflixAudioProfile(download["contentProfile"].toString());
            audio_track.is_encrypted = download["isEncrypted"].toBool();
            audio_track.size = download["size"].toInt();

            audio_track.urls.clear();

            for (const auto& url_ref : download["urls"].toObject()) {
                qDebug() << "audio trask url: " << url_ref.toString();
                audio_track.urls.append(url_ref.toString());
            }

            if (!audio_track.urls.isEmpty()) {
                normalizeTrack(audio_track);
                normalizeAudioTrack(audio_track);
                tracks_info_.audio_tracks.append(audio_track);
            }
        }
    }
}

void NetflixInterceptor::parseTextTracks(const QJsonArray& tracks) {
    QList<QString> language_tags;
    QList<QString> language_tags_additional;
    QList<QString> language_closed_captions;

    for (const auto& track_ref : tracks) {
        QJsonObject text_track = track_ref.toObject();

        SubtitleTrackInfo track;
        if (!text_track["isNone"].toBool()) {
            QString tag = text_track["bcp47"].toString().toLower();
            if (tag == "es")
                tag = SS2Q(getLangTagByLangName("Spanish Latin"));
            track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));

            QLocale locale(track.language_tag);
            QString country;
            if (track.language_tag.contains("-"))
                country = " " + locale.countryToString(locale.country());
            track.language = QString("%1%2").arg(QLocale::languageToString(locale.language())).arg(country);

            track.is_additional = text_track["isForced"].toBool();
            if (!track.is_additional
                && language_tags.contains(track.language_tag)
                && !language_closed_captions.contains(track.language_tag))
                track.is_closed_captions = true;

            int track_priority = INT_MAX;
            for (const auto& download_ref : text_track["downloadables"].toArray()) {
                QJsonObject download = download_ref.toObject();

                //Only one profile are best for getflix;
                auto profile = subtitleProfile(download["contentProfile"].toString());
                int downloadables_priority = (int)profile;
                if (downloadables_priority > 0) {
                    if (download["isImage"].toBool())
                        downloadables_priority += download["pixWidth"].toInt() *
                        download["pixHeight"].toInt();

                    if (downloadables_priority >= track_priority)
                        continue;
                }
                else {
                    qDebug() << "Unsupported SUBTITLE profile" << download["contentProfile"].toString();
                    continue;
                }

                track_priority = downloadables_priority;
                track.profile = profile;

                track.size = 0;
                track.image_size = QSize();
                track.urls.clear();

                if (download["isImage"].toBool()) {
                    track.size = download["offset"].toInt();
                    track.image_size = QSize(download["pixWidth"].toInt(),
                        download["pixHeight"].toInt());
                }

                //Parse URLS;
                for (const auto& url_ref : download["urls"].toObject()) {
                    qDebug() << "suttitle trask url: " << url_ref.toString();
                    track.urls.append(url_ref.toString());
                }
            }
            if (!track.urls.isEmpty()) {
                normalizeTrack(track);
                if (track.is_additional) {
                    if (!language_tags_additional.contains(track.language_tag)) {
                        tracks_info_.subtitle_tracks.append(track);
                        language_tags_additional << track.language_tag;
                    }
                }
                else if (track.is_closed_captions) {
                    if (!language_closed_captions.contains(track.language_tag)) {
                        tracks_info_.subtitle_tracks.append(track);
                        language_closed_captions << track.language_tag;
                    }
                }
                else {
                    if (!language_tags.contains(track.language_tag)) {
                        tracks_info_.subtitle_tracks.append(track);
                        language_tags << track.language_tag;
                    }
                }
            }
        }
    }
}
void NetflixInterceptor::parseManifest(const QJsonObject& manifest) {
    Status status;
    if (!manifest.isEmpty()
        && manifest.contains("drmType")
        && manifest["drmType"].toString() == "widevine") {

        parseAudioTracks(manifest["audioTracks"].toArray());
        parseVideoTracks(manifest["videoTracks"].toArray());
        parseTextTracks(manifest["textTracks"].toArray());

        // QString keyId = manifest.object()["keyId"].toString();

        /* if (manifest_object.contains("psshb64")) {
        QString psshb64 = manifest_object["psshb64"].toArray()[0].toString();
        QByteArray pssh = QByteArray::fromBase64(psshb64.toLatin1());
        std::vector<uint8_t> pssh_vec;
        pssh_vec.assign((uint8_t*)pssh.data(), (uint8_t*)pssh.data() + pssh.size());
        std::vector<std::vector<uint8_t>> keys = ParsePSSHKeys(pssh_vec);
        }*/
        //  tracks_info_.key_id = QByteArray::fromBase64(keyId.toLatin1());
        tracks_info_.duration = manifest["runtime"].toInt();    //not used now

        if (tracks_info_.video_tracks.isEmpty()
            || tracks_info_.audio_tracks.isEmpty())
            status.update(EStatusCode::ParseError, "Wrong manifest content.");

    }
    else {
        status.update(EStatusCode::ParseError, "Wrong manifest json object.");
    }

    if (status)
        setFlag(kTracksReady);
    //else
    //    emit error(status);
}

void NetflixInterceptor::setFlag(EInterceptFlags flag) {
    if (!is_tracks_ready_)
        is_tracks_ready_ = checkInterceptFlag(flag, kTracksReady);
    if (!is_video_info_ready_)
        is_video_info_ready_ = checkInterceptFlag(flag, kVideoInfoReady_);
    if (is_tracks_ready_ && is_video_info_ready_)
        emit metadataIntercepted(this->url());
}
