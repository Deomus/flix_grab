#pragma once

#include <common_config.h>
#include <product_config.h>

#include <QRecursiveMutex>

#include <presets.h>
#include <exec.h>
#ifdef DECRYPTSERVICE
#include <service_callback.h>
#endif

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"
#include "get_tags.h"

namespace recode {
    class PartedOp;
}


#ifdef DECRYPTSERVICE
class DecryptController : public QObject, public recode::ExecListener, public ServiceCallback {
#else
class DecryptController : public QObject, public recode::ExecListener {
#endif
    Q_OBJECT

enum EOperationType {
        kDownload,
        kDownloadSubtitles,
        kRecode,
        kThumbnail,
        kSave,
};

public:
    DecryptController(QObject* parent);

    virtual ~DecryptController();

signals:
    //Ready for decryption;
    void                        ready();
    void                        offline();

    void                        progressChanged(float value);
    
    void                        paused();
    void                        resumed();
    void                        cancelled();
    void                        finished();

    void                        error();
    
public:

    bool                        download(const QList<const VideoTrackInfo*>& video_tracks, const QList<const AudioTrackInfo*>& audio_tracks,
                                const QList <const SubtitleTrackInfo*>& subtitle_tracks, const QList<const ImageTrackInfo*>& image_tracks,
                                const QList<int> save_as_ids, const QList<int> saveas_subtitle_ids, const VideoInfo& info);

    bool                        cancel(bool wait_decrypt_service = true);

    bool                        pause();
    bool                        resume();
    //restrictions
    void                        setSlowdown(const float factor);
    void                        setLimitDuration(const int limit_duration);
    
    void                        setEncodingPreset(const QString& encoding_preset) { encoding_preset_ = encoding_preset; }
    void                        setMaximumCPUUsage(float factor);
    void                        saveOutputOnExit(bool save_output = false) { save_output_ = save_output; }

public:
    const QString&              mediaOutput() { return media_file_; }
    void                        setMediaOutput(const QString& media_file);
    void                        setSubtitleOutput(const QString& file_path);
    void                        setImageOutput(const QString& file_path);
    void                        setPortal(const EPortal& portal);
    void                        decryptRequired(bool req_decrypt) { req_decrypt_ = req_decrypt; }

public:
    float                       progress() const { return progress_; }
    bool                        isDownloading() const { return downloading_; }
#ifdef DECRYPTSERVICE
    bool                        isReady() const { return service_ != nullptr; }
#endif
    bool                        isInitialized() const { return initialized_; }
    bool                        isFinished() const { return finished_; }
    bool                        isPaused() const { return paused_; }

    bool                        isCancelling() const { return cancelling_; }

    const Status&               status() const { return status_; }

public:
    QList<QString>              audio_file_names_;
    QList<QString>              subtitle_file_names_;
    QList<QString>              collection_file_names_;

protected:

    /// Called when there is a progress update.
    virtual void                OnProgress(recode::Operation* current, float value);
    virtual void                OnPaused(recode::Operation* current, bool is_paused);
    virtual void                OnCancelled(recode::Operation* current);
    virtual void                OnFinished(void);

    void                        OnError(recode::Operation* current, const recode::Status&) override;
#ifdef DECRYPTSERVICE
//Service Interface
    virtual void                OnNewService(AbstractDecryptService* service);
    virtual void                OnReleaseService(AbstractDecryptService* service);
#endif

private:
    void                        clearMediaFile();
    void                        clearTemp(QList<QStringList> tmp_lists);
    QString                     makeTemp(EOperationType type = kDownload, const QString& lang = "");
    static int                  calcTotalReceived(const QString& file, const long& expected_file_size, const QList<QString>& urls, long& from);
    bool                        enableMetadata(const QString& str);
    void                        addDecryptOperation(const std::shared_ptr<recode::PartedOp>& op, const QString download_file, const QString download_file_recoded, int duration, int limit_duration, const QList<QString>& keys);
    void                        addTags();

private:
    Status                                      status_;
    QRecursiveMutex                             mutex_;
    std::unique_ptr<recode::Exec>               exec_;
#ifdef DECRYPTSERVICE
    AbstractDecryptService*                     service_ = nullptr;
#endif
    VideoInfo                                   video_info_;
    TagsInfo                                    tags_info_;
    float                                       progress_ = 0;

    bool                                        initialized_ = false;
    bool                                        downloading_ = false;
    bool                                        cancelling_ = false;
    bool                                        finished_ = false;
    bool                                        paused_ = true;

private:
    int                                         preset_ = kPreset_OriginalVideo;
    size_t                                      chunk_size_ = 2ULL << 20;
    size_t                                      image_chunk_size_ = 1ULL << 20;
    QElapsedTimer                               elapsed_;
    qint64                                      progress_timeout_ = 0;
    float                                       slowdown_ = 0;
    int                                         limit_duration_ = 0;
    qreal                                       maximum_cpu_usage_ = DEFAULT_MAXIMUM_CPU_USAGE;
    QString                                     encoding_preset_ = "superfast";
    bool                                        save_output_ = false;
    bool                                        req_decrypt_ = true;

private:
    QString                                     media_file_;
    QString                                     subtitle_file_;
    QString                                     image_file_;
    EPortal                                     portal_ = EPortal::kUnknown;
    QStringList                                 temps_download_;
    QStringList                                 temps_recode_;
    std::string                                 thumbnail_file_;
    bool                                        post_download_event_ = false;

private:
    std::string                                 getInstagramOutName(int i);

private:
    Q_DISABLE_COPY(DecryptController);
};


