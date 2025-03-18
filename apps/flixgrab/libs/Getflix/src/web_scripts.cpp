#include "web_scripts.h"

WebScripts::WebScripts(QObject* p /*= nullptr*/) : QObject(p) {

}

WebScripts::~WebScripts() {
    qDebug() << "~WebScripts";
}

void WebScripts::run(QWebEnginePage* page, bool enable_defer /*= false*/) {
    //qDebug() << "Run scripts: " << async_.size() << " + " << defer_.size();

    enable_defer_ = enable_defer;

    if (!current_)
        runInternal(page);
}

void WebScripts::clear() {
    async_.clear();
    defer_.clear();
    //running_ = false;
    enable_defer_ = false;
}

Q_INVOKABLE void WebScripts::runInternal(QWebEnginePage* page) {
    Q_ASSERT(!current_ && page);

    if (!async_.isEmpty())
        current_ = async_.takeFirst();
    else if (enable_defer_ && !defer_.isEmpty())
        current_ = defer_.takeFirst();

    if (current_) {
        auto runCurrent = [this](QWebEnginePage* page) {
            if (current_) {
                Q_ASSERT(current_);
                WebContent* script = current_;
                page->runJavaScript(script->data(), [=](const QVariant& result) {
                    if (current_) {
                        Q_ASSERT(script == current_);
                        //Callback in same thread;
                        qDebug() << "Script url=" << current_->url() << " result: " << result;
                        current_.clear();
                        //Next script;
                        runInternal(page);
                    }
                });
            }
        };

        if (!current_->isValid()) {
            QObject::connect(current_, &WebContent::finished, this, [=]() {
                Q_ASSERT(sender() == current_);

                if (!current_->isValid())
                    current_.clear();
                runCurrent(page);
                //Remove connection;
                QObject::disconnect(current_, &WebContent::finished, this, 0);
            }, Qt::QueuedConnection);
        }
        else
            runCurrent(page);
    }
    else {
        qDebug() << "Page Scripts run Finished";
        emit finished();
    }
}
