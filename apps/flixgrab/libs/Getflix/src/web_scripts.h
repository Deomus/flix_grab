#pragma once

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>

#include "status.h"
#include "web_content.h"

class WebScripts : public QObject {
    Q_OBJECT

public:
    WebScripts(QObject* p = nullptr);

    ~WebScripts();

public:
    void                addAsync(WebContent* script) {
        async_.push_back(script);
    }

    void                addDefer(WebContent* script) {
        defer_.push_back(script);
    }

    void                run(QWebEnginePage* page, bool enable_defer = false);
    void                clear();

    bool                isRunning() const {
        return current_;
    }

signals:
    void                finished();

private:
    Q_INVOKABLE void    runInternal(QWebEnginePage* page);


private:
    QList<WebContent*>              async_;
    QList<WebContent*>              defer_;
    QPointer<WebContent>            current_;

    bool                            enable_defer_ = false;
};
