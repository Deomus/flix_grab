#pragma once

#include "web_portal.h"
#include "dailymotion_parser.h"


class DailymotionInterceptor;

class DailymotionInterceptor : public WebPortal {
public:
    DailymotionInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr) : WebPortal(page, nam, p)
        , parser_(nullptr) {
    }

    DailymotionInterceptor::~DailymotionInterceptor() {
        delete parser_;
        parser_ = nullptr;
        qDebug() << "Interceptor: ~DailymotionInterceptor";
    }

private:
    void createParser(QWebEnginePage* page, QNetworkAccessManager* nam) override {
        Q_UNUSED(page);
        Q_UNUSED(nam);
        if (parser_ == nullptr)
            parser_ = new DailymotionParser(this);
    }

    bool parserExists() override {
        return (parser_ != nullptr);
    }

    Status parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) override {
        return parser_->parseHTML(data, video_info, tracks_info, url);
    }

private:
    DailymotionParser*              parser_;
};
