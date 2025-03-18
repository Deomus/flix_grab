#include "web_portal.h"


WebPortal::WebPortal(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p) 
    : WebContent(p)
    , page_(page)
    , nam_(nam) {
    qRegisterMetaType<ERequestType>();
}

WebPortal::~WebPortal() {
    qDebug() << "Interceptor: ~WebPortal";
}

void WebPortal::initialize() {
    auto initContent = [this](WebContent* content) {
        
        content->setNetworkAccess(nam_);

        QObject::connect(content, &WebContent::finished, this, [=]() {
            qDebug() << "WebPortal: WebContent::finished status: " << content->status();

            if (content->status().code() == EStatusCode::RegistrationFailed) {
                invalidate();
                signed_in_ = false;

                emit loginRequested(signinURL());
            }
            else {
                if (!content->status()) {
                    invalidate();
                    emit error(content->status());
                }
                else
                    signed_in_ = true;
            }
        });
    };

    initContent(this);
    //must be called after initContent!!!
    createParser(page_, nam_);

    QObject::connect(this, &WebContent::finished, this, [=]() {
        if (signed_in_
            && isValid()
            && !video_info_.video_title.isEmpty()
            && (!tracks_info_.video_tracks.isEmpty() || !tracks_info_.image_tracks.isEmpty())) {
            emit metadataIntercepted(this->url());
        }
    });
}

//void WebPortal::loadPlaylist(const QString& item) {
//    emit loadPlaylistItem(item);
//}

void WebPortal::interceptRequest(QWebEngineUrlRequestInfo &info) {
    if (signed_in_) {

        qDebug() << ">>>>>> " << info.requestUrl();

        if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeScript)
            qDebug() << "###### " << info.requestUrl();

        if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
            qDebug() << "Interceptor: interceptMetadata url:" << info.requestUrl().toEncoded();


            //QString a = info.requestUrl().toEncoded();
            //QUrl b = a;
            //QUrlQuery query(b);
            //query.addQueryItem("hl", "ru");
            //b.setQuery(query);

            //QUrlQuery query2(b);
            //QList<QPair<QString, QString> > list = query2.queryItems();

            QMetaObject::invokeMethod(this, "interceptMetadata", Qt::QueuedConnection, Q_ARG(const QUrl&, info.requestUrl()));
            info.block(true);
        }
    }
}

void WebPortal::interceptConfig(const QString& jsonStr, const QString& confName) {
    Q_UNUSED(jsonStr);
    Q_UNUSED(confName);
}

void WebPortal::interceptAppScript(const QUrl& url) {
    Q_UNUSED(url);
}

void WebPortal::autoLogin() {
    //
}

void WebPortal::startPage(bool forced_reload_page) {
    qDebug() << "Interceptor: startPage";
    Q_UNUSED(forced_reload_page);
    invalidate();
}

void WebPortal::finishPage(bool ok) {
    if (ok)
        scripts_.run(page_, true);
}

void WebPortal::invalidate() {
    scripts_.clear();
    reset();
    video_info_.playlist.clear();
    tracks_info_.req_decrypt = false;
    tracks_info_.key_id.clear();
    tracks_info_.pssh.clear();
    tracks_info_.audio_tracks.clear();
    tracks_info_.all_audio_tracks.clear();
    tracks_info_.audio_languages.clear();
    tracks_info_.all_audio_languages.clear();
    tracks_info_.video_tracks.clear();
    tracks_info_.image_tracks.clear();
    tracks_info_.all_image_tracks.clear();
    tracks_info_.subtitle_tracks.clear();
    video_info_ = {};
    tracks_info_ = {};
}

Status WebPortal::modifyContent() {
    Status status;
    if (parserExists()) {
        status = parse(data(), video_info_, tracks_info_, url().toEncoded());
    }
    else {
        status.update(EStatusCode::ParseError, "Could not create parser.");
    }

    return status;
}

void WebPortal::createParser(QWebEnginePage* page, QNetworkAccessManager* nam) {
    Q_UNUSED(page);
    Q_UNUSED(nam);
    qDebug() << "Interceptor: createParser";
}

bool WebPortal::parserExists() {
    qDebug() << "Interceptor: parserExists";
    return false;
}

Status WebPortal::parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) {
    Q_UNUSED(data);
    Q_UNUSED(video_info);
    Q_UNUSED(tracks_info);
    Q_UNUSED(url);
    qDebug() << "Interceptor: parse";
    Status status;
    return status;
}

QString WebPortal::signinURL() {
    return "";
}

Q_INVOKABLE void WebPortal::interceptDeferScript(const QUrl& url) {
    WebContent* script = nullptr;
    for (auto s : defer_scripts_) {
        if (s->url() == url) {
            script = s;
            break;
        }
    }

    if (script == nullptr) {
        script = new WebContent(this);
        script->setNetworkAccess(nam_);
        QObject::connect(script, &WebContent::finished, this, [=]() {
            if (!script->status())
                emit error(script->status());
        });
        defer_scripts_.push_back(script);
    }

    script->fetch(url);
    scripts_.addDefer(script);
}

Q_INVOKABLE void WebPortal::interceptMetadata(const QUrl& url) {
    fetch(url);
}