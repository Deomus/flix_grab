#pragma once

#include <map>
#include <string>

#include <QtCore>
#include <QtWebEngineCore>
#include <QtWebEngineWidgets>
//#include <QDomDocument>

#include "video_info.h"
#include "tracks_info.h"
#include "status.h"
#include "abortable.h"

#define USER_AGENT_WEB "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.157 Safari/537.36"
#define USER_AGENT_ANDROID "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/65.0.3325.181 Mobile Safari/537.36"  
#define USER_AGENT_ANDROID_APP "com.google.android.youtube/19.44.38 (Linux; U; Android 11) gzip"
#define USER_AGENT_IOS "Mozilla/5.0 (iPad; CPU OS 16_7_10 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.6 Mobile/15E148 Safari/604.1,gzip(gfe)"
#define USER_AGENT_IOS_APP "com.google.ios.youtube/19.45.4 (iPhone16,2; U; CPU iOS 18_1_0 like Mac OS X;)"
#define USER_AGENT_TV_HTML5 "Mozilla/5.0 (ChromiumStylePlatform) Cobalt/Version"

enum class YtContentKind { //!!! change also VideoControl::playlistSubname
    kVIDEO = 0,
    kPLAYLIST,
    kCHANNEL,
    kSHORTS,
    kVIDEOS,
    kCOMMUNITY,
    kEMPTYPLAYLIST,
    kEMPTYCHANNEL,
    kEMPTYSHORTS,
    kEMPTYVIDEOS,
    kEMPTYCOMMUNITY
};

enum class EVideoIdType {
    kVideoId = 0,
    kPlaylistId,
    kChannelId,
    kShortId,
    kCommunityId
};

enum class ERequestDataType {
    kAndroid = 0,
    kIos,
    kTvHTML5Embedded,
    kTVHTML5,
    kMWeb,
    kWeb
};

struct playlistVideoData {
    QString video_id;
    QString video_title;
    QString video_duration;
};

struct possibleVideoQuality {
    int video_height;
    int video_framerate;
    QString itag;
    VideoProfile profile;
    bool contains_audio;

    possibleVideoQuality(int video_height, int video_framerate, QString itag, VideoProfile profile, bool contains_audio = true) {
        this->video_height = video_height;
        this->video_framerate = video_framerate;
        this->itag = itag;
        this->profile = profile;
        this->contains_audio = contains_audio;
    }
};

struct videoQuality {
    int video_height;
    int video_framerate;
    qint64 video_size;
    VideoProfile profile;
    QString itag;

    videoQuality() {
        video_height = 0;
        video_framerate = 30;
        video_size = 0;
        profile = VideoProfile::AVC;
        QString itag;
    }
};

struct possibleAudioQuality {
    QString itag;
    AudioProfile profile;

    possibleAudioQuality(QString itag, AudioProfile profile) {
        this->itag = itag;
        this->profile = profile;
    }
};

struct audioQuality {
    qint64 audio_size;
    int audio_bitrate;
    AudioProfile profile;
    QString itag;
    QString lang_tag;

    audioQuality() {
        audio_size = 0;
        audio_bitrate = 0;
        profile = AudioProfile::AAC;
        itag;
        lang_tag;
    }
};

struct jsMethod {
    QString name;
    QString code;
};

struct MimeType { //! type: video or audio
    QString type;
    VideoProfile profile;
    QStringList codecs;
};

struct requestData {
	int clientId;
    QString clientName;
    QString clientVersion;
    QString userAgent;

    requestData(int clientId, const QString& client_name, const QString& client_version, const QString& user_agent) :
		clientId(clientId), clientName(client_name), clientVersion(client_version), userAgent(user_agent) { }
};

class YoutubeParser : public QObject
{
    Q_OBJECT
public:
    YoutubeParser(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* parent = 0);

    Status                          parseHTML(const QString& html_in, const QString& url, VideoInfo& info, TracksInfo& tracks_info,
                                        QList < QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>>& playlists_data, 
										Abortable* abortable);
    void	                        getPlaylistOrChannel(VideoInfo& info,
                                        QList < QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>>& playlists_data, Abortable* abortable);

signals:
    void                            loadPlaylist(const QString&);
    void                            loadPlaylistPart(const QString&);

private:
    Status                          parseVideoPage(const QString& html, VideoInfo& video_info);

private:
    bool                            requestBrowse(const QString extraBody, QString& response, Abortable* abortable);
    void                            getPlaylist(const QString& playlist_id,
                                        QList<QPair<YtContentKind,QList<QPair<QString,QList<QPair<QString,QString>>>>>>& playlists_data,
                                        VideoInfo& video_info, Abortable* abortable);
    void                            parseChannels(const QString& json, QStringList& channel_ids);
    void                            getChannel(const YtContentKind& content_kind, const QString& channel_id,
                                        QList<QPair<YtContentKind,QList<QPair<QString,QList<QPair<QString,QString>>>>>>& channels_data,
                                        VideoInfo& video_info, Abortable* abortable);
    void                            parseContinuation(const QString& json, QString& continuation);
    void                            parseVideosAndCont(const QString& json, QList<QPair<QString, QString>>& episodes,
                                        QString& continuation, QString playlist_id = "");
    void                            parseContinuationVideosAndCont(const QString& json, QList<QPair<QString, QString>>& episodes,
                                        QString& continuation, QString playlist_id = "");


private:
    MimeType                        getMimeType(const QString& mime_type, bool* ok);

    QString                         getVideoId(const QString& html, const QString& url);
    QString                         getPlaylistId(const QString& html, const QString& url);
    QString                         getChannelId(const QString& html, const QString& url, const QString& json_video_info, Abortable* abortable);

    QString                         getChannelIdFromJsonVideoInfo(const QString& json_video_info);

    QString                         getVideoIdFromHTML(const QString& html);
    QString                         getPlaylistIdFromHTML(const QString& html);
    QString                         getChannelIdFromHTML(const QString& html);

    QString                         getVideoIdFromURL(const QString& link);
    QString                         getPlaylistIdFromURL(const QString& link);
    QString                         getChannelIdFromURL(const QString& link, Abortable* abortable);

    bool                            checkHighVideoQualityExists(TracksInfo& tracks_info);
    Status                          parseTracksFromJson(QString& youtube_video_info,
                                        VideoInfo& video_info, TracksInfo& tracks_info, Abortable* abortable);
    QString                         downloadVideoInfo(requestData& req_data, Abortable* abortable);
    QString                         getURLUsingSignatureScript(const QJsonObject& format, bool& build_ranges, const Abortable* abortable);
    bool							updateURLNParam(QString& url, const Abortable* abortable);
    bool                            parseVideoTrackInfo(const QJsonObject& format, const QString& parentJsonPath,
                                        const QList<possibleVideoQuality>& qualities, QList<videoQuality>& added_qualities,
                                        VideoTrackInfo& video_track, const Abortable* abortable);
    bool                            parseAudioTrackInfo(const QJsonObject& format, const QString& parentJsonPath,
                                        const QList<possibleAudioQuality>& qualities, QList<audioQuality>& added_qualities,
                                        AudioTrackInfo& audio_track, const Abortable* abortable);
    bool                            parseFormats(const QJsonArray& formats, const QString& jsonParentPath,
                                        const QList<possibleVideoQuality>& video_qualities, const QList<possibleAudioQuality>& audio_qualities,
                                        QList<videoQuality>& added_video_qualities, QList<audioQuality>& added_audio_qualities,
                                        TracksInfo& tracks_info, const Abortable* abortable);
    bool                            parseCaptionTracks(const QJsonArray& captionTracks, const QString& jsonParentPath, TracksInfo& tracks_info);
    bool                            parseTracksInfo(const QJsonObject& info_json, TracksInfo& video_info, const Abortable* abortable);
    bool                            parseVideoInfo(const QJsonObject& info_json, VideoInfo& video_info);
    bool                            parseJsonInfo(const QString& info, VideoInfo& video_info, TracksInfo& tracks_info, const Abortable* abortable);

private:
    void                            parseSignatureJS(const QString& js, QString& signature_method, QList<jsMethod>& js_methods);
    void                            parseNJS(const QString& js, QString& n_method, QList<jsMethod>& js_methods);
    void                            patchNJS(const QString& n_method, QList<jsMethod>& js_methods);
	void							extractSignatureTimestamp(const QString& js);
    //void                            parseMpegDash(const QString& xml, QList<QStringList>& dash_quality_links);
    QString                         runSyncScript(const QString& script, const Abortable* abortable);

private:
    void                            extractJSMethod(const QString& name, const QString& js, QList<jsMethod>& js_methods);
    QString                         getContentQualityUrl(const QStringList& quality_list, const QString& tag);
    QString                         getContenDashQualityUrl(const QList<QStringList>& dash_quality_links, const QString& tag);
    QString                         getUrlFromQualityLink(const QString& link, const Abortable* abortable);
    QString                         getLinkTag(const QString& link);
    int                             getTagAudioBitrate(const int tag);
    qint64                          getLinkQualitySize(const QString& link);
    QString                         getLinkQualityLang(const QString& link);
    int                             getLinkQualityBitrate(const QString& link);
    int                             getLinkQualityFramerate(const QString& link, int current_value = 30);
    bool                            getLinkQualityHDR(const QString& link);
    VideoProfile                    getVideoProfile(const QString& type);
    QString                         getVideoFormatDesk(const VideoProfile type);
    QString                         getAudioFormatDesk(const AudioProfile type);

private:
    bool                            videoQualityExists(QList<videoQuality>& added_qualities, videoQuality& newQuality);
    bool                            audioQualityExists(QList<audioQuality>& added_qualities, audioQuality& new_quality);
    void                            setJSSignatureScripts(const QString& html, Abortable* abortable);
    QString                         getJSPlayerURL(const QString& html);
    Status                          mainParsePage(  const QString& html,
                                                    QString& js_data_type,
                                                    QString& signature_method,
                                                    QList<QStringList>& dash_quality_links,
                                                    const QString& received_data_type,
                                                    QString& mpeg_dash_data_type,
                                                    QMap<QString, QString>& download_required,
                                                    QString& youtubePlayerUrl,
                                                    QString& youtubePlayerJS,
                                                    QString& youtubePlayerSignatureMethodName,
                                                    QString& html5PlayerUrl,
                                                    const QString& player = "");

    void                            createVideoTrack(const QString& video_data, const QString& video_link, const possibleVideoQuality& quality, QList<videoQuality>& added_qualities, TracksInfo& tracks_info);
    void                            createAudioTrack(const QString& audio_data, const QString& audio_link, const possibleAudioQuality& quality, QList<audioQuality>& added_qualities, TracksInfo& tracks_info);
    Status                          getTracksData(const QString& html, TracksInfo& tracks_info, VideoInfo& video_info, Abortable* abortable);
    void                            getFormats(const QString& html, QJsonArray& formats);
    void                            getYouTubeVideoInfo(const QString& html, const QString& player, QString& video_info, Abortable* abortable);
    void                            fill_video_quality_list(QList<possibleVideoQuality> &video_qualities);
    void                            fill_avc_video_quality_list(QList<possibleVideoQuality>& video_qualities);
    void                            fill_webm_video_quality_list(QList<possibleVideoQuality>& video_qualities);
    void                            fill_av1_video_quality_list(QList<possibleVideoQuality>& video_qualities);
    void                            fill_avc_av1_audio_quality_list(QList<possibleAudioQuality> &audio_qualities);
    void                            fill_webm_audio_quality_list(QList<possibleAudioQuality>& audio_qualities);

    //int                             getServerSignature(std::string id, std::string& time_stamp); 
    //int                             getByTimeStamp(const std::string time_stamp, std::string& code) const;    
    //int                             parse(std::string sDyndata); 

    //int                             parseJSPlayer(const std::string& page, const std::string &player, std::string& sTimeStamp, std::vector<VideoTrackInfo> &vqDefs);
    //int                             parseFmtMap(bool & bUseCipherSignature, std::vector<VideoTrackInfo>& vqDefs, const std::string& sTimeStamp, const std::string& sMap, const char *pszTokenizer, bool AddHeaders, const std::string &page, const std::string &player);
    //int                             parseFmtMapEntry(VideoTrackInfo &vqDef, const std::string& sTimeStamp, const std::string& sFmt, bool bUseCipherSignature, const char *tokenizeBy, const std::string &page, const std::string &player);
    //int                             dp_ds(const char *pszTimeStamp, const char *psz, char *pszOut, size_t nBufSize, const std::string &page);
private:
    void                            extractVideosAndContPlaylist(QJsonArray contents, QList<playlistVideoData>* video_data, QString* continuation);
    void                            getPlaylist(QString playlist_id, QList<playlistVideoData>* video_data, QString* continuation, Abortable* abortable);
    void                            getPlaylistContinuation(QString continuation, QList<playlistVideoData>* video_data, QString* newContinuation, Abortable* abortable);
    void                            extractVideosAndContChannel(QJsonArray contents, QStringList* video_ids, QString* continuation);
    void                            getChannel(QString channel_id, QStringList* video_ids, QString* continuation, Abortable* abortable);
    void                            getChannelContinuation(QString continuation, QStringList* video_ids, QString* newContinuation, Abortable* abortable);
    void                            extractVideosAndContQuery(QJsonArray contents, QStringList* video_ids, QString* continuation);
    void                            getQuery(QString query, QStringList* video_ids, QString* continuation, Abortable* abortable);
    void                            getQueryContinuation(QString continuation, QStringList* video_ids, QString* newContinuation, Abortable* abortable);
    
    Status                          getChannelChannels(const QString& channel_id, QStringList& channel_ids, Abortable* abortable);
    Status                          downloadPlaylist(const QString& playlist_id, VideoInfo& video_info, bool fill_playlist, Abortable* abortable);
    Status                          downloadVideos(const QString& channel_id, Abortable* abortable);
    Status                          downloadSearch(const QString& url, Abortable* abortable);
    Status                          downloadShorts(const QString& channel_id, Abortable* abortable);
    Status                          downloadStreams(const QString& channel_id, Abortable* abortable);
    Status                          downloadPlaylists(const QString& channel_id, Abortable* abortable);
    Status                          downloadFeatured(const QString& channel_id, Abortable* abortable);
    //Status                          downloadCommunity(const QString& channel_id);

    void                            addDefaultHeaders(QList<QPair<QByteArray, QByteArray>>& headers);
    void                            addDefaultQuery(QList<QPair<QByteArray, QByteArray>>& url_query_params);


    QString                         getVisitorData(QNetworkAccessManager* mgr);
    QJsonObject                     find_tab(const QJsonArray& tabs, const QString& url_suffix);
    QString                         get_endpoint_param(QJsonObject tab);

    void                            extractShorts(QJsonArray contents, QString* continuation, QStringList* video_ids);
    void                            getShorts(const QString& channel_id, QString endpoint_param, const QJsonObject& ctx, QString* continuation, QStringList* video_ids, Abortable* abortable);
    void                            getShortsContinuation(QString cont, const QJsonObject& ctx, QString* continuation, QStringList* video_ids, Abortable* abortable);

    void                            extractStreams(QJsonArray contents, QString* continuation, QStringList* videos);
    void                            getStreams(const QString& channel_id, const QString& endpoint_param, const QJsonObject& ctx, QString* continuation, QStringList* video_ids, Abortable* abortable);
    void                            getStreamsContinuation(const QString& cont, const QJsonObject& ctx, QString* continuation, QStringList* video_ids, Abortable* abortable);

    void                            extractPlaylists(QJsonArray contents, QString* continuation, QStringList* video_ids);
    void                            getPlaylists(QString channel_id, QString endpoint_param, QJsonObject ctx, QString* continuation, QStringList* video_ids, Abortable* abortable);
    void                            getPlaylistsContinuation(QString cont, QJsonObject ctx, QString* continuation, QStringList* video_ids, Abortable* abortable);

    void                            extractContent(QJsonObject o, QMap<QString, YtContentKind>& map);
    void                            extractShelf(QJsonObject o, QMap<QString, YtContentKind>& map);
    void                            extractFeaturedShelves(QJsonArray shelvesJson, QMap<QString, YtContentKind>& map);
    void                            getFeatured(QString channel_id, QString endpoint_param, QJsonObject ctx, QMap<QString, YtContentKind>& map, Abortable* abortable);

    //void                            extractCommunity(QJsonArray contents, QString* continuation, QStringList* video_ids);
    //void                            getCommunity(QString channel_id, QString endpoint_param, QJsonObject ctx, QString* continuation, QStringList* video_ids);
    //void                            getCommunityContinuation(QString cont, const QJsonObject& ctx, QString* continuation, QStringList* links);

    QJsonObject                     generateCtx(Abortable* abortable);
    QJsonArray                      getTabs(const QJsonObject& ctx, const QString& channel_id, Abortable* abortable);
    QString                         getVisitorData(Abortable* abortable);

    void                            loadPlaylist(const EVideoIdType type, QStringList& video_ids, bool end_of_playlist = false);
    QString                         rangeUrlQuery(qint64 start, qint64 end);
    QStringList                     buildRanges(const QString& url, quint64 size);
    bool                            enableMetadata(const QString& title);
    QString                         getContentParam(const YtContentKind& content_kind = YtContentKind::kCHANNEL);
    requestData                     getRequestData(const ERequestDataType& type);

private:
    QNetworkAccessManager*          nam_ = nullptr;
    QWebEnginePage*                 page_ = nullptr;
	QSettings						cache_;
    QString                         video_id_;
    QString                         js_signature_script_;
    QString                         js_n_script_;
    QString                         signature_timestamp_;
    unsigned long long              chunk_size_ = (10 << 20) - 1; //< No more than 10MB, otherwise throttling
    QList<SubtitleTrackInfo::SubtitleProfile> youtube_tag_formats_ = { SubtitleTrackInfo::SubtitleProfile::WEBVTT/*, SubtitleTrackInfo::SubtitleProfile::SRV1, SubtitleTrackInfo::SubtitleProfile::SRV2, SubtitleTrackInfo::SubtitleProfile::SRV3, SubtitleTrackInfo::SubtitleProfile::SimpleSdh*/};
};
