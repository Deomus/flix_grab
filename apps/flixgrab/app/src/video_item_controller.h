#pragma once 

#include <product_config.h>

#include <QState>
#include <QStateMachine>

#include "table_view.h"
#include "abstract_item.h"

#include "video_handle.h"
#include "video_item_widget.h"
#include "utils.h"

enum EShowMenu {
    kShowMenu_VideoAndAudio = 0,
    kShowMenu_Video,
    kShowMenu_Audio,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EShowMenu)

enum EItemState {
    kItemState_Unknown = 0,
    kItemState_NotReady,
    //Ready For Download;
    kItemState_SettingUp,
    //Scheduled for download
    kItemState_Pending,
    kItemState_Downloading,
    kItemState_Paused,
    
    kItemState_Completed,
    kItemState_Cancelled,
    kItemState_Failed,

    NUM_ITEM_STATES
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EItemState)


class VideoItemWidget;
enum EWidgetState;

class VideoItemController : public AbstractItem {
    friend class VideoItemDelegate;
    Q_OBJECT

public:
    //Used for assignProperty when state is changed;
    Q_PROPERTY(EWidgetState widgetState READ widgetState WRITE setWidgetState);
    Q_PROPERTY(EItemState itemState READ itemState WRITE setItemState);

public:
    VideoItemController(VideoHandle* video, QObject* parent = nullptr);
    ~VideoItemController();

public:
    //Allocate precached item;
    virtual QWidget*                    getWidget(QWidget* parent);
    //Return Item To Pool;
    virtual void                        releaseWidget(void);
    virtual void                        showSelection(bool is_selected = true);

public:
    inline EWidgetState                 widgetState() const { return widget_state_; }
    inline EItemState                   itemState() const { return state_; }
    inline const VideoHandle*           video() const { return video_; }
    inline VideoHandle*                 video() { return video_; }

public:
    void                                filterAssistiveTracks();
    void                                filterAudioTracks(const QList<QString>& languages);
    void                                selectAudioTrack();
    void                                filterImageTracks();

    qint64 get_size_item_for_sort() {
        qint64 item_for_sort = 0;
        if (delegate_)
            item_for_sort = delegate_->get_size_item_for_sort();

        return item_for_sort;
    };
    int get_height_item_for_sort() {
        int item_for_sort = 0;
        if (delegate_)
            item_for_sort = delegate_->get_height_item_for_sort();

        return item_for_sort;
    };
    int get_date_in_milliseconds_now_for_sort() {
        int milliseconds_now_for_sort = 0;
        if (delegate_)
            milliseconds_now_for_sort = delegate_->get_date_in_milliseconds_now_for_sort();

        return milliseconds_now_for_sort;
    };
    void set_date_in_milliseconds_now_for_sort(const int date_in_milliseconds_now) {
        if (delegate_)
            delegate_->set_date_in_milliseconds_now_for_sort(date_in_milliseconds_now);
    };

public slots:
    void download() {
        if (video_)
            video_->download();
    }
    
    void pause() {
        if (video_)
            video_->pause();
    }

    //Remove Item from Lists;
    void    remove() override {
        if (video_)
            video_->cancel();
    }

    //Remove Item and all downloaded data;
    void    removeAndDelete() {
        delete_file_ = true;
        remove();
    }

    void    markToDelete() {
        delete_file_ = true;
    }

    void                                applySettings();
    void                                translateItem();
    void                                showOutputFolder();

signals:
    //Item state changed;
    void                                stateChanged();
    void                                stateAboutToChange(EItemState new_state, EItemState old_state);
    void                                openPlaylist(const QString&);
    void                                showOptions(QPushButton* button);
    
protected slots:
    void                                processWidgetState();
    void                                updateProgress(float value);
    void                                updateInfo(const QString& info);
            
private slots:
 
  /*  void    changeAudioTrack(int audio_track, bool show_offer);
    void    changeAudioTrackWithOffer(int audio_track) { changeAudioTrack(audio_track, true); }
    void    changeVideoTrack(int video_track, bool show_offer);
    void    changeVideoTrackWithOffer(int video_track) { changeVideoTrack(video_track, true); }
    void    changeSubtitleTrack(int subtitle_track, bool show_offer);
    void    changeSubtitleTrackWithOffer(int subtitle_track) { changeSubtitleTrack(subtitle_track, true); }*/

    /*void    changePriority(int priority) {
        if (video_)
            video_->setPriority(priority);
    }
*/
    void    cleanUp();

    //Uses When widget is destroyed;
    void    clearDestroyedWidget() {
        widget_ = nullptr;
    }

private:
    void                                updateState();
    QState*                             createVideoState(EWidgetState widget_state, QState* parent = nullptr);
    QState*                             createVideoState(EWidgetState widget_state, EItemState item_state, QState* parent = nullptr);

    //////////////////////////////////////////////////////////////////////////
    //State Machine;
    void                                createStateMachine();
    // Menus Visibility
    void                                fillBtnSettingsMenu(EShowMenu show_menu = kShowMenu_VideoAndAudio);
	void								updatePlaylistProgress(bool is_in_progress);
	void								updatePlaylist();
    //Selectors;
    void                                selectSaveAsTrack(const int& preset);
    void                                selectVideoTrack();
    void                                selectImageTrack();
    void                                selectSubtitleTracks(const QList<QString>& lang_tags);
    void                                selectSaveSubtitlesAsTracks(const QList<int>& tracks);
    void                                selectAudioLanguageTracks(const QList<QString>& lang_tags);

    //Output name
    QString                             getContentExtension(int preset = DEFAULTPRESET);
    QString                             getOutputPath(int preset = DEFAULTPRESET);
    void                                updateOutputPath(const QString& output_path, int preset = DEFAULTPRESET);

private slots:
    void                                setWidgetState(EWidgetState state);
    void                                setItemState(EItemState state);
    void                                playVideo();

    void                                tryActivate();


private:
    VideoItemDelegate*                  delegate_ = nullptr;
    VideoItemWidget*                    widget_ = nullptr;
    VideoHandle*                        video_ = nullptr;
  
    EWidgetState                        widget_state_ = kWidgetState_Unknown;
    EItemState                          state_ = kItemState_Unknown;
  
private:
    QStateMachine                       machine_;
    int                                 download_priority_ = kDownloadPriority_Normal;

private:
    bool                                delete_file_ = false;

private:
    QSettings                           settings_;
    //Cached settings;
    QVariantMap                         settings_cached_;
};
