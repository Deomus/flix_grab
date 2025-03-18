#pragma once

#include "web_portal.h"
#include "video_info.h"
#include "youtube_parser.h"


class YoutubeInterceptor;

class YoutubeInterceptor : public WebPortal {
public:
    YoutubeInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr) : WebPortal(page, nam, p)
        , parser_(nullptr) {
    }

    YoutubeInterceptor::~YoutubeInterceptor() {
		if (abortable_) {
			abortable_->abort();
			abortable_->waitForFinished();
		}
		waitForFinished();
        delete parser_;
        parser_ = nullptr;
        qDebug() << "Interceptor: ~YoutubeInterceptor";
    }

	void analyzePlaylist(VideoInfo& info) override {
		Q_ASSERT(parser_ != nullptr);
		Q_ASSERT(abortable_ != nullptr);
		auto operation = abortable_->startOperation();
		parser_->getPlaylistOrChannel(info, playlists_data_, abortable_);
	}

private:
    void createParser(QWebEnginePage* page, QNetworkAccessManager* nam) override {
        if (nam != nullptr && page != nullptr && parser_ == nullptr) {
            parser_ = new YoutubeParser(page, nam, this);
            QObject::connect(parser_, SIGNAL(loadPlaylist(const QString&)), this, SIGNAL(loadPlaylist(const QString&)), Qt::UniqueConnection);
            QObject::connect(parser_, SIGNAL(loadPlaylistPart(const QString&)), this, SIGNAL(loadPlaylistPart(const QString&)), Qt::UniqueConnection);
        }
    }

    bool parserExists() override { return (parser_ != nullptr); }

    Status parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) override {
		Q_ASSERT(parser_ != nullptr);
		Q_ASSERT(abortable_ != nullptr);
		auto operation = abortable_->startOperation();
        return parser_->parseHTML(data, url, video_info, tracks_info, playlists_data_, abortable_);
    }

private:
    YoutubeParser*              parser_;
    QList<QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>> playlists_data_; //QPair<YtContentKind, QList<QPair<plyalist_id, QList<QPair<video_title + video_duration, video_url>>>>>
};
