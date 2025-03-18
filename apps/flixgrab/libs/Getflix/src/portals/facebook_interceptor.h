#pragma once

#include "web_portal.h"
#include "facebook_parser.h"


class FacebookInterceptor;

class FacebookInterceptor : public WebPortal {
public:
    FacebookInterceptor(QWebEnginePage *page, QNetworkAccessManager* nam, QObject* p = nullptr) : WebPortal(page, nam, p)
        , parser_(nullptr) {
    }

    FacebookInterceptor::~FacebookInterceptor() {
        delete parser_;
        parser_ = nullptr;
        qDebug() << "Interceptor: ~FacebookInterceptor";
    }

private:
    void interceptRequest(QWebEngineUrlRequestInfo &info) override {//Called from IO thread of browser;
        if (signed_in_) {

            qDebug() << ">>>>>> " << info.requestUrl();

            if (info.resourceType() == QWebEngineUrlRequestInfo::ResourceTypeMainFrame) {
                qDebug() << "Interceptor: getMetadata url:" << info.requestUrl().toEncoded();
                getMetadata(info.requestUrl());
                info.block(true);
            }
            else if (info.requestUrl().toEncoded().contains("static.xx.")) {
                emit error(Status(EStatusCode::ForcedReloadPage));
            }
        }
    }

    void createParser(QWebEnginePage* page, QNetworkAccessManager* nam) override {
        if (nam != nullptr && page != nullptr && parser_ == nullptr)
            parser_ = new FacebookParser(page, nam, this);
    }

    bool parserExists() override {
        return (parser_ != nullptr);
    }

    Status parse(const QString& data, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) override {
        Q_UNUSED(url);
        return parser_->parseHTML(data, video_info, tracks_info);
    }

private:
    void getMetadata(const QUrl& url) {
        add_header("accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
        add_header("accept-encoding", "gzip, deflate");
        add_header("viewport-width", "3840");
        
        fetch(url);
    }

private:
    FacebookParser*              parser_;
};
