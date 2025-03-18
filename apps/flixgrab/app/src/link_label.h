#pragma once

#include <QtWidgets>

class LinkLabel : public QLabel {

    Q_OBJECT
public:
    LinkLabel(QWidget* parent = 0)
        : QLabel(parent)
        , is_underline_(false) {

        setAttribute(Qt::WA_Hover);
        
        setTextFormat(Qt::RichText);
        setOpenExternalLinks(true);

        setLink("Click Me !", "www.youtube.com");
    }

    void				setLink(const QString& text, const QString& url) {
        //<a href="www.youtube.com"><span style=" text-decoration: underline; color:#0000ff;">Click me!</span></a>
        text_ = text;
        url_ = url;
        updateLink();
    }

protected:
   

    void mouseMoveEvent(QMouseEvent * event) {
        QLabel::mouseMoveEvent(event);
        event->ignore();
    }

    void mouseReleaseEvent(QMouseEvent * event) {
        QLabel::mouseReleaseEvent(event);
        event->ignore();
    }


    bool event(QEvent * e) {
        switch (e->type())
        {
        case QEvent::HoverEnter:
            is_underline_ = true;
            updateLink();
            break;
        case QEvent::HoverLeave:
            is_underline_ = false;
            updateLink();
            break;
        default:
            break;
        }
        return QLabel::event(e);
    }

protected:
    void				updateLink(void) {
        //<a href="www.youtube.com"><span style=" text-decoration: underline; color:#0000ff;">Click me!</span></a>
        const QString strUnderline("underline");
        const QString strNone("none");
        setText(QString("<a href=\"%1\"><span style=\"text-decoration: %2; color:#2A8CF1; \">%3</span></a>").arg(url_, is_underline_ ? strUnderline : strNone, text_));
    }

private:
    QString						text_;
    QString						url_;

    bool                        is_underline_;

};