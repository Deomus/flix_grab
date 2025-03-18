#include "twitter_parser.h"


TwitterParser::TwitterParser(QNetworkAccessManager* nam, QObject* parent /*= 0*/) : QObject(parent) {
    nam_ = nam;
}

Status TwitterParser::getRequest(QNetworkRequest request, QByteArray &data) {
    Status status;
    //request.setHeader(QNetworkRequest::UserAgentHeader, user_agent_);
    QNetworkReply* reply = nullptr;
    if (nam_ != nullptr) {
        reply = nam_->get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&]() {
            if (reply->error() == QNetworkReply::NoError) {
                data = reply->readAll();
                if (data.isEmpty())
                    status.update(Status::ParseError, "QNetworkReply error url: " + reply->url().toEncoded() + ":" + "empty ");
            }
            else {
                //qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                status.update(Status::ParseError, "QNetworkReply error url: " + reply->url().toEncoded() + ":" + QString(reply->errorString().data()));
            }
            reply->deleteLater();
            loop.quit();
        });
        loop.exec();
    }
    return status;
}

Status TwitterParser::postRequest(QNetworkRequest request, QByteArray &data, const QByteArray& body) {
    Status status;
    //request.setHeader(QNetworkRequest::UserAgentHeader, user_agent_);
    QNetworkReply* reply = nullptr;
    if (nam_ != nullptr) {
        reply = nam_->post(request, body);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&]() {
            if (reply->error() == QNetworkReply::NoError) {
                data = reply->readAll();
                if (data.isEmpty())
                    status.update(Status::ParseError, "QNetworkReply error url: " + reply->url().toEncoded() + ":" + "empty ");
            }
            else {
                //qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
                status.update(Status::ParseError, "QNetworkReply error url: " + reply->url().toEncoded() + ":" + QString(reply->errorString().data()));
            }
            reply->deleteLater();
            loop.quit();
        });
        loop.exec();
    }
    return status;
}

Status TwitterParser::loadToken(QString& token) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(token_url_));
    request.setRawHeader("authorization", bearer_.toLocal8Bit());

    QByteArray data;
    status = postRequest(request, data);
    if (!status) 
        return status;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json_obj = doc.object();
    if (!json_obj.contains("guest_token")) {
        status.update(Status::ParseError, QString("Guest token not found"));
        return status;
    }

    token = json_obj.value("guest_token").toString().toLocal8Bit();
    
    if (token.isEmpty())
        status.update(Status::ParseError, "Guest token not found");

    return status;
}

Status TwitterParser::loadVideo(const QString& video_id, const QString& token, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(QString(video_info_url_).arg(video_id)));
    request.setRawHeader("authorization", bearer_.toLocal8Bit());
    request.setRawHeader("x-guest-token", token.toLocal8Bit());

    QByteArray data;
    status = getRequest(request, data);
    if (!status) 
        return status;

    //ответ запроса json
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json_obj = doc.object();

    //video may be like 
    //1 - mp4 file
    //2 - m3u8 file
    //3 - vmap - list of mp4 files with m3u8
    QJsonObject track = json_obj.find("track").value().toObject();

    if (track.isEmpty()) {
        status.update(Status::ParseError, QString("Could not retrieve video track"));
        return status;
    }
    else if (!track.contains("contentType")) {
        status.update(Status::ParseError, QString("Could not retrieve track content type"));
        return status;
    }

    //get artwork
    video_info.artwork_url = json_obj.find("posterImage").value().toString();
    if (video_info.artwork_url.contains("player-placeholder.png"))
        video_info.artwork_url.clear();

    //get duration
    if (track.contains("durationMs")) 
        video_info.duration = track.value("durationMs").toInt() / 1000;

    if (track.value("contentType").toString() == "vmap") {
        if (!track.contains("vmapUrl")
            || track.value("vmapUrl").toString().isEmpty()) {
            status.update(Status::ParseError, QString("Could not retrieve vmap url"));
            return status;
        }
        //отправка запроса загрузки vmap информации видео
        return loadVmap(track.value("vmapUrl").toString(), video_info, tracks_info);
    }
    else {
        if (!track.contains("playbackUrl")) {
            status.update(Status::ParseError, QString("Could not retrieve playback url"));
            return status;
        }
        QString playback_url = track.value("playbackUrl").toString();
        //check that video is m3u8
        if (QRegExp(video_re_str_).exactMatch(playback_url)) {
            return loadM3U8(playback_url, tracks_info);
        }
        //video is mp4 file
        else {
            VideoTrackInfo track;
            track.eval_type = ManualEvaluation::UnrecVideo;
            track.is_fake_size = true;
            track.size = 1000;
            track.urls.push_back(playback_url);
            tracks_info.video_tracks.append(track);

            return status;
        }
    }
}

Status TwitterParser::loadVmap(const QString& url, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QByteArray data;
    status = getRequest(request, data);
    if (!status) 
        return status;

    //parse vmap xml
    QXmlStreamReader xml(data);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            if (xml.name() == "videoVariant") {  //video quality in block - <videoVariant>
                QXmlStreamAttributes attributes = xml.attributes();
                if (attributes.hasAttribute("url")) {
                    QString url = QUrl::fromPercentEncoding(attributes.value("url").toString().toLocal8Bit()); //url in block - prcent encoding
                    
                    if (QRegExp(video_re_str_).exactMatch(url)) {
                        //отправка запроса загрузки m3u8 информации vmap видео
                        status = loadM3U8(url, tracks_info);
                        if (!status) 
                            return status;
                    }
                    else {
                        //если videoVariant содержит ссылку на файл добавляем его с список потоков
                        tracks_info.video_tracks.push_back(VideoTrackInfo());

                        if (attributes.hasAttribute("bit_rate"))
                            tracks_info.video_tracks.back().bitrate = attributes.value("bit_rate").toInt() / 1000;

                        QRegExp widthRe("(\\d+)x");
                        QRegExp heightRe("x(\\d+)");

                        int pos = widthRe.indexIn(url);
                        if (pos > -1)
                            tracks_info.video_tracks.back().width = widthRe.cap(1).toInt();

                        pos = heightRe.indexIn(url);
                        if (pos > -1)
                            tracks_info.video_tracks.back().height = heightRe.cap(1).toInt();

                        if (video_info.duration > 0) {
                            tracks_info.video_tracks.back().size = calculateTrackSize(tracks_info.video_tracks.back().height, tracks_info.video_tracks.back().width, tracks_info.video_tracks.back().bitrate, video_info.duration, tracks_info.video_tracks.back().frame_rate);
                            tracks_info.video_tracks.back().is_calc_size = true;
                        }

                        if (tracks_info.video_tracks.back().height <= 0)
                            tracks_info.video_tracks.back().eval_type = ManualEvaluation::UnrecVideo;

                        //Hack to show progress
                        if (tracks_info.video_tracks.back().size <= 0) {
                            tracks_info.video_tracks.back().is_fake_size = true;
                            tracks_info.video_tracks.back().size = 1000;
                        }

                        tracks_info.video_tracks.back().urls.push_back(url);
                    }
                }
            }
        }
    }

    return status;
}

Status TwitterParser::loadM3U8(const QString& url, TracksInfo& tracks_info) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QByteArray data;
    status = getRequest(request, data);
    if (!status) 
        return status;

    parseM3U8(data, url, tracks_info);

    return status;
}

Status TwitterParser::parseM3U8(const QString& data, const QString& url, TracksInfo& tracks_info) {
    Status status;
    
    QRegExp stream1Re("^#EXT-X-STREAM-INF:.*$");
    QRegExp bandWidhtRe("BANDWIDTH=(\\d+),");
    QRegExp widthRe("RESOLUTION=(\\d+)x");
    QRegExp heightRe("x(\\d+),");

    QStringList stringList = data.split("\n");
    bool urlString = false;
    foreach(QString str, stringList) {
        if (stream1Re.exactMatch(str)) {
            tracks_info.video_tracks.push_back(VideoTrackInfo());

            //Hack to show progress
            tracks_info.video_tracks.back().is_fake_size = true;
            tracks_info.video_tracks.back().size = 1000;

            int pos = bandWidhtRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().bitrate = bandWidhtRe.cap(1).toInt() / 1000;

            pos = widthRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().width = widthRe.cap(1).toInt();

            pos = heightRe.indexIn(str);
            if (pos > -1)
                tracks_info.video_tracks.back().height = heightRe.cap(1).toInt();

            if (tracks_info.video_tracks.back().height <= 0)
                tracks_info.video_tracks.back().eval_type = ManualEvaluation::UnrecVideo;

            urlString = true;
            continue;
        }
        if (urlString) {
            QString keyword = str.mid(1).split("/").first();
            QString url_base = url.split("/" + keyword).first();
            QString m3u8_url = url_base + str;

            QNetworkRequest request;
            request.setUrl(QUrl(m3u8_url));

            QByteArray data;
            status = getRequest(request, data);
            if (!status)
                return status;

            QStringList stringList = QString::fromLatin1(data.data()).split("\n");
            foreach(QString str, stringList) {
                if (str.contains(keyword)) {
                    tracks_info.video_tracks.back().urls.push_back(url_base + str);
                    tracks_info.video_tracks.back().is_chunked = true;
                }
            }

            urlString = false;
        }
    }

    return status;
}

Status TwitterParser::loadBroadcast(const QString& broadcast_id, const QString& token, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(QString(broadcast_info_url_).arg(broadcast_id)));
    request.setRawHeader("authorization", bearer_.toLocal8Bit());
    request.setRawHeader("x-guest-token", token.toLocal8Bit());

    QByteArray data;
    status = getRequest(request, data);
    if (!status) 
        return status;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = doc.object();

    QJsonObject broadcastsObj = jsonObj.find("broadcasts")->toObject();

    QJsonObject broadcastObj = broadcastsObj.find(broadcast_id)->toObject();

    video_info.video_title = normalizeTitle(broadcastObj.find("status")->toString());
    video_info.artwork_url = broadcastObj.find("image_url")->toString();

    QString start = broadcastObj.find("start_ms")->toString();
    QString end = broadcastObj.find("end_ms")->toString();
    video_info.duration = (end.toLong() / 1000 - start.toLong() / 1000);

    QString mediaKey = broadcastObj.find("media_key")->toString();

    return loadBroadcastStream(mediaKey, token, tracks_info);
}

Status TwitterParser::loadBroadcastStream(const QString& mediaKey, const QString& token, TracksInfo& tracks_info) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://api.twitter.com/1.1/live_video_stream/status/%1").arg(mediaKey)));
    request.setRawHeader("authorization", bearer_.toLocal8Bit());
    request.setRawHeader("x-guest-token", token.toLocal8Bit());

    QByteArray data;
    status = getRequest(request, data);
    if (!status) return status;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = doc.object();

    QJsonObject sourceObj = jsonObj.find("source")->toObject();

    QString url = sourceObj.find("location")->toString();

    tracks_info.video_tracks.push_back(VideoTrackInfo());
    tracks_info.video_tracks.back().urls.push_back(url);

    return status;
}

Status TwitterParser::loadTwit(QString twit_id) {
    Status status;
    QNetworkRequest request;
    request.setUrl(QUrl(QString(twit_info_url_).arg(twit_id)));
    request.setRawHeader("authorization", bearer_.toLocal8Bit());

    QByteArray data;
    status = getRequest(request, data);
    if (!status)
        return status;

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject jsonObj = doc.object();

    QJsonObject entities = jsonObj.find("entities")->toObject();
    qDebug() << entities;
    QJsonObject urls = entities.find("urls")->toArray().first().toObject();

    qDebug() << urls.find("expanded_url")->toString();

    return status;
}

Status TwitterParser::parseHTML(const QString& data_in, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) {
    Status status;

    //check for redirect
    QString data = data_in;
    QString keyword = ">redirected<";
    if (data.contains(keyword)) {
        QRegExp expression = QRegExp("href=\"(.*)" + keyword);
        expression.setMinimal(true);
        if (expression.indexIn(data) != -1) {
            QString redirect_url = expression.cap(1).split("\"").first();
            if (!redirect_url.isEmpty()) {
                QByteArray red_data;
                QNetworkRequest request;
                request.setUrl(QUrl(redirect_url));
                status = getRequest(request, red_data);
                if (status)
                    data = red_data;
            }
        }

    }
    //check for empty and protected
    if (data.isEmpty()) {
        status.update(Status::ParseError, "Video data not found.");
        return status;
    }
    else if (data.contains("Tweets are protected")) {
        status.update(Status::PrivateVideo, "This video is private.");
        return status;
    }

    //get token
    QString token;
    status = loadToken(token);
    if (!status)
        return status;

    QRegExp expression = QRegExp("<title>(.*)</title>");
    expression.setMinimal(true);

    //get title
    QString title;

    if (expression.indexIn(data) != -1) {
        title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText().split(": \"").last().split(" http").first();
    }
    //get title from meta  property
    if (title.isEmpty()) {
        expression = QRegExp("og:description\" content=\"(.*)[\r\n]");
        expression.setMinimal(true);
        if (expression.indexIn(data) != -1)
            title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();

        title = title.split(" http").first();
        title = title.simplified();

        if (!title.isEmpty()
            && title.at(0).unicode() == 8220)                       // “ -> 8220 unicode number
            title = title.mid(1);

        if (!title.isEmpty()
            && title.at(title.count() - 1).unicode() == 8221)       // ” -> 8221 unicode number
            title = title.mid(0, title.count() - 1);
    }
    video_info.video_title = normalizeTitle(title);

    //get artwork from meta property
    expression = QRegExp("og:image\" content=\"(.*)[\r\n]");
    expression.setMinimal(true);
    if (expression.indexIn(data) != -1) {
        video_info.artwork_url = expression.cap(1).split("\"").first();
        if (video_info.artwork_url.contains("player-placeholder.png"))
            video_info.artwork_url.clear();
    }

    //video
    QRegExp videoRe("^https://twitter.com/i/videos/(\\d+)$");
    QRegExp mediaContainerRe("(<div class=\"AdaptiveMediaOuterContainer\">)");//check if exists block - AdaptiveMediaOuterContainer то там видео, на мобильной версии не работает
    if (videoRe.indexIn(url) > -1 ||
        mediaContainerRe.indexIn(data) > -1) {

        QString video_id = videoRe.cap(1);
        if (video_id.isEmpty()) {
            QRegExp idRe = QRegExp("/(\\d+)$");
            if (idRe.indexIn(url) > -1)
                video_id = idRe.cap(1);
        }
        if (video_id.isEmpty()) {
            status.update(Status::ParseError, "Video id not found");
            return status;
        }
        else if (video_info.video_title.isEmpty())
            video_info.video_title = normalizeTitle(video_id);

        return loadVideo(video_id, token, video_info, tracks_info);
    }

    //broadcast
    QRegExp broadcastRe(broadcast_re_str_);
    if (broadcastRe.indexIn(url) > -1) {
        return status.update(Status::LiveStream, "Live streams are not supported.");

        return loadBroadcast(broadcastRe.cap(1), token, video_info, tracks_info);
    }

    //external url это текстовый твит с ссылкой на медиа
    QRegExp dataExpandedUrlRe("data-expanded-url=\"([^\"]+)\"");
    int pos = dataExpandedUrlRe.indexIn(data);
    if (pos > -1) {
        QString expandedUrl = dataExpandedUrlRe.cap(1);
        QRegExp broadcastRe(broadcast_re_str_);
        pos = broadcastRe.indexIn(expandedUrl);
        //twitter broadcast
        if (pos > -1) {
            return status.update(Status::LiveStream, "Live streams are not supported.");
            return loadBroadcast(broadcastRe.cap(1), token, video_info, tracks_info);
        }
        else {
            //видео не с твиттера
            status.update(Status::TwitterExternalUrl, QString("This is embed external url. Original url is %1").arg(expandedUrl));
            return status;
        }
    }
    else {
        status.update(Status::ParseError, "Parse error");
        return status;
    }

    return status;
}