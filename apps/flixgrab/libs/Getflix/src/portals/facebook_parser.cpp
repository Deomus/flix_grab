#include "facebook_parser.h"


FacebookParser::FacebookParser(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* parent /*= 0*/) : QObject(parent) {
    page_ = page;
    nam_ = nam;
    qRegisterMetaType<VideoInfo>();
}

void FacebookParser::getTracksData(const QString& html, TracksInfo& tracks_info) {
    QRegularExpression re_mpd("@([^~]+)[~]", QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator i = re_mpd.globalMatch(html);
    QStringList urls;
    while (i.hasNext()) {
        QRegularExpressionMatch match_mpd = i.next();
        QString mpd = match_mpd.captured(1);
        if (!mpd.isEmpty()) {
            mpd.replace("<AdaptationSet", "@").replace("</AdaptationSet>", "~");

            QRegularExpression re_set("@([^~]+)[~]", QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatchIterator j = re_set.globalMatch(mpd);

            while (j.hasNext()) {
                QRegularExpressionMatch match_url = j.next();
                QString adaptationset = match_url.captured(1);
                if (!adaptationset.isEmpty()) {
                    adaptationset.replace("<Representation", "@").replace("</Representation>", "~");

                    QRegularExpression re_set("@([^~]+)[~]", QRegularExpression::DotMatchesEverythingOption);
                    QRegularExpressionMatchIterator k = re_set.globalMatch(adaptationset);

                    while (k.hasNext()) {
                        QRegularExpressionMatch match_url = k.next();
                        QString representation = match_url.captured(1);
                        
                        if (!representation.isEmpty()) {
                            auto match = QRegularExpression("BaseURL>([^<]+)[<]").match(representation);
                            if (match.hasMatch()) {
                                QString url = match.captured(1).replace("&amp;", "&");
                                if (!url.isEmpty()
                                    && !urls.contains(url)) {
                                    urls << url;

                                    match = QRegularExpression("mimeType=\"([^\"]+)[\"]").match(representation);
                                    if (match.hasMatch()) {
                                        QString mimetype = match.captured(1);
                                        
                                        if (mimetype.contains("video")) {

                                            VideoTrackInfo video_track;
                                            video_track.urls << url;
                                            video_track.contains_audio = false;

                                            match = QRegularExpression("width=\"([^\"]+)[\"]").match(representation);
                                            if (match.hasMatch())
                                                video_track.width = match.captured(1).toInt();

                                            match = QRegularExpression("height=\"([^\"]+)[\"]").match(representation);
                                            if (match.hasMatch())
                                                video_track.height = match.captured(1).toInt();

                                            match = QRegularExpression("bandwidth=\"([^\"]+)[\"]").match(representation);
                                            if (match.hasMatch())
                                                video_track.bitrate = match.captured(1).toInt();

                                            normalizeTrack(video_track);
                                            normalizeVideoTrack(video_track);
                                            tracks_info.video_tracks.append(video_track);
                                        }
                                        else if (mimetype.contains("audio")) {

                                            AudioTrackInfo audio_track;
                                            audio_track.urls.append(url);

                                            match = QRegularExpression("bandwidth=\"([^\"]+)[\"]").match(representation);
                                            //if (match.hasMatch())
                                                audio_track.bitrate = 128000;// match.captured(1).toInt();

                                            match = QRegularExpression("codecs=\"([^\"]+)[\"]").match(representation);
                                            if (match.hasMatch()) {
                                                QString codec = match.captured(1);
                                                if (codec.contains("avc1"))
                                                    audio_track.profile = AudioProfile::AAC;
                                            }

                                            normalizeTrack(audio_track);
                                            normalizeAudioTrack(audio_track);
                                            tracks_info.audio_tracks.append(audio_track);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }

    //Get low quality with veru low speed
    QRegularExpression regexp("og:video\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
    QRegularExpressionMatch match = regexp.match(html);

    if (tracks_info.video_tracks.isEmpty()) {
        if (match.hasMatch()) {
            QString url = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
            url.remove("\" />");
            if (url.contains("http")) {

                VideoTrackInfo video_track;
                video_track.eval_type = ManualEvaluation::Low;

                video_track.urls.append(url);
                normalizeTrack(video_track);
                normalizeVideoTrack(video_track);
                tracks_info.video_tracks.append(video_track);

                AudioTrackInfo audio_track;
                audio_track.eval_type = ManualEvaluation::Low;
                audio_track.from_video_stream = true;
                audio_track.urls.append(url);
                normalizeTrack(audio_track);
                normalizeAudioTrack(audio_track);
                tracks_info.audio_tracks.append(audio_track);
            }
        }
        else {
            regexp = QRegularExpression("og:video:url\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
            match = regexp.match(html);
            if (match.hasMatch()) {
                QString url = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
                url.remove("\" />");
                if (url.contains("http")) {
                    VideoTrackInfo video_track;
                    video_track.eval_type = ManualEvaluation::Low;
                    video_track.urls.append(url);
                    normalizeTrack(video_track);
                    normalizeVideoTrack(video_track);
                    tracks_info.video_tracks.append(video_track);

                    AudioTrackInfo audio_track;
                    audio_track.eval_type = ManualEvaluation::Low;
                    audio_track.from_video_stream = true;
                    audio_track.urls.append(url);
                    normalizeTrack(audio_track);
                    normalizeAudioTrack(audio_track);
                    tracks_info.audio_tracks.append(audio_track);
                }
            }
        }
    }

    qDebug() << "Parsed tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size();
}

Status FacebookParser::parsePage(const QString& html, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;

    QRegularExpression regexp("\"og:title\" content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
    QRegularExpressionMatch match = regexp.match(html);

    QString title;
    
    if (match.hasMatch()) {
        title = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
        title.remove(" | Facebook");
    }

    if (title.isEmpty()) {
        regexp = QRegularExpression("\"twitter:title\" content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
        match = regexp.match(html);
        if (match.hasMatch()) {
            title = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
            title.remove(" | Facebook");
        }
    }

    if (title.isEmpty()) {
        regexp = QRegularExpression("<title>([^<]+)[<]", QRegularExpression::PatternOption::CaseInsensitiveOption);
        match = regexp.match(html);
        if (match.hasMatch()) {
            title = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
            title.remove(" | Facebook");
        }
    }
       
    //Obtain basic info from web page
    if (!title.isEmpty()) {
        video_info.video_title = normalizeTitle(title);
        video_info.metadata_title = video_info.video_title;

        //Get thumbnail url
        regexp = QRegularExpression("\"og:image\" content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
        match = regexp.match(html);
        if (match.hasMatch())
            video_info.artwork_url = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();

        if (video_info.artwork_url.isEmpty()) {
            regexp = QRegularExpression("\"twitter:image\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
            match = regexp.match(html);
            if (match.hasMatch())
                video_info.artwork_url = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText().replace("\\/", "/");
        }

        if (video_info.artwork_url.isEmpty()) {
            regexp = QRegularExpression("\"thumbnailImage\":\{\"uri\":\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
            match = regexp.match(html);
            if (match.hasMatch())
                video_info.artwork_url = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText().replace("\\/", "/");
        }

        //Get duration in seconds
        regexp = QRegularExpression("duration=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
        match = regexp.match(html);
        if (match.hasMatch()) {
            QString duration = match.captured(1).replace("PT","");
            int hour = duration.split("H").first().toInt() * 60 * 60;
            int min = duration.split("H").last().split("M").first().toInt() * 60; //PT0H5M20.179S
            int sec = duration.split("M").last().split("S").first().split(".").first().toInt();
            video_info.duration = hour + min + sec;
        }

#ifdef _DEBUG
        QFile   file("myhtml.html");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(html.toUtf8());
            file.close();
        }
#endif

        getTracksData(html, tracks_info);
        if (tracks_info.video_tracks.isEmpty())
            status.update(EStatusCode::ParseError, "Empty tracks info.");
    }
    else {
        status.update(EStatusCode::ParseError, "Could not retrieve video title.");
    }
    return status;

}

Status FacebookParser::parseHTML(const QString& html, VideoInfo& video_info, TracksInfo& tracks_info) {
    Status status;

    if (html.isEmpty()
        || html.contains("Log in or sign up to view")) {

        //QRegExp expression = QRegExp("<form id=\"login_form\" action=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        //QString login_url;
        //if (expression.indexIn(html) != -1)
        //    login_url = expression.cap(1);

        //status.update(EStatusCode::RegistrationFailed, "Login required.", login_url);
        status.update(EStatusCode::ParseError, "Registration failed");
    }
    else {
        QString html_in = html;
        html_in.replace("\\\"", "\"").replace("\\/", "/").replace("\\u003C", "<").replace("<MPD", "@").replace("</MPD>","~");

        status = parsePage(html_in, video_info, tracks_info);
    }

    return status;
}