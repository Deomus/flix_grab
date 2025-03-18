#pragma once

#include <QtCore>
#include <QtWebEngineWidgets>
#include <QtWebEngineCore>

#include <portals.h>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"
#include "abortable.h"


class Getflix;
class DecryptController;

enum EPostDownloadAction {
    kDoNothing = 0,
    kPlaySound = 1,
    kShutdownPC = 2,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EPostDownloadAction)

enum EEncodingSelector {
    kEncodingSelector_Ultrafast = 0,
    kEncodingSelector_Superfast,
    kEncodingSelector_Veryfast,
    kEncodingSelector_Faster,
    kEncodingSelector_Medium,
    kEncodingSelector_Slow,
    kEncodingSelector_2_pass,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EEncodingSelector)

enum EVideoSelector {
    kVideoSelector_HighQuality = 0,
    kVideoSelector_Default,
    kVideoSelector_MinimumSize,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EVideoSelector)

enum EAudioSelector {
    kAudioSelector_HighQuality = 0,
    kAudioSelector_Default,
    kAudioSelector_MinimumSize,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EAudioSelector)

enum EImageSelector {
    kImageSelector_HighQuality = 0,
    kImageSelector_Default,
    kImageSelector_MinimumSize,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EImageSelector)

enum ESaveSubtitlesAs {
    kSaveSubtitlesAs_Embedded = 0,
    kSaveSubtitlesAs_SRT,
    kSaveSubtitlesAs_Original,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(ESaveSubtitlesAs)

enum EAudioTypeOption {
    kAudioType_Original = 0,
    kAudioType_AudioForBlind,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EAudioTypeOption)

enum EDownloadPriority {
    kDownloadPriority_High = 0,
    kDownloadPriority_Normal,
    kDownloadPriority_Low,
};
//Make available for QVariant;
Q_DECLARE_METATYPE(EDownloadPriority)

namespace net {
    class HttpDownloader;
}

class VideoHandle : public Abortable
{
    friend class Getflix;
    friend class DecryptController;
    friend class DownloadManager;
    
    Q_OBJECT

    enum EVideoFlags {
        kVideoFlag_NotReady     = 0,
        kVideoFlag_Tracks       = 0x1,
        kVideoFlag_Info         = 0x2,
        kVideoFlag_Decryptor    = 0x4,
        kVideoFlag_Playlist     = 0x8,

        kVideoFlag_Media        = kVideoFlag_Tracks | kVideoFlag_Info,
        kVideoFlag_Ready        = kVideoFlag_Media | kVideoFlag_Decryptor,
    };

public:
    explicit VideoHandle(const EPortal& portal, const QUrl& url, Getflix *parent);
    ~VideoHandle();

public:
    const EPortal&          portal() const { return portal_; }
    bool                    isDownloadable() { return ((video_flag_ & kVideoFlag_Ready) == kVideoFlag_Ready); }

//Output file name may be changed due to lock;
//Path to base name of file;
    void                    correctOutputName(const QString& output_dir, QString& output_name);
    void                    setOutputPath(const QString& output_media_path);
    void                    setOutputPath(const QString& output_dir, const QString& output_name, QString ext = "");
   
    QString                 outputName() const { return output_name_; }
    QString                 outputPath() const { return output_path_; }
    QString                 outputMediaPath() const { return output_media_path_; }
    //Remove all created files and locks;
    void                    deleteOutputs();
    
    //SaveAs ids;
    void                    setSaveAsIds(const QList<int>& ids);
    const QList<int>&       saveAsIds() const { return save_as_ids_; }

    //Video ids;
    void                    setVideoIds(const QList<int>& ids);
    const QList<int>&       videoIds() const { return video_ids_; }

    //Audio selector id;
    void                    setAudioSelector(const EAudioSelector& selector = EAudioSelector::kAudioSelector_Default);

    //Audio ids;
    void                    setAudioIds(const QList<int>& ids);
    const QList<int>&       audioIds() const { return audio_ids_; }

    //Audio language ids;
    void                    setAudioLanguageIds(const QList<int>& ids);
    const QList<int>&       audioLanguageIds() const { return audio_language_ids_; }

    //Subtitle ids;
    void                    setSubtitleIds(const QList<int>& ids);
    const QList<int>&       subtitleIds() const { return subtitle_ids_; }

    //Save as subtitle ids;
    void                    setSaveSubtitlesAsIds(const QList<int>& ids);
    const QList<int>&       saveSubtitlesAsIds() const { return saveas_subtitle_ids_; }

    //Image selector id;
    void                    setImageSelector(const EImageSelector& selector = EImageSelector::kImageSelector_Default);
    //Image ids;
    void                    setImageIds(const QList<int>& ids);
    const QList<int>&       imageIds() const { return image_ids_; }
    void                    sortImagesTracks() { tracks_info_.sortImagesTracks(); }

    void                    updateTracksInfo(const TracksInfo& tracks_info) { tracks_info_ = tracks_info; }
    const TracksInfo&       tracksInfo() const { return tracks_info_; }
    void                    sortAudioTracks() { tracks_info_.sortAudioTracks(); }
    void                    sortAudioLanguages() { tracks_info_.sortAudioLanguages(); }
    
    
    const VideoInfo&        videoInfo() const { return video_info_; }
    
    const QUrl&             url() const { return media_url_; }
    void                    updateItemUrl(const QString& url) { media_url_ = url; }
            
    const Status&           status() const { return status_; }

    EVideoFlags             flag() const { return (EVideoFlags)video_flag_; }

    void                    setPriority(int priority = kDownloadPriority_Normal);
    int                     priority() const { return download_priority_; }

    bool                    isCancelling() const { return cancelling_; }
    bool                    isPending() const { return pending_; }
    bool                    isErrorState() const { return errorsetting_; }
    bool                    isFinished() const;

    //Slowdown factor: 0 - no slowdown, 1 - 2x, 2- 3x ...
    void                    setSlowdownFactor(const bool activated);
    float                   slowdown() const { return slowdown_; }

    void                    setLimitDuration(const bool activated);
    int                     limitDuration() const { return limit_duration_; }

    void                    setEncodingSelector(int encoding_selector = kEncodingSelector_Ultrafast);

    void                    setMaximumCPUUsage(qreal factor);
    void                    saveOutputOnExit(bool save_output = false);

    void                    setDownloadPermission(bool download_allowed);
    
public slots:
    //Convenience function for downloading;
    void                    download();
    //Convenience function for pause;
    void                    pause();
    //Cancel convenience function;
    void                    cancel();
    float                   progress() const;

    bool                    isDownloading() const;
    bool                    isPaused() const;
        

signals:
    //Video is ready to download;
    void                    analyzing();
    void                    described();
    //Ready for download;
    void                    ready();
	void					analyzingPlaylist();
	void					playlistReady();
           
    void                    error();

    //Pending download
    void                    pending();
    
    //Starting download
    void                    starting();

    void                    progressChanged(float value);
    
    //Pause/resume signals;
    void                    pausing();
    void                    paused();
    void                    resumed();

    void                    cancelling();
    void                    cancelled();
    void                    finished();
   
    //Information signals:
    void                    saveAsIdChanged();
    void                    videoIdChanged();
    void                    audioIdChanged();
    void                    audioLanguageIdChanged();
    void                    subtitleIdChanged();
    void                    saveSubtitlesAsIdChanged();
    void                    imageIdChanged();
      
private slots:
    void                    handleTracksInfo(const TracksInfo& info);
    void                    handleVideoInfo(const VideoInfo& info);

private slots:

    void                    doDownload();
    void                    doPause();
    void                    doPending();
	void                    doCancel();
    
    void                    handleAnalyzing();
	void                    handleAnalyzingPlaylist() { emit analyzingPlaylist(); }
  
    void                    setFlag(EVideoFlags flag);

    void                    setError(const Status& status);

private:
    bool                    downloadEnabled(QList<int>& saveas_ids, QList<const VideoTrackInfo*>& video_tracks,
                            QList<const AudioTrackInfo*>& audio_tracks, QList<const ImageTrackInfo*>& image_tracks,
                            QList<const SubtitleTrackInfo*>& subtitle_tracks);
    void                    addCompatibleAudioTrack(const VideoTrackInfo* video_track, QList<AudioTrackInfo*>& audio_tracks_transit, QList<const AudioTrackInfo*>& audio_tracks);
    
private:
    Getflix*                            getflix_ = nullptr;
    QUrl                                media_url_;

private:
    //Output stuff;
    //Path to base name of file;
    QString                             output_name_;
    QString                             output_path_;
    QString                             output_media_path_;
    //Output File Lock;
    QScopedPointer<QLockFile>           output_lock_;

private:
    DecryptController*                  downloader_ = nullptr;

    TracksInfo                          tracks_info_;
    VideoInfo                           video_info_;
    
    bool                                cancelling_ = false;
    bool                                pending_ = false;
    bool                                errorsetting_ = false;

    //Tracks Ids;
    QList<int>                          save_as_ids_;
    QList<int>                          video_ids_;
    QList<int>                          audio_ids_;
    QList<int>                          audio_language_ids_;
    QList<int>                          subtitle_ids_;
    QList<int>                          saveas_subtitle_ids_;
    QList<int>                          image_ids_;

    EAudioSelector                      audio_selector_ = EAudioSelector::kAudioSelector_Default;
    EImageSelector                      image_selector_ = EImageSelector::kImageSelector_Default;

    int                                 video_flag_ = EVideoFlags::kVideoFlag_NotReady;

private:
    Status                              status_;
    EPortal                             portal_ = EPortal::kUnknown;

    float                               slowdown_ = 0;
    int                                 limit_duration_ = 0;
    qreal                               maximum_cpu_usage_ = DEFAULT_MAXIMUM_CPU_USAGE;
    int                                 download_priority_ = kDownloadPriority_Normal;
    bool                                download_allowed_ = true;

private:
    Q_DISABLE_COPY(VideoHandle);

};
