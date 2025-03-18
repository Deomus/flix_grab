#pragma once

#include "web_portal.h"
#include "vimeo_parser.h"


class VimeoInterceptor;

class VimeoInterceptor : public WebPortal {
public:
    VimeoInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr) : WebPortal(page, nam, p)
        , parser_(nullptr) {
    }

    ~VimeoInterceptor() {
        delete parser_;
        parser_ = nullptr;
        qDebug() << "Interceptor: ~VimeoInterceptor";
    }

private:
    void createParser(QWebEnginePage* page, QNetworkAccessManager* nam) override {
        Q_UNUSED(page);
        Q_UNUSED(nam);
        if (parser_ == nullptr)
            parser_ = new VimeoParser(this);
    }

    bool parserExists() override {
        return (parser_ != nullptr);
    }

    Status parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) override {
        return parser_->parseHTML(data, video_info, tracks_info, url);
    }

private:
    VimeoParser*              parser_;
};
