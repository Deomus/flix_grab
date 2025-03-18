#pragma once

#include <QtWidgets>

#include "ui_video_item.h"

#include "video_item_delegate.h"
#include "video_control.h"


enum EWidgetState
{
    kWidgetState_Unknown = 0,
    //TODO: add signal for start analyzing
    kWidgetState_Enqueued,
    kWidgetState_Analyzing,
    kWidgetState_Described,
    //Ready For Download;
    kWidgetState_SettingUp,
    //Scheduled for download
    kWidgetState_Pending,
    //Time consuming Transitioning between states
    kWidgetState_Transitioning,
    kWidgetState_Downloading,
    kWidgetState_Paused,
    //Not Using Now;
    kWidgetState_Converting,
    kWidgetState_Completed,
    kWidgetState_Cancelled,
    kWidgetState_Failed,

};
//Make available for QVariant;
Q_DECLARE_METATYPE(EWidgetState)


class VideoItemWidget : public QWidget
{
    Q_OBJECT

public:
    VideoItemWidget(QWidget *parent);
    ~VideoItemWidget();

    virtual         void    ShowSelection(bool selected = false)
    {
      
        ui_.frame->setProperty("selected", selected);
  //      ui_.frame->style()->unpolish(ui_.frame);
        ui_.frame->style()->polish(ui_.frame);
        update();
            
    }

    virtual void        Release() {
        delegate_ = nullptr;
    }

    void                UpdateWidget(EWidgetState state, VideoItemDelegate* delegate);
    void                UpdateTitle(const QString& title);
    void                UpdateInfo(const QString& info);
    void                UpdateProgress(float value);
	void				updatePlaylistProgress(bool is_in_progress);
    void                translate() { ui_.retranslateUi(this); }
      
    VideoControl*      Control() { return control_; }

    //void    updateTrackStatus() {
    //    if (video_cached_) {
    //        const TracksInfo& info = video_cached_->tracksInfo();
    //        if (info.video_tracks.size() > control_->videoTrack() && 
    //            /*info.audio_tracks.size() > control_->audioTrack() && */
    //            control_->videoTrack() >= 0 /*&&
    //            control_->audioTrack() >= 0*/) {
    //            VideoTrackInfo video = info.video_tracks[control_->videoTrack()];
    //           // AudioTrackInfo audio = info.audio_tracks[control_->audioTrack()];
    //            //int size = (video.size + audio.size) / (1024 * 1024);
    //            int size = (video.size ) / (1024 * 1024);

    //            //QString text = QString("%1p / %2 / %3 Mb").arg(video.height).arg(audio.language).arg(size);
    //            QString text = QString("%1p / %2 / %3 Mb").arg(video.height).arg("TODO").arg(size);
    //            setInfo(text);
    //        }
    //        
    //    }
    //    
    //}

signals:
    //Offer clicked;
    void            offerClicked();

private:
    virtual bool        eventFilter(QObject *obj, QEvent *event) override;

private:
    void                updateItemInfo();
    void                setState(EWidgetState state);
    void                showOffer(const EWidgetState& state, const EPortal& portal);
    void                setDuration(int seconds) {
        if (seconds > 0) {
            QString duration = QDateTime::fromSecsSinceEpoch(seconds).toUTC().toString(seconds < 3600 ? "mm:ss" : "hh:mm:ss");
            ui_.lblDuration->setText(duration);
            ui_.lblDuration->setToolTip(duration);
        }
        else {
            ui_.lblDuration->setText("");
        }
    }

    void    setProgress(float value) {
        ui_.prgBar->setMaximum(1000);
        ui_.prgBar->setMinimum(0);

        if (value > 999)
            value = 999;

        ui_.prgBar->setValue(value * 10);
    }

    void    setAwaiting(bool is_awaiting = true) {
        if (is_awaiting) {
            ui_.prgBar->setMaximum(0);
            ui_.prgBar->setMinimum(0);
            ui_.prgBar->setValue(0);
        }
    }

protected:
    void enterHover()
    {
        //control_->setHovered(true);
        control_->setVisible(true);
    }

    void leaveHover()
    {
        //control_->setHovered(false);
        control_->setVisible(false);
    }

    bool event(QEvent * e)
    {
        switch (e->type())
        {
        case QEvent::HoverEnter:
            enterHover();
            return true;
            break;
        case QEvent::HoverLeave:
            leaveHover();
            return true;
            break;
        default:
            break;
        }
        return QWidget::event(e);
    }


private:
    Ui::VideoItem               ui_;
    EWidgetState                state_;

    VideoControl*               control_;

private:
    VideoItemDelegate*          delegate_ = nullptr;

};


