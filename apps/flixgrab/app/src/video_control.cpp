#include "video_control.h"

#include <coroutine>

#include <QScreen>

#include "utils.h"

#include "settings.h"
#include "menu_checkbox.h"
#include "trans_menu.h"
#include "video_item_controller.h"

#include "alert_dialog.h"

#include "product_config.h"


VideoControl::VideoControl(QWidget* parent /*= 0*/) : QFrame(parent) {
    ui_.setupUi(this);
    //setHovered(false);
    //hide();
    connect(ui_.btnCenter, &QPushButton::clicked, this, &VideoControl::stateToProcess);
    connect(ui_.btnRemove, &QPushButton::clicked, this, &VideoControl::itemToRemove);
    connect(ui_.btnPlaylist, &QPushButton::clicked, this, [this]() {
        if (delegate_
            && !delegate_->videoInfo().playlist.isEmpty()
            && delegate_->videoInfo().playlist_kind == PlaylistKind::kEmptyPlaylist) { //btnPlaylist without menu

            if (delegate_->isActivated())
                emit openPlaylist(delegate_->videoInfo().playlist.first().second.first().second);
            else
                AlertDialog::execDialog(EAlert::kAlert_LimitPlaylist, delegate_->portal());
        }
    });
}

void VideoControl::setState(EControlState state) {
    if (state == state_)
        return;

    for (auto playlist_submenu : playlist_submenus_)
        playlist_submenu->requestAction()->setEnabled(false);

    if (priority_submenu_)
        priority_submenu_->requestAction()->setEnabled(false);
    if (save_as_submenu_)
        save_as_submenu_->requestAction()->setEnabled(false);
    if (video_submenu_)
        video_submenu_->requestAction()->setEnabled(false);
    if (audio_submenu_)
        audio_submenu_->requestAction()->setEnabled(false);
    if (audio_languages_submenu_)
        audio_languages_submenu_->requestAction()->setEnabled(false);
    if (image_submenu_)
        image_submenu_->requestAction()->setEnabled(false);
    if (subtitle_submenu_)
        subtitle_submenu_->requestAction()->setEnabled(false);
    if (subtitle_saveas_submenu_)
        subtitle_saveas_submenu_->requestAction()->setEnabled(false);

    ui_.btnOptions->setEnabled(true);
    ui_.btnSettings->setEnabled(true);
    ui_.btnRemove->setEnabled(true);
    ui_.btnCenter->setEnabled(true);
    ui_.btnPlaylist->setEnabled(true);

    //Set Default Visibles;
    ui_.btnOptions->setVisible(true);
    ui_.btnSettings->setVisible(true);
    ui_.btnRemove->setVisible(true);
    ui_.btnCenter->setVisible(true);
    ui_.btnPlaylist->setVisible(false);


    ui_.btnOptions->setInverted(true);
    ui_.btnSettings->setInverted(true);
    ui_.btnRemove->setInverted(true);
    ui_.btnCenter->setInverted(true);
    ui_.btnPlaylist->setInverted(true);

    switch (state) {
        case kControlState_Preparing:
            ui_.btnCenter->setVisible(false);
            ui_.btnOptions->setVisible(false);
            ui_.btnSettings->setVisible(false);
            break;
        case kControlState_SettingUp:
            if (priority_submenu_
                && !priority_submenu_->subactions().isEmpty())
                priority_submenu_->requestAction()->setEnabled(true);

            if (save_as_submenu_
                && !save_as_submenu_->subactions().isEmpty())
                save_as_submenu_->requestAction()->setEnabled(true);

            if (video_submenu_
                && !video_submenu_->subactions().isEmpty())
                video_submenu_->requestAction()->setEnabled(true);

            if (audio_submenu_
                && !audio_submenu_->subactions().isEmpty())
                audio_submenu_->requestAction()->setEnabled(true);
        
            if (audio_languages_submenu_
                && !audio_languages_submenu_->subactions().isEmpty())
                audio_languages_submenu_->requestAction()->setEnabled(true);
        
            if (image_submenu_
                && !image_submenu_->subactions().isEmpty())
                image_submenu_->requestAction()->setEnabled(true);
        
            if (subtitle_submenu_
                && !subtitle_submenu_->subactions().isEmpty()) {
                subtitle_submenu_->requestAction()->setEnabled(true);

                if (subtitle_saveas_submenu_
                    && !subtitle_saveas_submenu_->subactions().isEmpty())
                    subtitle_saveas_submenu_->requestAction()->setEnabled(true);
            }

            ui_.btnOptions->setVisible(true);
            //ui_.btnCenter->setText(tr("Download"));
            ui_.btnCenter->setProperty("state", "Download");
        
			showBtnPlaylist();
            break;

        case kControlState_Pending:
            if (priority_submenu_
                && !priority_submenu_->subactions().isEmpty())
                priority_submenu_->requestAction()->setEnabled(true);

            ui_.btnCenter->setVisible(false);
            ui_.btnCenter->setEnabled(false);
            ui_.btnCenter->setProperty("state", "Return");
			showBtnPlaylist();
            break;
        case kControlState_Transitioning:
           /* ui_.btnRemove->setEnabled(false);
            ui_.btnCenter->setEnabled(false);
            ui_.btnSettings->setEnabled(false);*/
            ui_.btnOptions->setVisible(false);
            ui_.btnSettings->setVisible(false);
            ui_.btnRemove->setVisible(false);
            ui_.btnCenter->setVisible(false);
            break;
        case kControlState_Processing:
            if (priority_submenu_
                && !priority_submenu_->subactions().isEmpty())
                priority_submenu_->requestAction()->setEnabled(true);

			showBtnPlaylist();

            ui_.btnCenter->setProperty("state", "Pause");
            break;
        case kControlState_Paused:
            if (priority_submenu_
                && !priority_submenu_->subactions().isEmpty())
                priority_submenu_->requestAction()->setEnabled(true);

			showBtnPlaylist();

            ui_.btnCenter->setProperty("state", "Resume");
            break;
        case kControlState_Completed:
			showBtnPlaylist();
            ui_.btnSettings->setVisible(false);
            ui_.btnCenter->setProperty("state", "Play");
            break;
        case kControlState_Canceled:
            ui_.btnSettings->setVisible(false);
            ui_.btnCenter->setVisible(false);
            ui_.btnPlaylist->setVisible(false);
            // ui_.btnCenter->setText(tr("Reload"));
            break;
        case kControlState_Failed:
            ui_.btnSettings->setVisible(false);
            ui_.btnCenter->setVisible(false);
			showBtnPlaylist();
            break;
    }

    ui_.btnCenter->style()->polish(ui_.btnCenter);
    ui_.btnCenter->update();

    state_ = state;
}

//void VideoControl::setHovered(bool is_hovered /*= true*/) {
//    const float         kControlOpacity_Hovered = 1.0f;
//    const float         kControlOpacity_NotHovered = 1.0f;
//    
//    is_hovered_ = is_hovered;
//
//    float opacity = is_hovered ? kControlOpacity_Hovered : kControlOpacity_NotHovered;
//
//    ui_.btnCenter->setOpacity(opacity);
//    ui_.btnOptions->setOpacity(opacity);
//    ui_.btnRemove->setOpacity(opacity);
//    ui_.btnSettings->setOpacity(opacity);
//
//    switch (state_) {
//    case kControlState_Transitioning:
//    case kControlState_Preparing:
//    case kControlState_Canceled:
//    case kControlState_Failed:
//        this->setProperty("color", "normal");
//        break;
//
//    //case kControlState_Transitioning:
//    case kControlState_SettingUp:
//    case kControlState_Pending:
//    case kControlState_Processing:
//    case kControlState_Paused:
//    case kControlState_Completed:
//
//        //this->setProperty("color", is_hovered ? "darkened":"normal");
//        
//        break;
//    }
//
//    this->style()->polish(this);
//    this->update();
//}

void VideoControl::setup(VideoItemDelegate* delegate) {
    if (delegate != delegate_) {
        delegate_ = delegate;

		setupPlaylist(delegate);

        if (!ui_.btnSettings->menu())
            createSettings();

        fillVideoMenu();
        fillImageMenu();
        fillAudioMenu();
        fillSaveAsMenu();
        fillAudioLanguageMenu();
        fillSubtitleMenu();

        if (delegate_) {
            if (delegate_->portal() == EPortal::kNetflix
                || delegate_->portal() == EPortal::kDisney
                || delegate_->portal() == EPortal::kAmazon
                || delegate_->portal() == EPortal::kHBO
                || delegate_->portal() == EPortal::kHulu) {
                fillBtnSettingsMenu(kShowMenu_VideoAndAudio);
            }
            else if (!delegate_->selectedSaveAsTracks().isEmpty()) {
                if (delegate_->selectedSaveAsTracks()[0] != kPreset_OriginalVideo)
                    fillBtnSettingsMenu(kShowMenu_Audio);
                else
                    fillBtnSettingsMenu(kShowMenu_Video);
            }
        }
    }
}

void VideoControl::setupPlaylist(VideoItemDelegate* delegate) {
	if (delegate_
		&& (!delegate_->videoInfo().playlist.isEmpty() || delegate_->videoInfo().playlist_kind == PlaylistKind::kPlaylistPending)) {

		ui_.btnPlaylist->setText(playlistKindName(delegate_->videoInfo().playlist_kind));

		if (delegate_->videoInfo().playlist_kind != PlaylistKind::kEmptyPlaylist &&
			delegate_->videoInfo().playlist_kind != PlaylistKind::kPlaylistPending) {
			ui_.btnPlaylist->setEnabled(true);
			ui_.btnPlaylist->setMenu(nullptr);

			createPlaylist();
			ui_.btnPlaylist->setStyleSheet(btn_playlist_style_);
			fillBtnPlaylistMenu();
		}
	}
}

void VideoControl::translate() {
    ui_.retranslateUi(this);
    if (delegate_
        && !delegate_->videoInfo().playlist.isEmpty()) {

        ui_.btnPlaylist->setText(playlistKindName(delegate_->videoInfo().playlist_kind));

        if (delegate_->videoInfo().playlist_kind != PlaylistKind::kEmptyPlaylist)
            fillBtnPlaylistMenu();
    }
    fillPriorityMenu();
    fillSaveAsMenu();
    fillAudioMenu();
    fillAudioLanguageMenu();
    fillVideoMenu();
    fillSubtitleMenu();
    fillSubtitleSaveAsMenu();
    fillImageMenu();
    setState(state_);
}

void VideoControl::createSettings() {
    qApp->processEvents();

    TransMenu* menu = new TransMenu(ui_.btnSettings);
    menu->setStyleSheet(menu_style_);

    createPrioritySettings();

    if (delegate_) {
        if (VIDEOSELECTORENABLED)
            createVideoSettings();

        if (AUDIOSELECTORENABLED)
            createAudioSettings();

        if (SAVEASSELECTORENABLED)
            createSaveAsSettings();

        if (AUDIOLANGSELECTORENABLED)
            createAudioLanguagesSettings();

        if (IMAGESELECTORENABLED)
            createImageSettings();

        if (SUBTITLESELECTORENABLED) {
            createSubtitleSettings();
            createSaveSubtitlesAsSettings();
        }
    }

    menu->installEventFilter(this);
    ui_.btnSettings->setMenu(menu);
    //ui_.btnSettings->setPopupMode(QToolButton::InstantPopup);

    TransMenu* context_menu = new TransMenu(ui_.btnOptions);
    ui_.btnOptions->setMenu(context_menu);
    context_menu->installEventFilter(this);
    context_menu->setStyleSheet(menu_style_);

    qApp->processEvents();
}

void VideoControl::createPlaylist() {
    qApp->processEvents();

    TransMenu* menu = new TransMenu(ui_.btnPlaylist);
    menu->setStyleSheet(menu_style_);
    menu->installEventFilter(this);
    ui_.btnPlaylist->setMenu(menu);

    TransMenu* context_menu = new TransMenu(ui_.btnPlaylist);
    ui_.btnPlaylist->setMenu(context_menu);
    context_menu->installEventFilter(this);
    context_menu->setStyleSheet(menu_style_);

    qApp->processEvents();
}

bool VideoControl::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::Show
        && (obj == ui_.btnSettings->menu()
            || obj == ui_.btnOptions->menu()
            || obj == ui_.btnPlaylist->menu())) {
        
        QPushButton* button = ui_.btnSettings;

        if (obj == ui_.btnSettings->menu()) {
            QPushButton* button = ui_.btnSettings;
        }
        else if (obj == ui_.btnOptions->menu()) {
            QPushButton* button = ui_.btnOptions;
            signalshowOptions emit(ui_.btnOptions);
        }
        else if (obj == ui_.btnPlaylist->menu()) {
            QPushButton* button = ui_.btnPlaylist;
        }
        else {
            return false;
        }

        button->menu()->adjustSize();
        auto const rec = QGuiApplication::primaryScreen()->geometry();
        auto const height = rec.height();
        auto const width = rec.width();

        QPoint pos_menu_lower_left_corner = button->mapToGlobal(QPoint(0, button->height()))
            + QPoint(button->menu()->width(), button->menu()->height());

        QPoint pos = button->mapToGlobal(QPoint(0, button->height()));
        if (width < pos_menu_lower_left_corner.x()
            && height > pos_menu_lower_left_corner.y()) {

            pos = button->mapToGlobal(QPoint(button->width(), button->height()))
                - QPoint(button->menu()->width(), 0);
        }
        else if (width < pos_menu_lower_left_corner.x()
            && height < pos_menu_lower_left_corner.y()) {

            QPoint pos_2 = button->mapToGlobal(QPoint(button->width(), 0))
                - QPoint(button->menu()->width(), button->menu()->height());
            if (pos_2.y() > 0)
                pos = pos_2;
            else if (width < pos_menu_lower_left_corner.x())
                pos = button->mapToGlobal(QPoint(button->width(), button->height()))
                - QPoint(button->menu()->width(), 0);
        }
        else if (width > pos_menu_lower_left_corner.x()
            && height < pos_menu_lower_left_corner.y()) {
            QPoint pos_2 = button->mapToGlobal(QPoint(0, 0))
                - QPoint(0, button->menu()->height());
            if (pos_2.y() > 0)
                pos = pos_2;
            else if (width < pos_menu_lower_left_corner.x())
                pos = button->mapToGlobal(QPoint(button->width(), button->height()))
                - QPoint(button->menu()->width(), 0);
        }

        button->menu()->move(pos);
    }
    return false;
}

MenuSubmenu* VideoControl::createPlaylistSettings(const PlaylistKind& playlist_kind,
    const QPair<int, QList<QPair<QString, QString>>>& playlist) {

    MenuSubmenu* playlist_submenu = new MenuSubmenu(playlistKindName(playlist_kind), "", this);

    QObject::connect(playlist_submenu, &MenuSubmenu::aboutToShow, [=]() {

    });

    QObject::connect(playlist_submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        playlist_menu_visible_ = true;

        for (auto a : playlist_submenu->subactions())
            a->setChecked(false);
    });

    QObject::connect(playlist_submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        auto acts = playlist_submenu->subactions();

        if (a->data().toInt() == 0) { //all episodes case
            bool invert_selection = true;
            bool first = false;
            bool some_checked = false;
            int i = 0;
            for (auto act : acts) {
                if (act->data().toInt() != 0) {
                    if (act->isChecked())
                        some_checked = true;

                    if (i == 0)
                        first = act->isChecked();

                    if (act->isChecked() != first)
                        invert_selection = false;

                    i++;
                }
            }
            
            if (invert_selection) {
                for (auto act : acts)
                    act->setChecked(!first);
            }
            else if (some_checked) {
                for (auto act : acts)
                    act->setChecked(true);
            }

            a->setChecked(false);
        }
        else {
            if (!a->isChecked())
                a->setChecked(false);
            else
                a->setChecked(true);
        }

        if (is_multiple_playList_)
            setSubMenuChecked(playlist_submenu);
    });

    QObject::connect(playlist_submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        QString urls;

        playlist_menu_visible_ = false;

        if (!is_multiple_playList_
            || !playlist_submenu->getCheckBoxSelected()->isChecked()) {

            for (auto a : playlist_submenu->subactions()) {
                int id = a->data().toInt();
                if (a->isChecked()
                    && id > 0) { // id == 0 -> all episodes case
                    QString url = playlist.second[id - 1].second;
                    urls.append(url + "|");
                }
            }

            if (delegate_
                && !urls.isEmpty()) {

                if (delegate_->isActivated())
                    emit openPlaylist(urls);
                else
                    AlertDialog::execDialog(EAlert::kAlert_LimitPlaylist, delegate_->portal());
            }
        }
    });

    if (is_multiple_playList_) {
        QObject::connect(playlist_submenu->getCheckBoxSelected(), &QCheckBox::clicked, [=](bool checked) {
            if (!checked
                && !ui_.btnPlaylist->menu()->actions().isEmpty())
                ui_.btnPlaylist->menu()->actions()[0]->setChecked(false);
        });
    }

    return playlist_submenu;
}

MenuList* VideoControl::createOnePlaylistSettings(const PlaylistKind& playlist_kind, const QPair<int, QList<QPair<QString,
    QString>>>& playlist) {

    MenuList* menu = new MenuList();
    //menu->parentPos = ui_.btnPlaylist->pos();

    QObject::connect(menu, &MenuList::aboutToShow, [=]() {
        for (auto a : menu->actions())
            a->setChecked(false);

        menu->type_menu_ = TypeMenu::PlayList;

        QRect btnRect(ui_.btnPlaylist->mapToGlobal(ui_.btnPlaylist->pos()),
            QSize(ui_.btnPlaylist->rect().width(), ui_.btnPlaylist->rect().height()));
        menu->btn_rect_ = btnRect;
    });

    QObject::connect(menu, &MenuList::aboutToHide, [=]() {
        QString urls;

        for (auto a : menu->actions()) {
            int id = a->data().toInt();
            if (a->isChecked()
                && id > 0) { // id == 0 -> all episodes case
                QString url = playlist.second[id - 1].second;
                urls.append(url + "|");
            }
        }

        if (delegate_
            && !urls.isEmpty()) {

            if (delegate_->isActivated())
                emit openPlaylist(urls);
            else
                AlertDialog::execDialog(EAlert::kAlert_LimitPlaylist, delegate_->portal());
        }
    });

    return menu;
}

void VideoControl::createPrioritySettings() {
    priority_submenu_ = new MenuSubmenu(tr("Download priority"), tr("Normal"), this);

    QList<QAction*>     actions;
    QList<MenuCheckbox*> priority_acts;

    QList<EDownloadPriority> priorities_data = { kDownloadPriority_High, kDownloadPriority_Normal, kDownloadPriority_Low};

    for (auto it : priorities_data) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        priority_acts.push_back(menu_checkbox);

        auto a = priority_acts[it];

        actions.push_back(a->requestAction());

        a->setName(priorityDesc(it));
        a->requestAction()->setData(it);
        a->requestAction()->setCheckable(true);
    }

    priority_submenu_->createSubmenu(actions, true);

    QObject::connect(priority_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_) {

            auto acts = priority_submenu_->subactions();
            int priority = delegate_->selectedPriority();

            for (auto a : acts) {
                if (a->data().toInt() == priority) {
                    priority_submenu_->setValue(a->text());
                    break;
                }
            }
        }
        else {
            priority_submenu_->setValue(tr("Normal"));
        }
    });

    QObject::connect(priority_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = priority_submenu_->subactions();

        if (delegate_) {

            for (auto a : acts) {
                if (a->data().toInt() == delegate_->selectedPriority()) {
                    a->setChecked(true);
                    break;
                }
            }
        }
        else if (!acts.isEmpty()) {
            acts[0]->setChecked(true);
        }
    });

    QObject::connect(priority_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            int id = a->data().toInt();
            a = priority_submenu_->actionByData(id);
            Q_ASSERT(a);
            a->setChecked(true);

            delegate_->selectPriority(id);
        }
    });

    QObject::connect(priority_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {

            QAction* act = priority_submenu_->checkedSubaction();

            if (act)
                delegate_->selectPriority(act->data().toInt());
        }
    });
}

void VideoControl::createSaveAsSettings() {
    if (VIDEOCONTENTEXIST)
        save_as_submenu_ = new MenuSubmenu(tr("Save as"), tr("Original video"), this);
    else
        save_as_submenu_ = new MenuSubmenu(tr("Save as"), tr("Original audio"), this);

    QObject::connect(save_as_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_) {

            int track = DEFAULTPRESET;

            if (!delegate_->selectedSaveAsTracks().isEmpty())
                track = delegate_->selectedSaveAsTracks().last();
            if (QAction* a = save_as_submenu_->actionByData(track))
                save_as_submenu_->setValue(a->text());
        }
        else {
            if (VIDEOCONTENTEXIST)
                save_as_submenu_->setValue(tr("Original video"));
            else
                save_as_submenu_->setValue(tr("Original audio"));
        }

    });

    QObject::connect(save_as_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = save_as_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedSaveAsTracks().isEmpty()) {

            for (auto a : acts)
                a->setChecked(delegate_->selectedSaveAsTracks().contains(a->data().toInt()));
        }
        else if (!acts.isEmpty()) {
            acts[0]->setChecked(true);
        }
    });

    QObject::connect(save_as_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            a->setChecked(false);

            int id = a->data().toInt();
            int new_id = delegate_->checkSaveAsTrack(id);
            if (id != new_id)
                a = save_as_submenu_->actionByData(new_id);

            Q_ASSERT(a);
            a->setChecked(true);

            if (id != new_id)
                AlertDialog::execDialog(EAlert::kAlert_LimitPresets);
        }
    });

    QObject::connect(save_as_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {

            QList<int> tracks;
            for (auto a : save_as_submenu_->subactions()) {
                if (a->isChecked())
                    tracks.push_back(a->data().toInt());
            }

            delegate_->selectSaveAsTracks(tracks);
            delegate_->updateItemDesk();
        }
    });
}

void VideoControl::createAudioSettings() {
    QString portal_name;
    if (delegate_)
        portalName(delegate_->portal(), portal_name);

    if (!portal_name.isEmpty())
        audio_submenu_ = new MenuSubmenu(portal_name.append(" ").append(tr("audio quality")), tr("No audio"), this);
    else
        audio_submenu_ = new MenuSubmenu(tr("Audio quality"), tr("No audio"), this);

    QObject::connect(audio_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_)
            audio_submenu_->setValue(delegate_->selectedAudioTracksDesc());
        else
            audio_submenu_->setValue(tr("No audio"));
    });

    QObject::connect(audio_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = audio_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedAudioTracks().isEmpty()) {

            for (auto a : acts)
                a->setChecked(delegate_->selectedAudioTracks().contains(a->data().toInt()));
        }
    });

    QObject::connect(audio_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {

        qDebug() << "Audio Subaction Triggered " << a->isChecked();
        if (delegate_
            && a->isChecked()) {

            a->setChecked(false);
            
            int id = a->data().toInt();
            int new_id = delegate_->checkAudioTrack(id);
            if (id != new_id)
                a = audio_submenu_->actionByData(new_id);

            Q_ASSERT(a);
            a->setChecked(true);
            
            if (id != new_id)
                AlertDialog::execDialog(EAlert::kAlert_LimitHdAnd5_1);
        }
    });

    QObject::connect(audio_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {
            QList<int> tracks;

            for (auto a : audio_submenu_->subactions()) {
                if (a->isChecked())
                    tracks.push_back(a->data().toInt());
            }

            delegate_->selectAudioTracks(tracks);
            delegate_->updateItemDesk();
        }
    });
}

void VideoControl::createVideoSettings() {
    QString portal_name;
    if (delegate_)
        portalName(delegate_->portal(), portal_name);

    if (!portal_name.isEmpty())
        video_submenu_ = new MenuSubmenu(portal_name.append(" ").append(tr("video quality")), tr("No video"), this);
    else
        video_submenu_ = new MenuSubmenu(tr("Video quality"), tr("No video"), this);

    QObject::connect(video_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_)
            video_submenu_->setValue(delegate_->selectedVideoTracksDesc());
        else
            video_submenu_->setValue(tr("No video"));
    });

    QObject::connect(video_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = video_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedVideoTracks().isEmpty()) {

            for (auto a : acts)
                a->setChecked(delegate_->selectedVideoTracks().contains(a->data().toInt()));
        }
    });

    QObject::connect(video_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            a->setChecked(false);

            int id = a->data().toInt();
            int new_id = delegate_->checkVideoTrack(id);
            if (id != new_id)
                a = video_submenu_->actionByData(new_id);

            Q_ASSERT(a);
            a->setChecked(true);

            if (id != new_id)
                AlertDialog::execDialog(EAlert::kAlert_LimitHdAnd5_1);
        }
    });

    QObject::connect(video_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {

            QList<int> tracks;

            for (auto a : video_submenu_->subactions()) {
                if (a->isChecked())
                    tracks.push_back(a->data().toInt());
            }

            delegate_->selectVideoTracks(tracks);
            delegate_->updateItemDesk();
        }
    });
}

void VideoControl::createSubtitleSettings() {
    subtitle_submenu_ = new MenuSubmenu(tr("Subtitle languages"), settings::kSubtitleLanguageDefault, this);

    QObject::connect(subtitle_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_)
            subtitle_submenu_->setValue(delegate_->selectedSubtitlesTracksDesc());
        else
            subtitle_submenu_->setValue(tr("None"));

    });

    QObject::connect(subtitle_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = subtitle_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedSubtitleTracks().isEmpty()) {

            for (auto a : acts)
                a->setChecked(delegate_->selectedSubtitleTracks().contains(a->data().toInt()));
        }
        else if (!acts.isEmpty()) {
            acts[0]->setChecked(true);
        }
    });

    QObject::connect(subtitle_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            a->setChecked(false);

            int id = a->data().toInt();
            int new_id = delegate_->checkSubtitleTrack(id);
            if (id != new_id)
                a = subtitle_submenu_->actionByData(new_id);

            Q_ASSERT(a);

            auto acts = subtitle_submenu_->subactions();
            if (new_id == 0) {//checked No subtitles case
                for (auto a : acts)
                    a->setChecked(false);
            }
            else if (new_id > 0//checked not No subtitles case
                && !acts.isEmpty()) {
                acts[0]->setChecked(false);
            }

            a->setChecked(true);

            if (id != new_id)
                AlertDialog::execDialog(EAlert::kAlert_LimitSubtitle);
        }
    });

    QObject::connect(subtitle_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {
            QList<int> tracks;

            for (auto a : subtitle_submenu_->subactions()) {
                if (a->isChecked())
                    tracks.push_back(a->data().toInt());
            }

            delegate_->selectSubtitleTracks(tracks);
            delegate_->updateItemDesk();
        }
    });
}

void VideoControl::createSaveSubtitlesAsSettings() {
    subtitle_saveas_submenu_ = new MenuSubmenu(tr("Save subtitles as"), tr("Original format"), this);

    QList<QAction*>     actions;
    QList<MenuCheckbox*> saveas_acts;


    QList<QString> saveas = { tr("Embedded"), tr("External SRT"), tr("External original") };
    QList<QVariant> saveas_data = { kSaveSubtitlesAs_Embedded, kSaveSubtitlesAs_SRT, kSaveSubtitlesAs_Original };

    for (int i = 0; i < saveas.count(); ++i) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        saveas_acts.push_back(menu_checkbox);

        auto a = saveas_acts[i];

        actions.push_back(a->requestAction());

        a->setName(saveas.at(i));
        a->requestAction()->setData(saveas_data.at(i));
        a->requestAction()->setCheckable(true);
    }

    subtitle_saveas_submenu_->createSubmenu(actions, false);

    QObject::connect(subtitle_saveas_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        auto acts = subtitle_saveas_submenu_->subactions();
        QString menu_text;
        if (delegate_ 
            && !delegate_->selectedSaveSubtitlesAsTracks().isEmpty()) {
            
            QList<int> selected_items = delegate_->selectedSaveSubtitlesAsTracks();
            for (auto it : acts) {
                if (selected_items.contains(it->data().toInt()))
                    menu_text.append(it->text()).append(", ");
            }
            menu_text = menu_text.mid(0, menu_text.count() - 2);
            subtitle_saveas_submenu_->setValue(menu_text);
        }
        else if (!acts.isEmpty()) {
            subtitle_saveas_submenu_->setValue(acts[0]->text());
        }
    });

    QObject::connect(subtitle_saveas_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = subtitle_saveas_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedSaveSubtitlesAsTracks().isEmpty()) {

            for (auto a : acts)
                a->setChecked(delegate_->selectedSaveSubtitlesAsTracks().contains(a->data().toInt()));
        }
        else if (!acts.isEmpty()) {
            acts[0]->setChecked(true);
        }
    });

    QObject::connect(subtitle_saveas_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            a->setChecked(false);

            int id = a->data().toInt();
            int new_id = delegate_->checkSubtSaveSubtitlesAsTrack(id);
            if (id != new_id)
                a = subtitle_saveas_submenu_->actionByData(new_id);

            Q_ASSERT(a);
            a->setChecked(true);

            if (id != new_id)
                AlertDialog::execDialog(EAlert::kAlert_LimitSaveSubtitlesAs);
        }
    });

    QObject::connect(subtitle_saveas_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {
            QList<int> selected_items;
            for (auto it : subtitle_saveas_submenu_->subactions()) {
                if (it->isChecked())
                    selected_items << it->data().toInt();
            }

            if (selected_items.isEmpty())
                selected_items << 0; //Original format

            delegate_->selectSaveSubtitlesAsTracks(selected_items);
        }
    });
}

void VideoControl::createAudioLanguagesSettings() {
    audio_languages_submenu_ = new MenuSubmenu(tr("Audio languages"), kLanguagesAndCodes[0][0], this);

    QObject::connect(audio_languages_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_)
            audio_languages_submenu_->setValue(delegate_->selectedAudioLanguagesDesc());
        else
            audio_languages_submenu_->setValue(kLanguagesAndCodes[0][0]);
    });

    QObject::connect(audio_languages_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = audio_languages_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedAudioLanguagesTracks().isEmpty()) {

            for (auto a : acts)
                a->setChecked(delegate_->selectedAudioLanguagesTracks().contains(a->data().toInt()));
        }
    });

    QObject::connect(audio_languages_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            int id = a->data().toInt();
            auto acts = audio_languages_submenu_->subactions();

            if (id == 0) {  //  0 - none case
                for (auto it : acts) {
                    if (it->data().toInt() > 0)
                        it->setChecked(false);
                }
            }
            else if (delegate_->checkAudioLanguageTrack(id, acts)) {
                for (auto it : acts) {
                    if (it->data().toInt() != id)
                        it->setChecked(false);
                }

                AlertDialog::execDialog(EAlert::kAlert_LimitAudioLanguages);
            }
            else {
                for (auto it : acts) {
                    if (it->data().toInt() == 0 )   //  0 - none case
                        it->setChecked(false);
                }
            }
        }
    });

    QObject::connect(audio_languages_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {
            QList<int> tracks;
            for (auto a : audio_languages_submenu_->subactions()) {
                if (a->isChecked())
                    tracks.push_back(a->data().toInt());
            }
            if (!tracks.isEmpty()) {
                if (delegate_->controller()) {
                    delegate_->controller()->filterAssistiveTracks();
                    delegate_->controller()->filterAudioTracks(delegate_->selectedAudioLanguages(tracks));
                    delegate_->controller()->selectAudioTrack();
                    fillAudioMenu();
                }
            }

            delegate_->selectAudioLanguageTracks(tracks);
            delegate_->updateItemDesk();
        }
    });
}

void VideoControl::createImageSettings() {
    QString portal_name;
    if (delegate_)
        portalName(delegate_->portal(), portal_name);

    if (!portal_name.isEmpty())
        image_submenu_ = new MenuSubmenu(portal_name.append(" ").append(tr("images quality")), tr("No images"), this);
    else
        image_submenu_ = new MenuSubmenu(tr("Images quality"), tr("No images"), this);

    QObject::connect(image_submenu_, &MenuSubmenu::aboutToShow, [=]() {
        if (delegate_)
            image_submenu_->setValue(delegate_->selectedImageTracksDesc());
        else
            image_submenu_->setValue(tr("No images"));
    });

    QObject::connect(image_submenu_, &MenuSubmenu::submenuAboutToShow, [=]() {
        auto acts = image_submenu_->subactions();

        if (delegate_
            && !delegate_->selectedImageTracks().isEmpty()) {

            for (auto a : image_submenu_->subactions())
                a->setChecked(delegate_->selectedImageTracks().contains(a->data().toInt()));
        }
    });

    QObject::connect(image_submenu_, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (delegate_
            && a->isChecked()) {

            a->setChecked(false);

            int id = a->data().toInt();
            int new_id = delegate_->checkImageTrack(id);
            if (id != new_id)
                a = image_submenu_->actionByData(new_id);

            Q_ASSERT(a);
            a->setChecked(true);

            if (id != new_id)
                AlertDialog::execDialog(EAlert::kAlert_Image);
        }
    });

    QObject::connect(image_submenu_, &MenuSubmenu::submenuAboutToHide, [=]() {
        if (delegate_) {
            QList<int> tracks;

            for (auto a : image_submenu_->subactions()) {
                if (a->isChecked())
                    tracks.push_back(a->data().toInt());
            }

            delegate_->selectImageTracks(tracks);
            delegate_->updateItemDesk();
        }
    });
}

void VideoControl::showBtnPlaylist() {
	for (auto playlist_submenu : playlist_submenus_) {
		if (!playlist_submenu->subactions().isEmpty())
			playlist_submenu->requestAction()->setEnabled(true);
	}

	if (!delegate_)
		return;

	if (!delegate_->videoInfo().playlist.isEmpty() || 
		delegate_->videoInfo().playlist_kind == PlaylistKind::kPlaylistPending)
		ui_.btnPlaylist->setVisible(true);

	if (delegate_->videoInfo().playlist_kind == PlaylistKind::kPlaylistPending)
		ui_.btnPlaylist->setEnabled(false);
}

QString VideoControl::priorityDesc(const EDownloadPriority& it) {
    QString desk;

    switch (it) {
        case kDownloadPriority_High:
            desk = tr("High");
            break;
        case kDownloadPriority_Normal:
            desk = tr("Normal");
            break;
        case kDownloadPriority_Low:
            desk = tr("Low");
            break;
    }

    return desk;
}

void VideoControl::setSubMenuChecked(MenuSubmenu* submenu) {
    bool checked = true;

    for (auto a : submenu->subactions()) {
        int id = a->data().toInt();
        if (id > 0
            && !a->isChecked()) { // id == 0 -> all episodes case
            checked = false;
            break;
        }
    }

    submenu->getCheckBoxSelected()->setChecked(checked);
}

void VideoControl::fillPlaylistMenu(MenuSubmenu* submenu, const VideoInfo& info, QPair<int, QList<QPair<QString, QString>>> playlist) {
    if (submenu) {
        submenu->clearSubmenu();
        submenu->requestAction()->setEnabled(false);

        QList<QAction*>     actions;
        QList<MenuCheckbox*> episode_acts;

        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(tr("All"));
        menu_checkbox->requestAction()->setData(0);
        menu_checkbox->requestAction()->setCheckable(true);

        episode_acts.push_back(menu_checkbox);
        actions.push_back(menu_checkbox->requestAction());

        for (int i = 0; i < playlist.second.count(); ++i) {
            int data = i + 1;
            MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
            menu_checkbox->setName(QString::number(data) + " " + playlist.second[i].first);
            menu_checkbox->requestAction()->setData(data);
            menu_checkbox->requestAction()->setCheckable(true);

            if (is_multiple_playList_) {
                QObject::connect(menu_checkbox, &MenuCheckbox::aboutToShow, [=]() {
                    bool checked = submenu->getCheckBoxSelected()->isChecked();
                    menu_checkbox->requestAction()->setChecked(checked);
                });
            }

            episode_acts.push_back(menu_checkbox);
            actions.push_back(menu_checkbox->requestAction());

			if (i % 10 == 0) qApp->processEvents();
        }

        submenu->createSubmenu(actions, false);
        submenu->requestAction()->setEnabled(true);

        QString name = playlistKindName(info.playlist_kind);

        if (info.playlist_kind == PlaylistKind::kSeason) {
            submenu->setName(name.append(" ").append(QString::number(playlist.first)));
        }
        else {
            name = playlistSubname(playlist.first);
            submenu->setName(name);
        }
    }
}

MenuCheckbox* VideoControl::createBtnAllSubMenu() {
    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
    menu_checkbox->setName(tr("All"));
    menu_checkbox->requestAction()->setData(0);
    menu_checkbox->requestAction()->setCheckable(true);

    QObject::connect(menu_checkbox->requestAction(), &QAction::triggered, [=]() {
        for (auto playlist_submenuA : playlist_submenus_)
            playlist_submenuA->getCheckBoxSelected()->setChecked(!menu_checkbox->checked_);

        menu_checkbox->requestAction()->setChecked(false);
    });

    menu_checkbox->setChecked(false);

    return menu_checkbox;
}

void VideoControl::clickedAllList() {
    QAction* a = qobject_cast<QAction*>(sender());
    int id = ui_.btnPlaylist->menu()->actions().indexOf(a);
    if (id > -1) {
        auto acts = ui_.btnPlaylist->menu()->actions();

        if (a->data().toInt() == 0) { //all episodes case
            bool invert_selection = true;
            bool first = false;
            bool some_checked = false;
            int i = 0;
            for (auto act : acts) {
                if (act->data().toInt() != 0) {
                    if (act->isChecked())
                        some_checked = true;

                    if (i == 0)
                        first = act->isChecked();

                    if (act->isChecked() != first)
                        invert_selection = false;

                    i++;
                }
            }

            if (invert_selection) {
                for (auto act : acts)
                    act->setChecked(!first);
            }
            else if (some_checked) {
                for (auto act : acts)
                    act->setChecked(true);
            }

            a->setChecked(false);
        }
        else {
            if (!a->isChecked())
                a->setChecked(false);
            else
                a->setChecked(true);
        }
    }
}

void VideoControl::fillOnePlaylistMenu(QMenu* menu, const VideoInfo& info, QPair<int, QList<QPair<QString, QString>>> playlist) {
    if (menu) {
        qApp->processEvents();

        menu->clear();

        QList<QAction*>     actions;
        QList<MenuCheckbox*> episode_acts;

        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(tr("All"));
        menu_checkbox->requestAction()->setData(0);
        menu_checkbox->requestAction()->setCheckable(true);

        episode_acts.push_back(menu_checkbox);
        actions.push_back(menu_checkbox->requestAction());

        for (int i = 0; i < playlist.second.count(); ++i) {
            int data = i + 1;
            MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
            menu_checkbox->setName(QString::number(data) + " " + playlist.second[i].first);
            menu_checkbox->requestAction()->setData(data);
            menu_checkbox->requestAction()->setCheckable(true);

            episode_acts.push_back(menu_checkbox);
            actions.push_back(menu_checkbox->requestAction());

			if (i % 10 == 0) qApp->processEvents();
        }

		for (int i = 0; i < actions.count(); i++) {
			menu->addAction(actions[i]);

			if (i % 10 == 0) qApp->processEvents();
		}

        for (auto it : actions)
            connect(it, SIGNAL(triggered()), this, SLOT(clickedAllList()));

        
        QString name = playlistKindName(info.playlist_kind);

        if (info.playlist_kind == PlaylistKind::kSeason)
            name.append(" ").append(QString::number(playlist.first));
        else
            name = playlistSubname(playlist.first);

        ui_.btnPlaylist->setText(name);

        qApp->processEvents();
    }
}

void VideoControl::fillPriorityMenu() {
    if (priority_submenu_) {
        qApp->processEvents();

        priority_submenu_->clearSubmenu();
        priority_submenu_->requestAction()->setEnabled(false);

        QList<QAction*>     actions;
        QList<MenuCheckbox*> priority_acts;

        QList<EDownloadPriority> priorities_data = { kDownloadPriority_High, kDownloadPriority_Normal, kDownloadPriority_Low };

        for (auto it : priorities_data) {
            MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
            priority_acts.push_back(menu_checkbox);

            auto a = priority_acts[it];

            actions.push_back(a->requestAction());

            a->setName(priorityDesc(it));
            a->requestAction()->setData(it);
            a->requestAction()->setCheckable(true);
        }

        priority_submenu_->createSubmenu(actions, true);
        priority_submenu_->requestAction()->setEnabled(true);

        priority_submenu_->setName(tr("Download priority"));

        qApp->processEvents();
    }
}

void VideoControl::fillVideoMenu() {
    if (video_submenu_) {
        qApp->processEvents();

        video_submenu_->clearSubmenu();
        video_submenu_->requestAction()->setEnabled(false);

        if (delegate_ 
            && delegate_->videoTracksCount() > 0) {

            QList<QAction*>     actions;
            QList<MenuCheckbox*> video_acts;
            for (int i = 0; i < delegate_->videoTracksCount(); ++i) {

                if (video_acts.size() <= i) {
                    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                    video_acts.push_back(menu_checkbox);
                }

                auto a = video_acts[i];

                actions.push_back(a->requestAction());

                a->setName(delegate_->videoTrackDesc(i));
                a->requestAction()->setData(i);
                a->requestAction()->setCheckable(true);
            }

            if (!actions.isEmpty()) {
                video_submenu_->createSubmenu(actions, true);
                video_submenu_->requestAction()->setEnabled(true);
            }

            QString portal_name;
            portalName(delegate_->portal(), portal_name);

            if (!portal_name.isEmpty())
                video_submenu_->setName(portal_name.append(" ").append(tr("video quality")));
            else
                video_submenu_->setName(tr("Video quality"));
        }

        qApp->processEvents();
    }
}

void VideoControl::fillAudioMenu() {
    if (audio_submenu_) {
        qApp->processEvents();

        audio_submenu_->clearSubmenu();
        audio_submenu_->requestAction()->setEnabled(false);

        if (delegate_ 
            && delegate_->audioTracksCount() > 0) {

            QList<QAction*>     actions;
            QList<MenuCheckbox*> audio_acts;
            for (int i = 0; i < delegate_->audioTracksCount(); ++i) {

                if (audio_acts.size() <= i) {
                    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                    audio_acts.push_back(menu_checkbox);
                }

                auto a = audio_acts[i];

                actions.push_back(a->requestAction());

                a->setName(delegate_->audioTrackDesc(i));
                a->requestAction()->setData(i);
                a->requestAction()->setCheckable(true);
            }

            if (!actions.isEmpty()) {
                audio_submenu_->createSubmenu(actions, true);
                audio_submenu_->requestAction()->setEnabled(true);
            }

            QString portal_name;
            portalName(delegate_->portal(), portal_name);

            if (!portal_name.isEmpty())
                audio_submenu_->setName(portal_name.append(" ").append(tr("audio quality")));
            else
                audio_submenu_->setName(tr("Audio quality"));
        }

        qApp->processEvents();
    }
}

void VideoControl::fillSaveAsMenu() {
    if (save_as_submenu_) {
        qApp->processEvents();

        save_as_submenu_->clearSubmenu();
        save_as_submenu_->requestAction()->setEnabled(false);

        if (delegate_
            && (delegate_->videoTracksCount() > 0
                || delegate_->audioTracksCount() > 0)) {

            QList<QAction*>      actions;
            QList<MenuCheckbox*> save_as_acts;

            QList<int> presets;
            if (VIDEOCONTENTEXIST
                && DEFAULTPRESET != kPreset_MP3Original)
                presets << kPreset_OriginalVideo;

            presets << kPreset_MP3Original;
            presets << kPreset_MP3LameInsane;
            presets << kPreset_MP3LameExtreme;
            presets << kPreset_MP3LameStandard;
            presets << kPreset_MP3LameR3mix;
            
            if (forcedConversionToLowQualityEnabled())
                presets << kPreset_MP3LowQuality;

            presets << kPreset_OriginalAudio;

            if (VIDEOCONTENTEXIST
                && DEFAULTPRESET == kPreset_MP3Original)
                presets << kPreset_OriginalVideo;

            for (int i = 0; i < presets.size(); ++i) {

                if (save_as_acts.size() <= i) {
                    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                    save_as_acts.push_back(menu_checkbox);
                }

                auto a = save_as_acts[i];

                actions.push_back(a->requestAction());

                a->setName(delegate_->presetTrackDesk(static_cast<EPreset>(presets[i])));
                a->requestAction()->setData(presets[i]);
                a->requestAction()->setCheckable(true);
            }

            if (!actions.isEmpty()) {
                save_as_submenu_->createSubmenu(actions, true);
                save_as_submenu_->requestAction()->setEnabled(true);
            }
        }

        save_as_submenu_->setName(tr("Save as"));

        qApp->processEvents();
    }
}

void VideoControl::fillSubtitleMenu() {
    if (subtitle_submenu_) {
        qApp->processEvents();

        subtitle_submenu_->clearSubmenu();
        subtitle_submenu_->requestAction()->setEnabled(false);

        if (delegate_ 
            && delegate_->subtitleTracksCount() > 0) {

            QList<QAction*>     actions;
            QList<MenuCheckbox*> subtitle_acts;
            for (int i = 0; i < delegate_->subtitleTracksCount(); ++i) {

                if (subtitle_acts.size() <= i) {
                    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                    subtitle_acts.push_back(menu_checkbox);
                }

                auto a = subtitle_acts[i];

                actions.push_back(a->requestAction());

                a->setName(delegate_->subtitleTrackDesc(i));
                a->requestAction()->setData(i);
                a->requestAction()->setCheckable(true);
            }

            if (!actions.isEmpty()) {
                subtitle_submenu_->createSubmenu(actions, false);
                subtitle_submenu_->requestAction()->setEnabled(true);
            }
        }

        subtitle_submenu_->setName(tr("Subtitle languages"));

        qApp->processEvents();
    }
}

void VideoControl::fillSubtitleSaveAsMenu() {
    if (subtitle_saveas_submenu_) {
        qApp->processEvents();

        subtitle_saveas_submenu_->clearSubmenu();
        subtitle_saveas_submenu_->requestAction()->setEnabled(false);

        QList<QAction*>     actions;
        QList<MenuCheckbox*> priority_acts;

        QList<ESaveSubtitlesAs> saveas_data = { kSaveSubtitlesAs_Embedded, kSaveSubtitlesAs_SRT, kSaveSubtitlesAs_Original };

        for (auto it : saveas_data) {
            MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
            priority_acts.push_back(menu_checkbox);

            auto a = priority_acts[it];

            actions.push_back(a->requestAction());

            a->setName(saveSubtitlesAsDesc(it));
            a->requestAction()->setData(it);
            a->requestAction()->setCheckable(true);
        }

        subtitle_saveas_submenu_->createSubmenu(actions, false);
        subtitle_saveas_submenu_->requestAction()->setEnabled(true);

        subtitle_saveas_submenu_->setName(tr("Save subtitles as"));

        qApp->processEvents();
    }
}

void VideoControl::fillAudioLanguageMenu() {
    if (audio_languages_submenu_) {
        qApp->processEvents();

        audio_languages_submenu_->clearSubmenu();
        audio_languages_submenu_->requestAction()->setEnabled(false);

        if (delegate_ 
            && delegate_->audioLanguagesCount() > 0) {

            QList<QAction*>     actions;
            QList<MenuCheckbox*> audio_languages_acts;
            for (int i = 0; i < delegate_->audioLanguagesCount(); ++i) {

                if (audio_languages_acts.size() <= i) {
                    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                    audio_languages_acts.push_back(menu_checkbox);
                }

                auto a = audio_languages_acts[i];

                actions.push_back(a->requestAction());

                a->setName(delegate_->audioLanguageDesk(i));
                a->requestAction()->setData(i);
                a->requestAction()->setCheckable(true);
            }

            if (!actions.isEmpty()) {
                bool disable_multiselect = false;
                if (delegate_->portal() == EPortal::kYoutube)
                    disable_multiselect = true;
                audio_languages_submenu_->createSubmenu(actions, disable_multiselect);


                audio_languages_submenu_->requestAction()->setEnabled(true);
            }
        }

        audio_languages_submenu_->setName(tr("Audio languages"));

        qApp->processEvents();
    }
}

void VideoControl::fillImageMenu() {
    if (image_submenu_) {
        qApp->processEvents();

        image_submenu_->clearSubmenu();
        image_submenu_->requestAction()->setEnabled(false);

        if (delegate_ 
            && delegate_->imageTracksCount() > 0) {

            QList<QAction*> actions;
            QList<MenuCheckbox*> image_acts;
            for (int i = 0; i < delegate_->imageTracksCount(); ++i) {

                if (image_acts.size() <= i) {
                    MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                    image_acts.push_back(menu_checkbox);
                }

                auto a = image_acts[i];

                actions.push_back(a->requestAction());

                a->setName(delegate_->imageTrackDesc(i));
                a->requestAction()->setData(i);
                a->requestAction()->setCheckable(true);
            }

            if (!actions.isEmpty()) {
                image_submenu_->createSubmenu(actions, true);
                image_submenu_->requestAction()->setEnabled(true);
            }

            QString portal_name;
            portalName(delegate_->portal(), portal_name);

            if (!portal_name.isEmpty())
                image_submenu_->setName(portal_name.append(" ").append(tr("images quality")));
            else
                image_submenu_->setName(tr("Images quality"));
        }

        qApp->processEvents();
    }
}

void VideoControl::fillBtnSettingsMenu(int show_menu) {
    qApp->processEvents();

    if (ui_.btnSettings->menu() != nullptr) {
        for (auto act : ui_.btnSettings->menu()->actions()) {
            ui_.btnSettings->menu()->removeAction(act);
        }
    }
    
    if (priority_submenu_) {
        ui_.btnSettings->menu()->addAction(priority_submenu_->requestAction());
        TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
        priority_submenu_->setSignalButton(menu);
    }

    switch (show_menu) {
        case kShowMenu_VideoAndAudio:
            if (video_submenu_
                && !video_submenu_->subactions().isEmpty()) {
                ui_.btnSettings->menu()->addAction(video_submenu_->requestAction());
                TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
                video_submenu_->setSignalButton(menu);
            }
            if (audio_submenu_
                && !audio_submenu_->subactions().isEmpty()) {
                ui_.btnSettings->menu()->addAction(audio_submenu_->requestAction());
                TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
                audio_submenu_->setSignalButton(menu);
            }
            break;
        case kShowMenu_Video:
            if (video_submenu_
                && !video_submenu_->subactions().isEmpty()) {
                ui_.btnSettings->menu()->addAction(video_submenu_->requestAction());
                TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
                video_submenu_->setSignalButton(menu);
            }
            break;
        case kShowMenu_Audio:
            if (audio_submenu_
                && !audio_submenu_->subactions().isEmpty()) {
                ui_.btnSettings->menu()->addAction(audio_submenu_->requestAction());
                TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
                audio_submenu_->setSignalButton(menu);
            }
            break;
    }

    if (audio_languages_submenu_
        && !audio_languages_submenu_->subactions().isEmpty()) {
        ui_.btnSettings->menu()->addAction(audio_languages_submenu_->requestAction());
        TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
        audio_languages_submenu_->setSignalButton(menu);
    }

    if (save_as_submenu_
        && !save_as_submenu_->subactions().isEmpty()) {
        ui_.btnSettings->menu()->addAction(save_as_submenu_->requestAction());
        TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
        save_as_submenu_->setSignalButton(menu);
    }
    
    if (image_submenu_
        && !image_submenu_->subactions().isEmpty()) {
        ui_.btnSettings->menu()->addAction(image_submenu_->requestAction());
        TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
        image_submenu_->setSignalButton(menu);
    }

    if (subtitle_submenu_
        && !subtitle_submenu_->subactions().isEmpty()) {
        ui_.btnSettings->menu()->addAction(subtitle_submenu_->requestAction());
        TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
        subtitle_submenu_->setSignalButton(menu);
        
        if (subtitle_saveas_submenu_
            && !subtitle_saveas_submenu_->subactions().isEmpty()) {
            ui_.btnSettings->menu()->addAction(subtitle_saveas_submenu_->requestAction());
            TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnSettings->menu());
            subtitle_saveas_submenu_->setSignalButton(menu);
        }
    }

    qApp->processEvents();
}

void VideoControl::fillBtnPlaylistMenu() {
    qApp->processEvents();

    VideoInfo video_info = delegate_->videoInfo(); // rab

    for (auto playlist_submenu : playlist_submenus_)
        playlist_submenu->clearSubmenu();

    playlist_submenus_.clear();

    if (delegate_) {
        bool one = video_info.playlist.count() == 1;
        is_multiple_playList_ = video_info.playlist.count() > 1;
        
        if (!one) {
            for (auto it : video_info.playlist) {
                MenuSubmenu* playlist_submenu = createPlaylistSettings(video_info.playlist_kind, it);
                fillPlaylistMenu(playlist_submenu, video_info, it);
                playlist_submenus_.push_back(playlist_submenu);
            }
        }

        if (ui_.btnPlaylist->menu() != nullptr) {
            for (auto act : ui_.btnPlaylist->menu()->actions()) {
                ui_.btnPlaylist->menu()->removeAction(act);
            }
        }

        if (one) {
            auto it = video_info.playlist[0];
            ui_.btnPlaylist->setMenu(createOnePlaylistSettings(video_info.playlist_kind, it));
            fillOnePlaylistMenu(ui_.btnPlaylist->menu(), video_info, it);
        }
        else {
            if (is_multiple_playList_) {
                MenuCheckbox* menu_checkbox = createBtnAllSubMenu();
                ui_.btnPlaylist->menu()->addAction(menu_checkbox->requestAction());
            }

            for (auto playlist_submenu : playlist_submenus_) {
                ui_.btnPlaylist->menu()->addAction(playlist_submenu->requestAction());
                TransMenu* menu = qobject_cast<TransMenu*>(ui_.btnPlaylist->menu());
                playlist_submenu->setSignalButton(menu);
                
                if (is_multiple_playList_)
                    playlist_submenu->showCheckBox();
            }

            if (is_multiple_playList_)
                setAboutToHideSubMenu();
        }
    }

    qApp->processEvents();
}

void  VideoControl::setAboutToHideSubMenu() {
    QObject::connect(ui_.btnPlaylist->menu(), &QMenu::aboutToHide, [=]() {
        submenu_visible_ = false;

        if (!timer_is_running_
            && !playlist_menu_visible_) {

            timer_is_running_ = true;

            QTimer::singleShot(2 * 1000, this, [=]() {
                if (!submenu_visible_
                    && !playlist_menu_visible_) {

                    QString urls;

                    int idList = 0;
                    for (auto playlist_submenuA : playlist_submenus_) {
                        
                        bool submenuChecked = playlist_submenuA->getCheckBoxSelected()->isChecked();
                        if (submenuChecked) {
                            int count = delegate_->videoInfo().playlist.count();
                            if (count > idList) {
                                int countList = delegate_->videoInfo().playlist[idList].second.count();

                                for (auto a : playlist_submenuA->subactions()) {
                                    int id = a->data().toInt();

                                    if (id > 0
                                        && id <= countList) { // id == 0 -> all episodes case
                                        QString url = delegate_->videoInfo().playlist[idList].second[id - 1].second;
                                        urls.append(url + "|");
                                    }

                                    a->setChecked(false);
                                }
                            }
                        }

                        playlist_submenuA->getCheckBoxSelected()->setChecked(false);

                        idList++;
                    }
                    if (!ui_.btnPlaylist->menu()->actions().isEmpty())
                        ui_.btnPlaylist->menu()->actions()[0]->setChecked(false);

                    if (delegate_
                        && !urls.isEmpty())
                        emit openPlaylist(urls);
                }

                timer_is_running_ = false;
            });
        }
        else {
            submenu_visible_ = true;
        }
    });

    QObject::connect(ui_.btnPlaylist->menu(), &QMenu::aboutToShow, [=]() {
        submenu_visible_ = true;
    });
}

void VideoControl::removeItem() {
    delegate_ = nullptr;
}

QString VideoControl::playlistKindName(const PlaylistKind& playlist_kind) {
    QString name;

    switch (playlist_kind) {
        case PlaylistKind::kPlaylist:
            name = tr("Playlist");
            break;
        case PlaylistKind::kSeason:
            name = tr("Season");
            break;
        case PlaylistKind::kEmptyPlaylist:
            name = tr("Get playlist");
            break;
        case PlaylistKind::kPlaylistPending:
            name = tr("Loading playlist...");
            break;
    }

    return name;
}

QString VideoControl::playlistSubname(const int yt_content_kind) { //YoutubeParser enum class YtContentKind
    QString name;

    switch (yt_content_kind) {
        case 1:
            name = tr("Playlists");
            break;
        case 2:
            name = tr("Channel");
            break;
        case 3:
            name = tr("Shorts");
            break;
        case 4:
            name = tr("Videos");
            break;
        case 5:
            name = tr("Community");
            break;
        case 6:
            name = tr("Get playlist");
            break;
        case 7:
            name = tr("Get channel");
            break;
        case 8:
            name = tr("Get shorts");
            break;
        case 9:
            name = tr("Get videos");
            break;
        case 10:
            name = tr("Get community");
            break;
    }

    return name;
}

QString VideoControl::saveSubtitlesAsDesc(const ESaveSubtitlesAs& it) {
    QString desk;

    switch (it) {
    case kSaveSubtitlesAs_Embedded:
        desk = tr("Embedded");
        break;
    case kSaveSubtitlesAs_SRT:
        desk = tr("External SRT");
        break;
    case kSaveSubtitlesAs_Original:
        desk = tr("External original");
        break;
    }

    return desk;
}