#include "instagram_parser.h"


InstagramParser::InstagramParser(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* parent /*= 0*/) : QObject(parent) {
    page_ = page;
    nam_ = nam;
    qRegisterMetaType<VideoInfo>();
}

void InstagramParser::getTracksData(const QString& html, TracksInfo& tracks_info, VideoInfo& video_info) {
    //get video urls
    QString video_block_start = "\"video_versions\":";
    bool some_videos = false;
    if (html.count(video_block_start) > 1)
        some_videos = true;

    QRegularExpression re_url_vid_block(video_block_start + "\\s*([^\]]+)[\]]", QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator i = re_url_vid_block.globalMatch(html);

    QList<QString> video_urls;

    while (i.hasNext()) {
        QRegularExpressionMatch match_url = i.next();
        QString video_block = match_url.captured(1).replace("\\u0026", "&");
        
        if (!video_block.isEmpty()) {
            QRegularExpression re_url_vid("\"type\":([^}]+)[}]", QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatchIterator j = re_url_vid.globalMatch(video_block);
            
            while (j.hasNext()) {
                QRegularExpressionMatch match_url = j.next();
                QString video_item = match_url.captured(1);

                if (!video_item.isEmpty()) {
                    auto match = QRegularExpression("\"url\":\"\\s*([^\"]+)[\"]").match(video_item);
                    if (match.hasMatch()) {
                        QString url = match.captured(1);
                        if (!url.isEmpty()
                            && !video_urls.contains(url)) {
                            video_urls << url;

                            if (some_videos) {
                                break;
                            }
                            else {
                                VideoTrackInfo video_track;
                                video_track.urls << url;

                                match = QRegularExpression("\"width\":\\s*([^,]+)[,]").match(video_item);
                                if (match.hasMatch())
                                    video_track.width = match.captured(1).toInt();

                                match = QRegularExpression("\"height\":\\s*([^,]+)[,]").match(video_item);
                                if (match.hasMatch())
                                    video_track.height = match.captured(1).toInt();

                                normalizeTrack(video_track);
                                normalizeVideoTrack(video_track);
                                tracks_info.video_tracks.append(video_track);
                            }
                        }
                    }
                }

            }
        }
    }

    if (some_videos 
        && !video_urls.isEmpty()) {

        VideoTrackInfo video_track;
        video_track.urls << video_urls;

        if (video_urls.size() > 1)
            video_track.is_chunked = true;

        normalizeTrack(video_track);
        normalizeVideoTrack(video_track);
        tracks_info.video_tracks.append(video_track);
    }

    //get images urls
    QRegularExpression re_candidates("\"candidates\":([^\]]+)[\]]", QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator j = re_candidates.globalMatch(html);
    QList<QString> urls;
    int images_number = 0;
    while (j.hasNext()) {
        QRegularExpressionMatch match_url = j.next();
        QString images_block = match_url.captured(1).replace("\\u0026", "&");
        if (!images_block.isEmpty()) {
            QRegularExpression re_url_img("{([^}]+)[}]", QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatchIterator k = re_url_img.globalMatch(images_block);
            QList<QString> urls;
            while (k.hasNext()) {
                QRegularExpressionMatch match_url = k.next();
                QString image_item = match_url.captured(1);
                auto match = QRegularExpression("\"url\":\"\\s*([^\"]+)[\"]").match(image_item);
                if (match.hasMatch()) {
                    QString url = match.captured(1);
                    if (!url.isEmpty()
                        && !urls.contains(url)) {

                        urls << url;

                        ImageTrackInfo image_track;
                        image_track.image_number = images_number;
                        image_track.urls.append(url);

                        match = QRegularExpression("\"width\":\\s*([^,]+)[,]").match(image_item);
                        if (match.hasMatch())
                            image_track.width = match.captured(1).toInt();

                        match = QRegularExpression("\"height\":\\s*([^,]+)[,]").match(image_item);
                        if (match.hasMatch())
                            image_track.height = match.captured(1).toInt();

                        if (video_info.artwork_url.isEmpty())
                            video_info.artwork_url = url;

                        normalizeTrack(image_track);
                        normalizeImageTrack(image_track);
                        tracks_info.image_tracks.append(image_track);
                    }
                }
            }
            images_number++;
        }
    }

    if (video_info.artwork_url.isEmpty()) {
        for (auto track : tracks_info.image_tracks) {
            if (!track.urls.isEmpty()) {
                video_info.artwork_url = track.urls[0];
                break;
            }
        }
    }

    if (!tracks_info.video_tracks.isEmpty()) {
        for (int i = 0; i < tracks_info.video_tracks.size(); ++i) {
            if (!tracks_info.video_tracks[i].urls.isEmpty()) {
                AudioTrackInfo audio_track;
                audio_track.from_video_stream = true;
                audio_track.urls.append(tracks_info.video_tracks[i].urls[0]);
                normalizeTrack(audio_track);
                normalizeAudioTrack(audio_track);
                tracks_info.audio_tracks.append(audio_track);
            }
        }
    }

    qDebug() << "Parsed tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size() << " images " << tracks_info.image_tracks.size();
}

void InstagramParser::formatTitle(QString& title) {
    if (!title.isEmpty()) {
        title = title.split("Instagram: ").last();
        title = title.split("Instagram post: ").last();
        title = title.split("(Tags)").first();
        title = title.split(" -Tags").first();
        title = title.split("Tags").first();
        title = title.split("#").first();
        title = title.remove(QRegExp("[<>:\"\/\\|?*']"));
        if (title == "Instagram")
            title.clear();
        
        if (!title.isEmpty() 
            && title.at(0).unicode() == 8220)                      // “ -> 8220 unicode number
            title = title.mid(1);
        if (!title.isEmpty()
            && title.at(0).unicode() == 171)                       // « -> 171 unicode number
            title = title.mid(1);
        if (!title.isEmpty()
            && title.at(title.count() - 1).unicode() == 8221)      // ” -> 8221 unicode number
            title = title.mid(0, title.count() - 1);
        if (!title.isEmpty()
            && title.at(title.count() - 1).unicode() == 187)       // » -> 187 unicode number
            title = title.mid(0, title.count() - 1);
    }
}

Status InstagramParser::parsePage(QString& html, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) {
    Status status;
    html = html.replace("\\u0026", "&");
    
    //Get title
    QString title;
    QRegExp expression = QRegExp("<title>([^<]+)[<]", Qt::CaseInsensitive);
    if (expression.indexIn(html) != -1) {
        title = expression.cap(1);
        formatTitle(title);
    }

    if (title.isEmpty()) {
        expression = QRegExp("\"og:title\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1) {
            title = expression.cap(1);
            formatTitle(title);
        }
    }
    if (title.isEmpty()) {
        expression = QRegExp("\"og:description\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1) {
            title = expression.cap(1);
            formatTitle(title);
        }
    }

    title = normalizeTitle(title);
    if (title.isEmpty()) {
        QStringList list = url.split("/");
        list.removeAll("");
        if (!list.isEmpty())
            title = list.last();
    }

    if (!title.isEmpty()) {
        video_info.video_title = title;
        video_info.url_type = "Image";

        //Get thumbnail url
        expression = QRegExp("\"og:image\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);

        if (expression.indexIn(html) != -1)
            video_info.artwork_url = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();

        //Get embedded thumbnail url
        if (video_info.artwork_url.isEmpty()) {
            expression = QRegExp("EmbeddedMediaImage\" src=\"(.*)[\r\n]");

            if (expression.indexIn(html) != -1) {
                QString url = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();
                url = url.split("\"").first();
                video_info.artwork_url = url;
            }
        }

        //Get duration in seconds
        expression = QRegExp("\"video_duration\":([^\,]+)[\,]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1) {
            QString duration = expression.cap(1);
            if (duration != "null") {
                QList<QString> time = duration.split(".");
                QString min;
                QString sec;
                QString msec;
                if (!time.isEmpty()) {
                    if (time.size() > 2) {
                        min = time[0];
                        sec = time[1];
                        msec = time[2];
                    }
                    else if (time.size() == 2) {
                        sec = time[0];
                        msec = time[1];
                    }
                    else {
                        msec = time[0];
                    }
                }
                int add_sec = 0;
                if (msec.toInt() > 500)
                    add_sec = 1;

                video_info.duration = min.toInt() * 60 + sec.toInt() + add_sec;
            }
        }

        getTracksData(html, tracks_info, video_info);
        
        if (tracks_info.video_tracks.isEmpty()
            && tracks_info.image_tracks.isEmpty())
            status.update(EStatusCode::ParseError, "Empty tracks info.");
    }
    else {
        status.update(EStatusCode::ParseError, "Could not retrieve video title.");
    }
    return status;
}

Status InstagramParser::parseHTML(const QString& html_in, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) {
    Status status;
    QString html = html_in;
    //Check for login required
    //QRegExp expression = QRegExp("<div id=\"watch7-player-age-gate-content\">.*<button.*href=\"([^\"]+)\"");
    //expression.setMinimal(true);

    if (html.isEmpty()) {
        status.update(EStatusCode::RegistrationFailed, "Login required.");
        return status;
    }
    QRegExp expression = QRegExp("\"canonical\"\\s*href=\"([^\"]+)[\"]", Qt::CaseInsensitive);
    if (expression.indexIn(html) != -1) {
        QString url = expression.cap(1);
        if (url.contains("accounts/login")) {
            status.update(EStatusCode::RegistrationFailed, "Login required.");
            return status;
        }
    }

    status = parsePage(html, video_info, tracks_info, url);

    return status;
}
