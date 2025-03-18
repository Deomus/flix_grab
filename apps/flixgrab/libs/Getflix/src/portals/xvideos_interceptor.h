#pragma once

#include "web_portal.h"
#include "xvideos_parser.h"


class XVideosInterceptor;

class XVideosInterceptor : public WebPortal {
public:
    XVideosInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr) : WebPortal(page, nam, p)
        , parser_(nullptr) {
    }

    XVideosInterceptor::~XVideosInterceptor() {
        delete parser_;
        parser_ = nullptr;
        qDebug() << "Interceptor: ~XVideosInterceptor";
    }

private:
    void createParser(QWebEnginePage* page, QNetworkAccessManager* nam) override {
        Q_UNUSED(page);
        Q_UNUSED(nam);
        if (parser_ == nullptr)
            parser_ = new XVideosParser(this);
    }

    bool parserExists() override {
        return (parser_ != nullptr);
    }

    Status parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) override {
        Q_UNUSED(url);
        return parser_->parseHTML(data, video_info, tracks_info);
    }

private:
    XVideosParser*              parser_;

};
