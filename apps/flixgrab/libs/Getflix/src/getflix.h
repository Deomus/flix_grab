#pragma once

#include <QtWebEngineWidgets>
#include <QtWebEngineCore>
#include <QtGui>

#include <thread>

#include <common_config.h>

#include "web_page.h"
#include "video_handle.h"
#include "web_engine.h"
#include "priority_list.h"


class WebEngine;
class WebPage;
class PrivateKeysClient;

class Getflix : public QObject
{
    friend class VideoHandle;

    Q_OBJECT

public:
    explicit Getflix(QObject *parent = 0);
    ~Getflix();

    
    //void setProxy(const QNetworkProxy& proxy);
    VideoHandle*    add(const QString& url);

    int             downloadsSize() const { return downloads_.size(); }

    //Current Error Status;
    const Status&   status() const { return status_; }

    VideoHandle*    current() { return video_in_process_; }

    bool            isDownloading() const { return (downloads_.size() > 0); }

    void            setActivated(bool activated = false) { activated_ = activated; }
    bool            isActivated() { return  activated_; }
    void            setRetryLoadInterval(int retry_load_interval) { retry_load_interval_ = retry_load_interval;}

public slots:
    void            signIn();
    void            deleteCookies();

signals:
    void            created(VideoHandle*);

    //About to change status of downloading;
    void            aboutToParsingPlaylist(EPortal&);
    void            aboutToDownload(VideoHandle*);
    void            aboutToPending(VideoHandle*);
    void            aboutToPause(VideoHandle*);
 
    void            finished(VideoHandle*);
    void            finisheditems();
    void            error();
    void            saveSettings(const QString&);
    void            loadSettings(const QString&);
    void            loadAudioLanguages();
    void            loadPlaylistItem(const QString&);

private slots:
//Recieve modified script;
      

    //Data from page and engine;
    void            handleTracksInfo(const TracksInfo& info);
    void            handleVideoInfo(const VideoInfo& info);

    void            requestNextVideo();

    void            handleIconChanged(const QIcon& icon);
    void            handleWindowHidden();

private:
    Q_INVOKABLE void analyze(VideoHandle* handle );
    Q_INVOKABLE void analyzePlaylist(VideoHandle* handle );
    Q_INVOKABLE void stopPage();
    
    void            setupWebEngine();
    
    void            newError(const Status&);

   // void            cancel(VideoHandle* video);
    void            handleVideoReady(VideoHandle* video);
    void            handleVideoCancelling(VideoHandle* video);
    void            removeVideoInProcess(VideoHandle* video);

    VideoHandle*    createVideo(const QString& url);
    void            clickPage(const QString& pos);
    void            saveSetting(const QString& data);
    void            loadSetting(const QString& key);
    void            getAudioLanguages();
    void            onLoadPlaylist(const QString& playlist, bool end_of_playlist = false);
    void            loadPlaylist(const QString& playlist);
    void            loadPlaylistPart(const QString& playlist);

private:
    static bool isInDownloadQueue(const VideoHandle* video) {
        return (video->isPending() || (video->isDownloading() && !video->isPaused()));
    }

    //private;
    void        removeFromDownloads(VideoHandle* video) {
        //if (isInDownloadQueue(video)) 
        {
            auto pos = downloads_.remove(video);
            if (pos >= 0 && pos < max_downloads_) {
                if (auto v = downloads_.get(max_downloads_ - 1)) {
                    emit aboutToDownload(v);
                    v->doDownload();
                }
            }
            adjustCPUUsage();
        }
    }

    void        download(VideoHandle* video) {
        if (video->isDownloadable()) {
           
            //if (!downloads_.contains(video)) {
            if (!isInDownloadQueue(video)) {
                auto pos = downloads_.insert(video->priority(), video);
                if (pos < max_downloads_) {
                    if (auto v = downloads_.get(max_downloads_)) {
                        emit aboutToPending(v);
                        v->doPending();
                    }
                    emit aboutToDownload(video);
                    video->doDownload();
                }
                else {
                    emit aboutToPending(video);
                    video->doPending();
                }
                adjustCPUUsage();
            }
           
        }
        else {
            //
            postponed_downloads_.insert(video);
        }
           
        
    }

    void        pause(VideoHandle* video) {
        if (pauseModeEnabled(video->portal())) {
            if (video->isDownloadable()) {
                if (isInDownloadQueue(video)) {
                    emit aboutToPause(video);
                    video->doPause();
                    removeFromDownloads(video);
                    /*auto pos = downloads_.remove(video);
                    if (pos >= 0 && pos < max_downloads_) {
                        if (auto v = downloads_.get(max_downloads_ - 1))
                            v->doDownload();
                    }*/
                }
            }
        }
    }

    void        cancel(VideoHandle* video){
                
        video->doCancel();
        removeVideoInProcess(video);
        
        if (video->isDownloadable()) {
            videos_.removeOne(video);
               
            removeFromDownloads(video);
        }
        else {
            video_queue_.removeOne(video);
            postponed_downloads_.remove(video);
        }
		playlist_queue_.removeOne(video);
        //video->doCancel();
    }

    void        handleFinished(VideoHandle* video) {

        if (video->isFinished()) {
            videos_.removeOne(video);
               
            //removeFromDownloads(video);

            auto pos = downloads_.remove(video);

            emit finished(video);

            if (pos >= 0
                && pos < max_downloads_) {
                if (auto v = downloads_.get(max_downloads_ - 1)) {
                    emit aboutToDownload(v);
                    v->doDownload();
                }
            }

            if (!isDownloading())
                emit finisheditems();

            adjustCPUUsage();
        }
    }

    void        handleVideoError(VideoHandle* video) {
        removeFromDownloads(video);
        qDebug() << "Video Error: " << video->status() << " :: " << video->status().message();
        status_ = video->status();
        emit error();
    }

    void        restart(VideoHandle* video) {
        if (isInDownloadQueue(video)) {
            auto prev_pos = downloads_.remove(video);

            if (prev_pos >= 0) {
                auto next_pos = downloads_.insert(video->priority(), video);

                if (prev_pos < max_downloads_ && next_pos >= max_downloads_) {
                    emit aboutToPending(video);
                    video->doPending();
                    if (auto v = downloads_.get(max_downloads_ - 1)) {
                        emit aboutToDownload(v);
                        v->doDownload();
                    }
                }
                if (prev_pos >= max_downloads_ && next_pos < max_downloads_) {
                    if (auto v = downloads_.get(max_downloads_)) {
                        emit aboutToPending(v);
                        v->doPending();
                    }
                    emit aboutToDownload(video);
                    video->doDownload();
                }
            }
            adjustCPUUsage();
        }
    }

    void        setStayAwake(const bool stay_awake, const bool display_required = false) {
        if (stay_awake) {
            if (display_required)
                SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
            else
                SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);
        }
        else {
            SetThreadExecutionState(ES_CONTINUOUS);
        }
    }

    void        adjustCPUUsage() {
        bool stay_awake = false;
        bool display_required = false;

        if (downloads_.size() > 0) {
            auto lst = downloads_.get(0, downloads_.size());
            int active_downloads = 0;
            int postponed_downloads = 0;

            for (auto v : lst) {
                if (v->isDownloading())
                    active_downloads++;
                if (v->isPaused())
                    postponed_downloads++;
            }

            if (active_downloads > 0
                || postponed_downloads > 0) {

                stay_awake = true;

                for (auto v : lst) {
                    if (v->portal() == EPortal::kAmazon
                        || v->portal() == EPortal::kHBO)
                        display_required = true;

                    if (active_downloads > 0)
                        v->setMaximumCPUUsage(DEFAULT_MAXIMUM_CPU_USAGE / active_downloads);
                }
            }
        }

        setStayAwake(stay_awake, display_required);
    }

public:
    void        saveOutputOnEsit() {
        if (downloads_.size() > 0) {
            auto lst = downloads_.get(0, downloads_.size());
            int active_downloads = 0;
            for (auto v : lst) {
                if (v->isDownloading())
                    v->saveOutputOnExit(true);
            }
        }
    }

    void        setDownloadThreads(int count) {
        int new_downloads = qMin(count, downloads_.size());
        int old_downloads = qMin(max_downloads_, downloads_.size());

        if (new_downloads > old_downloads) {
            auto lst = downloads_.get(old_downloads, new_downloads - old_downloads);
            for (auto v : lst) {
                emit aboutToDownload(v);
                v->doDownload();
            }
        }
        else if (new_downloads < old_downloads) {
            auto lst = downloads_.get(new_downloads, old_downloads - new_downloads);
            for (auto v : lst) {
                emit aboutToPending(v);
                v->doPending();
            }
        }

        adjustCPUUsage();

        max_downloads_ = count;
    }

    int         downloadThreads() const { return max_downloads_; }

    void        downloadAll() {
        //Postponed download set;
        for (auto v : video_queue_) {
            postponed_downloads_.insert(v);
        }

        if (video_in_process_)
            postponed_downloads_.insert(video_in_process_);

        int last_downloading_pos = qMin(downloads_.size(), max_downloads_);

        for (auto v : videos_) {
            if (!isInDownloadQueue(v)) {
                auto pos = downloads_.insert(v->priority(), v);

                if (pos >= 0) {
                    if (pos < max_downloads_) {
                        emit aboutToDownload(v);
                        v->doDownload();
                        last_downloading_pos++;
                    }
                    else {
                        emit aboutToPending(v);
                        v->doPending();
                    }
                   
                }
            }
        }

        if (last_downloading_pos > max_downloads_) {
            auto pendings = downloads_.get(max_downloads_
                , last_downloading_pos - max_downloads_);

            for (auto v : pendings) {
                emit aboutToPending(v);
                v->doPending();
            }
        }

        adjustCPUUsage();
    }

    void        pauseAll() {
        auto lst = downloads_.get(0, downloads_.size());

        for (auto v : lst) {
            if (pauseModeEnabled(v->portal())) {
                pause(v);
            }
        }
    }

    void        cancelAll() {
        
        for (auto v : videos_) {
            v->doCancel();
            removeVideoInProcess(v);
        }

        for (auto v : video_queue_)
            v->doCancel();

        if (video_in_process_) {
            video_in_process_->doCancel();
            removeVideoInProcess(video_in_process_);
        }

        postponed_downloads_.clear();
        videos_.clear();
        video_queue_.clear();
		playlist_queue_.clear();
        //video_in_process_ = nullptr;
        
    }
    void        download(const QList<VideoHandle*>& list) {
        int last_downloading_pos = qMin(downloads_.size(), max_downloads_);
        for (auto v : list) {

            if (video_in_process_ == v)
                postponed_downloads_.insert(v);
            else if (video_queue_.contains(v))
                postponed_downloads_.insert(v);
            else if (videos_.contains(v)) {
                auto pos = downloads_.insert(v->priority(), v);
                if (pos >= 0) {
                    if (pos < max_downloads_) {
                        emit aboutToDownload(v);
                        v->doDownload();
                        last_downloading_pos++;
                    }
                    else {
                        emit aboutToPending(v);
                        v->doPending();
                    }
                }
            }
        }

        if (last_downloading_pos > max_downloads_) {
            auto pendings = downloads_.get(max_downloads_
                , last_downloading_pos - max_downloads_);

            for (auto v : pendings) {
                emit aboutToPending(v);
                v->doPending();
            }
        }

        adjustCPUUsage();
    }

    void        pause(const QList<VideoHandle*>& list) {

        int last_downloading_pos = qMin(downloads_.size(), max_downloads_);

        for (auto v : list) {
            if (pauseModeEnabled(v->portal())
                && !postponed_downloads_.remove(v)) {

                auto pos = downloads_.remove(v);
                if (pos >= 0 && pos < last_downloading_pos) {
                    emit aboutToPause(v);
                    v->doPause();
                    last_downloading_pos--;
                }
            }

        }

        if (last_downloading_pos < max_downloads_) {
            auto downloadings = downloads_.get(last_downloading_pos
                , max_downloads_ - last_downloading_pos);

            for (auto v : downloadings) {
                emit aboutToDownload(v);
                v->doDownload();
            }
        }

        adjustCPUUsage();
    }

    void        cancel(const QList<VideoHandle*>& list) {
        int last_downloading_pos = qMin(downloads_.size(), max_downloads_);
        if (!list.isEmpty()) {
            for (auto v : list) {
				if (!playlist_queue_.isEmpty())
					playlist_queue_.removeOne(v);
                if (!video_queue_.empty()
                    && video_queue_.removeOne(v)
                    && !postponed_downloads_.empty()) {
                    postponed_downloads_.remove(v);
                }
                else if (!videos_.empty()
                    && videos_.removeOne(v)
                    && downloads_.size() > 0) {
                    auto pos = downloads_.remove(v);
                    if (pos >= 0 && pos < last_downloading_pos)
                        last_downloading_pos--;
                }
            }

            for (auto v : list) {
                v->doCancel();
                removeVideoInProcess(v);
            }

            if (last_downloading_pos < max_downloads_
                && downloads_.size() > 0) {
                auto downloadings = downloads_.get(last_downloading_pos
                    , max_downloads_ - last_downloading_pos);

                for (auto v : downloadings) {
                    emit aboutToDownload(v);
                    v->doDownload();
                }
            }

            adjustCPUUsage();
        }
    }

    void        setSetting(const QString& data) { web_page_->setSetting(data); }
    void        setAudioLanguages(const QList<QString>& selected_languages) { web_page_->setAudioLanguages(selected_languages); }

private:
    QQueue<VideoHandle*>                video_queue_;
	QQueue<VideoHandle*>				playlist_queue_;

    WebEngine*                          web_engine_ = nullptr;
 
    QTimer                              watchdog_;
    int                                 retry_load_interval_ = 0;
    VideoHandle*                        video_in_process_ = nullptr;
    int                                 load_trying_ = 0;

    WebPage*                            web_page_ = nullptr;
    //todo: QPointer<PrivateKeysClient>         private_keys_ = nullptr;

private:
    //Download video immediately after parse;
    QSet<VideoHandle*>                  postponed_downloads_;
 
    QList<VideoHandle*>                 videos_;
    PriorityList<VideoHandle*>          downloads_;
    int                                 max_downloads_ = 1;
    
    Status                              status_;
    bool                                activated_ = false;
    bool                                show_minimized_ = false;

    QWebEngineProfile*                  profile_ = nullptr;

private:
    QUrl                                correctUrl(const EPortal& portal, const QUrl& url);
    bool                                urlRecognize(const QList<QRegularExpression>& url_regexp_list, const QUrl& url);
    void                                recognizeUrl(const QUrl& url, EPortal& portal, bool& playlist_url);
};