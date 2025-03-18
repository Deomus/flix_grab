#pragma once

#include <QtWebEngineWidgets>

//Proxy Web Engine used For emitting signals;
class WebEngine : public QWebEngineView {
    Q_OBJECT
public:

signals:
    void        windowShown();
    void        windowHidden();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

};

class AuthWidget : public QWidget {
    Q_OBJECT
public:

signals :
    void            windowShown();
        void        windowHidden();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

};