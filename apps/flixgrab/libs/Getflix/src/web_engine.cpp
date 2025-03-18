#include "web_engine.h"

void WebEngine::showEvent(QShowEvent *event)
{
    QWebEngineView::showEvent(event);
    emit windowShown();
}

void WebEngine::hideEvent(QHideEvent *event)
{
    QWebEngineView::hideEvent(event);
    //QWidget::hideEvent(event);
    emit windowHidden();
}


void AuthWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit windowShown();
}

void AuthWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    //QWidget::hideEvent(event);
    emit windowHidden();
}

