#pragma once

#include <QtWidgets>

#include "ui_video_control.h"

#include "qmenu.h"
#include "menu.h"
#include "menu_submenu.h"
#include "menu_checkbox.h"

#include "video_item_delegate.h"


class MenuList : public Menu {

};

enum EControlState {
    kControlState_Unknown = 0,
    kControlState_Preparing,
    kControlState_SettingUp,
    kControlState_Pending,
    kControlState_Transitioning,
    kControlState_Processing,
    kControlState_Paused,
    kControlState_Completed,
    //Cancel download or fail;
    kControlState_Canceled,
    kControlState_Failed,
};


class VideoControl : public QFrame {
    Q_OBJECT
public:
    VideoControl(QWidget* parent = 0);

    void            setState(EControlState state);

    //void            setHovered(bool is_hovered = true);
    //void            updateTracksInfo(const TracksInfo& tracks_info);
    void            setup(VideoItemDelegate* delegate);
    void            setupPlaylist(VideoItemDelegate* delegate);
    void            translate();

    void            fillBtnSettingsMenu(int show_menu = 0);
    void            fillBtnPlaylistMenu();

    void            fillPlaylistMenu(MenuSubmenu* submenu, const VideoInfo& info, QPair<int, QList<QPair<QString, QString>>> playlist);
    MenuCheckbox*   createBtnAllSubMenu();
    void            fillOnePlaylistMenu(QMenu* submenu, const VideoInfo& info, QPair<int, QList<QPair<QString, QString>>> playlist);
    void            fillPriorityMenu();
    void            fillSaveAsMenu();
    void            fillAudioMenu();
    void            fillVideoMenu();
    void            fillSubtitleMenu();
    void            fillSubtitleSaveAsMenu();
    void            fillAudioLanguageMenu();
    void            fillImageMenu();
    //Item left button pressed
    void            removeItem();

    void            setAboutToHideSubMenu();
    void            setSubMenuChecked(MenuSubmenu* submenu);

    bool            is_multiple_playList_ = false;
    bool            submenu_visible_ = false;
    bool            playlist_menu_visible_ = false;
    bool            timer_is_running_ = false;

signals:
            
    //void            audioChanged();
    //void            videoChanged(int videoTrack);
    //void            subtitleChanged(int subtitleTrack);
    //void            imageChanged(int imageTrack);

    //Main (Center) Button is Pressed
    void            stateToProcess();
    //Folder (left) Button is Pressed;
    void            signalshowOptions(QPushButton* button);
    //void            outputFolderToShow();
    //Playlist Button is Pressed;
    void            openPlaylist(const QString&);

    //Remove Right Button Pressed
    void            itemToRemove();
    void            itemToRemoveAndDelete();

    void            outputPathChanged(QString path);

private slots:
    void            clickedAllList();

private:
    MenuSubmenu*    createPlaylistSettings(const PlaylistKind& playlist_kind,
        const QPair<int, QList<QPair<QString, QString>>>& playlist);
    MenuList*       createOnePlaylistSettings(const PlaylistKind& playlist_kind,
        const QPair<int, QList<QPair<QString, QString>>>& playlist);
    void            createPlaylist();
    void            createSettings();
    void            createPrioritySettings();
    void            createSaveAsSettings();
    void            createAudioSettings();
    void            createVideoSettings();
    void            createSubtitleSettings();
    void            createSaveSubtitlesAsSettings();
    void            createAudioLanguagesSettings();
    void            createImageSettings();

	void			showBtnPlaylist();

    QString         playlistKindName(const PlaylistKind&);
    QString         playlistSubname(const int yt_content_kind);
    QString         priorityDesc(const EDownloadPriority&);
    QString         saveSubtitlesAsDesc(const ESaveSubtitlesAs&);

    bool            eventFilter(QObject* o, QEvent* e);

private:
    Ui::VideoControl                ui_;
    EControlState                   state_ = kControlState_Unknown;

    //bool                            is_hovered_ = false;
private:
    QList<MenuSubmenu*>             playlist_submenus_;
    MenuSubmenu*                    priority_submenu_ = nullptr; //todo move to video_item_delegate
    MenuSubmenu*                    save_as_submenu_ = nullptr;
    MenuSubmenu*                    audio_submenu_ = nullptr;
    MenuSubmenu*                    audio_languages_submenu_ = nullptr;
    MenuSubmenu*                    video_submenu_ = nullptr;
    MenuSubmenu*                    subtitle_submenu_ = nullptr;
    MenuSubmenu*                    subtitle_saveas_submenu_ = nullptr;
    MenuSubmenu*                    image_submenu_ = nullptr;

    VideoItemDelegate*              delegate_ = nullptr;

private:
    QString  menu_style_ = "QMenu{"
        "font-family: \"Tahoma\";"
        "font-size: 9pt;"
        "border: none;"
        "}"
        "QMenu::separator{"
        "background-color: #858A9B;"
        "height: 1px;"
        "}"
        "QMenu::separator:hover{"
        "background-color: #858A9B;"
        "}";

    QString  btn_playlist_style_ =
        "#btnPlaylist {"
        "text-align: right;"
        "font: bold 9pt \"Tahoma\";"
        "color:rgba(0,0,0,150);"
        "padding-right:13px;"
        "}"
        "#btnPlaylist[inverted=true] {"
        "color:rgba(255,255,255,140);"
        "}"
        "#btnPlaylist:hover {"
        "color: #F2F3F3;"
        "}"
        "#btnPlaylist:pressed {"
        "text-align: right;"
        "color: #F2F3F3;"
        "background-color: #2A2E3A;"
        "}";
};
