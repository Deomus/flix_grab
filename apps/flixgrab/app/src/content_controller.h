#pragma once
#include <QtCore>
#include "settings.h"

#include "getflix.h"
#include "table_view.h"
#include "video_item_controller.h"
#include "mutable_list.h"

#include "alert_dialog.h"
#include "tutorial.h"


#define     ITEM_WIDTH                  (253)
#define     ITEM_HIGTH                  (172)
#define     ITEM_MARGIN_WIDTH           (19)
#define     ITEM_MARGIN_HIGTH           (19)
#define     ITEM_BORDER_WIDTH           (17)
#define     ITEM_BORDER_HIGTH           (17)


class VideoItemController;

enum EContentFilter {
    kContentFilter_All = 0,         //Items At All States;
    kContentFilter_SettingUp,       //Items at setting_up state;
    //kContentFilter_Pending,       //Pendings items;
    kContentFilter_Downloading,     //Downloadings Now Items;
    kContentFilter_Paused,          //Paused Now Items;
    kContentFilter_Completed,       //Completed items;
    kContentFilter_Failed,          //Failed items;
    NUM_FILTERS
};

enum EContentSorting {
    kContentSorting_DateAdded = 0,  //newly added first;
    kContentSorting_VideoTitle,
    kContentSorting_VideoSize,
    kContentSorting_VideoDuration,
    kContentSorting_VideoResolution,
    kContentSorting_PortalName,
    kContentSorting_outputMediaPath,
};

class ContentController : public QObject {
    Q_OBJECT
public:
    ContentController(Getflix* getflix, TableView* view,  TableContent* content, Tutorial* tutorial, QObject* parent = nullptr)
        : QObject(parent)
        , content_(content)
        , view_(view)
        , tutorial_(tutorial)
        , getflix_(getflix) {

        visible_list_ = new MutableList(content, this);

        items_in_filter_.resize(NUM_FILTERS);

        //Init Grid Layout for Tutorial;
        QGridLayout *layout_of_tutorial = new QGridLayout(view_->viewport());
        layout_of_tutorial->setContentsMargins(0, 0, 0, 0);
        layout_of_tutorial->addWidget(tutorial_);

        content_->setItemGeometry(QSize(ITEM_WIDTH, ITEM_HIGTH), QSize(ITEM_MARGIN_WIDTH, ITEM_MARGIN_HIGTH), QSize(ITEM_BORDER_WIDTH, ITEM_BORDER_HIGTH), view->verticalScrollBar()->sizeHint().width());
        content_->setItems(visible_list_);
        QObject::connect(content_, &TableContent::contextMenuRequested, this, &ContentController::execContextMenu);
        QObject::connect(content_, &TableContent::updateItemsOnDragItems, this, &ContentController::updateItemsOnDragItems);

        view_->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(view_, &TableView::customContextMenuRequested, this, &ContentController::execContextMenuGroup);
    }

    ~ContentController() {
        qDebug() << "~ContentController()";
    }

signals:
    //Size of filter content is changed;
    void                    filterSizeChanged(EContentFilter filter, int size);
    //Current filter is empty, select this filter;
    void                    filterAboutToSwitch(EContentFilter filter);

    void                    filterChanged(EContentFilter filter);
    void                    sortingChanged(EContentSorting sorting);

public slots:
    void                    paste(const QString& url) {
        addItem(url);
        /*VideoHandle* video = getflix_->add(url);

        VideoItemController* item = new VideoItemController(video, this);

        items_.push_back(item);
        QObject::connect(item, &AbstractItem::removed, this, &ContentController::removeSender);

        QObject::connect(item, &VideoItemController::stateAboutToChange, this, &ContentController::onItemStateChanging);*/
                
        setFilter(kContentFilter_All);
        
        //deferred call
        QMetaObject::invokeMethod(this, "ensureLastAddedItemVisible", Qt::QueuedConnection);
    }

    void                    pasteFromClipboard() {
        paste(QApplication::clipboard()->text());
    }

    void downloadAll() {
        QList<VideoHandle*>             videos;

        for (auto item : visible_list_->items()) {
            if (auto video_item = qobject_cast<VideoItemController*>(item)) {
                if (!video_item->video()->isFinished()
                    && !video_item->video()->isErrorState()
                    && ((!video_item->video()->isDownloadable())
                        || (video_item->video()->isDownloadable() && !video_item->video()->isDownloading())
                        || (video_item->video()->isDownloading() && video_item->video()->isPaused())))
                    videos.push_back(video_item->video());
            }
        }

        getflix_->download(videos);
        //getflix_->downloadAll();
        //TODO: switch to download filter;
        //setFilter(kContentFilter_Downloading);
    }

    void pauseAll() {
        getflix_->pauseAll();
        //TODO: switch to pause filter;
        //setFilter(kContentFilter_Paused);
    }
   
    void ensureItemVisible(int index) {
        QRect item_rect = content_->itemRect(index);
        QPoint item_center = item_rect.center();

        view_->ensureVisible(item_center.x(), item_center.y(), item_rect.width() / 2, item_rect.height() / 2);
    }
    
    EContentSorting sorting() const {
        return sorting_; 
    }
    
    void selectAllVisibles() {
        auto items = visible_list_->items();
        for (auto item : items)
            item->setSelected(!item->isSelected());
    }

    void removeAllVisibles() {
        auto items = visible_list_->items();
        QList<VideoHandle*> videos;
        for (auto item : items) {
            if (auto video_item = qobject_cast<VideoItemController*>(item))
                videos.push_back(video_item->video());
                //video_item->remove();
        }

        getflix_->cancel(videos);
    }

    void                    deleteAllVisibles() {
        auto items = visible_list_->items();
        QList<VideoHandle*> videos;
        
        if (AlertDialog::execDialog(EAlert::kAlert_DeleteItems) == QDialog::Accepted) {
            for (auto item : items) {
                if (auto video_item = qobject_cast<VideoItemController*>(item)) {
                    video_item->markToDelete();
                    videos.push_back(video_item->video());
                }
            }
        }

        getflix_->cancel(videos);
    }

    void                    autosaveIncompletes() {}
    void                    autoloadIncompletes(){}

    bool                    saveList(const QString& file_name, bool user_action = true) const {
        bool result = false;

        QList<QString> urls;
        for (auto item : visible_list_->items()) {
            if (auto video_controller = qobject_cast<VideoItemController*>(item)) {
                auto item_video = video_controller->video();
                //TODO: remake error checking!
                if (item_video
                    && !item_video->isFinished()
                    && item_video->status().ok()
                    && !urls.contains(item_video->url().toString())) {
                    urls << item_video->url().toString();
                }
            }
        }

        QFile   file(file_name);
        if (file.open(QFile::WriteOnly | QFile::Text)) {
            QTextStream out(&file);

            int index = 0;
            if (user_action
                || urls.size() < 15) {

                result = true;
                for (auto url : urls) {
                    if (index++ > 0)
                        out << Qt::endl;
                    out << url;
                }
            }
        }

        if (urls.isEmpty())
            result = true;

        return result;

     /*   QThread* thread = new QThread();
        QObject* o = new QObject(thread);
        thread->start();
        o->moveToThread(thread);

        QTimer::singleShot(0, o, [=]() {
                qDebug() << "Thread ID: " << QThread::currentThreadId();
                thread->deleteLater();
        });*/
    }

    void loadList(const QString& file_name) {
        QFile   file(file_name);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            QString url;
            while (!in.atEnd()) {
                in >> url;
                if (!url.isEmpty())
                    addItem(url);
            }
        }
    }

    void                    loadPlayList(const QString& playlist) {
        QList<QString> list = playlist.split("|");
        list.removeAll(QString(""));

        for (auto url : list) {
            if (!url.isEmpty())
                addItem(url);
        }
    }

    void        applySettings() {
        if (!items_.isEmpty()) {
            for (auto item : items_) {
                if (item->itemState() == kItemState_SettingUp)
                    item->applySettings();
            }
        }
    }

    void        translateItem() {
        for (auto item : items_)
            item->translateItem();
    }

private slots:
    int    addItem(const QString& url) {
        if (VideoHandle* video = getflix_->add(url)) {
            VideoItemController* item = new VideoItemController(video, this);

            items_.push_back(item);
            QObject::connect(item, &AbstractItem::removed, this, &ContentController::removeSender);

            QObject::connect(item, &VideoItemController::stateAboutToChange, this, &ContentController::onItemStateChanging);
            QObject::connect(item, &VideoItemController::showOptions, this, &ContentController::showOptionsMenu);
            QObject::connect(item, SIGNAL(openPlaylist(const QString&)), this, SLOT(loadPlayList(const QString&)));

            if (!items_.empty())
                tutorial_->setVisible(false);

            int date_in_milliseconds_now = QDateTime::currentMSecsSinceEpoch() + items_.count();
            item->set_date_in_milliseconds_now_for_sort(date_in_milliseconds_now);

            return items_.size() - 1;
        }

        return -1;
    }

    void removeItem(int index) {
        VideoItemController* item = items_[index];
        item->remove();
    }

    void                    removeSender() {
        if (VideoItemController* item = qobject_cast<VideoItemController*>(sender())) {
            items_.removeAll(item);
            visible_list_->removeItem(item);
            QObject::disconnect(item, &AbstractItem::removed, this, &ContentController::removeSender);

            if (items_.empty())
                tutorial_->setVisible(true);
        }
    }

public slots:
    void setSorting(EContentSorting sorting) {
        if (sorting_ != sorting) {
            sorting_ = sorting;
            sort_direction_ = 0;
            applyFilterAndSorting(filter_, sorting_, sort_direction_);
            emit sortingChanged(sorting_);
        }
        else {
            if (sort_direction_ == 0) sort_direction_ = 1;
            else sort_direction_ = 0;
            applyFilterAndSorting(filter_, sorting_, sort_direction_);
            emit sortingChanged(sorting_);
        }
    }
    
    void setFilter(EContentFilter filter) {
        if (filter_ != filter) {
            filter_ = filter;
            applyFilterAndSorting(filter_, sorting_, sort_direction_);
            emit filterChanged(filter_);
        }
    }

private slots:
    //COMMENT: ensure that call will be last in event loop;
    void        ensureLastAddedItemVisible() {
        ensureItemVisible(last_added_visible_);
    }

    void        execContextMenuGroup(const QPoint &pos) {
        QMenu context_menu_group(view_);
        context_menu_group.setStyleSheet(context_menu_stylesheet_);

        QObject::connect(context_menu_group.addAction(tr("Show output"))
            , &QAction::triggered, [&]() {
            QDesktopServices::openUrl(QUrl::fromLocalFile(settings_.value(settings::kOutputFolder).toString()));
        });

        if (visible_list_->items().size() > 0) {
            QObject::connect(context_menu_group.addAction(tr("Select all"))
                , &QAction::triggered, [&]() {
                selectAllVisibles();
            });

            QObject::connect(context_menu_group.addAction(tr("Remove all"))
                , &QAction::triggered, [&]() {
                removeAllVisibles();
            });

            QObject::connect(context_menu_group.addAction(tr("Delete all"))
                , &QAction::triggered, [&]() {
                deleteAllVisibles();
            });

            QObject::connect(context_menu_group.addAction(tr("Save list"))
                , &QAction::triggered, [&]() {

                QString path = settings_.value(settings::kListPath).toString();
                path = QFileDialog::getSaveFileName(nullptr
                    , tr("Save list"), path
                    , tr("Text files") + " (*.txt *.lst *.list)");
                settings_.setValue(settings::kListPath, path);
                saveList(path);
            });
        }

        QObject::connect(context_menu_group.addAction(tr("Load list"))
            , &QAction::triggered, [&]() {

            QString path = settings_.value(settings::kListPath).toString();
            path = QFileDialog::getOpenFileName(nullptr
                , tr("Load list"), path
                , tr("Text files") + " (*.txt *.lst *.list)");
            settings_.setValue(settings::kListPath, path);
            loadList(path);
        });

        if (enable_show_menu_group_)
            context_menu_group.exec(view_->mapToGlobal(pos));
        else
            enable_show_menu_group_ = true;
    }

    void execContextMenu(const QPoint &pos, const QList<AbstractItem*>& items) {
        QList<VideoItemController*> video_items;
        QList<VideoHandle*> videos;

        QMenu context_menu(content_);
        context_menu.setStyleSheet(context_menu_stylesheet_);

        bool    saveto_enabled = false;
        bool    download_enabled = false;
        bool    pause_enabled = false;
        bool    delete_enabled = false;
        bool    show_output = false;
        bool    redownload_enabled = false;

        for (auto item : items) {
            if (auto video_item = qobject_cast<VideoItemController*>(item)) {
                if (video_item->isSelected()) {
                    video_items.push_back(video_item);
                    videos.push_back(video_item->video());

                    if (!video_item->video()->isFinished()
                        && !video_item->video()->isErrorState()
                        && ((!video_item->video()->isDownloadable())
                            || (video_item->video()->isDownloadable() && !video_item->video()->isDownloading())
                            || (video_item->video()->isDownloading() && video_item->video()->isPaused())))
                        download_enabled = true;

                    if (video_item->video()->isDownloading()
                        && !video_item->video()->isPaused())
                        pause_enabled = true;

                    if (video_item->video()->isFinished()) {
                        show_output = true;
                        delete_enabled = true;
                    }

                    if (video_item->video()->isDownloadable()
                        || video_item->video()->isFinished()
                        || video_item->video()->isErrorState())
                        redownload_enabled = true;

                    if (video_item->video()->isDownloadable()
                        && !video_item->video()->isDownloading()
                        && !video_item->video()->isFinished()
                        && !video_item->video()->isErrorState())
                        saveto_enabled = true;
                }
            }
        }

        if (saveto_enabled) {
            QObject::connect(context_menu.addAction(tr("Save to"))
                , &QAction::triggered, [&]() {

                enable_show_menu_group_ = false;
                if (!video_items.isEmpty())
                    video_items[0]->showOutputFolder();
            });
        }

        if (show_output) {
            QObject::connect(context_menu.addAction(tr("Show output"))
                , &QAction::triggered, [&]() {

                enable_show_menu_group_ = false;
                if (!video_items.isEmpty())
                    video_items[0]->showOutputFolder();
            });
        }

        if (download_enabled) {
            QObject::connect(context_menu.addAction(tr("Download"))
                , &QAction::triggered, [&]() {

                enable_show_menu_group_ = false;
                getflix_->download(videos);
            });
        }

        if (pause_enabled) {
            QObject::connect(context_menu.addAction(tr("Pause"))
                , &QAction::triggered, [&]() {

                enable_show_menu_group_ = false;
                getflix_->pause(videos);
            });
        }

        QObject::connect(context_menu.addAction(tr("Remove"))
            , &QAction::triggered, [&]() {

            /*for (auto item : video_items)
                item->remove();*/
            enable_show_menu_group_ = false;
            getflix_->cancel(videos);
        });

        if (delete_enabled) {
            QObject::connect(context_menu.addAction(tr("Delete"))
                , &QAction::triggered, [&]() {

                if (AlertDialog::execDialog(EAlert::kAlert_DeleteItems) == QDialog::Accepted) {
                    for (auto item : video_items)
                        item->markToDelete();

                    enable_show_menu_group_ = false;
                    getflix_->cancel(videos);
                }
            });
        }

        QObject::connect(context_menu.addAction(tr("Copy link"))
            , &QAction::triggered, [&]() {

            if (!videos.isEmpty()) {
                QApplication::clipboard()->clear();
                QString text;

                for (auto video : videos) {
					text += video->url().toEncoded();
					if (video != videos.back())
						text += "\n";
                }

                QApplication::clipboard()->setText(text);
                enable_show_menu_group_ = false;
            }
        });

        if (redownload_enabled) {
            QObject::connect(context_menu.addAction(tr("Redownload"))
                , &QAction::triggered, [&]() {

                for (auto video : videos)
                    addItem(video->url().toString());

                enable_show_menu_group_ = false;
            });
        }


        QObject::connect(context_menu.addAction(tr("Open in browser"))
            , &QAction::triggered, [&]() {

                for (auto video : videos)
                    QDesktopServices::openUrl(video->url());

                enable_show_menu_group_ = false;
        });

        context_menu.exec(content_->mapToGlobal(pos));
    }

    void showOptionsMenu(QPushButton* button) {
        auto video_item_s = sender();
        VideoItemController* video_item = qobject_cast<VideoItemController*>(video_item_s);
        if (video_item) {
            VideoHandle* video = video_item->video();
            QList<VideoHandle*> videos;
            videos.push_back(video);

            QMenu* context_menu = button->menu();
            context_menu->clear();
            context_menu->setStyleSheet(context_menu_stylesheet_);

            bool    saveto_enabled = false;
            bool    download_enabled = false;
            bool    pause_enabled = false;
            bool    delete_enabled = false;
            bool    show_output = false;
            bool    redownload_enabled = false;

            if (!video->isFinished()
                && !video->isErrorState()
                && ((!video->isDownloadable())
                    || (video->isDownloadable() && !video->isDownloading())
                    || (video->isDownloading() && video->isPaused())))
                download_enabled = true;

            if (video->isDownloading()
                && !video->isPaused())
                pause_enabled = true;

            if (video->isFinished()) {
                show_output = true;
                delete_enabled = true;
            }

            if (video->isDownloadable()
                || video->isFinished()
                || video->isErrorState())
                redownload_enabled = true;

            if (video->isDownloadable()
                && !video->isDownloading()
                && !video->isFinished()
                && !video->isErrorState())
                saveto_enabled = true;

            if (saveto_enabled) {
                QObject::connect(context_menu->addAction(tr("Save to"))
                    , &QAction::triggered, this, [video_item]() {

                    if (video_item)
                        video_item->showOutputFolder();
                }, Qt::UniqueConnection);
            }

            if (show_output) {
                QObject::connect(context_menu->addAction(tr("Show output"))
                    , &QAction::triggered, this, [video_item]() {

                    if (video_item)
                        video_item->showOutputFolder();
                }, Qt::UniqueConnection);
            }

            if (download_enabled) {
                QObject::connect(context_menu->addAction(tr("Download"))
                    , &QAction::triggered, this, [this, videos]() {

                    if (!videos.empty())
                        getflix_->download(videos);
                });
            }

            if (pause_enabled) {
                QObject::connect(context_menu->addAction(tr("Pause"))
                    , &QAction::triggered, this, [this, videos]() {

                    if (!videos.empty())
                        getflix_->pause(videos);
                });
            }

            QObject::connect(context_menu->addAction(tr("Remove"))
                , &QAction::triggered, this, [this, videos]() {

                if (!videos.empty())
                    getflix_->cancel(videos);
            });

            if (delete_enabled) {
                QObject::connect(context_menu->addAction(tr("Delete"))
                    , &QAction::triggered, this, [this, videos, video_item]() {

                    if (video_item) {
                        if (AlertDialog::execDialog(EAlert::kAlert_DeleteItems) == QDialog::Accepted) {
                            video_item->markToDelete();

                            if (!videos.empty())
                                getflix_->cancel(videos);
                        }
                    }
                });
            }

            QObject::connect(context_menu->addAction(tr("Copy link"))
                , &QAction::triggered, this, [this, video]() {

                if (video) {
                    QApplication::clipboard()->clear();
                    QString text;

                    text += video->url().toEncoded();

                    QApplication::clipboard()->setText(text);
                }
            });

            if (redownload_enabled) {
                QObject::connect(context_menu->addAction(tr("Redownload"))
                    , &QAction::triggered, this, [this, video]() {

                    if (video)
                        addItem(video->url().toString());
                });
            }

            QObject::connect(context_menu->addAction(tr("Open in browser"))
                , &QAction::triggered, this, [this, video]() {

                if (video)
                    QDesktopServices::openUrl(video->url());
            });
        }
    }

    void onItemStateChanging(EItemState new_state, EItemState old_state) {
        if (auto item = qobject_cast<VideoItemController*>(sender())) {
            bool was_rendered = !isItemFiltered(filter_, old_state);
            bool will_rendered = !isItemFiltered(filter_, new_state);
           
            if (!was_rendered
                && will_rendered)
                addVisibleItem(item);
            else if (was_rendered
                && !will_rendered)
                removeVisibleItem(item);
        }

        EContentFilter fallback_filter = kContentFilter_All;
        //int prev_filtered = items_in_filter_[filter_];
        for (int f = 0; f < items_in_filter_.size(); ++f) {
            auto prev = items_in_filter_[f];
            EContentFilter  filter = static_cast<EContentFilter>(f);
            if (!isItemFiltered(filter, old_state))
                items_in_filter_[f]--;

            if (!isItemFiltered(filter, new_state)) {
                items_in_filter_[f]++;
                fallback_filter = filter;
            }

            if (prev != items_in_filter_[f])
                emit filterSizeChanged(filter, items_in_filter_[f]);    
                        
            Q_ASSERT(items_in_filter_[f] >= 0);
        }
        //Check of filter have no items in it;
        //if (prev_filtered > 0 && items_in_filter_[filter_] == 0)
        if (items_in_filter_[filter_] == 0 
            && !isItemFiltered(filter_, old_state))
            emit filterAboutToSwitch(fallback_filter);
    }

private:
    void addVisibleItem(AbstractItem* item) {
        //Last index in visible list;
        int adding_index = visible_list_->count();
        auto items = visible_list_->items();

        auto video_item = qobject_cast<VideoItemController*>(item);
        if (video_item->itemState() >= EItemState::kItemState_SettingUp) {

            qint64 video_size_select = video_item->get_size_item_for_sort();
            VideoHandle* video = video_item->video();
            const VideoInfo videoInfo = video->videoInfo();

            QString video_title_select = videoInfo.video_title;
            int video_duration_select = videoInfo.duration;
            int video_resolution_select = video_item->get_height_item_for_sort();
            const EPortal& video_portal_select = video->portal();
            QString video_output_path_select = video->outputMediaPath();

            int index_item_add = 0;

            if (sort_direction_ == 1)
                adding_index = 0;

            switch (sorting_) {
                case kContentSorting_DateAdded: {
                    if (sort_direction_ == 1)
                        adding_index = 0;

                    break;
                }
                case kContentSorting_VideoSize: {
                    for (auto item : items) {
                        auto video_item = qobject_cast<VideoItemController*>(item);
                        qint64 video_size = video_item->get_size_item_for_sort();

                        if (video_size > video_size_select
                            && sort_direction_ == 0) {
                            adding_index = index_item_add;
                            break;
                        }
                        else if (video_size < video_size_select
                            && sort_direction_ != 0) {
                            adding_index = index_item_add;
                            break;
                        }

                        index_item_add += 1;
                    }
                    break;
                }
                case kContentSorting_VideoDuration: {
                    for (auto item : items) {
                        auto video_item = qobject_cast<VideoItemController*>(item);
                        VideoHandle* video = video_item->video();
                        const VideoInfo videoInfo = video->videoInfo();

                        int video_duration = videoInfo.duration;

                        if (video_duration > video_duration_select
                            && sort_direction_ == 0) {
                            adding_index = index_item_add;
                            break;
                        }
                        else if (video_duration < video_duration_select
                            && sort_direction_ != 0) {
                            adding_index = index_item_add;
                            break;
                        }

                        index_item_add += 1;
                    }
                    break;
                }
                case kContentSorting_VideoResolution: {
                    for (auto item : items) {
                        auto video_item = qobject_cast<VideoItemController*>(item);
                        int video_resolution = video_item->get_height_item_for_sort();

                        if (video_resolution > video_resolution_select
                            && sort_direction_ == 0) {
                            adding_index = index_item_add;
                            break;
                        }
                        else if (video_resolution < video_resolution_select
                            && sort_direction_ != 0) {
                            adding_index = index_item_add;
                            break;
                        }

                        index_item_add += 1;
                    }
                    break;
                }
                case kContentSorting_PortalName: {
                    for (auto item : items) {
                        auto video_item = qobject_cast<VideoItemController*>(item);
                        VideoHandle* video = video_item->video();
                        const EPortal& video_portal = video->portal();

                        if (video_portal > video_portal_select
                            && sort_direction_ == 0) {
                            adding_index = index_item_add;
                            break;
                        }
                        else if (video_portal < video_portal_select
                            && sort_direction_ != 0) {
                            adding_index = index_item_add;
                            break;
                        }

                        index_item_add += 1;
                    }
                    break;
                }
                case kContentSorting_VideoTitle:
                case kContentSorting_outputMediaPath: {
                    for (auto item : items) {
                        auto video_item = qobject_cast<VideoItemController*>(item);
                        VideoHandle* video = video_item->video();
                        QString video_output_path = video->outputMediaPath();

                        if (video_output_path > video_output_path_select
                            && sort_direction_ == 0) {
                            adding_index = index_item_add;
                            break;
                        }
                        else if (video_output_path < video_output_path_select
                            && sort_direction_ != 0) {
                            adding_index = index_item_add;
                            break;
                        }

                        index_item_add += 1;
                    }
                    break;
                }
            }
        }

        visible_list_->addItem(item, adding_index);
        last_added_visible_ = adding_index;
    }

    void updateItemsOnDragItems(const QList<AbstractItem*>& items, AbstractItem* item_start_, AbstractItem* item_pos) {
        visible_list_->setItems(items);

        QList<AbstractItem*>	items_drag;
        QList<AbstractItem*>	items_drag_is_selected;

        for (auto item : items_) {
            if (!item->isSelected() || item == qobject_cast<VideoItemController*>(item_pos))
                items_drag.push_back(item);

            if (item->isSelected())
                items_drag_is_selected.push_back(item);
        }

        int index_for_insert = items_drag.indexOf(item_pos);
        VideoItemController* item_last = items_[items_.size() - 1];

        if (item_last == item_pos)
            index_for_insert = items_drag.count();

        if (item_pos != item_start_) {
            if (item_pos->isSelected())
                items_drag.replace(index_for_insert, item_start_);
            else
                items_drag.insert(index_for_insert, item_start_);
        }

        for (int i = 0; i < items_drag_is_selected.count(); ++i) {
            auto item = items_drag_is_selected.at(i);
            if (item != item_start_) {
                if (index_for_insert < items_.size())
                    index_for_insert += 1;

                items_drag.insert(index_for_insert, item);
            }
        }

        items_.clear();
        for (auto item : items_drag) {
            if (auto video_item = qobject_cast<VideoItemController*>(item))
                items_.push_back(video_item);
        }

    }

    void        removeVisibleItem(AbstractItem* item) {
        visible_list_->removeItem(item);
    }

    void applyFilterAndSorting(EContentFilter filter, EContentSorting sorting, int sort_direction) {
        QList<AbstractItem*>        filtered;

        for (auto item : items_) {
            if (!isItemFiltered(filter, item->itemState()))
                filtered.push_back(item);
        }

        switch (sorting_) {
            case kContentSorting_DateAdded: {
                if (sort_direction == 0) {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        int video_date_in_milliseconds_a = video_item_a->get_date_in_milliseconds_now_for_sort();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        int video_date_in_milliseconds_b = video_item_b->get_date_in_milliseconds_now_for_sort();

                        return video_date_in_milliseconds_a < video_date_in_milliseconds_b;
                    });
                }
                else {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        int video_date_in_milliseconds_a = video_item_a->get_date_in_milliseconds_now_for_sort();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        int video_date_in_milliseconds_b = video_item_b->get_date_in_milliseconds_now_for_sort();

                        return video_date_in_milliseconds_a > video_date_in_milliseconds_b;
                    });
                }
                break;
            }
            case kContentSorting_VideoSize: {
                if (sort_direction == 0) {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        qint64 video_size_a = video_item_a->get_size_item_for_sort();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        qint64 video_size_b = video_item_b->get_size_item_for_sort();

                        return video_size_a < video_size_b;
                    });
                }
                else {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        qint64 video_size_a = video_item_a->get_size_item_for_sort();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        qint64 video_size_b = video_item_b->get_size_item_for_sort();

                        return video_size_a > video_size_b;
                    });
                }
                break;
            }
            case kContentSorting_VideoDuration: {
                if (sort_direction == 0) {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        VideoHandle* video_a = video_item_a->video();
                        const VideoInfo videoInfo_a = video_a->videoInfo();
                        int video_duration_a = videoInfo_a.duration;

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        VideoHandle* video_b = video_item_b->video();
                        const VideoInfo videoInfo_b = video_b->videoInfo();

                        int video_duration_b = videoInfo_b.duration;

                        return video_duration_a < video_duration_b;
                    });
                }
                else {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        VideoHandle* video_a = video_item_a->video();
                        const VideoInfo videoInfo_a = video_a->videoInfo();

                        int video_duration_a = videoInfo_a.duration;

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        VideoHandle* video_b = video_item_b->video();
                        const VideoInfo videoInfo_b = video_b->videoInfo();

                        int video_duration_b = videoInfo_b.duration;
                        return video_duration_a > video_duration_b;
                    });
                }
                break;
            }
            case kContentSorting_VideoResolution: {
                if (sort_direction == 0) {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        qint64 video_height_a = video_item_a->get_height_item_for_sort();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        qint64 video_height_b = video_item_b->get_height_item_for_sort();

                        return video_height_a < video_height_b;
                    });
                }
                else {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        qint64 video_height_a = video_item_a->get_height_item_for_sort();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        qint64 video_height_b = video_item_b->get_height_item_for_sort();

                        return video_height_a > video_height_b;
                    });
                }
                break;
            }
            case kContentSorting_PortalName: {
                if (sort_direction == 0) {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        VideoHandle* video_a = video_item_a->video();
                        const EPortal& video_portal_a = video_a->portal();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        VideoHandle* video_b = video_item_b->video();
                        const EPortal& video_portal_b = video_b->portal();

                        return video_portal_a < video_portal_b;
                    });
                }
                else {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        VideoHandle* video_a = video_item_a->video();
                        const EPortal& video_portal_a = video_a->portal();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        VideoHandle* video_b = video_item_b->video();
                        const EPortal& video_portal_b = video_b->portal();

                        return video_portal_a > video_portal_b;
                    });
                }
                break;
            }
            case kContentSorting_VideoTitle:
            case kContentSorting_outputMediaPath: {
                if (sort_direction == 0) {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        VideoHandle* video_a = video_item_a->video();
                        QString video_output_path_a = video_a->outputMediaPath();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        VideoHandle* video_b = video_item_b->video();

                        QString video_output_path_b = video_b->outputMediaPath();
                        return video_output_path_a < video_output_path_b;
                    });
                }
                else {
                    std::sort(filtered.begin(), filtered.end(), [](AbstractItem* a, AbstractItem* b) {
                        auto video_item_a = qobject_cast<VideoItemController*>(a);
                        VideoHandle* video_a = video_item_a->video();
                        QString video_output_path_a = video_a->outputMediaPath();

                        auto video_item_b = qobject_cast<VideoItemController*>(b);
                        VideoHandle* video_b = video_item_b->video();

                        QString video_output_path_b = video_b->outputMediaPath();

                        return video_output_path_a > video_output_path_b;
                    });
                }
                break;
            }
        }
        
        visible_list_->setItems(filtered);
    }

    static bool isItemFiltered(EContentFilter filter, EItemState state) {
        if (state == kItemState_Unknown
            || state == kItemState_Cancelled)
            return true;

        switch (filter) {
            case kContentFilter_All:
                return false;

            case kContentFilter_SettingUp:
                return (state != kItemState_SettingUp);
            /*case kContentFilter_Pending:
                return (state != kItemState_Pending);*/

            case kContentFilter_Downloading:
                return (state != kItemState_Downloading) 
                    && (state != kItemState_Pending);

            case kContentFilter_Paused:
                return (state != kItemState_Paused);

            case kContentFilter_Completed:
                return (state != kItemState_Completed);

            case kContentFilter_Failed:
                return (state != kItemState_Failed);
        }

        return false;
    }
    
private:
    //Items in sequence of adding;
    QList<VideoItemController*>                     items_;
    //Netflix Engine;
    Getflix*                                        getflix_;
    TableContent*                                   content_;
    TableView*                                      view_;

private:
    
    MutableList*                                    visible_list_;
 
    int                                             last_added_visible_ = 0;
    
private:
    QVector<int>                                    items_in_filter_;
    //Current filter of contents;
    EContentFilter                                  filter_ = kContentFilter_All;
    EContentSorting                                 sorting_ = kContentSorting_DateAdded;
    int                                             sort_direction_ = 1;

private:
    Tutorial*                                       tutorial_;
    QSettings                                       settings_;
    bool                                            enable_show_menu_group_ = true;

private:
    const QString context_menu_stylesheet_ =
        "QMenu{border: none;}"
        "QMenu::item{"
        "background-color: #2A2E3A;"
        "color: #F2F3F3;"
        "padding: 6 14 6 14;"
        "font-family: \"Tahoma\";"
        "font-size: 9pt;"
        "}"
        "QMenu::item:hover{"
        "background-color: #414755;"
        "}"
        "QMenu::item:selected{"
        "background-color: #414755;"
        "}"
        "QMenu::separator{"
        "background-color: #414755;"
        "height: 1px;"
        "}"
        "QMenu::separator:hover{"
        "background-color: #414755;"
        "}"
        ;
};