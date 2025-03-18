#include "video_item_widget.h"

#include <QFileInfo>
#include <QMouseEvent>
#include <QMovie>

#include <limitations.h>

#include "video_item_controller.h"
#include "product_controller.h"

#define LINK_LENGTH_MAX         42
#define TOOLTIP_LENGTH_MAX      120


VideoItemWidget::VideoItemWidget(QWidget *parent) : QWidget(parent)
{
    ui_.setupUi(this);

    control_ = new VideoControl(ui_.wgtMain);
    control_->setVisible(false);

    ui_.lineTitle->installEventFilter(this);

    /*QObject::connect(control_, &VideoControl::tracksSelected, this, [this]() {
        if (delegate_)
            setInfo(delegate_->tracksStatusDesc());
        else
            setInfo("");
    });*/
    //QObject::connect(control_, &VideoControl::audioChanged, this, &VideoItemWidget::updateTrackStatus);
    //QObject::connect(control_, &VideoControl::videoChanged, this, &VideoItemWidget::updateTrackStatus);

    QObject::connect(ui_.btnOffer, &QPushButton::clicked, this, &VideoItemWidget::offerClicked);

    //Qt::Window - drop parent positions - do not use it;;
    QWidget::setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover);

    ui_.wgtWaiting->setColor(ui_.wgtWaiting->palette().color(QPalette::Highlight));
    ui_.wgtWaitingPlaylist->setColor(ui_.wgtWaitingPlaylist->palette().color(QPalette::Highlight));
	ui_.wgtWaitingPlaylist->setVisible(false);
}

VideoItemWidget::~VideoItemWidget()
{
    qDebug() << "~VideoItemWidget()";
}

bool VideoItemWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::FocusOut
        && delegate_) {
        QLineEdit *ed = static_cast<QLineEdit *> (obj);
        if (ed->text() != ui_.lineTitle->toolTip()) {
            QString ed_text_out = makeFilename(ed->text());
            delegate_->correctOutputName(delegate_->outputPath(), ed_text_out);
            UpdateTitle(ed_text_out);
            delegate_->setOutputPath(delegate_->outputPath(), ed_text_out);
        }
    }

    return false; // We return false to ignore the event and allow the child to recieve the event normally
}

void VideoItemWidget::showOffer(const EWidgetState& state, const EPortal& portal) {
    if (state == EWidgetState::kWidgetState_Pending) {
        ui_.lblOfferThreads->setText(tr("Parallel download locked"));
        ui_.btnOffer->setText(tr("Click to unlock"));

        ui_.lblOfferThreads->setVisible(true);
    }
    else {
        if (contentLengthLimitEnabled()) {
            QString video_limit = QString::number(durationLimit(portal));
            ui_.lblOfferSpeed->setText(tr("%1 minute limit").arg(video_limit));
            ui_.btnOffer->setText(tr("Click to download full!"));
        }
        else if (loadingSlowdownEnabled()) {
            ui_.lblOfferSpeed->setText(tr("Reduced speed"));
            ui_.btnOffer->setText(tr("Click to download faster!"));
        }

        ui_.lblOfferSpeed->setVisible(true);
    }

    ui_.btnOffer->setVisible(true);
    ui_.lblProgress->setVisible(false);
}

void VideoItemWidget::setState(EWidgetState state) {
    if (state == state_)
        return;

    //Offer on Item
    ui_.btnOffer->setVisible(false);
    ui_.lblOfferThreads->setVisible(false);
    ui_.lblOfferSpeed->setVisible(false);
    ui_.lblProgress->setVisible(true);
    ui_.lblError->setVisible(false);
    ui_.lineTitle->setEnabled(false);
    ui_.lineTitle->setCursor(QCursor(Qt::ArrowCursor));

    switch (state) {
        case kWidgetState_Enqueued:
            ui_.stkMain->setCurrentWidget(ui_.main_transparent);
            ui_.stkTitle->setCurrentWidget(ui_.link);
            ui_.stkStatus->setCurrentWidget(ui_.status_transparent);

            ui_.wgtGradient->setProperty("state", "normal");

            control_->setState(kControlState_Preparing);
            break;
        case kWidgetState_Analyzing:
            ui_.stkMain->setCurrentWidget(ui_.waiting);
            ui_.stkTitle->setCurrentWidget(ui_.link);
            ui_.stkStatus->setCurrentWidget(ui_.status_transparent);

            ui_.wgtWaiting->setType(ESpinnerType::kSpinnerType_Analize);
            ui_.wgtGradient->setProperty("state", "normal");

            control_->setState(kControlState_Preparing);
            break;
        case kWidgetState_Described:
            ui_.stkMain->setCurrentWidget(ui_.waiting);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.status_transparent);
            
            ui_.wgtWaiting->setType(ESpinnerType::kSpinnerType_Analize);
            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_ 
                && highContrastImageMaskEnabled(delegate_->portal()))
                ui_.wgtGradient->setProperty("state", "high_contrast_image");

            control_->setState(kControlState_Preparing);
            break;
        case kWidgetState_SettingUp:
            ui_.stkMain->setCurrentWidget(ui_.main_transparent);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.progress);

            ui_.lineTitle->setEnabled(true);
            ui_.lineTitle->setCursor(QCursor(Qt::IBeamCursor));
            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_ 
                && highContrastImageMaskEnabled(delegate_->portal()))
                ui_.wgtGradient->setProperty("state", "high_contrast_image");

            control_->setState(kControlState_SettingUp);
            break;
        case kWidgetState_Pending:
            ui_.stkMain->setCurrentWidget(ui_.waiting);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.progress);

            ui_.lineTitle->setEnabled(true);
            ui_.lineTitle->setCursor(QCursor(Qt::IBeamCursor));
            ui_.prgBar->setFormat(tr("Pending..."));
            ui_.wgtWaiting->setType(ESpinnerType::kSpinnerType_Pending);
            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_) {
                if (highContrastImageMaskEnabled(delegate_->portal()))
                    ui_.wgtGradient->setProperty("state", "high_contrast_image");
                if (!delegate_->isActivated())
                    showOffer(state, delegate_->portal());
            }

            control_->setState(kControlState_Pending);
            break;
        case kWidgetState_Transitioning:
            ui_.stkMain->setCurrentWidget(ui_.main_transparent);

            if (delegate_
                && delegate_->isDownloadable()) {
                ui_.stkTitle->setCurrentWidget(ui_.title);
                ui_.stkStatus->setCurrentWidget(ui_.progress);

                ui_.wgtGradient->setProperty("state", "image");
                if (highContrastImageMaskEnabled(delegate_->portal()))
                    ui_.wgtGradient->setProperty("state", "high_contrast_image");
            }
            else {
                ui_.stkTitle->setCurrentWidget(ui_.link);
                ui_.stkStatus->setCurrentWidget(ui_.status_transparent);

                ui_.wgtGradient->setProperty("state", "normal");
            }

            setAwaiting();

            control_->setState(kControlState_Transitioning);
            break;
        case kWidgetState_Downloading:
            ui_.stkMain->setCurrentWidget(ui_.main_transparent);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.progress);
            
            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_) {
                if (highContrastImageMaskEnabled(delegate_->portal()))
                    ui_.wgtGradient->setProperty("state", "high_contrast_image");

                if (!delegate_->isActivated()) {
                    if (loadingSlowdownEnabled()
                        || contentLengthLimitEnabled())
                        showOffer(state, delegate_->portal());
                }
            }

            setAwaiting(false);
            
            control_->setState(kControlState_Processing);
            break;
        case kWidgetState_Paused:
            ui_.stkMain->setCurrentWidget(ui_.main_transparent);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.progress);

            ui_.prgBar->setFormat(tr("Paused"));
            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_) {
                if (highContrastImageMaskEnabled(delegate_->portal()))
                    ui_.wgtGradient->setProperty("state", "high_contrast_image");

                if (!delegate_->isActivated()) {
                    if (loadingSlowdownEnabled()
                        || contentLengthLimitEnabled())
                        showOffer(state, delegate_->portal());
                }
            }

            setAwaiting(false);

            control_->setState(kControlState_Paused);
            break;
            //kVideoItemState_Converting,
        case kWidgetState_Completed:
            ui_.stkMain->setCurrentWidget(ui_.main_transparent);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.status_transparent);

            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_ 
                && highContrastImageMaskEnabled(delegate_->portal()))
                ui_.wgtGradient->setProperty("state", "high_contrast_image");

            control_->setState(kControlState_Completed);
            break;
        case kWidgetState_Cancelled:
            ui_.stkMain->setCurrentWidget(ui_.waiting);
            ui_.stkTitle->setCurrentWidget(ui_.title);
            ui_.stkStatus->setCurrentWidget(ui_.status_transparent);

            ui_.wgtWaiting->setType(ESpinnerType::kSpinnerType_Analize);
            ui_.wgtGradient->setProperty("state", "image");
            if (delegate_ 
                && highContrastImageMaskEnabled(delegate_->portal()))
                ui_.wgtGradient->setProperty("state", "high_contrast_image");

            control_->setState(kControlState_Canceled);
            break;
        case kWidgetState_Failed:
            ui_.stkMain->setCurrentWidget(ui_.error);
            ui_.stkTitle->setCurrentWidget(ui_.link);
            ui_.stkStatus->setCurrentWidget(ui_.status_transparent);

            ui_.lblError->setVisible(true);
            ui_.wgtGradient->setProperty("state", "alert");

            control_->setState(kControlState_Failed);
            break;
    }

    ui_.wgtGradient->style()->polish(ui_.wgtGradient);
    ui_.wgtGradient->update();

    state_ = state;
}

void VideoItemWidget::UpdateWidget(EWidgetState state, VideoItemDelegate* delegate) {
    Q_ASSERT(delegate != nullptr);

    VideoItemDelegate* prev_delegate = std::exchange(delegate_, delegate);

    //video_cached_ = video;
    if (state_ != state
        || prev_delegate != delegate) {

        if (delegate_) {
            QString link_text = delegate_->videoUrl();
            if (link_text.length() > LINK_LENGTH_MAX) {
                link_text.truncate(LINK_LENGTH_MAX);
                link_text.append("...");
            }

            QString tooltip_link_text = delegate_->videoUrl();
            QString tooltip_text;
            if (tooltip_link_text.length() > TOOLTIP_LENGTH_MAX + 10) {
                tooltip_text.append(tooltip_link_text.left(TOOLTIP_LENGTH_MAX)).append("\n");
                tooltip_link_text.remove(0, TOOLTIP_LENGTH_MAX);
                tooltip_text.append(tooltip_link_text);
            }
            else {
                tooltip_text = tooltip_link_text;
            }

            switch (state) {
                case kWidgetState_Enqueued:
                    ui_.lblLink->setLink(link_text, delegate_->videoUrl());
                    ui_.lblLink->setToolTip(tooltip_text);
                    ui_.lineTitle->clear();
                    UpdateInfo("");
                    this->setDuration(0);
                    this->UpdateProgress(0);
                    break;
                case kWidgetState_Described:
                    ui_.body->setBackground(portalArtwork(delegate_->portal()));
                    ui_.body->loadImage(delegate_->videoInfo().artwork_url);
                    break;
                case kWidgetState_SettingUp:
                    control_->setup(delegate_);
                    updateItemInfo();
                    this->UpdateProgress(0);
                    break;
                case kWidgetState_Pending:
                case kWidgetState_Downloading:
                case kWidgetState_Paused:
                    updateItemInfo();
                    this->UpdateProgress(delegate_->progress());
                    break;
                case kWidgetState_Completed:
                    updateItemInfo();
                    break;
                case kWidgetState_Failed:
                    ui_.lblLink->setLink(link_text, delegate_->videoUrl());
                    ui_.lblLink->setToolTip(tooltip_text);
                    ui_.lblError->setText(delegate_->statusDesc());
                    //ui_.body->setBackground(portalArtwork(delegate_->portal()));
                    break;
            }
        }
    }

    setState(state);
}

void VideoItemWidget::updateItemInfo() {
    if (delegate_) {
        UpdateTitle(delegate_->outputName());
        delegate_->updateItemDesk();
        setDuration(delegate_->videoInfo().duration);
        ui_.body->loadImage(delegate_->videoInfo().artwork_url);
    }
}

void VideoItemWidget::UpdateTitle(const QString& title) {
    ui_.lineTitle->setText(title);
    ui_.lineTitle->setToolTip(title);
    ui_.lineTitle->setCursorPosition(0);
}

void VideoItemWidget::UpdateInfo(const QString& info) {
    //QFontMetrics metric(ui_.lblInfo->font());
    //QString clipped_text = metric.elidedText(info, Qt::ElideLeft, ui_.lblInfo->width());
    ui_.lblInfo->setText(info);
    ui_.lblInfo->setToolTip(info);
}

void VideoItemWidget::UpdateProgress(float value) {
    if (value <= 0)
        value = 0;

    if (value > 0 &&
        value < 1)
        value = 1;

    setProgress(value);

    if (value > 100)
        value = 100;
    QString progress_text = QString("%1%").arg(value, 1, 'f', 1, '0');
    ui_.prgBar->setFormat(progress_text);
    ui_.lblProgress->setText(QString("%1%").arg((int)value));
}

void VideoItemWidget::updatePlaylistProgress(bool is_in_progress) {
	ui_.wgtWaitingPlaylist->setVisible(is_in_progress);
}
