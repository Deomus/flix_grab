#include "youtube_parser.h"

#include <array>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <languages.h>
#include <company_info.h>

#include "json_utils.h"
#include "get_utils.h"
#include "utils/qs_utils.h"
#include "utils/search_helpers.h"

#define     PLAYLIST_MAXIMUM_CYCLES      (1000)
#define     SEARCH_MAXIMUM_ITEMS        (1000)
#define     PLAYLISTS_MAXIMUM_ITEMS     (200)
#define		MAX_BOT_CIRCUMVENTION_ATTEMPTS	10
#define     YT_BASE                     "https://www.youtube.com/youtubei/v1/"
#define     YT_WATCH                    "https://www.youtube.com/watch?v="
#define     YT_PLAYLIST                 "https://www.youtube.com/playlist?list="
#define     YT_CHANNEL                  "https://www.youtube.com/channel/"
#define     YT_SHORTS                   "https://www.youtube.com/shorts/"
#define     YT_GET_VIDEO                "http://www.youtube.com/get_video?"

#define     CHANNEL_PLAYLISTS_PARAM     "EglwbGF5bGlzdHM%3D"
#define     CHANNEL_VIDEOS_PARAM        "EgZ2aWRlb3M%3D"
#define     CHANNEL_SHORTS_PARAM        "EgZzaG9ydHM%3D"
#define     CHANNEL_CHANNELS_PARAM      "EghjaGFubmVsc_IGBAoCUgA%3D"
#define     CHANNEL_COMMUNITY_PARAM     "Egljb21tdW5pdHnyBgQKAkoA"


#define CHECK_WITH_RETURN(ok, tag, RET) \
if(!ok) { \
    qDebug() << tag; \
    return RET; \
}

QString YoutubeParser::getContentParam(const YtContentKind& content_kind) {
    QString param;

    switch (content_kind) {
        case YtContentKind::kPLAYLIST:
            param = CHANNEL_PLAYLISTS_PARAM;
            break;
        case YtContentKind::kCHANNEL:
        case YtContentKind::kVIDEOS:
            param = CHANNEL_VIDEOS_PARAM;
            break;
        case YtContentKind::kSHORTS:
            param = CHANNEL_SHORTS_PARAM;
            break;
        case YtContentKind::kCOMMUNITY:
            param = CHANNEL_COMMUNITY_PARAM;
            break;
    }

    return param;
}

requestData YoutubeParser::getRequestData(const ERequestDataType& type) {
    switch (type) {
        case ERequestDataType::kAndroid:
            return requestData(                         //works fast (sometimes not get 1080p, 720p max) 
                3,
				"ANDROID",
                "19.44.38",
                USER_AGENT_ANDROID_APP
            );
        case ERequestDataType::kIos:
            return requestData(                                //works fast (sometimes not get 1080p, 720p max) (no webm formats)
                5,
				"IOS",
                "19.45.4",
                USER_AGENT_IOS_APP
            );
        case ERequestDataType::kTvHTML5Embedded:
            return requestData(                                //works slowly (always get 1080p) (add "isDrc": true quality)
                85,
				"TVHTML5_SIMPLY_EMBEDDED_PLAYER",
                "2.0",
                USER_AGENT_TV_HTML5
            );
        case ERequestDataType::kTVHTML5:
            return requestData(                                //works slowly (always get 1080p)
                7,
				"TVHTML5",
                "7.20250120.19.00",
                USER_AGENT_TV_HTML5
            );
        case ERequestDataType::kMWeb:
            return requestData(                                //works slowly (always get 1080p) (add very lower quality bitrate)
                2,
				"MWEB",
                "2.20241202.07.00",
                USER_AGENT_IOS
            );
        case ERequestDataType::kWeb:
            return requestData(                                //works slowly (always get 1080p)
                1,
				"WEB",
                "2.20241126.01.00",
                USER_AGENT_WEB
            );
    }
	throw std::exception("Invalid request data type.");
}

bool YoutubeParser::enableMetadata(const QString& title) {
    return (title.contains("music", Qt::CaseInsensitive)
        || title.contains("official", Qt::CaseInsensitive)
        || title.contains("(video)", Qt::CaseInsensitive)
        || title.contains("vevo", Qt::CaseInsensitive));
}

QString YoutubeParser::rangeUrlQuery(qint64 start, qint64 end) {
    return QString("&range=%1-%2").arg(start).arg(end);
};

QStringList YoutubeParser::buildRanges(const QString & url, quint64 size) {
    if (size <= 0)
        return QStringList(url);

    QStringList urls;
    for (qint64 start = 0; start < size;) {
        const auto remain = std::min(size - start, chunk_size_);
        urls.push_back(url + rangeUrlQuery(start, start + remain));
        start += remain + 1;
    }

    return urls;
}

void YoutubeParser::setJSSignatureScripts(const QString& html, Abortable* abortable) {
    QString js_player_url = getJSPlayerURL(html);
	if (js_player_url.isEmpty()) {
		return;
	}

	QString cached_js_player_url = cache_.value("player_url").toString();
	if (cached_js_player_url == js_player_url) {
		if (js_signature_script_.isEmpty()) {
			js_signature_script_ = cache_.value("js_signature_script").toString();
		}
		if (js_n_script_.isEmpty()) {
			js_n_script_ = cache_.value("js_n_script").toString();
		}
		if (signature_timestamp_.isEmpty()) {
			signature_timestamp_ = cache_.value("signature_timestamp").toString();
		}
		if (!js_signature_script_.isEmpty() && !js_n_script_.isEmpty() && !signature_timestamp_.isEmpty()) {
			return;
		}		
	}

    js_signature_script_.clear();
	js_n_script_.clear();
	signature_timestamp_.clear();

    QString js_player;
    downloadToString(js_player_url, js_player, abortable);

    if (!js_player.isEmpty()) {
        QString method_name;
        QList<jsMethod> js_methods;
        parseSignatureJS(js_player, method_name, js_methods);

        for (int i = 0; i < js_methods.length(); i++)
            js_signature_script_.append(js_methods.at(i).code).append(";");
        js_signature_script_.append(method_name);

		method_name.clear();
		js_methods.clear();
		parseNJS(js_player, method_name, js_methods);

        for (int i = 0; i < js_methods.length(); i++)
			js_n_script_.append(js_methods.at(i).code).append(";");
		js_n_script_.append(method_name);

		extractSignatureTimestamp(js_player);

		cache_.setValue("player_url", js_player_url);
		cache_.setValue("js_signature_script", js_signature_script_);
		cache_.setValue("js_n_script", js_n_script_);
		cache_.setValue("signature_timestamp", signature_timestamp_);
    }
}

namespace yt
{
    namespace internals
    {
        inline std::string  cloneSign(const std::string& sIn, int nBytes)
        {
            return sIn.substr(nBytes);
        }

        inline std::string  swapSign(const std::string& sIn, int nNum)
        {
            char cFirst = sIn[0];
            char cSecond = sIn[nNum % sIn.size()];
            std::string sOut = sIn;
            sOut[0] = cSecond;
            sOut[nNum] = cFirst;
            return sOut;
        }

        inline std::string  reverseSign(const std::string& sIn)
        {
            return std::string(sIn.rbegin(), sIn.rend());
        }
    }

    inline std::string decipherEx(const std::string& sIn, const std::string& sPseudoCode)
    {
        if (sPseudoCode.empty() || sIn.empty())
            return "";

        std::string sRet = sIn;

        std::vector<std::string> vL; //vLines
        str::tokenize<char>(sPseudoCode, "|", vL);

        for (size_t i = 0; i < vL.size(); ++i)
        {
            std::string s = vL[i]; // The current instruction
            if (s.find("ss") == 0) //swap numbered character with 0pos character
            {
                s.erase(0, 3);
                sRet = internals::swapSign(sRet, str::stringToInt(s));
            }
            else if (s.find("cs") == 0) //remove count of characters from start
            {
                s.erase(0, 3);
                sRet = internals::cloneSign(sRet, str::stringToInt(s));
            }
            else if (s.find("rs") == 0)
            {
                sRet = internals::reverseSign(sRet); //revers all characters
            }
        }

        return sRet;
    }

    inline std::string getTimeStamp(const std::string &sPage) {
        std::string sTimeStamp = "";
        std::string sMark = "\"STS\":";
        int nPos = sPage.find(sMark);
        if (nPos == -1) {
            sMark = ".sts=\"";
            nPos = sPage.find(sMark);
        }
        if (nPos != -1) {
            nPos += sMark.size();
            bool bStartTS = false;
            for (size_t i = 0; i < sPage.size() - nPos; i++) {
                char chr = sPage.at(nPos + i);
                if (str::isNumber(chr)) {
                    if (!bStartTS)
                        bStartTS = true;
                    sTimeStamp.push_back(chr);
                }
                else if (bStartTS) {
                    break;
                }
            }
        }
        return sTimeStamp;
    }
}


YoutubeParser::YoutubeParser(QWebEnginePage* page, QNetworkAccessManager* nam, QObject* parent /*= 0*/) : 
	cache_(COMPANY_NAME, "youtube_cache") {
    page_ = page;
    nam_ = nam;
    qRegisterMetaType<VideoInfo>();
}

bool YoutubeParser::videoQualityExists(QList<videoQuality>& added_qualities, videoQuality& new_quality) {
    QMutableListIterator<videoQuality> i(added_qualities);

    while (i.hasNext()) {
        videoQuality quality = i.next();

        if (new_quality.video_height == quality.video_height
            && new_quality.video_framerate == quality.video_framerate) {

            if (new_quality.profile == VideoProfile::AVC) {
                if (new_quality.video_size > 0
                    && quality.video_size > 0
                    && (abs(new_quality.video_size - quality.video_size) < 50 * 1024 * 1024)) //50 * 1024 * 1024 = 50mb

                return true;
            }
            else {
                return true;
            }
        }
    }

    return false;
}

bool YoutubeParser::audioQualityExists(QList<audioQuality>& added_qualities, audioQuality& new_quality) {
    QMutableListIterator<audioQuality> i(added_qualities);

    while (i.hasNext()) {
        audioQuality quality = i.next();

        if (new_quality.profile == quality.profile) {
            if (new_quality.audio_size == quality.audio_size 
                && new_quality.audio_bitrate == quality.audio_bitrate
                && new_quality.lang_tag == quality.lang_tag)
                return true;
        }
    }

    return false;
}

QString YoutubeParser::runSyncScript(const QString& script, const Abortable* abortable) {
    QString out;

	if (abortable && abortable->isAborted()) {
		return {};
	}

    if (page_ != nullptr) {
        if (QThread::currentThread() == qApp->thread()) {
            // Этот старый путь можно удалить, если парсер всегда будет вызываться
            // не из главного потока
            QEventLoop loop;
            page_->runJavaScript(script,
                [&](const QVariant &data) {
                out = data.toString();
                loop.quit();
            });
            loop.exec();
        }
        else {
            QMutex mutex;
            QWaitCondition cond;

            QMutexLocker lock(&mutex);
            QMetaObject::invokeMethod(qApp, [&] {// Для Qt <=5.10 можно заменить на QTimer::singleShot(0, qApp, [&] {
                page_->runJavaScript(script,
                    [&](const QVariant &data) {
                    QMutexLocker lock(&mutex);
                    out = data.toString();
                    cond.wakeOne();
                });
            });

			cond.wait(&mutex);
        }
    }

    return out;
}

void YoutubeParser::fill_avc_video_quality_list(QList<possibleVideoQuality>& video_qualities) {
    //avc
    VideoProfile profile = VideoProfile::AVC;
    video_qualities << possibleVideoQuality(144, 30, "160", profile, false);
    video_qualities << possibleVideoQuality(144, 30, "597", profile, false);

    video_qualities << possibleVideoQuality(240, 30, "5", profile);
    video_qualities << possibleVideoQuality(270, 30, "6", profile);
    video_qualities << possibleVideoQuality(240, 30, "133", profile, false);

    video_qualities << possibleVideoQuality(360, 30, "18", profile);            //contains audio
    video_qualities << possibleVideoQuality(360, 30, "34", profile);
    video_qualities << possibleVideoQuality(360, 30, "134", profile, false);

    video_qualities << possibleVideoQuality(480, 30, "35", profile);
    video_qualities << possibleVideoQuality(480, 30, "59", profile);
    video_qualities << possibleVideoQuality(480, 30, "78", profile);
    video_qualities << possibleVideoQuality(480, 30, "135", profile, false);
    video_qualities << possibleVideoQuality(480, 30, "212", profile, false);

    video_qualities << possibleVideoQuality(720, 30, "22", profile);            //contains audio
    video_qualities << possibleVideoQuality(720, 30, "136", profile, false);
    video_qualities << possibleVideoQuality(720, 60, "298", profile, false);

    video_qualities << possibleVideoQuality(1080, 30, "37", profile);
    video_qualities << possibleVideoQuality(1080, 30, "137", profile, false);
    video_qualities << possibleVideoQuality(1080, 60, "299", profile, false);

    video_qualities << possibleVideoQuality(1440, 30, "264", profile, false);

    video_qualities << possibleVideoQuality(2160, 30, "38", profile);
    video_qualities << possibleVideoQuality(2160, 30, "266", profile, false);

    video_qualities << possibleVideoQuality(3072, 30, "138", profile, false);         //!!!not checked
}

void YoutubeParser::fill_webm_video_quality_list(QList<possibleVideoQuality>& video_qualities) {
    VideoProfile profile = VideoProfile::WEBM;
    video_qualities << possibleVideoQuality(144, 30, "278", profile, false);
    video_qualities << possibleVideoQuality(144, 60, "330", profile, false);
    video_qualities << possibleVideoQuality(144, 30, "598", profile, false);

    video_qualities << possibleVideoQuality(240, 30, "242", profile, false);
    video_qualities << possibleVideoQuality(240, 60, "331", profile, false);

    video_qualities << possibleVideoQuality(360, 30, "43", profile);
    video_qualities << possibleVideoQuality(360, 30, "243", profile, false);
    video_qualities << possibleVideoQuality(360, 30, "167", profile, false);
    video_qualities << possibleVideoQuality(360, 60, "332", profile, false);

    video_qualities << possibleVideoQuality(480, 30, "44", profile);
    video_qualities << possibleVideoQuality(480, 30, "218", profile, false);
    video_qualities << possibleVideoQuality(480, 30, "219", profile, false);
    video_qualities << possibleVideoQuality(480, 30, "244", profile, false);
    video_qualities << possibleVideoQuality(480, 30, "245", profile, false);
    video_qualities << possibleVideoQuality(480, 30, "246", profile, false);
    video_qualities << possibleVideoQuality(480, 30, "168", profile, false);
    video_qualities << possibleVideoQuality(480, 60, "333", profile, false);

    video_qualities << possibleVideoQuality(720, 30, "45", profile);
    video_qualities << possibleVideoQuality(720, 30, "247", profile, false);
    video_qualities << possibleVideoQuality(720, 30, "169", profile, false);
    video_qualities << possibleVideoQuality(720, 60, "302", profile, false);
    video_qualities << possibleVideoQuality(720, 60, "334", profile, false);

    video_qualities << possibleVideoQuality(1080, 30, "46", profile);
    video_qualities << possibleVideoQuality(1080, 30, "248", profile, false);
    video_qualities << possibleVideoQuality(1080, 30, "170", profile, false);
    video_qualities << possibleVideoQuality(1080, 60, "303", profile, false);
    video_qualities << possibleVideoQuality(1080, 60, "335", profile, false);

    video_qualities << possibleVideoQuality(1440, 30, "271", profile, false);
    video_qualities << possibleVideoQuality(1440, 60, "308", profile, false);
    video_qualities << possibleVideoQuality(1440, 60, "336", profile, false);

    video_qualities << possibleVideoQuality(2160, 30, "313", profile, false);
    video_qualities << possibleVideoQuality(2160, 60, "315", profile, false);
    video_qualities << possibleVideoQuality(2160, 60, "337", profile, false);

    video_qualities << possibleVideoQuality(4320, 60, "272", profile, false);

}

void YoutubeParser::fill_av1_video_quality_list(QList<possibleVideoQuality>& video_qualities) {
    VideoProfile profile = VideoProfile::AV1; //!!!checked only with aac sound
    video_qualities << possibleVideoQuality(144, 30, "394", profile, false);
    video_qualities << possibleVideoQuality(144, 60, "694", profile, false);

    video_qualities << possibleVideoQuality(240, 30, "395", profile, false);
    video_qualities << possibleVideoQuality(240, 60, "695", profile, false);

    video_qualities << possibleVideoQuality(360, 30, "396", profile, false);
    video_qualities << possibleVideoQuality(360, 60, "696", profile, false);

    video_qualities << possibleVideoQuality(480, 30, "397", profile, false);
    video_qualities << possibleVideoQuality(480, 60, "697", profile, false);

    video_qualities << possibleVideoQuality(720, 60, "398", profile, false);
    video_qualities << possibleVideoQuality(720, 60, "698", profile, false);

    video_qualities << possibleVideoQuality(1080, 60, "399", profile, false);
    video_qualities << possibleVideoQuality(1080, 60, "699", profile, false);

    video_qualities << possibleVideoQuality(1440, 60, "400", profile, false);
    video_qualities << possibleVideoQuality(1440, 60, "700", profile, false);

    video_qualities << possibleVideoQuality(2160, 60, "401", profile, false);
    video_qualities << possibleVideoQuality(2160, 60, "701", profile, false);

    video_qualities << possibleVideoQuality(4320, 60, "571", profile, false);
    video_qualities << possibleVideoQuality(4320, 60, "702", profile, false);
}

void YoutubeParser::fill_video_quality_list(QList<possibleVideoQuality> &video_qualities) {
    fill_av1_video_quality_list(video_qualities);
    fill_webm_video_quality_list(video_qualities);
    fill_avc_video_quality_list(video_qualities);
}

void YoutubeParser::fill_avc_av1_audio_quality_list(QList<possibleAudioQuality> &audio_qualities) {
    audio_qualities << possibleAudioQuality("599", AudioProfile::AAC); //32k
    audio_qualities << possibleAudioQuality("258", AudioProfile::AAC);
    audio_qualities << possibleAudioQuality("256", AudioProfile::AAC);
    audio_qualities << possibleAudioQuality("139", AudioProfile::AAC); //48k
    audio_qualities << possibleAudioQuality("140", AudioProfile::AAC); //128k
    audio_qualities << possibleAudioQuality("141", AudioProfile::AAC); //256k
}

void YoutubeParser::fill_webm_audio_quality_list(QList<possibleAudioQuality>& audio_qualities) {
    audio_qualities << possibleAudioQuality("171", AudioProfile::OGG); //Vorbis128k
    audio_qualities << possibleAudioQuality("172", AudioProfile::OGG); //Vorbis128k

    audio_qualities << possibleAudioQuality("600", AudioProfile::OGG); //Opus40k_lower_bitrate
    audio_qualities << possibleAudioQuality("249", AudioProfile::OGG); //Opus50k_lower_bitrate
    audio_qualities << possibleAudioQuality("250", AudioProfile::OGG); //Opus78k
    audio_qualities << possibleAudioQuality("251", AudioProfile::OGG); //Opus150k
}

QString YoutubeParser::getLinkTag(const QString& link) {
    QString tag;
    QRegExp expression = QRegExp("itag=([^&]+)");
    if (expression.indexIn(link) > -1)
        tag = expression.cap(1);

    if (tag.isEmpty()) {
        expression = QRegExp("itag=([^,]+)");
        if (expression.indexIn(link) > -1)
            tag = expression.cap(1);
    }

    qDebug() << "tag: " << tag;

    return tag;
}

int YoutubeParser::getTagAudioBitrate(const int tag) {
    int bitrate = 0;

    if (tag > 0) {
        switch (tag) {
            //mp3_compatible
            case 5:
            case 6:
                bitrate = 64000;
                break;
            //avc_compatible
            case 13://???
            case 17:
                bitrate = 24000;
                break;
            case 599:
                bitrate = 32000;
                break;
            case 36:
                bitrate = 36000;
                break;
            case 139:
                bitrate = 48000;
                break;
            case 18:
            case 82:
            case 83:
                bitrate = 96000;
                break;
            case 140:
            case 120:
            case 78:
            case 59:
            case 35:
            case 34:
            case 22:
                bitrate = 128000;
                break;
            case 38:
            case 37:
            case 84:
            case 85:
                bitrate = 192000;
                break;
            case 141:
            case 256://???
            case 258://???
                bitrate = 256000;
                break;
            //vp_compatible
            case 171:
            case 43:
            case 44:
            case 100:
                bitrate = 128000;
                break;
            case 45:
            case 46:
            case 101:
            case 102:
            case 172:
                bitrate = 192000;
                break;
            //opus_compatible
            case 251:
                bitrate = 128000;
                break;
            case 250:
                bitrate = 70000;
                break;
            case 249:
                bitrate = 50000;
                break;
            case 600:
                bitrate = 40000;
                break;
        }
    }

    return bitrate;
}

QString YoutubeParser::getChannelIdFromJsonVideoInfo(const QString& json_video_info) {
    QRegularExpression regexChannelId("\"channelId\":\\s*\"([^\"]+)\"");
    QRegularExpressionMatch match = regexChannelId.match(json_video_info);

    if (match.hasMatch())
        return match.captured(1);

    return {};
}

QString YoutubeParser::getVideoIdFromHTML(const QString& html) {
    QRegularExpression regexp("\"video_id\": \"(.*)\"", QRegularExpression::PatternOption::CaseInsensitiveOption);
    QRegularExpressionMatch match = regexp.match(html);
    if (match.hasMatch())
        return match.captured(1);

    QRegularExpression regexp2("video_id=([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
    QRegularExpressionMatch match2 = regexp2.match(html);
    if (match2.hasMatch())
        return match2.captured(1);
    
    return {};
}

QString YoutubeParser::getChannelIdFromHTML(const QString& html) {
    QRegularExpression regexChannelId("\"externalId\":\"([^\"]+)\"");
    QRegularExpressionMatch match = regexChannelId.match(html);

    if (match.hasMatch())
        return match.captured(1);

    return {};
}

QString YoutubeParser::getPlaylistIdFromHTML(const QString& html) {
    QRegularExpression regexChannelId("\"playlistId\":\"([^\"]+)\"");
    QRegularExpressionMatch match = regexChannelId.match(html);

    if (match.hasMatch())
        return match.captured(1);

    return {};
}

QString YoutubeParser::getVideoIdFromURL(const QString& link) {
    QStringList splits = link.split("v=");
    if (splits.size() >= 2) {
        const QString& part = splits[1];
        int sep_pos = part.indexOf("&");
        sep_pos = (sep_pos == -1 ? part.indexOf("?") : sep_pos);
        if (sep_pos != -1)
            return part.chopped(part.size() - sep_pos);
        else
            return part;
    }
    return {};
}

QString YoutubeParser::getPlaylistIdFromURL(const QString& link) {
    QUrl url(link);
    QUrlQuery query(link);

    QString lst = "list";
    QString id = query.queryItemValue(lst);
    if (id.isEmpty()
        && link.contains(lst))
        id = link.split(lst + "=").last().split("&").first();

    return id;
}

QString YoutubeParser::getChannelIdFromURL(const QString& link, Abortable* abortable) {
    QUrl url(link);
    QString channelUsername;

    QStringList pathElements = url.path().split("/", QString::SkipEmptyParts);
    if (!pathElements.isEmpty()) {
        if (pathElements[0] == "c")
            channelUsername = pathElements[1];
        else if (pathElements[0].startsWith('@'))
            channelUsername = pathElements[0].mid(1);
    }
    qDebug() << "channelUsername: " << channelUsername;

    QString data;
    if (!channelUsername.isEmpty()) {
        QList<QPair<QByteArray, QByteArray>>  headers;

        QList<QPair<QByteArray, QByteArray>>  url_query_params;
        url_query_params.append(qMakePair(QByteArray("part"), QByteArray("about")));
        url_query_params.append(qMakePair(QByteArray("handle"), QByteArray(channelUsername.toLocal8Bit())));

        QString info_url = ("https://yt.lemnoslife.com/channels");
        if (downloadToStringExtended(info_url, data, headers, url_query_params, nullptr, abortable)
            && !data.isEmpty()) {
#if _DEBUG
            QFile   file("channelId.json");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(data.toStdString().c_str());
                file.close();
            }
#endif
            QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
            QJsonObject jsonResponseObj = jsonResponse.object();
            if (!jsonResponseObj.isEmpty()) {
                const auto items = jsonResponseObj["items"].toArray();
                if (!items.isEmpty())
                    return items[0].toObject()["id"].toString();
            }
        }
    }
    else {
        if (downloadToString(link, data, abortable)
            && !data.isEmpty()) {
#if _DEBUG
            QFile   file("channelId.html");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(data.toStdString().c_str());
                file.close();
            }
#endif
            QRegExp rx("\"externalId\":\"([^\"]+)\"");
            rx.indexIn(data.toStdString().c_str());
            return rx.cap(1);
        }
    }

    return {};
}

QString YoutubeParser::getVideoId(const QString& html, const QString& url) {
    QString video_id = getVideoIdFromURL(url);
    if (video_id.isEmpty())
        video_id = getVideoIdFromHTML(html) ;

    !video_id.isEmpty() ? qDebug() << "video_id " << video_id : qDebug() << "Failed to extract video_id";

    return video_id;
}

QString YoutubeParser::getPlaylistId(const QString& html, const QString& url) {
    QString playlist_id = getPlaylistIdFromHTML(html);
    if (playlist_id.isEmpty())
        playlist_id = getPlaylistIdFromURL(url);

    !playlist_id.isEmpty() ? qDebug() << "playlist_id " << playlist_id : qDebug() << "Failed to extract playlist_id";

    return playlist_id;
}

QString YoutubeParser::getChannelId(const QString& html, const QString& url, const QString& json_video_info, Abortable* abortable) {
    QString channel_id;
    if (!json_video_info.isEmpty())
        channel_id = getChannelIdFromJsonVideoInfo(json_video_info);
    if (!html.isEmpty()
        && channel_id.isEmpty())
        channel_id = getChannelIdFromHTML(html);
    if (channel_id.isEmpty())
        channel_id = getChannelIdFromURL(url, abortable);

    !channel_id.isEmpty() ? qDebug() << "channel_id " << channel_id : qDebug() << "Failed to extract channel_id";

    return channel_id;
}

MimeType YoutubeParser::getMimeType(const QString& mime_type, bool* ok) {
    // Example, "video/3gpp; codecs=\"mp4v.20.3, mp4a.40.2\""
    // "audio/mp4; codecs=\"mp4a.40.5\""
    MimeType mime{};

    QStringList type_codecs = mime_type.split(";");
    if (type_codecs.size() != 2) {
        qDebug() << "Invalid mime type: " << mime_type
            << ". Expected \"type/profile; codecs=[]\" pattern";
        *ok = false;
        return mime;
    }
    QStringList type_profile = type_codecs.first().split("/");
    if (type_profile.size() != 2) {
        qDebug() << "Invalid type and format: " << type_codecs.first()
            << ". Expected \"type/profile\" pattern";
        *ok = false;
        return mime;
    }
    std::initializer_list<QString> type_possible_values = { "video", "audio" };
    if (std::any_of(type_possible_values.begin(), type_possible_values.end(), [&](const auto& type) { return type == type_profile.first(); })) {
        mime.type = type_profile.first();
    }
    else {
        qDebug() << "Invalid type: " << type_profile.first();
        *ok = false;
        return mime;
    }
    mime.profile = getVideoProfile(type_profile.back());
    if (mime.profile == VideoProfile::UNKNOWN) {
        qDebug() << "Unknown video profile: " << type_profile.back();
    }

    mime.codecs = type_codecs.back().split(",");

    *ok = true;
    return mime;
}

qint64 YoutubeParser::getLinkQualitySize(const QString& link) {
    QRegExp expression = QRegExp("clen=([^&]+)");
    if (expression.indexIn(link) > -1)
        return expression.cap(1).toLongLong();

    expression = QRegExp("clen=([^,]+)");
    if (expression.indexIn(link) > -1)
        return expression.cap(1).toLongLong();

    return 0;
}

VideoProfile YoutubeParser::getVideoProfile(const QString& type) {
    std::initializer_list<QString> possible_types = { "mp4", "av1", "webm" };
    std::array<VideoProfile, 3> possible_profiles = { VideoProfile::AVC, VideoProfile::AV1, VideoProfile::WEBM };

    const auto it = std::find(possible_types.begin(), possible_types.end(), type);
    if (it == possible_types.end())
        return VideoProfile::UNKNOWN;

    return possible_profiles[static_cast<size_t>(std::distance(possible_types.begin(), it))];
}

QString YoutubeParser::getLinkQualityLang(const QString& link) {
    QString lang_tag;

    QRegExp expression = QRegExp("lang=([^&]+)");
    if (expression.indexIn(link) > -1)
        lang_tag = expression.cap(1);

    if (lang_tag.isEmpty()) {
        expression = QRegExp("lang=([^,]+)");
        if (expression.indexIn(link) > -1)
            lang_tag = expression.cap(1);
    }

    return lang_tag;
}

int YoutubeParser::getLinkQualityBitrate(const QString& link) {
    QRegExp expression = QRegExp("bitrate=([^&]+)");
    if (expression.indexIn(link) > -1)
        return expression.cap(1).toLongLong();

    expression = QRegExp("bitrate=([^,]+)");
    if (expression.indexIn(link) > -1)
        return expression.cap(1).toInt();

    return 0;
}

int YoutubeParser::getLinkQualityFramerate(const QString& link, int current_value) {
    int res = current_value;
    QString tag = getLinkTag(link);

    if (!tag.isEmpty()) {
        //tags with framerate 60 fps
        QList<QString> high_fps_tags = {/*mp4*/ "299","298",/*webm*/"272","315","337","308","336","303","335","302","334","333","332","331","330",/*av1*/"571","702","401","701","400","700","399","699","398","698","697","696","695","694"};
        int i = high_fps_tags.indexOf(tag);
        if (i > -1)
            res = 60;
    }

    return res;
}

bool YoutubeParser::getLinkQualityHDR(const QString& link) {
    bool res = false;
    QString tag = getLinkTag(link);

    if (!tag.isEmpty()) {
        //tags with framerate 60 fps
        QList<QString> high_fps_tags = {/*mp4*//*webm*/"337","336","335","334","333","332","331","330",/*av1*/"571","702","701","700","699","698","697","696","695","694","401","400","399","398","397","396","395","394"};
        int i = high_fps_tags.indexOf(tag);
        if (i > -1)
            res = true;
    }

    return res;
}

QString YoutubeParser::getVideoFormatDesk(const VideoProfile type) {
    QString desk;

    switch (type) {
        case AVC:
            desk = "mp4";
            break;
        case AV1:
            desk = "av1";
            break;
        case WEBM:
            desk = "webm";
            break;
    }

    return desk;
}

QString YoutubeParser::getAudioFormatDesk(const AudioProfile type) {
    QString desk;

    switch (type) {
    case AAC:
        desk = "aac";
        break;
    case OGG:
        desk = "ogg";
        break;
    }

    return desk;
}

QString YoutubeParser::getContentQualityUrl(const QStringList& quality_list, const QString& tag) {
    QRegExp urlExpression = QRegExp("(http[s]?[^,]+)");
    QRegExp itagExpression = QRegExp("[,]?itag=([^,]+)");

    for (int i = 0; i < quality_list.size(); i++) {
        bool urlExpressionMatch = (urlExpression.indexIn(quality_list.at(i)) > -1);
        bool itagExpressionMatch = (itagExpression.indexIn(quality_list.at(i)) > -1);

        if ((urlExpressionMatch && itagExpressionMatch && itagExpression.cap(1) == tag))
            return quality_list.at(i);
    }
    return "";
}

//works only for audio streams now
QString YoutubeParser::getContenDashQualityUrl(const QList<QStringList>& dash_quality_links, const QString& tag) {
    QString url;
    QRegExp itagExpression = QRegExp("[/]?itag/([^/]+)");
    QStringList link_segments;

    for (int j = 0; j < dash_quality_links.length(); j++) {
        if (!(itagExpression.indexIn(dash_quality_links.at(j).at(0)) > -1))
            continue;

        if (url.isEmpty() && itagExpression.cap(1) == tag) {
            QString base = dash_quality_links.at(j).at(0).split("range/").first();
            link_segments = QStringList(dash_quality_links.at(j));
            QString max_range_value = link_segments.last().split("range/").last().split("-").last();
            url = base + "range/0-" + max_range_value;
        }
    }

    return url;
}

QString YoutubeParser::getUrlFromQualityLink(const QString& link, const Abortable* abortable) {
	if (js_signature_script_.isEmpty()) {
		qDebug() << "Unable to build URL due to missing signature script.";
		return {};
	}

    QRegExp clen0 = QRegExp("clen=0");
    QRegExp urlExpression = QRegExp("(http[s]?[^,]+)");

    if (clen0.indexIn(link) == -1 && urlExpression.indexIn(link) > -1) {
        QString url = QUrl::fromPercentEncoding(urlExpression.cap(1).toLatin1());
        if (url.endsWith("==", Qt::CaseInsensitive))
            url.chop(2);
        
        QString signature;
        QRegExp sigExpression = QRegExp("(?:^|[^a-zA-Z])[,]?s(ig)?=([^,]+)");
        if (sigExpression.indexIn(link) > -1) {
            
            if (sigExpression.cap(1) == "ig") {
                signature = QUrl::fromPercentEncoding(sigExpression.cap(2).toLatin1()).split("&").first();
            }
            else {
                QString script = js_signature_script_ + "(\"" + QUrl::fromPercentEncoding(sigExpression.cap(2).toLatin1()) + "\")";
                signature = runSyncScript(script, abortable).split("&").first();
            }

            QString signatureParameter = "signature";
            QRegExp signatureParameterExpression = QRegExp("(?:^|[^a-zA-Z])[,]?sp=([^,]+)");
            if (signatureParameterExpression.indexIn(link) > -1)
                signatureParameter = signatureParameterExpression.cap(1).split("&").first();

            if (!url.contains(signature))
                url = url.append("&" + signatureParameter + "=" + signature);
        }

        return url;
    }

    return "";
}

void YoutubeParser::extractJSMethod(const QString& name, const QString& js, QList<jsMethod>& js_methods) {
    for (int i = 0; i < js_methods.length(); i++) {
        if (js_methods.at(i).name == name) {
            return;
        }
    }

    QString escapedName = name;
    escapedName.replace("$", "\\$");

    QRegExp expression("((?:function\\s*" + escapedName + "|(var\\s*|,\\s*|\\n)" + escapedName + "\\s*=(?:\\s*function)?)\\s*(\\([\\w,\\s]*\\))?\\s*)(\\{.*\\})");
    expression.setCaseSensitivity(Qt::CaseSensitive);
    
    if (expression.indexIn((js)) != -1) {
        jsMethod method;
        method.name = name;
        QString descriptor = expression.cap(1);
        if (!expression.cap(2).isEmpty()) {
            descriptor = descriptor.right(descriptor.length() - expression.cap(2).length());
        }
        QString code = expression.cap(4);
        QString parameters = expression.cap(3).replace(QRegExp("[\\(\\)\\s]"), "");

		QRegExp exprEndExpr("[\\w$\\.\'\"\\)\\]\\}]\\s*$");
		QStack<QChar> parseStack;
		bool isEscape = false;
        for (int i = 0; i < code.length(); i++) {
			QChar topOfStack = parseStack.isEmpty() ? '\0' : parseStack.top();
			bool isQuoted = topOfStack == '\'' || topOfStack == '\"';
			bool isInRegex = parseStack.contains('/');
			QChar ch = code.at(i);
			switch (ch.toLatin1()) {
			case '\\':
				if (!isQuoted && !isInRegex) {
					qDebug() << "Invalid escape sequence at " << i;
				}
				isEscape = !isEscape;
				break;
			case '{':
			case '(':
			case '[':
				if (!isEscape) {
					if (!isQuoted && !isInRegex) {
						parseStack.push(ch);
					}
					if (topOfStack == '/' && ch == '[') {
						parseStack.push(ch);
					}
					break;
				}
				[[fallthrough]];
			case '}':
			case ')':
			case ']':
				if (!isEscape) {
					if (!isQuoted && !isInRegex) {
						if (topOfStack == '{' && ch == '}' || topOfStack == '(' && ch == ')' || topOfStack == '[' && ch == ']') {
							parseStack.pop();
						}
						else {
							qDebug() << "Unmatched parenthesis at " << i;
							return;
						}
					}
					if (isInRegex && topOfStack == '[' && ch == ']') {
						parseStack.pop();
					}
					break;
				}
				[[fallthrough]];
			case '\'':
			case '\"':
			case '/':
				if (!isEscape) {
					if (topOfStack == ch) {
						parseStack.pop();
					}
					else if (!isQuoted && !isInRegex) {
						bool isQuote = ch != '/';
						if (!isQuote) {
							isQuote = exprEndExpr.indexIn(code.left(i)) < 0;
						}
						if (isQuote) {
							parseStack.push(ch);
						}
					}
					break;
				}
				[[fallthrough]];
			default:
				if (isEscape) {
					isEscape = false;
				}
				break;
			}
			if (i > 0 && parseStack.isEmpty()) {
				method.code = descriptor + code.left(i + 1);
				break;
			}
        }

        if (!method.code.isEmpty()) {
            js_methods.append(method);

            expression = QRegExp("([\\w\\$]+)(?:[\\w\\.\\$])*\\s*(\\([\\w\\s,\"\\$]*)\\)");
            int pos = expression.indexIn(method.code);
            QStringList expressions;
            expressions << "function" << "return" << "if" << "elseif";
            expressions.append(parameters.split(","));

            while (pos != -1) {
                if (expressions.indexOf(expression.cap(1)) == -1) {
                    extractJSMethod(expression.cap(1), js, js_methods);
                }
                pos = expression.indexIn(method.code, pos + expression.cap(0).length());
            }
        }
    }
}

void YoutubeParser::parseSignatureJS(const QString& js, QString& signature_method_name, QList<jsMethod>& js_methods) {
    QString methodName;

    QRegExp expression("[$\\w]=([$\\w]+)\\(decodeURIComponent\\([$\\w]\\.s\\)");
    expression.setMinimal(true);
    if (expression.indexIn(js) > -1) {
        methodName = expression.cap(1);
    }

    if (methodName.isEmpty()) {
        expression = QRegExp("signature=|set\\(\"signature\",|set\\([\\w\\.]+\\|\\|\"signature\",([$\\w]+)\\(");
        expression.setMinimal(true);
        if (expression.indexIn(js) > -1) {
            methodName = expression.cap(1);
        }
    }

    if (methodName.isEmpty()) {
        expression = QRegExp("([$\\w]+)\\s*=\\s*function\\(([$\\w]+)\\)\\{\\2=\\2\\.split\\(\"\"\\)");
        expression.setMinimal(true);
        if (expression.indexIn(js) != -1) {
            methodName = expression.cap(1);
        }
    }

    if (!methodName.isEmpty()) {
        signature_method_name = methodName;
        extractJSMethod(methodName, js, js_methods);
    }
}

void YoutubeParser::parseNJS(const QString& js, QString& n_method_name, QList<jsMethod>& js_methods) {
    QString methodName, idx;

    QRegExp expression(
		"(?:"
			"\\.get\\(\"n\"\\)\\)&&\\(b=|"
			"(?:"
				"b=String\\.fromCharCode\\(110\\)|"
				"([\\w$.]+)&&\\(b=\"nn\"\\[\\+\\1\\]" // \1 = str_idx
			")"
			"(?:"
				",[\\w$]+\\(a\\))?,c=a\\."
				"(?:"
					"get\\(b\\)|"
					"[\\w$]+\\[b\\]\\|\\|null"
				")\\)&&\\(c="
		")([\\w$]+)(?:\\[(\\d+)\\])?\\(\\w\\)" // \2 = nfunc, \3 = idx
	);
    expression.setMinimal(true);
    if (expression.indexIn(js) > -1) {
        methodName = expression.cap(2);
		idx = expression.cap(3);
    }

	if (methodName.isEmpty()) {
		expression = QRegExp(
			"(?:"
				"\\.get\\(\"n\"\\)\\)&&\\(b=|"
				"(?:"
					"b=String\\.fromCharCode\\(110\\)|"
					"([\\w$.]+)&&\\(b=\"nn\"\\[\\+\\1\\]" // \1 = str_idx
				")"
				"(?:"
					",[\\w$]+\\(a\\))?,c=a\\."
					"(?:"
						"get\\(b\\)|"
						"[\\w$]+\\[b\\]\\|\\|null"
					")\\)&&\\(c=|"
				"\\b([\\w$]+)=" // \2 = var
			")([\\w$]+)(?:\\[(\\d+)\\])?\\(\\w\\)" // \3 = nfunc, \4 = idx
			"(?:,[\\w$]+\\.set\\((?:\"n+\"|[\\w$]+)\\,\\2\\))"
		);
		expression.setMinimal(true);
		if (expression.indexIn(js) != -1) {
			methodName = expression.cap(3);
			idx = expression.cap(4);
		}
	}

	if (methodName.isEmpty()) {
		expression = QRegExp(
			";\\s*([\\w$]+)\\s*=\\s*function\\([\\w$]+\\)" // \1 = name
			"\\s*\\{(?:.(?!\\};))+return\\s*([\"'])[\\w-]+_w8_(?:\\2)\\s*\\+\\s*[\\w$]+" // \2 = quote
		);
		expression.setMinimal(true);
		if (expression.indexIn(js) != -1) {
			methodName = expression.cap(1);
		}
	}

    if (!methodName.isEmpty()) {
		if (!idx.isEmpty()) {
			// TODO
			bool ok = false;
			int methodIndex = idx.toInt(&ok);
			if (!ok) {
				qDebug() << "Failed to parse n method index " << idx;
				return;
			}
			QRegExp methodArrayExp = QRegExp("var " + QRegExp::escape(methodName) + "\\s*=\\s*\\[(.+)\\]\\s*[,;]");
			methodArrayExp.setMinimal(true);
			if (methodArrayExp.indexIn(js) < 0) {
				qDebug() << "Failed to find n method array.";
				return;
			}
			QString methodArray = methodArrayExp.cap(1);
			methodName = methodArray.split(',').at(methodIndex);
		}
		n_method_name = methodName;
        extractJSMethod(methodName, js, js_methods);
		patchNJS(methodName, js_methods);
    }
}

void YoutubeParser::patchNJS(const QString& n_method, QList<jsMethod>& js_methods) {
	auto it = std::find_if(js_methods.begin(), js_methods.end(), [&](jsMethod m) { return m.name == n_method; });
	if (it == js_methods.end()) {
		qDebug() << "N method not found.";
		return;
	}

	// Remove isolated function check.
	QRegExp expression(
		";\\s*if\\s*\\(\\s*typeof\\s+[\\w$]+\\s*===?\\s*([\"'])undefined\\1\\s*\\)\\s*return\\s+[\\w$]+;" // \1 = quote
	);
	(*it).code = (*it).code.replace(expression, ";");
}

void YoutubeParser::extractSignatureTimestamp(const QString& js) {
	QRegExp expression = QRegExp("(?:signatureTimestamp|sts)\s*:\s*([0-9]{5})");
	if (expression.indexIn(js) != -1) {
		signature_timestamp_ = expression.cap(1);
	}
}

//void YoutubeParser::parseMpegDash(const QString& xml, QList<QStringList>& dash_quality_links) {
//    QDomDocument mpd;
//    mpd.setContent(xml);
//    QDomNodeList adaptionSets = mpd.elementsByTagName("AdaptationSet");
//
//    for (int i = 0; i < adaptionSets.length(); i++) {
//        QDomElement set = adaptionSets.at(i).toElement();
//        if (set.attribute(("mimeType")) == "video/mp4" || set.attribute(("mimeType")) == "audio/mp4") {
//            QDomNodeList representations = set.elementsByTagName(("Representation"));
//            for (int j = 0; j < representations.length(); j++) {
//                QDomElement representation = representations.at(j).toElement();
//
//                QString baseUrl;
//                QStringList segments;
//                if (representation.elementsByTagName("BaseURL").length() == 1) {
//                    baseUrl = representation.elementsByTagName("BaseURL").at(0).toElement().text();
//                    QDomNode initializationNode = representation.elementsByTagName("Initialization").at(0);
//                    QDomNodeList segmentNodes = representation.elementsByTagName("SegmentURL");
//                    if (initializationNode.isElement() && !segmentNodes.isEmpty()) {
//                        segments << baseUrl + initializationNode.toElement().attribute("sourceURL");
//                        for (int k = 0; k < segmentNodes.length(); k++) {
//                            segments << baseUrl + segmentNodes.at(k).toElement().attribute("media");
//                        }
//                    }
//                }
//                if (!segments.empty()) {
//                    dash_quality_links << segments;
//                }
//            }
//        }
//    }
//}

QString YoutubeParser::getJSPlayerURL(const QString& html) {
    QString html5_player_url;

    QRegularExpression regexp = QRegularExpression("html5player.+\\.js", QRegularExpression::PatternOption::CaseInsensitiveOption);
    QRegularExpressionMatch match = regexp.match(html);
    if (match.hasMatch())
        html5_player_url = "http://s.ytimg.com/yts/jsbin/" + match.captured(0).replace("\\/", "/");

    if (html5_player_url.isEmpty()) {
        regexp = QRegularExpression("//[^\"]*html5player[^\"]*\\.js", QRegularExpression::PatternOption::CaseInsensitiveOption);
        match = regexp.match(html);
        if (match.hasMatch())
            html5_player_url = "https:" + match.captured(0);
    }
        
    if (html5_player_url.isEmpty()) {                                     //jsUrl":"/s/player/2e6e57d8/player_ias.vflset/en_US/base.js"
        //regexp = QRegExp("(//?)[^\"]*player[^\"]*\\/base\\.js");
        regexp = QRegularExpression("\"jsUrl\":\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
        match = regexp.match(html);
        if (match.hasMatch()) {
            QString url_part = match.captured(1);
            if (url_part.contains("base.js"))
                html5_player_url = "https://www.youtube.com" + url_part;
            else
                html5_player_url = "https://www.youtube.com" + match.captured(0).replace("\\", "");
        }
    }

    return html5_player_url;
}

Status YoutubeParser::mainParsePage(const QString& html,
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
    const QString& player) {

    Status status;

    QRegExp expression;
    if (received_data_type.isEmpty() || received_data_type == "html") {
        //Does this video support MPEG-DASH?
        expression = QRegExp("\"dashManifestUrl\":\"([^\"]*)");
        expression.setMinimal(false);

        QString content;
        if (player.isEmpty())
            content = html;
        else
            content = player;

        if (mpeg_dash_data_type.isEmpty() && expression.indexIn(html) != -1) {
            QString url = expression.cap(1).replace("\\", "");
            download_required["dashmpd"] = url;
        }

        if (js_data_type.isEmpty()) {
            html5PlayerUrl = getJSPlayerURL(content);

            if (!html5PlayerUrl.isEmpty()) {
                // TODO: Remove false in future version.
                if (false && youtubePlayerUrl == html5PlayerUrl) {
                    qDebug() << "Loading cached player " << html5PlayerUrl;
                    js_data_type = youtubePlayerJS;
                    signature_method = youtubePlayerSignatureMethodName;
                }
                else {
                    qDebug() << "Downloading player " << html5PlayerUrl;
                    download_required["js"] = html5PlayerUrl;
                }
            }
            else {
                status.update(EStatusCode::ParseError, "Could not retrieve player URL.");
            }
        }
    }
    //Parse player JS
    else if (!player.isEmpty() && received_data_type == "js") {
        QList<jsMethod> js_methods;
        parseSignatureJS(player, signature_method, js_methods);
        js_data_type = "";

        for (int i = 0; i < js_methods.length(); i++) {
            js_data_type.append(js_methods.at(i).code).append(";");
        }

        youtubePlayerUrl = html5PlayerUrl;
        youtubePlayerJS = js_data_type;
        youtubePlayerSignatureMethodName = signature_method;
    }
    //Parse MPEG-DASH MPD
    //else if (!player.isEmpty() && received_data_type == "dashmpd") {
    //    parseMpegDash(player, dash_quality_links);
    //}

    return status;
}

void YoutubeParser::createVideoTrack(const QString& video_data, const QString& video_link, const possibleVideoQuality& quality, QList<videoQuality>& added_qualities, TracksInfo& tracks_info) {
    qDebug() << "video link " << video_link;
    
    videoQuality new_quality;
    new_quality.profile = quality.profile;
    new_quality.itag = quality.itag;
    new_quality.video_height = quality.video_height;
    new_quality.video_framerate = quality.video_framerate;
    new_quality.video_size = getLinkQualitySize(video_link);

    if (!videoQualityExists(added_qualities, new_quality)) {
        int tag = new_quality.itag.toInt();
        
        if (new_quality.itag.isEmpty())
            tag = getLinkTag(video_link).toInt();

        VideoTrackInfo video_track;

        QStringList data = video_data.split(",");
        data.removeAll("");
        for each (auto &it in data) {
            if (it.contains("averageBitrate")) {
                video_track.bitrate = it.split("=").last().toInt();
            }
            else if (it.contains("width")) {
                int width = it.split("=").last().toInt();
                if (width > 0)
                    video_track.width = width;
            }
            else if (it.contains("height")) {
                int height = it.split("=").last().toInt();
                if (height > 0)
                    video_track.height = height;
            }
            else if (it.contains("fps")) {
                int frame_rate = it.split("=").last().toInt();
                if (frame_rate > 0)
                    video_track.framerate = frame_rate;
            }
            else if (it.contains("contentLength")) {
                int size = it.split("=").last().toInt();
                if (size > 0)
                    video_track.size = size;
            }
        }

        video_track.height = quality.video_height;
        video_track.framerate = getLinkQualityFramerate(video_link, video_track.framerate);
        if (video_track.bitrate == 0)
            video_track.bitrate = getLinkQualityBitrate(video_link);
        if (video_track.size == 0)
            video_track.size = new_quality.video_size;

        video_track.urls = buildRanges(video_link, video_track.size);
        if (video_track.urls.size() > 1)
            video_track.is_chunked = true;

        video_track.contains_audio = quality.contains_audio;
        video_track.profile = quality.profile;
        video_track.extended_desk = getVideoFormatDesk(video_track.profile);

        normalizeTrack(video_track);
        normalizeVideoTrack(video_track);
        tracks_info.video_tracks.append(video_track);

        added_qualities.append(new_quality);

        // add audio track from video stream
        if (quality.contains_audio 
            && tracks_info.audio_tracks.isEmpty()) {

            AudioTrackInfo audio_track;
            audio_track.from_video_stream = true;
            audio_track.bitrate = getTagAudioBitrate(tag);
            audio_track.profile = AudioProfile::AAC;

            if (quality.profile == VideoProfile::WEBM)
                audio_track.profile = AudioProfile::OGG;

            //audio_track.urls.append(video_link);

            audio_track.urls = buildRanges(video_link, video_track.size);
            if (audio_track.urls.size() > 1)
                audio_track.is_chunked = true;
            
            normalizeTrack(audio_track);
            normalizeAudioTrack(audio_track);
            tracks_info.audio_tracks.append(audio_track);
        }
    }
}

void YoutubeParser::createAudioTrack(const QString& audio_data, const QString& audio_link, const possibleAudioQuality& quality, QList<audioQuality>& added_qualities, TracksInfo& tracks_info) {
    qDebug() << "audio link " << audio_link;

    audioQuality new_quality;
    new_quality.profile = quality.profile;
    new_quality.itag = quality.itag;
    new_quality.audio_bitrate = getLinkQualityBitrate(audio_link);
    new_quality.audio_size = getLinkQualitySize(audio_link);
    new_quality.lang_tag = getLinkQualityLang(audio_link);

    if (!audioQualityExists(added_qualities, new_quality)) {

        AudioTrackInfo audio_track;

        QStringList data = audio_data.split(",");
        data.removeAll("");
        for each (auto & it in data) {
            if (it.contains("averageBitrate")) {
                audio_track.bitrate = it.split("=").last().toInt();
            }
            else if (it.contains("audioChannels")) {
                int channels = it.split("=").last().toInt();
                if (channels > 0)
                    audio_track.channels = channels;
            }
            else if (it.contains("contentLength")) {
                int size = it.split("=").last().toInt();
                if (size > 0)
                    audio_track.size = size;
            }
        }

        if (audio_track.bitrate == 0)
            audio_track.bitrate = new_quality.audio_bitrate;
        if (audio_track.size == 0)
            audio_track.size = new_quality.audio_size;
        audio_track.profile = new_quality.profile;

        audio_track.urls = buildRanges(audio_link, audio_track.size);
        if (audio_track.urls.size() > 1)
            audio_track.is_chunked = true;

        if (audio_track.bitrate == 0) {
            int tag = new_quality.itag.toInt();
            
            if (new_quality.itag.isEmpty())
                tag = getLinkTag(audio_link).toInt();

            audio_track.bitrate = getTagAudioBitrate(tag);
        }

        normalizeTrack(audio_track);
        normalizeAudioTrack(audio_track);
        tracks_info.audio_tracks.append(audio_track);

        added_qualities.append(new_quality);
    }
}

void YoutubeParser::getYouTubeVideoInfo(const QString& html, const QString& player, QString& video_info, Abortable* abortable) {
    QString time_stamp = SS2Q(yt::getTimeStamp(QS2S(html)));
    if (time_stamp.isEmpty()
        && !player.isEmpty())
        time_stamp = SS2Q(yt::getTimeStamp(QS2S(player)));

    if (video_id_.isEmpty()) {
        QRegExp expression = QRegExp("videoId\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        expression.setMinimal(true);
        if (expression.indexIn(html))
            video_id_ = QString(expression.cap(1));

        if (video_id_.isEmpty()) {
            expression = QRegExp("\"videoId\":\"([^\"]+)[\"]", Qt::CaseInsensitive);
            expression.setMinimal(true);
            if (expression.indexIn(html))
                video_id_ = QString(expression.cap(1));
        }
    }

    if (!video_id_.isEmpty()) {
        //QString video_info_url = "https://www.youtube.com/get_video_info?html5=1";
        //video_info_url.append("&video_id=" + video_id_);
        //video_info_url.append("&eurl=https://youtube.googleapis.com/v/" + video_id_);
        //if (!time_stamp.isEmpty())
        //    video_info_url.append("&sts=" + time_stamp);

        //if (downloadToString(video_info_url, video_info))
        //    video_info = QUrl::fromPercentEncoding(video_info.toLatin1());

        //QString body = QString("{\n    \"context\": {\n        \"client\": {\n            \"clientName\": \"WEB\",\n            \"clientScreen\": \"WATCH\",\n            \"clientVersion\": \"2.20210721.00.00\"\n        },\n        \"thirdParty\": {\n            \"embedUrl\": \"https://www.youtube.com\"\n        }\n    },\n    \"videoId\": \"%1\"\n}").arg(video_id_);
        //QString body = QString("{\n    \"context\": {\n        \"client\": {\n            \"clientName\": \"ANDROID\",\n            \"clientScreen\": \"EMBED\",\n            \"clientVersion\": \"16.02\"\n        },\n        \"thirdParty\": {\n            \"embedUrl\": \"https://www.youtube.com\"\n        }\n    },\n    \"videoId\": \"%1\"\n}").arg(video_id_);
        QByteArray body("{\"context\": {\"client\": {\"clientName\": \"TVHTML5_SIMPLY_EMBEDDED_PLAYER\", \"clientVersion\": \"2.0\", \"hl\": \"en\", \"timeZone\": \"UTC\", \"utcOffsetMinutes\": 0}, \"thirdParty\": {\"embedUrl\": \"https://www.youtube.com/\"}}}");

        QString data;
        QList<QPair<QByteArray, QByteArray>>  headers;

        headers.append(qMakePair(QByteArray("User-Agent"), QByteArray("Mozilla/5.0")));
        headers.append(qMakePair(QByteArray("accept-language"), QByteArray("en-US,en")));
        headers.append(qMakePair(QByteArray("Content-Type"), QByteArray("application/json")));

        QList<QPair<QByteArray, QByteArray>>  url_query_params;

        url_query_params.append(qMakePair(QByteArray("videoId"), video_id_.toUtf8()));
        addDefaultQuery(url_query_params);
        // Other keys to api used by google:
        // AIzaSyAO_FJ2SlqU8Q4STEHLGCilw_Y9_11qcW8
        // AIzaSyCtkvNIR1HCEwzsqK6JuE6KqpyjusIRI30
        // AIzaSyA8eiZmM1FaDVjRy-df2KTyQ_vz_yYM39w
        // AIzaSyC8UYZpvA2eknNex0Pjid0_eTLJoDu6los
        // AIzaSyCjc_pVEDi4qsv5MtC2dMXzpIaDoRFLsxw
        // AIzaSyDHQ9ipnphqTzDqZsbtd8_Ru4_kiKVQe2k
        url_query_params.append(qMakePair(QByteArray("contentCheckOk"), QByteArray("True")));
        url_query_params.append(qMakePair(QByteArray("racyCheckOk"), QByteArray("True")));

        QString info_url = QString(YT_BASE) + "player";
        if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable))
            video_info = data.toLocal8Bit();
        //else {
        //    QStringList keywords = { "embedded", "detailpage", "vevo", "" };
        //    for (int i = 0; i < keywords.size(); i++) {
        //        QString video_info_url = "https://www.youtube.com/get_video_info?";
        //        video_info_url.append("video_id=" + video_id_);
        //        video_info_url.append("&ps=default");
        //        video_info_url.append("&eurl=");
        //        video_info_url.append("&gl=US");
        //        video_info_url.append("&hl=en");

        //        if (!keywords[i].isEmpty())
        //            video_info_url.append("&el=" + keywords[i]);
        //        if (!time_stamp.isEmpty())
        //            video_info_url.append("&sts=" + time_stamp);
        //        QString info;
        //        if (downloadToString(video_info_url, info))
        //            info = QUrl::fromPercentEncoding(info.toLatin1());
        //        if (info.contains("itag")
        //            && (info.contains("adaptive_fmts=") || info.contains("url_encoded_fmt_stream_map="))) {
        //            video_info = info;
        //            break;
        //        }
        //    }
        //}
    }
}

void YoutubeParser::getFormats(const QString& html, QJsonArray& formats) {
    QRegExp expression = QRegExp("\"formats\":\\s*\\[([^\]]+)[\]]");
    expression.setMinimal(true);
    if (expression.indexIn(html) != -1 && expression.cap(1) != "") {
        QString input = "[" + expression.cap(1).replace(("\\\\"), "\\") + "]";
        QJsonParseError parseError;
        QJsonArray parsed_formats = QJsonDocument::fromJson(input.toLatin1(), &parseError).array();
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Error parsing adaptive format JSON data" << parseError.errorString() << parseError.offset;
            qDebug() << input.mid(parseError.offset > 16 ? parseError.offset - 16 : 0, 32);
        }

        for (int i = 0; i < parsed_formats.count(); i++) {
            formats << parsed_formats.at(i);
        }
    }

    expression = QRegExp("\"adaptiveFormats\":\\s*\\[([^\]]+)[\]]");
    expression.setMinimal(true);
    if (expression.indexIn(html) != -1 && expression.cap(1) != "") {
        QString input = "[" + expression.cap(1).replace(("\\\\"), "\\") + "]";

        QJsonParseError parseError;
        QJsonArray parsed_adaptiveFormats = QJsonDocument::fromJson(input.toLatin1(), &parseError).array();
        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Error parsing format JSON data" << parseError.errorString() << parseError.offset;
            qDebug() << input.mid(parseError.offset > 16 ? parseError.offset - 16 : 0, 32);
        }

        for (int i = 0; i < parsed_adaptiveFormats.count(); i++) {
            formats << parsed_adaptiveFormats.at(i);
        }
    }
}

Status YoutubeParser::getTracksData(const QString& html, TracksInfo& tracks_info, VideoInfo& video_info, Abortable* abortable) {
    Status status;

    QRegExp expression;
    QString player;
    QString js_data_type;
    QString signature_method;
    QList<QStringList> dash_quality_links;
    QString received_data_type;
    QString mpeg_dash_data_type;
    QString youtubePlayerUrl;
    QString youtubePlayerJS;
    QString youtubePlayerSignatureMethodName;
    QString html5PlayerUrl;

    QMap<QString, QString> download_required;

    status = mainParsePage(html, js_data_type, signature_method, dash_quality_links, received_data_type, mpeg_dash_data_type, download_required, youtubePlayerUrl, youtubePlayerJS, youtubePlayerSignatureMethodName, html5PlayerUrl);

    //Check if additional urls need to be downloaded
    if (!download_required.empty()) {
        QString player_url;
        QString key;
        //Always download player first
        if (download_required.contains("js")) {
            key = "js";
            player_url = download_required["js"];
        }
        else {
            key = download_required.begin().key();
            player_url = download_required.begin().value();
        }

        //Signature parsing for MPEG-DASH MPD
        if (key == "dashmpd") {
            QStringList urlFragments = player_url.split("/");
            int signaturePosition = urlFragments.indexOf("s");
            if (signaturePosition > -1) {
                int signatureParameterPosition = urlFragments.indexOf("sp");
                
                if (signatureParameterPosition > -1)
                    urlFragments.replace(signaturePosition, urlFragments.at(signatureParameterPosition + 1));
                else
                    urlFragments.replace(signaturePosition, "signature");
                
                QString script = js_data_type + signature_method + "(\"" + QUrl::fromPercentEncoding(urlFragments.at(signaturePosition + 1).toLatin1()) + "\")";
                urlFragments.replace(signaturePosition + 1, runSyncScript(script, abortable));
                player_url = urlFragments.join("/");
            }
        }

        received_data_type = key;
        download_required.remove(key);

        if (!downloadToString(player_url, player) || player.isEmpty(), this)
            status.update(EStatusCode::ParseError, "Could not retrieve video link.");
        else
            status = mainParsePage(html, js_data_type, signature_method, dash_quality_links, received_data_type, mpeg_dash_data_type, download_required, youtubePlayerUrl, youtubePlayerJS, youtubePlayerSignatureMethodName, html5PlayerUrl, player);
    }

    expression = QRegExp("<meta name=\"title\" content=\"(.*)\"");
    expression.setMinimal(true);

    QString title;
    if (expression.indexIn(html) != -1) {
        title = expression.cap(1);
    }
    else {
        expression = QRegExp("\\\"title\\\":\\\"(.*)\\\"");
        expression.setMinimal(true);
        if (expression.indexIn(html) != -1)
            title = expression.cap(1);
    }

    QList<videoQuality> added_video_qualities;
    QList<audioQuality> added_audio_qualities;

    if (!title.isEmpty()) {
        title = title.replace("&amp;quot;", "\"").replace("&amp;amp;", "&").replace("&#39;", "'").replace("&quot;", "\"").replace("&amp;", "&");
        video_info.video_title = normalizeTitle(title);
        
       QStringList qualityLinks;
        QJsonArray formats;
        getFormats(html, formats);

        if (formats.isEmpty()) {//Try to get videoinfo
            QString video_info;
            getYouTubeVideoInfo(html, player, video_info, abortable);
            if (!video_info.isEmpty())
                getFormats(video_info, formats);
        }

        if (!formats.isEmpty()) {
            for (int i = 0; i < formats.size(); i++) {
                QJsonObject adaptiveFormat = formats.at(i).toObject();
                QStringList qualityLinkSegements;
                QStringList intKeys(QStringList() << "itag" << "bitrate" << "width" << "height" <<"averageBitrate" << "audioChannels" << "fps");
                QStringList stringKeys(QStringList() << "url" << "contentLength");
                QStringList appendKeys(QStringList() << "cipher" << "signatureCipher");
                for (int j = 0; j < adaptiveFormat.keys().length(); j++) {
                    QString key = adaptiveFormat.keys().at(j);

                    // FORMAT_STREAM_TYPE_OTF throws 404 errors
                    if (key == "type" && adaptiveFormat.value(key).toString().toUpper() == "FORMAT_STREAM_TYPE_OTF") {
                        qualityLinkSegements.clear();
                        break;
                    }

                    if (intKeys.contains(key)) {
                        qualityLinkSegements << key + "=" + QString::number(adaptiveFormat.value(key).toInt());
                    }
                    else if (stringKeys.contains(key)) {
                        qualityLinkSegements << key + "=" + adaptiveFormat.value(key).toString();
                    }
                    else if (appendKeys.contains(key)) {
                        qualityLinkSegements << QUrl::fromPercentEncoding(adaptiveFormat.value(key).toString().replace("&", ",").toLatin1());
                    }
                    //else {
                    //    qualityLinkSegements << key + "=" + QUrl::toPercentEncoding(adaptiveFormat.value(key).toString());
                    //}
                }
                if (!qualityLinkSegements.isEmpty())
                    qualityLinks << qualityLinkSegements.join(",");
            }
        }

        QRegularExpression expression_vid("\"t\": \"(.*)\"", QRegularExpression::PatternOption::CaseInsensitiveOption);
        QRegularExpressionMatch match_vid = expression_vid.match(html);

        QString video_data;
        if (!qualityLinks.isEmpty() 
            || !dash_quality_links.isEmpty()) {

            //get separated audio tracks
            QList<possibleAudioQuality> audio_qualities;
            fill_avc_av1_audio_quality_list(audio_qualities);
            fill_webm_audio_quality_list(audio_qualities);

            QMutableListIterator<possibleAudioQuality> j(audio_qualities);
            j.toBack();
            while (j.hasPrevious()) {
                possibleAudioQuality quality = j.previous();

                QString audio_link;
                QString audio_data = getContentQualityUrl(qualityLinks, quality.itag);
                if (!audio_data.isEmpty())
                    audio_link = getUrlFromQualityLink(audio_data, abortable);

                //try get link from mpeg-Dash
                if (audio_link.isEmpty() && 
                    !dash_quality_links.isEmpty())
                    audio_link = getContenDashQualityUrl(dash_quality_links, quality.itag);//works only for audio streams now

                if (!audio_link.isEmpty())
                    createAudioTrack(audio_data, audio_link, quality, added_audio_qualities, tracks_info);
            }

            //get video tracks
            QList<possibleVideoQuality> video_qualities;
            fill_video_quality_list(video_qualities);

            QMutableListIterator<possibleVideoQuality> i(video_qualities);
            i.toBack();
            QMap<QString, QString> audioLinks;
            while (i.hasPrevious()) {
                possibleVideoQuality quality = i.previous();

                QString video_link;
                video_data = getContentQualityUrl(qualityLinks, quality.itag);
                if (!video_data.isEmpty())
                    video_link = getUrlFromQualityLink(video_data, abortable);

                //try get link from mpeg-Dash
                //if (video_link.isEmpty())
                //    video_link = getContenDashQualityUrl(dash_quality_links, quality.itag);//works only for audio streams now
                
                if (!video_link.isEmpty())
                    createVideoTrack(video_data, video_link, quality, added_video_qualities, tracks_info);
            }
        }
        else if (match_vid.hasMatch()
            && !video_id_.isEmpty()) {
            QString video_link = QUrl::fromEncoded(QString(QString(YT_GET_VIDEO) + "video_id = " + video_id_ + " & t = " + expression.cap(1)).toLatin1()).toString(QUrl::None);
            if (video_link.isEmpty()) {
                createVideoTrack(video_data, video_link, possibleVideoQuality(360, 30, "43", VideoProfile::AVC), added_video_qualities, tracks_info);//TODO set right quality
            }
            else {
                status.update(EStatusCode::ParseError, "Could not retrieve video link.");
                return status;
            }
        }
        else {
            status.update(EStatusCode::ParseError, "Could not retrieve video link.");
            return status;
        }
    }
    else {
        status.update(EStatusCode::ParseError, "Could not retrieve video link.");
        return status;
    }
    
    //get subtitles tracks
    if (!tracks_info.video_tracks.isEmpty()) {
        QRegExp expression = QRegExp("\"captionTracks\":\\[([^\]]+)[\]]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1) {
            QString caption_tracks_block = expression.cap(1).replace("\\u0026", "&");
            if (!caption_tracks_block.isEmpty()) {
                
                //get normal subtitles
                QRegularExpression re_url("\"baseUrl\":\"([^\"]+)[\"]", QRegularExpression::DotMatchesEverythingOption);
                QRegularExpressionMatchIterator i = re_url.globalMatch(caption_tracks_block);
                QList<QString> lang_tags;
                while (i.hasNext()) {
                    QRegularExpressionMatch match = i.next();
                    QString url = match.captured(1);
                    if (!url.isEmpty()) {
                        url.append("&fmt=vtt");
                        QString lang_tag = url.split("&lang=").last().split("&").first().toLower();
                        if (!url.contains("kind=asr") //auto-generated
                            && !lang_tags.contains(lang_tag)) { 

                            SubtitleTrackInfo track;
                            track.language_tag = SS2Q(normalizeLangTag(QS2S(lang_tag)));
                            track.profile = SubtitleTrackInfo::SubtitleProfile::WEBVTT;
                            track.urls.append(url);

                            //normalizeTrack(track);
                            tracks_info.subtitle_tracks.append(track);
                            lang_tags << lang_tag;
                        }
                    }
                }

                //get auto-generated subtitles
                expression = QRegExp("\"translationLanguages\":\\[([^\]]+)[\]]", Qt::CaseInsensitive);
                if (expression.indexIn(html) != -1) {
                    QString translation_langs = expression.cap(1);
                    QStringList lang_codes;
                    for (const auto& it : translation_langs.split("},{")) {
                        if (it.contains("languageCode")) {
                            QString tag = it.split(",").first().split(":").last().replace("\"", "").toLower();
                            QString language = it.split(",").last().split(":").last().split("}").first().replace("\"", "");
                            lang_codes.push_back(tag + ":" + language);
                        }
                    }
                    lang_codes.removeAll("");
                    lang_codes.removeDuplicates();

                    if (!lang_codes.isEmpty()) {
                        QString base_url = caption_tracks_block.split("\"baseUrl\":\"").last().split("\",").first();

                        if (!base_url.isEmpty()) {
                            QList<QString> langs;
                            for (QList<SubtitleTrackInfo::SubtitleProfile>::iterator it_fmt = youtube_tag_formats_.begin(); it_fmt != youtube_tag_formats_.end(); it_fmt++) {
                                for (auto& it : lang_codes) {
                                    it.replace("\"", "");

                                    QString lang = it.split(":").first();

                                    if (!langs.contains(lang)) {
                                        SubtitleTrackInfo track;
                                        QString tag = lang.toLower();
                                        track.language_tag = SS2Q(normalizeLangTag(QS2S(tag)));

                                        track.language = it.split(":").last();
                                        track.is_additional = true;
                                        track.profile = *it_fmt;

                                        QString track_url = base_url;
                                        track_url.append("&tlang=" + track.language_tag);
                                        track_url.append("&fmt=" + getSubtitleTrackExt(track.profile));
                                        track.urls.append(track_url);

                                        //normalizeTrack(track);
                                        tracks_info.subtitle_tracks.append(track);
                                        langs << lang;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    qDebug() << "YoutubeParser: parsed JS tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size() << " subtitles " << tracks_info.subtitle_tracks.size();

    return status;
}

void YoutubeParser::extractVideosAndContPlaylist(QJsonArray contents, QList<playlistVideoData>* video_data, QString* continuation) {
    QString newContinuation = "";

    for (const auto& e : contents) {
        QJsonObject asObj = e.toObject();
        if (!asObj.isEmpty()) {
            if (asObj.contains("continuationItemRenderer")) {
                QJsonObject continuationItemRenderer = asObj["continuationItemRenderer"].toObject();
                if (!continuationItemRenderer.isEmpty()) {
                    QJsonObject continuationEndpoint = continuationItemRenderer["continuationEndpoint"].toObject();
                    if (!continuationEndpoint.isEmpty()) {
                        QJsonObject continuationCommand = continuationEndpoint["continuationCommand"].toObject();
                        if (!continuationCommand.isEmpty())
                            newContinuation = continuationCommand["token"].toString();
                    }
                }
            }
            else {
                QJsonObject playlistVideoRenderer = asObj["playlistVideoRenderer"].toObject();
                if (!playlistVideoRenderer.isEmpty()) {
                    playlistVideoData data;

                    data.video_id = playlistVideoRenderer["videoId"].toString();
                    auto title = playlistVideoRenderer["title"].toObject();
                    if (!title.isEmpty()) {
                        auto runs = title["runs"].toArray();
                        if (!runs.isEmpty()) 
                            data.video_title = runs[0].toObject().value("text").toString();
                    }
                    int seconds = playlistVideoRenderer["lengthSeconds"].toString().toInt();
                    data.video_duration = QDateTime::fromTime_t(seconds).toUTC().toString(seconds < 3600 ? "mm:ss" : "hh:mm:ss");

                    video_data->push_back(data);
                }
                else {
                    QJsonObject richItemRenderer = asObj["richItemRenderer"].toObject();
                    if (!richItemRenderer.isEmpty()) {
                        QJsonObject content = richItemRenderer["content"].toObject();
                        if (!content.isEmpty()) {;
                            QJsonObject reelItemRenderer = content["reelItemRenderer"].toObject();
                            if (!reelItemRenderer.isEmpty()) {
                                playlistVideoData data;
                                data.video_id = reelItemRenderer["videoId"].toString();
                                auto title = reelItemRenderer["title"].toObject();
                                if (!title.isEmpty()) {
                                    auto runs = title["runs"].toArray();
                                    if (!runs.isEmpty())
                                        data.video_title = runs[0].toObject().value("text").toString();
                                }

                                if (title.isEmpty()) {
                                    QJsonObject headline = reelItemRenderer["headline"].toObject();
                                    if (!headline.isEmpty())
                                        data.video_title = headline["simpleText"].toString();
                                }

                                int seconds = reelItemRenderer["lengthSeconds"].toString().toInt();
                                data.video_duration = QDateTime::fromTime_t(seconds).toUTC().toString(seconds < 3600 ? "mm:ss" : "hh:mm:ss");

                                video_data->push_back(data);
                            }
                        }
                    }
                }
            }
        }
    }

    continuation->clear();
    continuation->append(newContinuation);
}

void YoutubeParser::getPlaylist(QString playlist_id, QList<playlistVideoData>* video_data, QString* continuation, Abortable* abortable) {
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject jsonClient;
    jsonClient["clientName"] = "WEB";
    jsonClient["clientVersion"] = "2.20201209.01.00";
    jsonClient["hl"] = "en";
    jsonClient["gl"] = "US";

    QJsonObject jsonContext;
    jsonContext["client"] = jsonClient;
    
    QJsonObject reqJson;
    reqJson["context"] = jsonContext;
    reqJson["browseId"] = "VL" + playlist_id;

    QJsonDocument doc(reqJson);
    QByteArray body = doc.toJson();

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable)) {
#if _DEBUG
        QFile   file("playlist.json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            const auto contents = jsonResponseObj["contents"].toObject();
            if (!contents.isEmpty()) {
                const auto twoColumnBrowseResultsRenderer = contents["twoColumnBrowseResultsRenderer"].toObject();
                if (!twoColumnBrowseResultsRenderer.isEmpty()) {
                    const auto tabs = twoColumnBrowseResultsRenderer["tabs"].toArray();
                    if (!tabs.isEmpty()) {
                        for (const auto& t : tabs) {
                            const auto tabRenderer = t.toObject()["tabRenderer"].toObject();
                            if (!tabRenderer.isEmpty()) {
                                const auto content = tabRenderer["content"].toObject();
                                if (!content.isEmpty()) {
                                    const auto sectionListRenderer = content["sectionListRenderer"].toObject();
                                    if (!sectionListRenderer.isEmpty()) {
                                        const auto contents = sectionListRenderer["contents"].toArray();
                                        if (!contents.isEmpty()) {
                                            for (const auto& c : contents) {
                                                const auto itemSectionRenderer = c.toObject()["itemSectionRenderer"].toObject();
                                                if (!itemSectionRenderer.isEmpty()) {
                                                    const auto contents = itemSectionRenderer["contents"].toArray();
                                                    if (!contents.isEmpty()) {
                                                        for (const auto& m : contents) {
                                                            auto playlistVideoListRenderer = m.toObject()["playlistVideoListRenderer"].toObject();
                                                            
                                                            if (playlistVideoListRenderer.isEmpty())
                                                                playlistVideoListRenderer = m.toObject()["richGridRenderer"].toObject();

                                                            if (!playlistVideoListRenderer.isEmpty()) {
                                                                const auto contents = playlistVideoListRenderer["contents"].toArray();
                                                                if (!contents.isEmpty())
                                                                    extractVideosAndContPlaylist(contents, video_data, continuation);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void YoutubeParser::getPlaylistContinuation(QString continuation, QList<playlistVideoData>* video_data, QString* newContinuation, Abortable* abortable) {
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject jsonClient;
    jsonClient["clientName"] = "WEB";
    jsonClient["clientVersion"] = "2.20201209.01.00";
    jsonClient["hl"] = "en";
    jsonClient["gl"] = "US";

    QJsonObject jsonContext;
    jsonContext["client"] = jsonClient;

    QJsonObject reqJson;
    reqJson["context"] = jsonContext;
    reqJson["continuation"] = continuation;

    QJsonDocument doc(reqJson);
    QByteArray body = doc.toJson();

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("playlist_continuation_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            const auto onResponseReceivedActions = jsonResponseObj["onResponseReceivedActions"].toArray();
            if (!onResponseReceivedActions.isEmpty()) {
                for (const auto& a : onResponseReceivedActions) {
                    const auto appendContinuationItemsAction = a.toObject()["appendContinuationItemsAction"].toObject();
                    if (!appendContinuationItemsAction.isEmpty()) {
                        const auto continuationItems = appendContinuationItemsAction["continuationItems"].toArray();
                        if (!continuationItems.isEmpty())
                            extractVideosAndContPlaylist(continuationItems, video_data, newContinuation);
                    }
                }
            }
        }
    }
}

void YoutubeParser::parseChannels(const QString& json, QStringList& channel_ids) {
    QRegularExpression regexChannelId("\"browseEndpoint\":\\s*{\\s*\"browseId\":\\s*\"([^\"]+)\",\\s*\"canonicalBaseUrl\":\\s*\"[^\"]+\"\\s*}\\s*},\\s*\"title\"");
    QRegularExpressionMatchIterator matchIterator = regexChannelId.globalMatch(json);

    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        if (match.hasMatch()) {
            QString channel_id = match.captured(1);
            channel_ids.append(channel_id);
        }
    }
}

Status YoutubeParser::getChannelChannels(const QString& channel_id, QStringList& channel_ids, Abortable* abortable) {
    Status status;
    qDebug() << "getChannelChannels channel_id: " << channel_id;
    if (channel_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };

    QString response;
    QString continuation;

    if (requestBrowse(QString("\"browseId\": \"%1\",\"params\": \"%2\"").arg(channel_id, CHANNEL_CHANNELS_PARAM), response, abortable)) {
        parseChannels(response, channel_ids);
        if (channel_ids.isEmpty())
            status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

void YoutubeParser::extractVideosAndContChannel(QJsonArray contents, QStringList* video_ids, QString* continuation) {
    QString newContinuation = "";
    
    for (const auto& e : contents) {
        QJsonObject asObj = e.toObject();
        if (!asObj.isEmpty()) {
            if (asObj.contains("continuationItemRenderer")) {
                QJsonObject continuationItemRenderer = asObj["continuationItemRenderer"].toObject();
                if (!continuationItemRenderer.isEmpty()) {
                    QJsonObject continuationEndpoint = continuationItemRenderer["continuationEndpoint"].toObject();
                    if (!continuationEndpoint.isEmpty()) {
                        QJsonObject continuationCommand = continuationEndpoint["continuationCommand"].toObject();
                        if (!continuationCommand.isEmpty())
                            newContinuation = continuationCommand["token"].toString();
                    }
                }
            }
            else {
                QJsonObject richItemRenderer = asObj["richItemRenderer"].toObject();
                if (!richItemRenderer.isEmpty()) {
                    QJsonObject content = richItemRenderer["content"].toObject();
                    if (!content.isEmpty()) {
                        QJsonObject videoRenderer = content["videoRenderer"].toObject();
                        if (!videoRenderer.isEmpty())
                            video_ids->append(videoRenderer["videoId"].toString());
                    }
                }
            }
        }
    }

    continuation->clear();
    continuation->append(newContinuation);
}

void YoutubeParser::getChannel(QString channel_id, QStringList* video_ids, QString* continuation, Abortable* abortable) {
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject jsonClient;
    jsonClient["clientName"] = "WEB";
    jsonClient["clientVersion"] = "2.20201209.01.00";
    jsonClient["hl"] = "en";
    jsonClient["gl"] = "US";

    QJsonObject jsonContext;
    jsonContext["client"] = jsonClient;

    QJsonObject reqJson;
    reqJson["context"] = jsonContext;
    reqJson["browseId"] = channel_id;
    reqJson["params"] = CHANNEL_VIDEOS_PARAM;

    QJsonDocument doc(reqJson);
    QByteArray body = doc.toJson();

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable)) {
#if _DEBUG
        QFile   file("channel.json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            const auto contents = jsonResponseObj["contents"].toObject();
            if (!contents.isEmpty()) {
                const auto twoColumnBrowseResultsRenderer = contents["twoColumnBrowseResultsRenderer"].toObject();
                if (!twoColumnBrowseResultsRenderer.isEmpty()) {
                    const auto tabs = twoColumnBrowseResultsRenderer["tabs"].toArray();
                    for (const auto &t : tabs) {
                        const auto tabRenderer = t.toObject()["tabRenderer"].toObject();
                        if (!tabRenderer.isEmpty()) {
                            const auto content = tabRenderer["content"].toObject();
                            if (!content.isEmpty()) {
                                const auto richGridRenderer = content["richGridRenderer"].toObject();
                                if (!richGridRenderer.isEmpty()) {
                                    const auto contents = richGridRenderer["contents"].toArray();
                                    if (!contents.isEmpty())
                                        extractVideosAndContChannel(contents, video_ids, continuation);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void YoutubeParser::getChannelContinuation(QString continuation, QStringList* video_ids, QString* newContinuation, Abortable* abortable) {
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject jsonClient;
    jsonClient["clientName"] = "WEB";
    jsonClient["clientVersion"] = "2.20201209.01.00";
    jsonClient["hl"] = "en";
    jsonClient["gl"] = "US";

    QJsonObject jsonContext;
    jsonContext["client"] = jsonClient;

    QJsonObject reqJson;
    reqJson["context"] = jsonContext;
    reqJson["params"] = CHANNEL_VIDEOS_PARAM;
    reqJson["continuation"] = continuation;

    QJsonDocument doc(reqJson);
    QByteArray body = doc.toJson();

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("channel_continuation_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            const auto onResponseReceivedActions = jsonResponseObj["onResponseReceivedActions"].toArray();
            for (const auto& r : onResponseReceivedActions) {
                const auto appendContinuationItemsAction = r["appendContinuationItemsAction"].toObject();
                if (!appendContinuationItemsAction.isEmpty()) {
                    const auto continuationItems = appendContinuationItemsAction["continuationItems"].toArray();
                    if (!continuationItems.isEmpty())
                        extractVideosAndContChannel(continuationItems, video_ids, newContinuation);
                }
            }
        }
    }
}

void YoutubeParser::extractVideosAndContQuery(QJsonArray contents, QStringList* video_ids, QString* continuation) {
    QString newContinuation = "";

    if (contents.count() == 2) {
        const auto contents1 = contents[1].toObject();
        if (!contents1.isEmpty()) {
            const auto continuationItemRenderer = contents1["continuationItemRenderer"].toObject();
            if (!continuationItemRenderer.isEmpty()) {
                const auto continuationEndpoint = continuationItemRenderer["continuationEndpoint"].toObject();
                if (!continuationEndpoint.isEmpty()) {
                    const auto continuationCommand = continuationEndpoint["continuationCommand"].toObject();
                    if (!continuationCommand.isEmpty())
                        newContinuation = continuationCommand["token"].toString();
                }
            }
        }
    }
    
    QJsonArray items;
    if (!contents.isEmpty()) {
        const auto contents0 = contents[0].toObject();
        if (!contents0.isEmpty()) {
            const auto itemSectionRenderer = contents0["itemSectionRenderer"].toObject();
            if (!itemSectionRenderer.isEmpty())
                items = itemSectionRenderer["contents"].toArray();
        }
    }

    for (const auto& e : items) {
        QJsonObject asObj = e.toObject();
        if (!asObj.isEmpty()) {
            const auto videoRenderer = asObj["videoRenderer"].toObject();
            if (!videoRenderer.isEmpty())
                video_ids->append(videoRenderer["videoId"].toString());
        }
    }

    continuation->clear();
    continuation->append(newContinuation);
}

void YoutubeParser::getQuery(QString query, QStringList* video_ids, QString* continuation, Abortable* abortable) {
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject jsonClient;
    jsonClient["clientName"] = "WEB";
    jsonClient["clientVersion"] = "2.20201209.01.00";
    jsonClient["hl"] = "en";
    jsonClient["gl"] = "US";

    QJsonObject jsonContext;
    jsonContext["client"] = jsonClient;

    QJsonObject reqJson;
    reqJson["context"] = jsonContext;
    reqJson["params"] = "EgIQAQ==";
    reqJson["query"] = query;

    QJsonDocument doc(reqJson);
    QByteArray body = doc.toJson();

    QString info_url = QString(YT_BASE) + "search";
    if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable)) {
#if _DEBUG
        QFile   file("search.json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            const auto contents = jsonResponseObj["contents"].toObject();
            if (!contents.isEmpty()) {
                const auto twoColumnSearchResultsRenderer = contents["twoColumnSearchResultsRenderer"].toObject();
                if (!twoColumnSearchResultsRenderer.isEmpty()) {
                    const auto primaryContents = twoColumnSearchResultsRenderer["primaryContents"].toObject();
                    if (!primaryContents.isEmpty()) {
                        const auto sectionListRenderer = primaryContents["sectionListRenderer"].toObject();
                        if (!sectionListRenderer.isEmpty()) {
                            const auto contents = sectionListRenderer["contents"].toArray();
                            if (!contents.isEmpty())
                                extractVideosAndContQuery(contents, video_ids, continuation);
                        }
                    }
                }
            }
        }
    }
}

void YoutubeParser::getQueryContinuation(QString continuation, QStringList* video_ids, QString* newContinuation, Abortable* abortable) {
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject jsonClient;
    jsonClient["clientName"] = "WEB";
    jsonClient["clientVersion"] = "2.20201209.01.00";
    jsonClient["hl"] = "en";
    jsonClient["gl"] = "US";

    QJsonObject jsonContext;
    jsonContext["client"] = jsonClient;

    QJsonObject reqJson;
    reqJson["context"] = jsonContext;
    reqJson["params"] = "EgIQAQ==";
    reqJson["continuation"] = continuation;

    QJsonDocument doc(reqJson);
    QByteArray body = doc.toJson();

    QString info_url = QString(YT_BASE) + "search";
    if (downloadToStringPost(info_url, body, data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("search_continuation_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            const auto onResponseReceivedCommands = jsonResponseObj["onResponseReceivedCommands"].toArray();
            for (const auto& c : onResponseReceivedCommands) {
                const auto appendContinuationItemsAction = c["appendContinuationItemsAction"].toObject();
                if (!appendContinuationItemsAction.isEmpty()) {
                    const auto continuationItems = appendContinuationItemsAction["continuationItems"].toArray();
                    if (!continuationItems.isEmpty())
                        extractVideosAndContQuery(continuationItems, video_ids, newContinuation);
                }
            }
        }
    }
}

Status YoutubeParser::downloadSearch(const QString& url, Abortable* abortable) {
    Status status;
    qDebug() << "downloadSearch url:" << url;

    QStringList all_ids;
    QStringList video_ids;
    QString continuation;
    QString first_link;

    getQuery(url, &video_ids, &continuation, abortable);
    
    if (!video_ids.isEmpty()) {
        all_ids << video_ids;

        if (continuation != "")
            first_link = video_ids.takeFirst();

        loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");

        while (continuation != ""
            && all_ids.length() < SEARCH_MAXIMUM_ITEMS) {
            getQueryContinuation(continuation, &video_ids, &continuation, abortable);
            qDebug() << video_ids.count();
            all_ids << video_ids;

            if (continuation == ""
                || all_ids.length() >= SEARCH_MAXIMUM_ITEMS) {
                video_ids.push_front(first_link);
                continuation = "";
            }

            loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");
        }
        
    }
    else {
        status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

Status YoutubeParser::downloadVideos(const QString& channel_id, Abortable* abortable) {
    Status status;
    qDebug() << "downloadVideos channel_id: " << channel_id;
    if (channel_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };
    
    QStringList video_ids;
    QString continuation;
    QString first_link;

    if (!channel_id.isEmpty()) {
        getChannel(channel_id, &video_ids, &continuation, abortable);

        if (!video_ids.isEmpty()) {
            if (continuation != "")
                first_link = video_ids.takeFirst();

            loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");

            while (continuation != "") {
                qDebug() << video_ids.length();
                getChannelContinuation(continuation, &video_ids, &continuation, abortable);

                if (continuation == "")
                    video_ids.push_front(first_link);

                loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");
            }
        }
        else {
            status.update(EStatusCode::ParseError, "Empty playlist");
        }
    }
    else {
        status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

Status YoutubeParser::downloadPlaylist(const QString& playlist_id, VideoInfo& video_info, bool fill_playlist, Abortable* abortable) {
    Status status;
    qDebug() << "downloadPlaylist playlist_id: " << playlist_id;
    if (playlist_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };

    QList<playlistVideoData> video_data;
    QString continuation;
    playlistVideoData first_link;

    getPlaylist(playlist_id, &video_data, &continuation, abortable);

    if (!video_data.isEmpty()) {
        if (!playlist_id.isEmpty()) {
            for (auto& id : video_data)
                id.video_id.append("&list=" + playlist_id);
        }

        QList<QPair<QString, QString>> episodes;
        QStringList video_ids;
        QStringList video_titles;
        for (const auto& id : video_data) {
            if (fill_playlist) {
                if (!video_titles.contains(id.video_title)) {
                    video_titles.push_back(id.video_title);

                    //if (id.video_id != video_id) {
                    QString title = normalizeTitle(id.video_title, 40);
                    QString ep_data = title + " " + id.video_duration;
                    episodes.push_back(QPair(ep_data, YT_WATCH + id.video_id + "&list=" + playlist_id));
                    //}
                }
            }
            else {
                if (continuation != "")
                    first_link = video_data.takeFirst();

                if (!video_ids.contains(id.video_id))
                    video_ids.push_back(id.video_id);
            }
        }

        while (continuation != "") {
            qDebug() << video_data.length();
            getPlaylistContinuation(continuation, &video_data, &continuation, abortable);

            if (!playlist_id.isEmpty()) {
                for (auto& id : video_data)
                    id.video_id.append("&list=" + playlist_id);
            }

            for (const auto& id : video_data) {
                if (fill_playlist) {
                    if (!video_titles.contains(id.video_title)) {
                        video_titles.push_back(id.video_title);

                        //if (id.video_id != video_id) {
                        QString title = normalizeTitle(id.video_title, 40);
                        QString ep_data = title + " " + id.video_duration;
                        episodes.push_back(QPair(ep_data, YT_WATCH + id.video_id + +"&list=" + playlist_id));
                        //}
                    }
                }
                else {
                    if (continuation != "")
                        first_link = video_data.takeFirst();

                    if (!video_ids.contains(id.video_id))
                        video_ids.push_back(id.video_id);
                }
            }
        }

        if (!video_ids.isEmpty())
            loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");

        if (!episodes.isEmpty()) {
            video_info.playlist.push_back(QPair((int)YtContentKind::kPLAYLIST, episodes));
#if _DEBUG
            unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
            QFile   file("episodes_" + QString::number(ms) + ".txt");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                for (const auto& ep : episodes) {
                    file.write(ep.first.toUtf8() + " : " + ep.second.toUtf8());
                    file.write("\n");
                }
                file.close();
            }
#endif
        }
    }
    else {
        status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

void YoutubeParser::addDefaultHeaders(QList<QPair<QByteArray, QByteArray>>&  headers) {
    headers.append(qMakePair(QByteArray("x-youtube-client-version"), QByteArray("2.20201209.01.00")));
    headers.append(qMakePair(QByteArray("x-youtube-client-name"), QByteArray("1")));
    headers.append(qMakePair(QByteArray("content-type"), QByteArray("application/json")));
}

void YoutubeParser::addDefaultQuery(QList<QPair<QByteArray, QByteArray>>& url_query_params) {
    url_query_params.append(qMakePair(QByteArray("key"), QByteArray("AIzaSyCjc_pVEDi4qsv5MtC2dMXzpIaDoRFLsxw")));
    url_query_params.append(qMakePair(QByteArray("prettyPrint"), QByteArray("false")));
}

QString YoutubeParser::getVisitorData(QNetworkAccessManager* mgr) {
    QNetworkRequest request(QUrl("https://www.youtube.com//sw.js_data"));
    request.setRawHeader("accept-language", "en-US");
    request.setRawHeader("user-agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 "
        "(KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36");
    request.setRawHeader("referer", "https://www.youtube.com/sw.js");

    QEventLoop loop;
    QNetworkReply* reply = mgr->get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll().trimmed().split('\n').last());
    QJsonArray jsonArray = jsonDoc.array();
    QString visitorData = jsonArray[0].toArray()[2].toArray()[0].toArray()[0].toArray()[13].toString();
    return visitorData;
}

QJsonObject YoutubeParser::find_tab(const QJsonArray& tabs, const QString& url_suffix) {
    for (const QJsonValue& t : tabs) {
        if (!t.toObject().contains("tabRenderer"))
            continue;

        QString url = t.toObject()["tabRenderer"]
            .toObject()["endpoint"]
            .toObject()["commandMetadata"]
            .toObject()["webCommandMetadata"]
            .toObject()["url"]
            .toString();
        if (url.split("/").last() == url_suffix)
            return t.toObject()["tabRenderer"].toObject();
    }

    return QJsonObject();
}

QString YoutubeParser::get_endpoint_param(QJsonObject tab) {
    return tab["endpoint"].toObject()["browseEndpoint"].toObject()["params"].toString();
}

void YoutubeParser::extractShorts(QJsonArray contents, QString* continuation, QStringList* links) {
    *continuation = "";
    for (const QJsonValue& e : contents) {
        if (e.toObject().contains("continuationItemRenderer")) {
            *continuation = e.toObject()["continuationItemRenderer"]
                .toObject()["continuationEndpoint"]
                .toObject()["continuationCommand"]
                .toObject()["token"]
                .toString();
        }
        else {
            links->append(e.toObject()["richItemRenderer"]
                .toObject()["content"]
                .toObject()["reelItemRenderer"]
                .toObject()["videoId"]
                .toString());
        }
    }
}

void YoutubeParser::getShorts(const QString& channel_id, QString endpoint_param, const QJsonObject& ctx,
    QString* continuation, QStringList* links, Abortable* abortable) {

    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["browseId"] = channel_id;
    payload["params"] = endpoint_param;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("shorts_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray tabs = jsonResponseObj["contents"].toObject()["twoColumnBrowseResultsRenderer"].toObject()["tabs"].toArray();
            QJsonObject t = find_tab(tabs, "shorts");
            QJsonArray contents = t["content"].toObject()["richGridRenderer"].toObject()["contents"].toArray();
            extractShorts(contents, continuation, links);
        }
    }
}

void YoutubeParser::getShortsContinuation(QString cont, const QJsonObject& ctx, QString* continuation,
    QStringList* links, Abortable* abortable) {

    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["continuation"] = cont;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("shorts_continuation" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray contents = jsonResponseObj["onResponseReceivedActions"]
                .toArray()[0]
                .toObject()["appendContinuationItemsAction"]
                .toObject()["continuationItems"]
                .toArray();
            extractShorts(contents, continuation, links);
        }
    }
}

Status YoutubeParser::downloadShorts(const QString& channel_id, Abortable* abortable) {
    Status status;
    qDebug() << "downloadShorts channel_id: " << channel_id;
    if (channel_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };

    QStringList video_ids;

    QJsonObject ctx = generateCtx(abortable);
    QJsonArray tabs = getTabs(ctx, channel_id, abortable);
    
    QString param;
    QJsonObject shorts_tab = find_tab(tabs, "shorts");
    if (!shorts_tab.isEmpty())
        param = get_endpoint_param(shorts_tab);

    if (param.isEmpty())
        param = QString(CHANNEL_SHORTS_PARAM);

    if (!param.isEmpty()) {
        QString continuation;
        QString first_link;
        
        getShorts(channel_id, param, ctx, &continuation, &video_ids, abortable);
        if (!video_ids.isEmpty()) {
            if (continuation != "")
                first_link = video_ids.takeFirst();

            loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");

            while (continuation != "") {
                getShortsContinuation(continuation, ctx, &continuation, &video_ids, abortable);
                qDebug() << video_ids.count();

                if (continuation == "")
                    video_ids.push_front(first_link);

                loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");
            }
        }
        else {
            status.update(EStatusCode::ParseError, "Empty playlist");
        }
    }
    else {
        qDebug() << "NO STREAMS ON CHANNEL";
        status.update(EStatusCode::ParseError, "Empty playlist");  
    }

    return status;
}

QString YoutubeParser::getVisitorData(Abortable* abortable) {
    QString visitor_data;
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("accept-language"), QByteArray("en-US")));
    headers.append(qMakePair(QByteArray("user-agent"), QByteArray("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36")));
    headers.append(qMakePair(QByteArray("referer"), QByteArray("https://www.youtube.com/sw.js")));

    QList<QPair<QByteArray, QByteArray>>  url_query_params;

    QString info_url = ("https://www.youtube.com//sw.js_data");
    if (downloadToStringExtended(info_url, data, headers, url_query_params, nullptr, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
            QFile   file("visitor" + QString::number(ms) + ".json");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                    file.write(data.toStdString().c_str());
                    file.close();
            }
#endif
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8().trimmed().split('\n').last());
        QJsonArray jsonArray = jsonDoc.array();
        visitor_data = jsonArray[0].toArray()[2].toArray()[0].toArray()[0].toArray()[13].toString();
    }

    return visitor_data;
}

QJsonObject YoutubeParser::generateCtx(Abortable* abortable) {
    QString visitorData = getVisitorData(abortable);
    QJsonObject context;
    QJsonObject client;
    client["clientName"] = "WEB";
    client["clientVersion"] = "2.20221220.09.00";
    client["visitorData"] = visitorData;
    client["hl"] = "en";
    client["gl"] = "US";
    context["client"] = client;
    return context;
}

QJsonArray YoutubeParser::getTabs(const QJsonObject& ctx, const QString& channel_id, Abortable* abortable) {
    QJsonArray tabs;
    
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["browseId"] = channel_id;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("tabs" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty())
            tabs = jsonResponseObj["contents"].toObject()["twoColumnBrowseResultsRenderer"].toObject()["tabs"].toArray();
    }

    return tabs;
}

void YoutubeParser::extractStreams(QJsonArray contents, QString* continuation, QStringList* videos) {
    *continuation = "";
    for (const QJsonValue& e : contents) {
        if (e.toObject().contains("continuationItemRenderer")) {
            *continuation = e.toObject()["continuationItemRenderer"]
                .toObject()["continuationEndpoint"]
                .toObject()["continuationCommand"]
                .toObject()["token"]
                .toString();
        }
        else {
            videos->append(e.toObject()["richItemRenderer"]
                .toObject()["content"]
                .toObject()["videoRenderer"]
                .toObject()["videoId"]
                .toString());
        }
    }
}

void YoutubeParser::getStreams(const QString& channel_id, const QString& endpoint_param, const QJsonObject& ctx, QString* continuation, QStringList* videos, Abortable* abortable) {
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["browseId"] = channel_id;
    payload["params"] = endpoint_param;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("streams_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray tabs = jsonResponseObj["contents"].toObject()["twoColumnBrowseResultsRenderer"].toObject()["tabs"].toArray();
            QJsonObject t = find_tab(tabs, "streams");
            QJsonArray contents = t["content"].toObject()["richGridRenderer"].toObject()["contents"].toArray();
            extractStreams(contents, continuation, videos);
        }
    }
}

void YoutubeParser::getStreamsContinuation(const QString& cont, const QJsonObject& ctx, QString* continuation, QStringList* videos, Abortable* abortable) {
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["continuation"] = cont;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("streams_continuation" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray contents = jsonResponseObj["onResponseReceivedActions"].toArray()[0].toObject()["appendContinuationItemsAction"].toObject()["continuationItems"].toArray();
            extractStreams(contents, continuation, videos);
        }
    }
}

Status YoutubeParser::downloadStreams(const QString& channel_id, Abortable* abortable) {
    Status status;
    qDebug() << "downloadStreams channel_id: " << channel_id;
    if (channel_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };

    QStringList video_ids;

    QJsonObject ctx = generateCtx(abortable);
    QJsonArray tabs = getTabs(ctx, channel_id, abortable);

    QJsonObject streams_tab = find_tab(tabs, "streams");
    if (!streams_tab.isEmpty()) {
        QString param = get_endpoint_param(streams_tab);
        QString continuation;
        QString first_link;

        getStreams(channel_id, param, ctx, &continuation, &video_ids, abortable);
        if (!video_ids.isEmpty()) {
            if (continuation != "")
                first_link = video_ids.takeFirst();

            loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");

            while (continuation != "") {
                getStreamsContinuation(continuation, ctx, &continuation, &video_ids, abortable);
                qDebug() << video_ids.count();

                if (continuation == "")
                    video_ids.push_front(first_link);

                loadPlaylist(EVideoIdType::kVideoId, video_ids, continuation == "");
            }
        }
        else {
            status.update(EStatusCode::ParseError, "Empty playlist");
        }
    }
    else {
        qDebug() << "NO STREAMS ON CHANNEL";
        status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

void YoutubeParser::extractPlaylists(QJsonArray contents, QString* continuation, QStringList* video_ids) {
    *continuation = "";
    for (const QJsonValue& e : contents) {
        if (e.toObject().contains("continuationItemRenderer")) {
            *continuation = e.toObject()["continuationItemRenderer"]
                .toObject()["continuationEndpoint"]
                .toObject()["continuationCommand"]
                .toObject()["token"]
                .toString();
        }
        else {
            video_ids->append(e.toObject()["gridPlaylistRenderer"].toObject()["playlistId"].toString());
        }
    }
}

void YoutubeParser::getPlaylists(QString channel_id, QString endpoint_param, QJsonObject ctx, QString* continuation, QStringList* video_ids, Abortable* abortable) {
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["browseId"] = channel_id;
    payload["params"] = endpoint_param;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("playlists_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray tabs = jsonResponseObj["contents"].toObject()["twoColumnBrowseResultsRenderer"].toObject()["tabs"].toArray();
            QJsonObject t = find_tab(tabs, "playlists");
            QJsonArray contents = t["content"]
                .toObject()["sectionListRenderer"]
                .toObject()["contents"]
                .toArray()[0]
                .toObject()["itemSectionRenderer"]
                .toObject()["contents"]
                .toArray()[0]
                .toObject()["gridRenderer"]
                .toObject()["items"]
                .toArray();

            if (contents.isEmpty())
                contents = t["content"]
                .toObject()["sectionListRenderer"]
                .toObject()["contents"]
                .toArray()[0]
                .toObject()["itemSectionRenderer"]
                .toObject()["contents"]
                .toArray()[0]
                .toObject()["shelfRenderer"]
                .toObject()["content"]
                .toObject()["horizontalListRenderer"]
                .toObject()["items"]
                .toArray();

            extractPlaylists(contents, continuation, video_ids);
        }
    }
}

void YoutubeParser::getPlaylistsContinuation(QString cont, const QJsonObject ctx, QString* continuation, QStringList* video_ids, Abortable* abortable) {
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["continuation"] = cont;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("playlists_continuation" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray contents = jsonResponseObj["onResponseReceivedActions"]
                .toArray()[0]
                .toObject()["appendContinuationItemsAction"]
                .toObject()["continuationItems"]
                .toArray();
            extractPlaylists(contents, continuation, video_ids);
        }
    }
}

Status YoutubeParser::downloadPlaylists(const QString& channel_id, Abortable* abortable) {
    Status status;
    qDebug() << "downloadPlaylists channel_id: " << channel_id;
    if (channel_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };

    QJsonObject ctx = generateCtx(abortable);
    QJsonArray tabs = getTabs(ctx, channel_id, abortable);

    QJsonObject playlists_tab = find_tab(tabs, "playlists");
    if (!playlists_tab.isEmpty()) {
        QString param = get_endpoint_param(playlists_tab);
        QString continuation;
        QString first_link;
        QStringList all_ids;
        QStringList video_ids;

        getPlaylists(channel_id, param, ctx, &continuation, &video_ids, abortable);
        if (!video_ids.isEmpty()) {
            all_ids << video_ids;

            if (continuation != "")
                first_link = video_ids.takeFirst();

            loadPlaylist(EVideoIdType::kPlaylistId, video_ids, continuation == "");

            while (continuation != ""
                && all_ids.length() < PLAYLISTS_MAXIMUM_ITEMS) {
                getPlaylistsContinuation(continuation, ctx, &continuation, &video_ids, abortable);
                qDebug() << video_ids.count();
                all_ids << video_ids;

                if (continuation == ""
                    || all_ids.length() >= PLAYLISTS_MAXIMUM_ITEMS) {
                    video_ids.push_front(first_link);
                    continuation = "";
                }

                loadPlaylist(EVideoIdType::kPlaylistId, video_ids, continuation == "");
            }
        }
        else {
            status.update(EStatusCode::ParseError, "Empty playlist");
        }
    }
    else {
        qDebug() << "NO STREAMS ON CHANNEL";
        status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

void YoutubeParser::extractContent(QJsonObject o, QMap<QString, YtContentKind>& map) {
    if (o.contains("gridVideoRenderer"))
        map.insert(o["gridVideoRenderer"].toObject()["videoId"].toString(), YtContentKind::kVIDEO);
    else if (o.contains("gridChannelRenderer"))
        map.insert(o["gridChannelRenderer"].toObject()["channelId"].toString(), YtContentKind::kCHANNEL);
    else if (o.contains("gridPlaylistRenderer"))
        map.insert(o["gridPlaylistRenderer"].toObject()["playlistId"].toString(), YtContentKind::kPLAYLIST);
    else if (o.contains("videoRenderer"))
        map.insert(o["videoRenderer"].toObject()["videoId"].toString(), YtContentKind::kVIDEO);
    else if (o.contains("channelRenderer"))
        map.insert(o["channelRenderer"].toObject()["channelId"].toString(), YtContentKind::kCHANNEL);
    else if (o.contains("playlistRenderer"))
        map.insert(o["playlistRenderer"].toObject()["playlistId"].toString(), YtContentKind::kPLAYLIST);
    else if (o.contains("reelItemRenderer"))
        map.insert(o["reelItemRenderer"].toObject()["videoId"].toString(), YtContentKind::kSHORTS);
    else {
        map.insert("UNIMPLEMENTED", YtContentKind::kVIDEO);
        qDebug().noquote() << "UNIMPLEMENTED CONTENT TYPE" << o;
    }
}

void YoutubeParser::extractShelf(QJsonObject o, QMap<QString, YtContentKind>& map) {
    if (o.contains("channelVideoPlayerRenderer")) {
        QString videoId = o["channelVideoPlayerRenderer"].toObject()["videoId"].toString();
        map.insert(videoId, YtContentKind::kVIDEO);
    }
    else if (o.contains("shelfRenderer")) {
        QJsonObject shelfContent = o["shelfRenderer"].toObject()["content"].toObject();
        QJsonObject renderer;
        renderer = shelfContent[shelfContent.keys()[0]].toObject();
        for (const auto& e : renderer["items"].toArray())
            extractContent(e.toObject(), map);
    }
    else if (o.contains("reelShelfRenderer")) {
        QJsonArray shelfContent = o["reelShelfRenderer"].toObject()["items"].toArray();
        for (const auto& e : shelfContent)
            extractContent(e.toObject(), map);
    }
}

void YoutubeParser::extractFeaturedShelves(QJsonArray shelvesJson, QMap<QString, YtContentKind>& map) {
    for (const auto& e : shelvesJson) {
        QJsonObject o = e.toObject()["itemSectionRenderer"].toObject()["contents"].toArray()[0].toObject();
        extractShelf(o, map);
    }
}

void YoutubeParser::getFeatured(QString channel_id, QString endpoint_param, QJsonObject ctx, QMap<QString, YtContentKind>& map, Abortable* abortable) {
    QString data;

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QList<QPair<QByteArray, QByteArray>>  url_query_params;
    addDefaultQuery(url_query_params);

    QJsonObject payload;
    payload["context"] = ctx;
    payload["browseId"] = channel_id;
    payload["params"] = endpoint_param;
    QJsonDocument doc(payload);

    QString info_url = QString(YT_BASE) + "browse";
    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("featured" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(data.toStdString().c_str());
            file.close();
        }
#endif
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
        QJsonObject jsonResponseObj = jsonResponse.object();
        if (!jsonResponseObj.isEmpty()) {
            QJsonArray tabs = jsonResponseObj["contents"].toObject()["twoColumnBrowseResultsRenderer"].toObject()["tabs"].toArray();
            QJsonObject t = find_tab(tabs, "featured");

            qDebug().noquote() << QJsonDocument(t).toJson();

            QJsonArray shelvesJson = t["content"].toObject()["sectionListRenderer"].toObject()["contents"].toArray();
            extractFeaturedShelves(shelvesJson, map);
            qDebug() << map.size();
        }
    }
}

Status YoutubeParser::downloadFeatured(const QString& channel_id, Abortable* abortable) {
    Status status;
    qDebug() << "downloadFeatured channel_id: " << channel_id;
    if (channel_id.isEmpty()) {
        status.update(EStatusCode::ParseError, "Empty playlist");
        return status;
    };

    QJsonObject ctx = generateCtx(abortable);
    QJsonArray tabs = getTabs(ctx, channel_id, abortable);

    QJsonObject featured_tab = find_tab(tabs, "featured");
    if (!featured_tab.isEmpty()) {
        QString param = get_endpoint_param(featured_tab);
        QStringList video_ids;
        QStringList shorts_ids;
        QStringList playlist_ids;
        QStringList channel_ids;
        QMap<QString, YtContentKind> shelves;

        getFeatured(channel_id, param, ctx, shelves, abortable);

        if (!shelves.isEmpty()) {
            for (auto it = shelves.begin(); it != shelves.end(); ++it) {
                if (it.value() == YtContentKind::kVIDEO)
                    video_ids.push_back(it.key());
                else if (it.value() == YtContentKind::kSHORTS)
                    shorts_ids.push_back(it.key());
                else if (it.value() == YtContentKind::kPLAYLIST)
                    playlist_ids.push_back(it.key());
                else if (it.value() == YtContentKind::kCHANNEL)
                    channel_ids.push_back(it.key());
            }
            if (!video_ids.isEmpty()
                || !shorts_ids.isEmpty()
                || !playlist_ids.isEmpty()) {
                if (!video_ids.isEmpty())
                    loadPlaylist(EVideoIdType::kVideoId, video_ids, true);
                else if (!shorts_ids.isEmpty())
                    loadPlaylist(EVideoIdType::kShortId, shorts_ids, true);
                else if (!playlist_ids.isEmpty())
                    loadPlaylist(EVideoIdType::kPlaylistId, playlist_ids, true);
                //if (!channel_ids.isEmpty())
                //    loadPlaylist(EVideoIdType::kChannelId, channel_ids, true);
            }
            else {
                status.update(EStatusCode::ParseError, "Empty playlist");
            }
            shelves.clear();
        }
        else {
            status.update(EStatusCode::ParseError, "Empty playlist");
        }
    }
    else {
        status.update(EStatusCode::ParseError, "Empty playlist");
    }

    return status;
}

//void YoutubeParser::extractCommunity(QJsonArray contents, QString* continuation, QStringList* links) {
//    *continuation = "";
//    for (const QJsonValue& e : contents) {
//        QJsonObject itemSectionRenderer = e.toObject()["itemSectionRenderer"].toObject();
//        if (!itemSectionRenderer.isEmpty()) {
//            QJsonArray contents = itemSectionRenderer["contents"].toArray();
//            if (!contents.isEmpty()) {
//                for (const QJsonValue& c : contents) {
//
//                    if (c.toObject().contains("continuationItemRenderer")) {
//                        *continuation = c.toObject()["continuationItemRenderer"]
//                            .toObject()["continuationEndpoint"]
//                            .toObject()["continuationCommand"]
//                            .toObject()["token"]
//                            .toString();
//                    }
//                    else {
//                        QString video_id = c.toObject()["backstagePostThreadRenderer"]
//                            .toObject()["post"]
//                            .toObject()["backstagePostRenderer"]
//                            .toObject()["backstageAttachment"]
//                            .toObject()["videoRenderer"]
//                            .toObject()["videoId"]
//                            .toString();
//                        if (!video_id.isEmpty())
//                            links->append(video_id);
//                    }
//                }
//            }
//        }
//    }
//}
//
//void YoutubeParser::getCommunity(QString channel_id, QString endpoint_param, QJsonObject ctx, QString* continuation, QStringList* video_ids) {
//    QString data;
//
//    QList<QPair<QByteArray, QByteArray>>  headers;
//    addDefaultHeaders(headers);
//
//    QList<QPair<QByteArray, QByteArray>>  url_query_params;
//    addDefaultQuery(url_query_params);
//
//    QJsonObject payload;
//    payload["context"] = ctx;
//    payload["browseId"] = channel_id;
//    payload["params"] = endpoint_param;
//    QJsonDocument doc(payload);
//
//    QString info_url = QString(YT_BASE) + "browse";
//    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params)) {
//#if _DEBUG
//        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
//        QFile   file("community" + QString::number(ms) + ".json");
//        if (file.open(QIODevice::ReadWrite)) {
//            file.resize(0);
//            file.write(data.toStdString().c_str());
//            file.close();
//        }
//#endif
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
//        QJsonObject jsonResponseObj = jsonResponse.object();
//        if (!jsonResponseObj.isEmpty()) {
//            QJsonArray tabs = jsonResponseObj["contents"].toObject()["twoColumnBrowseResultsRenderer"].toObject()["tabs"].toArray();
//            QJsonObject t = find_tab(tabs, "community");
//
//            qDebug().noquote() << QJsonDocument(t).toJson();
//
//            QJsonArray shelvesJson = t["content"].toObject()["sectionListRenderer"].toObject()["contents"].toArray();
//            extractCommunity(shelvesJson, continuation, video_ids);
//        }
//    }
//}
//
//void YoutubeParser::getCommunityContinuation(QString cont, const QJsonObject& ctx, QString* continuation, QStringList* links) {
//
//    QString data;
//
//    QList<QPair<QByteArray, QByteArray>>  headers;
//    addDefaultHeaders(headers);
//
//    QList<QPair<QByteArray, QByteArray>>  url_query_params;
//    addDefaultQuery(url_query_params);
//
//    QJsonObject payload;
//    payload["context"] = ctx;
//    payload["continuation"] = cont;
//    QJsonDocument doc(payload);
//
//    QString info_url = QString(YT_BASE) + "browse";
//    if (downloadToStringPost(info_url, doc.toJson(), data, headers, url_query_params)) {
//#if _DEBUG
//        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
//        QFile   file("shorts_continuation" + QString::number(ms) + ".json");
//        if (file.open(QIODevice::ReadWrite)) {
//            file.resize(0);
//            file.write(data.toStdString().c_str());
//            file.close();
//        }
//#endif
//        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toStdString().c_str());
//        QJsonObject jsonResponseObj = jsonResponse.object();
//        if (!jsonResponseObj.isEmpty()) {
//            QJsonArray contents = jsonResponseObj["onResponseReceivedActions"]
//                .toArray()[0]
//                .toObject()["appendContinuationItemsAction"]
//                .toObject()["continuationItems"]
//                .toArray();
//            extractCommunity(contents, continuation, links);
//        }
//    }
//}
//
//Status YoutubeParser::downloadCommunity(const QString& channel_id) {
//    Status status;
//    qDebug() << "downloadCommunity channel_id: " << channel_id;
//    if (channel_id.isEmpty()) {
//        status.update(EStatusCode::ParseError, "Empty playlist");
//        return status;
//    };
//
//    QStringList video_ids;
//
//    QJsonObject ctx = generateCtx();
//    QJsonArray tabs = getTabs(ctx, channel_id);
//
//    QString param;
//    QJsonObject community_tab = find_tab(tabs, "community");
//    if (!community_tab.isEmpty())
//        param = get_endpoint_param(community_tab);
//
//    if (param.isEmpty())
//        param = QString(CHANNEL_COMMUNITY_PARAM);
//
//    if (!param.isEmpty()) {
//        QString continuation;
//        QString first_link;
//
//        getCommunity(channel_id, param, ctx, &continuation, &video_ids);
//
//        if (!video_ids.isEmpty()
//            && continuation != "")
//            first_link = video_ids.takeFirst();
//
//        loadPlaylist(EVideoIdType::kCommunityId, video_ids, continuation == "");
//
//        while (continuation != "") {
//            getCommunityContinuation(continuation, ctx, &continuation, &video_ids);
//            qDebug() << video_ids.count();
//
//            if (continuation == "")
//                video_ids.push_front(first_link);
//
//            loadPlaylist(EVideoIdType::kCommunityId, video_ids, continuation == "");
//        }
//
//        if (video_ids.isEmpty())
//            status.update(EStatusCode::ParseError, "Empty playlist");
//    }
//    else {
//        qDebug() << "NO STREAMS ON CHANNEL";
//        status.update(EStatusCode::ParseError, "Empty playlist");
//    }
//
//    return status;
//}

void YoutubeParser::loadPlaylist(const EVideoIdType type, QStringList& video_ids, bool end_of_playlist) {
    QString playlist;

    for (const auto& id : video_ids) {
        QString link;
        if (type == EVideoIdType::kVideoId)
            link = YT_WATCH + id;
        else if (type == EVideoIdType::kPlaylistId)
            link = YT_PLAYLIST + id;
        else if (type == EVideoIdType::kChannelId)
            link = YT_CHANNEL + id;
        else if (type == EVideoIdType::kShortId)
            link = YT_SHORTS + id;
        else if (type == EVideoIdType::kCommunityId)
            link = YT_WATCH + id;

        playlist.append(link + "|");
    }

    if (!playlist.isEmpty()) {
        if (end_of_playlist)
            emit loadPlaylist(playlist);
        else
            emit loadPlaylistPart(playlist);
    }

    video_ids.clear();
}

QString YoutubeParser::getURLUsingSignatureScript (const QJsonObject& format, bool& build_ranges, const Abortable* abortable) {
    QString url;

    build_ranges = true;
    bool ok = true;

    QString signature_data = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "signatureCipher", &ok);
    if (signature_data.isEmpty())
        signature_data = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "cipher", &ok);

    if (!signature_data.isEmpty()) {
        signature_data.replace("&", ",");
        signature_data = QUrl::fromPercentEncoding(signature_data.toLatin1());

        url = getUrlFromQualityLink(signature_data, abortable);
    }

    return url;
}

bool YoutubeParser::updateURLNParam(QString& url, const Abortable* abortable) {
	QRegExp nExpression = QRegExp("([&?]n=)([^&]+)");
	if (nExpression.indexIn(url) > -1) {
		if (js_n_script_.isEmpty()) {
			qDebug() << "Unable to build URL due to missing n script.";
			return false;
		}
		QString n = nExpression.cap(2);
		QString script = js_n_script_ + "(\"" + n + "\")";
		n = runSyncScript(script, abortable);
		url = url.replace(nExpression, "\\1" + n);
	}
	return true;
}

bool YoutubeParser::parseVideoTrackInfo(const QJsonObject& format, const QString& parentJsonPath,
    const QList<possibleVideoQuality>& qualities, QList<videoQuality>& added_qualities,
    VideoTrackInfo& video_track, const Abortable* abortable) {
    bool ok = true;

    QString itag = QString::number(static_cast<int>(jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "itag", &ok)));
    CHECK_WITH_RETURN(ok, "Failed to get " + parentJsonPath + ".itag", false);

    const auto it = std::find_if(qualities.begin(), qualities.end(), [itag](const possibleVideoQuality& q) {
        return q.itag == itag;
    });

    if (it == qualities.end()) {
        qDebug() << "Video quality not found. itag: " << itag;
        return false;
    }

    const possibleVideoQuality quality = *it;

    bool build_ranges = true;

    QString url = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "url", &ok);
    if (url.isEmpty())
        url = getURLUsingSignatureScript(format, build_ranges, abortable);

    if (!url.isEmpty()) {
		if (!updateURLNParam(url, abortable)) {
			return false;
		}

        video_track.size = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "contentLength", &ok).toInt();
        if (!ok) {
            qDebug() << "Failed to get " << parentJsonPath << ".contentLength";
            video_track.size = getLinkQualitySize(url);
        }

        videoQuality new_quality;
        new_quality.profile = quality.profile;
        new_quality.itag = quality.itag;
        new_quality.video_height = quality.video_height;
        new_quality.video_framerate = quality.video_framerate;
        new_quality.video_size = video_track.size;

        if (videoQualityExists(added_qualities, new_quality)) {
            qDebug() << "Video quality already added. itag: " << new_quality.itag;
            return false;
        }

        if (build_ranges) {
            video_track.urls = buildRanges(url, video_track.size);
            if (video_track.urls.size() > 1)
                video_track.is_chunked = true;
        }
        else {
            video_track.urls.push_back(url);
        }

        video_track.width = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "width", &ok);
        if (!ok)
            qDebug() << "Failed to get " + parentJsonPath + ".width";

        video_track.height = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "height", &ok);
        if (!ok)
            qDebug() << "Failed to get " + parentJsonPath + ".height";

        video_track.bitrate = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "averageBitrate", &ok);
        if (video_track.bitrate == 0)
            video_track.bitrate = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "bitrate", &ok);
        if (!ok) {
            qDebug() << "Failed to get " << parentJsonPath << ".averageBitrate";
            video_track.bitrate = getLinkQualityBitrate(url);
        }

        video_track.framerate = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "fps", &ok);
        if (!ok) {
            qDebug() << "Failed to get " << parentJsonPath << ".fps";
            video_track.framerate = getLinkQualityFramerate(url, video_track.framerate);
        }

        video_track.contains_audio = quality.contains_audio;
        video_track.profile = quality.profile;
        video_track.extended_desk = getVideoFormatDesk(video_track.profile);

        added_qualities.push_back(new_quality);
    }

    return !video_track.urls.isEmpty();
}

bool YoutubeParser::parseAudioTrackInfo(const QJsonObject& format, const QString& parentJsonPath,
    const QList<possibleAudioQuality>& qualities, QList<audioQuality>& added_qualities, AudioTrackInfo& audio_track,
	const Abortable* abortable) {
    bool ok = true;

    const QString itag = QString::number(static_cast<int>(jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "itag", &ok)));
    CHECK_WITH_RETURN(ok, "Failed to get " + parentJsonPath + ".itag", false);

    const auto it = std::find_if(qualities.begin(), qualities.end(), [itag](const possibleAudioQuality& q) {
        return q.itag == itag;
    });

    if (it == qualities.end()) {
        qDebug() << "Audio quality not found. itag: " << itag;
        return false;
    }

    QJsonObject audioTrack = jsonutils::getJsonValue<QJsonValue::Type::Object>(format, "audioTrack", &ok);
    if (!audioTrack.isEmpty())
        audio_track.language_tag = jsonutils::getJsonValue<QJsonValue::Type::String>(audioTrack, "id", &ok).split(".").first();

    audio_track.channels = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "audioChannels", &ok);
    audio_track.bitrate = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "averageBitrate", &ok);
    if (audio_track.bitrate == 0)
        audio_track.bitrate = jsonutils::getJsonValue<QJsonValue::Type::Double>(format, "bitrate", &ok);

    const possibleAudioQuality quality = *it;

    bool build_ranges = true;

    QString url = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "url", &ok);
    if (url.isEmpty())
        url = getURLUsingSignatureScript(format, build_ranges, abortable);

    if (!url.isEmpty()) {
		if (!updateURLNParam(url, abortable)) {
			return false;
		}

        audio_track.size = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "contentLength", &ok).toInt();
        if (!ok) {
            qDebug() << "Failed to get " << parentJsonPath << ".contentLength";
            audio_track.size = getLinkQualitySize(url);
        }

        audioQuality new_quality;
        new_quality.profile = quality.profile;
        new_quality.itag = quality.itag;
        new_quality.audio_bitrate = audio_track.bitrate;
        new_quality.audio_size = audio_track.size;
        new_quality.lang_tag = audio_track.language_tag;

        if (audioQualityExists(added_qualities, new_quality)) {
            qDebug() << "Audio quality already added. itag: " << new_quality.itag;
            return false;
        }

        if (build_ranges) {
            audio_track.urls = buildRanges(url, audio_track.size);
            if (audio_track.urls.size() > 1)
                audio_track.is_chunked = true;
        }
        else {
            audio_track.urls.push_back(url);
        }

        if (!ok) {
            qDebug() << "Failed to get " << parentJsonPath << ".averageBitrate";
            audio_track.bitrate = getLinkQualityBitrate(url);
            if (audio_track.bitrate == 0)
                audio_track.bitrate = getTagAudioBitrate(itag.toInt());
        }

        audio_track.profile = quality.profile;
        added_qualities.push_back(new_quality);
    }

    return !audio_track.urls.isEmpty();
}

bool YoutubeParser::parseFormats(const QJsonArray& formats, const QString& jsonParentPath,
    const QList<possibleVideoQuality>& video_qualities, const QList<possibleAudioQuality>& audio_qualities,
    QList<videoQuality>& added_video_qualities, QList<audioQuality>& added_audio_qualities, TracksInfo& tracks_info, 
	const Abortable* abortable) {

    if (video_qualities.isEmpty()
        && audio_qualities.isEmpty())
        return false;


    auto arrayJsonPathStr = [&jsonParentPath](int idx) {
        return jsonParentPath + QString("[") + QString::number(idx) + "]";
    };

    qDebug() << jsonParentPath << " size: " << formats.size();

    for (int id = 0; id < formats.size(); ++id) {
        bool ok = true;
        const QString jsonPath = arrayJsonPathStr(id);
        QJsonObject format = jsonutils::getJsonValue<QJsonValue::Type::Object>(formats, id, &ok);
        CHECK_WITH_RETURN(ok, "Failed to read format. " + jsonPath, false);

        QString mime_type = jsonutils::getJsonValue<QJsonValue::Type::String>(format, "mimeType", &ok);
        CHECK_WITH_RETURN(ok, "Failed to get " + jsonPath + ".mimeType", false);

        MimeType mime = getMimeType(mime_type, &ok);
        if (!ok)
            continue;

        if (mime.type == "video") {
            VideoTrackInfo video_track;
            if (parseVideoTrackInfo(format, jsonPath, video_qualities, added_video_qualities, video_track, abortable)) {
                normalizeTrack(video_track);
                normalizeVideoTrack(video_track);
                tracks_info.video_tracks.push_back(video_track);
            }
        }
        else if (mime.type == "audio") {
            AudioTrackInfo audio_track;
            if (parseAudioTrackInfo(format, jsonPath, audio_qualities, added_audio_qualities, audio_track, abortable)) {
                normalizeTrack(audio_track);
                normalizeAudioTrack(audio_track);
                tracks_info.audio_tracks.push_back(audio_track);
            }
        }
        else {
            return false;
        }
    }

    return (!tracks_info.video_tracks.isEmpty() || !tracks_info.audio_tracks.isEmpty());
}

bool YoutubeParser::parseCaptionTracks(const QJsonArray& captionTracks, const QString& playerCaptionsPath, TracksInfo& tracks_info) {
    bool ok = true;
    bool res = false;

    auto arrayJsonPathStr = [&playerCaptionsPath](int idx) {
        return playerCaptionsPath + QString("[") + QString::number(idx) + "]";
    };

    QList<QString> caption_urls;
    for (int id = 0; id < captionTracks.size(); ++id) {
        const QString jsonPath = arrayJsonPathStr(id);
        QJsonObject caption = jsonutils::getJsonValue<QJsonValue::Type::Object>(captionTracks, id, &ok);
        CHECK_WITH_RETURN(ok, "Failed to read caption. " + jsonPath, false);

        QString base_url = jsonutils::getJsonValue<QJsonValue::Type::String>(caption, "baseUrl", &ok);
        if (base_url.isEmpty()) {
            continue;
        }
        else if (!caption_urls.contains(base_url)) {
            caption_urls.push_back(base_url);
            
            for (QList<SubtitleTrackInfo::SubtitleProfile>::iterator it_fmt = youtube_tag_formats_.begin(); it_fmt != youtube_tag_formats_.end(); it_fmt++) {

                SubtitleTrackInfo track;
                QString languageCode = jsonutils::getJsonValue<QJsonValue::Type::String>(caption, "languageCode", &ok).toLower();
                CHECK_WITH_RETURN(ok, "Failed to get " + jsonPath + ".languageCode", false);

                track.language_tag = SS2Q(normalizeLangTag(QS2S(languageCode)));
                track.profile = *it_fmt;
                QString delim = "fmt=";
				int delim_idx = base_url.lastIndexOf(delim);
                QString url = delim_idx >= 0 ? 
					base_url.mid(0, delim_idx + delim.size()) + getSubtitleTrackExt(track.profile) :
					base_url + "&fmt=" + getSubtitleTrackExt(track.profile);
                track.urls.append(url);

                QString kind = jsonutils::getJsonValue<QJsonValue::Type::String>(caption, "kind", &ok);
                if (ok && kind == "asr")
                    track.is_additional = ok;
                
                //normalizeTrack(track);
                tracks_info.subtitle_tracks.append(track);

                res = true;
            }
        }
    }

    return res;
}

bool YoutubeParser::parseTracksInfo(const QJsonObject& info_json, TracksInfo& tracks_info, const Abortable* abortable) {
    bool ok = true;
    QJsonObject streaming_data = jsonutils::getJsonValue<QJsonValue::Type::Object>(info_json, "streamingData", &ok);
    if (!streaming_data.isEmpty()) {
        const QString formatsJsonPath = "streamingData.formats";
        QJsonArray formats = jsonutils::getJsonValue<QJsonValue::Type::Array>(streaming_data, "formats", &ok);

        const QString adaptiveFormatsJsonPath = "streamingData.adaptiveFormats";
        QJsonArray adaptiveFormats = jsonutils::getJsonValue<QJsonValue::Type::Array>(streaming_data, "adaptiveFormats", &ok);

        if (!formats.isEmpty()
            || !adaptiveFormats.isEmpty()) {

            QList<videoQuality> added_video_qualities;
            QList<audioQuality> added_audio_qualities;

            QList<possibleVideoQuality> avc_video_qualities;
            fill_avc_video_quality_list(avc_video_qualities);

            QList<possibleAudioQuality> avc_av1_audio_qualities;
            fill_avc_av1_audio_quality_list(avc_av1_audio_qualities);

            bool avc = parseFormats(adaptiveFormats, adaptiveFormatsJsonPath, avc_video_qualities, avc_av1_audio_qualities, added_video_qualities, added_audio_qualities, tracks_info, abortable)
                && parseFormats(formats, formatsJsonPath, avc_video_qualities, avc_av1_audio_qualities, added_video_qualities, added_audio_qualities, tracks_info, abortable);

            QList<possibleVideoQuality> av1_video_qualities;
            fill_av1_video_quality_list(av1_video_qualities);

            bool av1 = parseFormats(adaptiveFormats, adaptiveFormatsJsonPath, av1_video_qualities, avc_av1_audio_qualities, added_video_qualities, added_audio_qualities, tracks_info, abortable);

            QList<possibleVideoQuality> webm_video_qualities;
            fill_webm_video_quality_list(webm_video_qualities);

            QList<possibleAudioQuality> webm_audio_qualities;
            fill_webm_audio_quality_list(webm_audio_qualities);

            bool webm = parseFormats(adaptiveFormats, adaptiveFormatsJsonPath, webm_video_qualities, webm_audio_qualities, added_video_qualities, added_audio_qualities, tracks_info, abortable);

            QJsonObject captions = jsonutils::getJsonValue<QJsonValue::Type::Object>(info_json, "captions", &ok);
            if (!captions.isEmpty()) {
                QJsonObject playerCaptionsTracklistRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(captions, "playerCaptionsTracklistRenderer", &ok);
                if (!playerCaptionsTracklistRenderer.isEmpty()) {
                    const QString playerCaptionsPath = "playerCaptionsTracklistRenderer.captionTracks";
                    if (!playerCaptionsTracklistRenderer.isEmpty()) {
                        QJsonArray captionTracks = jsonutils::getJsonValue<QJsonValue::Type::Array>(playerCaptionsTracklistRenderer, "captionTracks", &ok);
                        if (!captionTracks.isEmpty()) {
                            bool subtitles = parseCaptionTracks(captionTracks, playerCaptionsPath, tracks_info);
                        }
                    }
                }
            }

            return (avc || av1 || webm);
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool YoutubeParser::parseVideoInfo(const QJsonObject& info_json, VideoInfo& video_info) {
    bool ok = false;
    QJsonObject video_details = jsonutils::getJsonValue<QJsonValue::Type::Object>(info_json, "videoDetails", &ok);
    CHECK_WITH_RETURN(ok, "Failed to get `videoDetails`", false);

	if (jsonutils::getJsonValue<QJsonValue::Type::Bool>(video_details, "isLiveContent", &ok))
		video_info.is_live_content = true;

    QString title = jsonutils::getJsonValue<QJsonValue::Type::String>(video_details, "title", &ok);
    if (!title.isEmpty())
        video_info.video_title = normalizeTitle(title);

    video_info.enable_metadata = enableMetadata(video_info.video_title);
    if (video_info.enable_metadata) {
        video_info.metadata_title = video_info.video_title;
        QString artist = jsonutils::getJsonValue<QJsonValue::Type::String>(video_details, "author", &ok);
        if (!artist.isEmpty())
            video_info.metadata_artist = artist;
    }
    int duration = jsonutils::getJsonValue<QJsonValue::Type::String>(video_details, "lengthSeconds", &ok).toInt();
    if (duration > 0)
        video_info.duration = duration;

    QString thumbnail_url;
    QJsonObject thumbnail = jsonutils::getJsonValue<QJsonValue::Type::Object>(video_details, "thumbnail", &ok);
    if (ok) {
        const QString thumbnailsJsonPath = "thumbnail.thumbnails";
        QJsonArray thumbnails = jsonutils::getJsonValue<QJsonValue::Type::Array>(thumbnail, "thumbnails", &ok);
        if (ok) {
            auto arrayJsonPathStr = [&thumbnailsJsonPath](int idx) {
                return thumbnailsJsonPath + QString("[") + QString::number(idx) + "]";
            };

            for (int id = 0; id < thumbnails.size(); ++id) {
                const QString jsonPath = arrayJsonPathStr(id);
                QJsonObject thumbnail = jsonutils::getJsonValue<QJsonValue::Type::Object>(thumbnails, id, &ok);
                if (ok) {
                    thumbnail_url = jsonutils::getJsonValue<QJsonValue::Type::String>(thumbnail, "url", &ok);
                    int height = jsonutils::getJsonValue<QJsonValue::Type::Double>(thumbnail, "height", &ok);
                    if (!thumbnail_url.isEmpty()
                        && height > 340) //340 - item width 2k display
                        break;
                }
            }
        }
    }
    
    if (!thumbnail_url.isEmpty()) {
        video_info.artwork_url = thumbnail_url;
        video_info.metadata_artwork_url = video_info.artwork_url;
    }

    return true;
}

bool YoutubeParser::parseJsonInfo(const QString& info, VideoInfo& video_info, TracksInfo& tracks_info, const Abortable* abortable) {
    QJsonParseError parseError;
    QJsonDocument info_json = QJsonDocument::fromJson(info.toLocal8Bit(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse video info json. Reason: " << parseError.errorString();
        return false;
    }
    CHECK_WITH_RETURN(info_json.isObject(), "json must be an object", false)

    QJsonObject info_json_object = info_json.object();
    if (!parseVideoInfo(info_json_object, video_info))
        return false;

    return parseTracksInfo(info_json_object, tracks_info, abortable);
}

QString YoutubeParser::downloadVideoInfo(requestData& req_data, Abortable* abortable) {
    const QString url(QString(YT_BASE) + "player");
    QByteArray body = QString(R"(
    {
      "context":{
        "client":{
          "clientName":"%1",
          "clientVersion":"%2",
          "androidSdkVersion":30
        },
        "thirdParty":{
          "embedUrl":"https://www.youtube.com/"
        }
      },
      "playbackContext":{
        "contentPlaybackContext":{
          "html5Preference":"HTML5_PREF_WANTS",
          "signatureTimestamp":"%3"
        },
      },
      "racyCheckOk":true,
      "contentCheckOk":true
    }
    )").arg(req_data.clientName, req_data.clientVersion, signature_timestamp_).toUtf8();

    const QList<QPair<QByteArray, QByteArray>> headers = {
        {{"Content-Type"}, {"application/json"}},
        {{"Accept-Language"}, {"en-US,en"}},
        {{"User-Agent"}, {req_data.userAgent.toUtf8()}},
        {{"X-Goog-Visitor-Id"}, {"CgtkcnZZSUpfaU0wVSiqlMi8BjIKCgJSVRIEGgAgHg%3D%3D"}},
        {{"X-Youtube-Client-Name"}, {QString::number(req_data.clientId).toUtf8()}},
        {{"X-Youtube-Client-Version"}, {req_data.clientVersion.toUtf8()}},
    };

    const QList<QPair<QByteArray, QByteArray>> query_params = {
        {{"videoId"}, {video_id_.toUtf8()}}
    };

    QString response;
    downloadToStringPost(url, body, response, headers, query_params, abortable);
    return response;
}

bool YoutubeParser::checkHighVideoQualityExists(TracksInfo& tracks_info) {
    bool hd_video_exists = false;
    bool full_hd_video_exists = false;
    bool ultra_hd_video_exists = false;
    bool ultra_hd_8k_video_exists = false;
    
    for (const auto& track : tracks_info.video_tracks) {
		int min_dimenstion = std::min(track.width, track.height);
        if (min_dimenstion > 480) {
            hd_video_exists = true;
        }
        if (min_dimenstion > 720) {
            full_hd_video_exists = true;
        }
        if (min_dimenstion > 1080) {
			ultra_hd_video_exists = true;
        }
        if (min_dimenstion > 2160) {
			ultra_hd_8k_video_exists = true;
        }
    }

	if (ultra_hd_video_exists) {
		return ultra_hd_8k_video_exists;
	}

	if (hd_video_exists) {
		return full_hd_video_exists;
	}

	return true;
}

Status YoutubeParser::parseTracksFromJson(QString& json_video_info, VideoInfo& video_info, TracksInfo& tracks_info, Abortable* abortable) {

    Status last_status = EStatusCode::UnknownError;
    
    ERequestDataType request_types[] = {ERequestDataType::kIos, ERequestDataType::kTVHTML5/*, ERequestDataType::kTvHTML5Embedded,
		ERequestDataType::kMWeb, ERequestDataType::kWeb, ERequestDataType::kAndroid, */};

    QList<VideoTrackInfo> taked_video_tracks;

    for (const ERequestDataType& type: request_types) {
        tracks_info = {};
        requestData request_data = getRequestData(type);
		int attempt = 0;
		bool bot_protection_triggered = false;
		while (attempt++ < MAX_BOT_CIRCUMVENTION_ATTEMPTS) {
			json_video_info = downloadVideoInfo(request_data, abortable);
			bot_protection_triggered = false;
			if (!json_video_info.isEmpty() && !json_video_info.contains(L"Sign in to confirm you’re not a bot")) break;
			bot_protection_triggered = true;
			qDebug() << "Bot protection triggered, retrying...";
			QThread::sleep(1);
		}
		if (bot_protection_triggered) {
			qDebug() << "Max bot protection circumvention retry attempts exceedd, giving up.";
		}
        if (!json_video_info.isEmpty()) {
#if _DEBUG
            unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
            QFile   file("video_info_" + request_data.clientName + "_" + QString::number(ms) + ".json");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(json_video_info.toUtf8());
                file.close();
            }
#endif
            bool ok = parseJsonInfo(json_video_info, video_info, tracks_info, abortable);
            qDebug() << "YoutubeParser: parsed JS tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size();
            last_status = ok ? last_status : last_status.update(EStatusCode::ParseError);

			if (ok && video_info.is_live_content && video_info.duration == 0) {
				last_status = EStatusCode::LiveStream;
				break;
			}

            if (ok
                && (type == ERequestDataType::kAndroid || type == ERequestDataType::kIos)
                && DEFAULTPRESET == kPreset_OriginalVideo) {
                
                ok = checkHighVideoQualityExists(tracks_info);
                if (!ok)
                    taked_video_tracks = tracks_info.video_tracks;
            }

            if (ok) {
                if (!taked_video_tracks.isEmpty()) {
                    for (const auto& track : tracks_info.video_tracks) {
                        bool exist = false;

                        for (const auto& taked_track : taked_video_tracks) {
                            if (taked_track.height == track.height
                                && taked_track.framerate == track.framerate) {
                                exist = true;
                                break;
                            }
                        }

                        if (!exist)
                            taked_video_tracks.push_front(track);
                    }

                    tracks_info.video_tracks = taked_video_tracks;
                }

                last_status = EStatusCode::Success;
                break;
            }
        }
        else {
            qDebug() << "Failed to download info";
            last_status = last_status.update(EStatusCode::NetError);
        }
    }

    return last_status;
}

Status YoutubeParser::parseVideoPage(const QString& html, VideoInfo& video_info) {
    qDebug() << "YoutubeParser: start parsing page";
#if _DEBUG
    QFile   file("youtube.html");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(html.toStdString().c_str());
        file.close();
    }
#endif
    Status status;

    QRegExp expression = QRegExp("\"title\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);

    //get title
    QString title;

    if (expression.indexIn(html) != -1)
        title = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText();

    if (title.isEmpty()) {
        expression = QRegExp("\\\"title\\\":\\\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1)
            title = expression.cap(1);
    }

    if (title.isEmpty()) {
        expression = QRegExp("\"og:title\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1)
            title = expression.cap(1);
    }

    if (title.isEmpty()) {
        expression = QRegExp("\"twitter:title\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1)
            title = expression.cap(1);
    }

    if (!title.isEmpty()) {
        video_info.video_title = normalizeTitle(title);

        //get thumbnail
        expression = QRegExp("\"isCrawlable\":([^]]+)[]]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1) {
            QString thambnails_block = expression.cap(1);

            QRegularExpression re_url("\"url\":\"([^\"]+)[\"]", QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatchIterator i = re_url.globalMatch(thambnails_block);
            QString thambnail;
            while (i.hasNext()) {
                QRegularExpressionMatch match_url = i.next();
                thambnail = match_url.captured(1);
                //if (!thambnail.isEmpty())
                //    break;
            }
            video_info.artwork_url = thambnail;
        }

        if (video_info.artwork_url.isEmpty()) {
            expression = QRegExp("\"og:image\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
            if (expression.indexIn(html) != -1)
                video_info.artwork_url = expression.cap(1);
        }

        if (video_info.artwork_url.isEmpty()) {
            expression = QRegExp("\"twitter:image\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
            if (expression.indexIn(html) != -1)
                video_info.artwork_url = expression.cap(1);
        }

        video_info.metadata_artwork_url = video_info.artwork_url;

        //get duration in seconds
        expression = QRegExp("\"duration\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
        if (expression.indexIn(html) != -1) {
            QString time_exp = expression.cap(1).split("PT").last();
            int min = time_exp.split("M").first().toInt();
            int sec = time_exp.split("M").last().split("S").first().toInt();
            video_info.duration = min * 60 + sec;
        }

        video_info.enable_metadata = enableMetadata(video_info.video_title);
        if (video_info.enable_metadata) {
            QString auto_generated_metadata = "Auto-generated by YouTube";
            if (html.contains(auto_generated_metadata)) {
                expression = QRegExp("\"shortDescription\":\"([^\"]+)[\"]", Qt::CaseInsensitive);
                if (expression.indexIn(html) != -1) {
                    QString short_description = expression.cap(1);

                    if (!short_description.isEmpty()) {
                        QStringList data = short_description.split("\\n\\n");
                        data.removeFirst();

                        if (!data.isEmpty()) {
                            QString info = data.first().replace(QString::fromWCharArray(L" \u00B7 "), "|").replace("\\u0026", "&");
                            QStringList items = info.split("|");
                            if (items.size() > 1) {
                                video_info.metadata_title = normalizeTitle(QTextDocumentFragment::fromHtml(items.first()).toPlainText());
                                video_info.metadata_artist = info.split(video_info.metadata_title + "|").last().split("|").first().replace("|", ",");
                                video_info.metadata_artist = QTextDocumentFragment::fromHtml(video_info.metadata_artist).toPlainText();
                            }

                            if (data.size() > 1)
                                video_info.metadata_album = normalizeTitle(QTextDocumentFragment::fromHtml(data[1]).toPlainText());
                        }
                    }
                }
            }
            else {
                QRegularExpression re_url("\"metadataRowRenderer\":([^]]+)[]]", QRegularExpression::DotMatchesEverythingOption);
                QRegularExpressionMatchIterator i = re_url.globalMatch(html);
                while (i.hasNext()) {
                    QRegularExpressionMatch match_url = i.next();
                    QString data = match_url.captured(1);
                    if (!data.isEmpty()) {
                        expression = QRegExp("\"contents\":([^}]+)[}]", Qt::CaseInsensitive);
                        if (expression.indexIn(data) != -1) {
                            QString metadata = expression.cap(1);
                            if (!metadata.isEmpty()) {
                                QString keyword = "\"simpleText\":\"";
                                if (!metadata.contains(keyword))
                                    keyword = "\"text\":\"";
                                expression = QRegExp(keyword + "([^\"]+)[\"]", Qt::CaseInsensitive);
                                if (expression.indexIn(metadata) != -1) {
                                    QString info = QTextDocumentFragment::fromHtml(expression.cap(1)).toPlainText().replace("\\u0026", "&");

                                    if (data.contains("\"Song\""))
                                        video_info.metadata_title = normalizeTitle(info);
                                    else if (data.contains("\"Artist\""))
                                        video_info.metadata_artist = info;
                                    else if (data.contains("\"Album\""))
                                        video_info.metadata_album = normalizeTitle(info);
                                }
                            }
                        }
                    }
                }
            }

            //get year metadata
            if (!video_info.metadata_title.isEmpty()) {
                expression = QRegExp("\"datePublished\" content=\"([^\"]+)[\"]", Qt::CaseInsensitive);
                if (expression.indexIn(html) != -1)
                    video_info.metadata_year = expression.cap(1).split("-").first();
            }
        }
    }
    else {
        status.update(EStatusCode::ParseError, "Could not retrieve video title.");
    }

    return status;
}

Status YoutubeParser::parseHTML(const QString& html_in, const QString& url, VideoInfo& video_info, TracksInfo& tracks_info,
    QList<QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>>& playlists_data, Abortable* abortable) {
    Status status;
    QString html = html_in;

    video_id_ = getVideoId(html, url);

    if (html_in.isEmpty()
        && !video_id_.isEmpty()) { //URL with redirection, like age verification, using next_url parameter
        QRegExp expression = QRegExp("[\?&]next_url=(.*)</script>");
        expression.setMinimal(true);
        if (expression.indexIn(html) != -1) {
            QString html_url = YT_WATCH + QString("%1&gl=US&hl=en&has_verified=1&bpctr=9999999999").arg(video_id_);
            downloadToString(html_url, html, abortable);
            if (html.isEmpty())
                return Status(EStatusCode::ParseError);
        }
    }

#if _DEBUG
    QFile   file("youtube.html");
    if (file.open(QIODevice::ReadWrite)) {
        file.resize(0);
        file.write(html.toStdString().c_str());
        file.close();
    }
#endif
    QRegExp playlist_rx("http[s]?://.*youtube.*/playlist\\?list\\=.*", Qt::CaseInsensitive);
    QRegExp featured_rx("http[s]?://.*youtube.*/@.*/featured", Qt::CaseInsensitive);  //featured type
    QRegExp channel_rx("http[s]?://.*youtube.*/channel/.*", Qt::CaseInsensitive);     //featured type
    QRegExp channels_rx("http[s]?://.*youtube.*/@.*/channels.*", Qt::CaseInsensitive);
    QRegExp videos_rx("http[s]?://.*youtube.*/@.*/videos", Qt::CaseInsensitive);
    QRegExp shorts_rx("http[s]?://.*youtube.*/@.*/shorts", Qt::CaseInsensitive);
    QRegExp community_rx("http[s]?://.*youtube.*/@.*/community", Qt::CaseInsensitive);
    QRegExp streams_rx("http[s]?://.*youtube.*/@.*/streams", Qt::CaseInsensitive);
    QRegExp playlists_rx("http[s]?://.*youtube.*/@.*/playlists", Qt::CaseInsensitive);
    QRegExp search_rx("http[s]?://.*youtube.*search_query=.*", Qt::CaseInsensitive);
    QRegExp featured_rx2("http[s]?://.*youtube.*/@.*", Qt::CaseInsensitive);  //featured type
    QRegExp user_rx("http[s]?://.*youtube.*/user/.*", Qt::CaseInsensitive);  //user type

    if (playlist_rx.indexIn(url) != -1) {
        status = downloadPlaylist(getPlaylistId(html, url), video_info, false, abortable);
    }
    else if (videos_rx.indexIn(url) != -1) {
        status = downloadVideos(getChannelId(html, url, "", abortable), abortable);
    }
    else if (featured_rx.indexIn(url) != -1
        || (channel_rx.indexIn(url) != -1
            && !url.endsWith("playlists"))) {
        if (url.endsWith("/live"))
            status.update(EStatusCode::LiveStream);
        else {
            status = downloadFeatured(getChannelId(html, url, "", abortable), abortable);
        }
    }
    else if (shorts_rx.indexIn(url) != -1) {
        status = downloadShorts(getChannelId(html, url, "", abortable), abortable);
    }
    else if (community_rx.indexIn(url) != -1) {
        status.update(EStatusCode::WrongUrl);
        //status = downloadCommunity(getChannelId(html, url));
    }
    else if (streams_rx.indexIn(url) != -1) {
        status.update(EStatusCode::LiveStream);
        //status = downloadStreams(getChannelId(html, url));
    }
    else if (channels_rx.indexIn(url) != -1) {
        QStringList channel_ids;
        status = getChannelChannels(getChannelId(html, url, "", abortable), channel_ids, abortable);
        if (status) {
            bool status_any_playlist = false;
            for (QString channel_id : channel_ids) {
                status = downloadPlaylists(channel_id, abortable);  // загрузить плэйлисты с каждого канала
                //downloadFeatured(channel_id);  // загрузить все каналы с каждого канала
                if (!status_any_playlist
                    && status.ok())
                    status_any_playlist = true;
            }
            if (status_any_playlist)
                status.update(EStatusCode::Success);
        }
    }
    else if (playlists_rx.indexIn(url) != -1
        || (channel_rx.indexIn(url) != -1
            && url.endsWith("playlists"))) {
        status = downloadPlaylists(getChannelId(html, url, "", abortable), abortable);
    }
    //else if (search_rx.indexIn(url) != -1) {
    //    qDebug() << "YoutubeParser: found channel page";

    //    status = downloadSearch(url);
    //}
    else if (featured_rx2.indexIn(url) != -1
        || user_rx.indexIn(url) != -1) { //check last playlist type
        status = downloadFeatured(getChannelId(html, url, "", abortable), abortable);
    }
    //else if (html.contains("This video requires payment")) {
    //    qDebug() << "YoutubeParser: This video requires payment to watch.";
    //    status.update(EStatusCode::PaidVideo, "This video requires payment to watch.");
    //}
    else if (!html.contains("This video requires payment")
        && (html.contains("blocked it in your country")
            || html.contains("not made this video available in your country")
            || html.contains("UNPLAYABLE"))) {
        qDebug() << "YoutubeParser: video is unavailable in your location";
        status.update(EStatusCode::VideoBlockedInLocation, "This video is unavailable in your location.");
    }
    //else if (html.contains("\"YouTube Movies\"")) {
    //    qDebug() << "YoutubeParser: YouTube Movies are not supported";
    //    status.update(Status::YouTubeMovies, "YouTube Movies are not supported.");
    //}
    else if (!html.contains("meta property=\"og:title\"")) {
        qDebug() << "YoutubeParser: this video is unavailable";
        status.update(EStatusCode::VideoUnavailable, "This video is unavailable on YouTube.");
    }
    else {
        VideoInfo vi;
        TracksInfo ti;
        QString json_video_info;

        setJSSignatureScripts(html, abortable);

        status = parseVideoPage(html, vi);
        if (status.ok())
            video_info = vi;

        status = parseTracksFromJson(json_video_info, vi, ti, abortable);
        if (status.ok()) {
            video_info = vi;
            tracks_info = ti;
        }
        else if (status.code() != EStatusCode::LiveStream) {
            status = getTracksData(html, tracks_info, video_info, abortable);
        }

        if (status.ok()) {
            QUrl qurl(url);
            QUrlQuery query(qurl);

            if (query.hasQueryItem("list")) { //download the video's playlist
                QString playlist_id = query.queryItemValue("list");
                qDebug() << "Playlist (video) id is: " << playlist_id;

                if (!playlist_id.isEmpty()) {
					video_info.playlist_kind = PlaylistKind::kPlaylistPending;
					video_info.playlist_id = "L:" + playlist_id;
                }
            }
            else { // download the video's channel
                QString channel_id = getChannelId(html, url, json_video_info, abortable);
                qDebug() << "Channel (video) id is: " << channel_id;

                if (!channel_id.isEmpty()) {
                    YtContentKind content_kind = YtContentKind::kCHANNEL;
                    QRegExp shorts_rx("http[s]?://.*youtube.*/shorts/.*", Qt::CaseInsensitive);
                    if (shorts_rx.indexIn(url) != -1)
                        content_kind = YtContentKind::kSHORTS;

					video_info.playlist_kind = PlaylistKind::kPlaylistPending;
					video_info.playlist_id = QString::number(static_cast<int>(content_kind)) + ":" + channel_id;	
                }
            }
        }
    }

    return status;
}

void YoutubeParser::getPlaylistOrChannel(VideoInfo& info,
	QList<QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>>& playlists_data, Abortable* abortable) {
	if (info.playlist_id.isEmpty()) {
		return;
	}

	bool isPlaylist = info.playlist_id.startsWith("L:");
	if (isPlaylist)	{
		QString playlist_id = info.playlist_id.mid(2);

		for (const auto& it : playlists_data) {    //get playlists_data from previous item
		    if (it.first == YtContentKind::kPLAYLIST ||
		        it.first == YtContentKind::kEMPTYPLAYLIST) {

				info.playlist_kind = PlaylistKind::kPlaylist;
		        if (it.first == YtContentKind::kEMPTYPLAYLIST)
					info.playlist_kind = PlaylistKind::kEmptyPlaylist;

		        for (const auto& pl : it.second) {
		            if (pl.first == playlist_id) {
						info.playlist.push_back(QPair(static_cast<int>(it.first), pl.second));
		                break;
		            }
		        }
		    }
		}

		if (info.playlist.isEmpty())
		    getPlaylist(playlist_id, playlists_data, info, abortable);
	}
	else {
		auto parts = info.playlist_id.splitRef(':');
		QString channel_id = info.playlist_id.mid(parts[0].length() + 1);
		YtContentKind content_kind = static_cast<YtContentKind>(parts[0].toInt());

		bool shorts_only_in_channel = true;
		for (const auto& it : playlists_data) { //get channels_data from previous item
			info.playlist_kind = PlaylistKind::kPlaylist;

		    if (it.first == YtContentKind::kEMPTYVIDEOS
		        || it.first == YtContentKind::kEMPTYSHORTS
		        || it.first == YtContentKind::kEMPTYCHANNEL)
				info.playlist_kind = PlaylistKind::kEmptyPlaylist;
		        
		    for (const auto& ch : it.second) {

		        if (content_kind == YtContentKind::kCHANNEL){
		            if (ch.first == channel_id) {
		                if (it.first != YtContentKind::kSHORTS
		                    && it.first != YtContentKind::kEMPTYSHORTS)
		                    shorts_only_in_channel = false;

						info.playlist.push_back(QPair(static_cast<int>(it.first), ch.second));
		            }
		        }
		        else if (content_kind == it.first
		            && ch.first == channel_id) {
					info.playlist.push_back(QPair(static_cast<int>(it.first), ch.second));
		            break;
		        }
		    }
		}

		if (info.playlist.isEmpty()
		    || shorts_only_in_channel) {
		    
		    if (shorts_only_in_channel)
				info.playlist.clear();

		    getChannel(content_kind, channel_id, playlists_data, info, abortable);
		}
	}
}

bool YoutubeParser::requestBrowse(const QString extraBody, QString& response, Abortable* abortable) {
    QString body(R"(
    {
      "context": {
        "client": {
          "clientName": "WEB",
          "clientVersion": "2.20201209.01.00", 
          "hl": "en",
          "gl": "US"
        }
      },
      %1
    }
    )");

    QList<QPair<QByteArray, QByteArray>>  headers;
    addDefaultHeaders(headers);

    QString browse_url = QString(YT_BASE) + "browse";
    return downloadToStringPost(browse_url, body.arg(extraBody).toUtf8(), response, headers, QList<QPair<QByteArray, QByteArray >>(), abortable);
}

void YoutubeParser::parseContinuation(const QString& json, QString& continuation) {
    continuation.clear();

    QRegularExpression regexContinuation("continuationCommand\":\\s*\\{\\s*\"token\":\\s*\"([^\"]+)\"");
    QRegularExpressionMatch match = regexContinuation.match(json);

    if (match.hasMatch()) {
        QString token = match.captured(1);
        continuation.append(token);
    }
}

void YoutubeParser::parseVideosAndCont(const QString& json, QList<QPair<QString, QString>>& episodes, QString& continuation,
    QString playlist_id) {
    bool ok = false;
    QJsonDocument json_doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject json_object = json_doc.object();
    if (!json_object.isEmpty()) {
        QJsonObject contents = jsonutils::getJsonValue<QJsonValue::Type::Object>(json_object, "contents", &ok);
        if (!contents.isEmpty()) {
            QJsonObject twoColumnBrowseResultsRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(contents, "twoColumnBrowseResultsRenderer", &ok);
            if (!twoColumnBrowseResultsRenderer.isEmpty()) {
                QJsonArray tabs = jsonutils::getJsonValue<QJsonValue::Type::Array>(twoColumnBrowseResultsRenderer, "tabs", &ok);
                if (!tabs.isEmpty()) {
                    for (int id = 0; id < tabs.size(); ++id) {
                        QJsonObject tab = jsonutils::getJsonValue<QJsonValue::Type::Object>(tabs, id, &ok);
                        if (!tab.isEmpty()) {
                            QJsonObject tabRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(tab, "tabRenderer", &ok);
                            if (!tabRenderer.isEmpty()) {
                                QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(tabRenderer, "content", &ok);
                                if (!content.isEmpty()) {

                                    QJsonObject renderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "richGridRenderer", &ok);
                                    if (renderer.isEmpty())
                                        renderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "sectionListRenderer", &ok);
                                    
                                    if (!renderer.isEmpty()) {
                                        QJsonArray contents = jsonutils::getJsonValue<QJsonValue::Type::Array>(renderer, "contents", &ok);
                                        if (!contents.isEmpty()) {
                                            for (int id = 0; id < contents.size(); ++id) {
                                                QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(contents, id, &ok);
                                                if (!content.isEmpty()) {
                                                    
                                                    QJsonObject itemRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "richItemRenderer", &ok);
                                                    if (itemRenderer.isEmpty())
                                                        itemRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "itemSectionRenderer", &ok);

                                                    if (!itemRenderer.isEmpty()) {
                                                        QJsonArray contents;
                                                        
                                                        QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(itemRenderer, "content", &ok);
                                                        if (!content.isEmpty())
                                                            contents = {content};
                                                        
                                                        if (contents.isEmpty())
                                                            contents = jsonutils::getJsonValue<QJsonValue::Type::Array>(itemRenderer, "contents", &ok);

                                                        if (!contents.isEmpty()) {
                                                            for (int id = 0; id < contents.size(); ++id) {
                                                                QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(contents, id, &ok);
                                                                if (!content.isEmpty()) {
                                                                    QJsonArray items;

                                                                    QJsonObject videoRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "videoRenderer", &ok);
                                                                    if (!videoRenderer.isEmpty())
                                                                        items = { videoRenderer };
                                                                    else {
                                                                        QJsonObject videoRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "playlistVideoListRenderer", &ok);
                                                                        if (videoRenderer.isEmpty())
                                                                            videoRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "richGridRenderer", &ok);

                                                                        QJsonObject gridRenderer; //playlists case
                                                                        if (videoRenderer.isEmpty()) {
                                                                            gridRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "gridRenderer", &ok);
                                                                            if (!gridRenderer.isEmpty())
                                                                                items = jsonutils::getJsonValue<QJsonValue::Type::Array>(gridRenderer, "items", &ok);
                                                                        }

                                                                        QJsonObject backstagePostThreadRenderer; //community case
                                                                        if (videoRenderer.isEmpty()) {
                                                                            backstagePostThreadRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "backstagePostThreadRenderer", &ok);
                                                                            if (!backstagePostThreadRenderer.isEmpty()) {
                                                                                QJsonObject post = jsonutils::getJsonValue<QJsonValue::Type::Object>(backstagePostThreadRenderer, "post", &ok);
                                                                                if (!post.isEmpty()) {
                                                                                    QJsonObject backstagePostRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(post, "backstagePostRenderer", &ok);
                                                                                    if (!backstagePostRenderer.isEmpty()) {
                                                                                        QJsonObject backstageAttachment = jsonutils::getJsonValue<QJsonValue::Type::Object>(backstagePostRenderer, "backstageAttachment", &ok);
                                                                                        if (!backstageAttachment.isEmpty()) {
                                                                                            QJsonObject videoRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(backstageAttachment, "videoRenderer", &ok);
                                                                                            if (!videoRenderer.isEmpty())
                                                                                                items.push_back(videoRenderer);
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }

                                                                        QJsonObject reelItemRenderer; //shorts case
                                                                        if (videoRenderer.isEmpty()) {
                                                                            reelItemRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "reelItemRenderer", &ok);
                                                                            if (!reelItemRenderer.isEmpty())
                                                                                items.push_back(reelItemRenderer);
                                                                        }

                                                                        if (!videoRenderer.isEmpty()) {
                                                                            QJsonArray contents = jsonutils::getJsonValue<QJsonValue::Type::Array>(videoRenderer, "contents", &ok);
                                                                            if (!contents.isEmpty()) {
                                                                                for (int id = 0; id < contents.size(); ++id) {
                                                                                    QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(contents, id, &ok);
                                                                                    
                                                                                    QJsonObject playlistVideoRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "playlistVideoRenderer", &ok);
                                                                                    if (!playlistVideoRenderer.isEmpty()) {
                                                                                        items.push_back(playlistVideoRenderer);
                                                                                    }
                                                                                    else {
                                                                                        QJsonObject richItemRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "richItemRenderer", &ok);
                                                                                        if (!richItemRenderer.isEmpty()) {
                                                                                            QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(richItemRenderer, "content", &ok);
                                                                                            if (!content.isEmpty()) {
                                                                                                QJsonObject reelItemRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "reelItemRenderer", &ok);
                                                                                                if (!reelItemRenderer.isEmpty())
                                                                                                    items.push_back(reelItemRenderer);
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    if (!contents.isEmpty()) {
                                                                        for (int id = 0; id < items.size(); ++id) {
                                                                            QJsonObject item = jsonutils::getJsonValue<QJsonValue::Type::Object>(items, id, &ok);
                                                                            QString video_url;

                                                                            bool playlist_item = false;

                                                                            QString videoId = jsonutils::getJsonValue<QJsonValue::Type::String>(item, "videoId", &ok);
                                                                            if (!videoId.isEmpty()) {//video case
                                                                                video_url = YT_WATCH + videoId;
                                                                            }
                                                                            else { //playlist case
                                                                                item = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "gridPlaylistRenderer", &ok);
                                                                                if (!item.isEmpty()) {
                                                                                    playlist_item = true;
                                                                                    QString playlistId = jsonutils::getJsonValue<QJsonValue::Type::String>(item, "playlistId", &ok);
                                                                                    if (!playlistId.isEmpty())
                                                                                        video_url = YT_PLAYLIST + playlistId;
                                                                                }
                                                                            }

                                                                            if (!video_url.isEmpty()) {
                                                                                QString ep_title;

                                                                                QJsonObject title = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "title", &ok);
                                                                                if (title.isEmpty()) {
                                                                                    QJsonObject headline = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "headline", &ok);
                                                                                    if (!headline.isEmpty())
                                                                                        ep_title = normalizeTitle(jsonutils::getJsonValue<QJsonValue::Type::String>(headline, "simpleText", &ok), 40);
                                                                                }
                                                                                else {
                                                                                    QJsonArray runs = jsonutils::getJsonValue<QJsonValue::Type::Array>(title, "runs", &ok);
                                                                                    if (!runs.isEmpty()) {
                                                                                        for (int id = 0; id < runs.size(); ++id) {
                                                                                            QJsonObject run = jsonutils::getJsonValue<QJsonValue::Type::Object>(runs, id, &ok);
                                                                                            ep_title = normalizeTitle(jsonutils::getJsonValue<QJsonValue::Type::String>(run, "text", &ok), 40);
                                                                                            if (!ep_title.isEmpty())
                                                                                                break;
                                                                                        }
                                                                                    }
                                                                                }

                                                                                QString ep_ext;
                                                                                QJsonObject lengthText = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "lengthText", &ok);
                                                                                if (!lengthText.isEmpty()) {
                                                                                    ep_ext = jsonutils::getJsonValue<QJsonValue::Type::String>(lengthText, "simpleText", &ok);
                                                                                    //if (!simpleText.isEmpty()) {
                                                                                    //    int mins = simpleText.split(":").first().toInt();
                                                                                    //    int secs = simpleText.split(":").last().toInt();
                                                                                    //    int seconds = mins * 60 + secs;
                                                                                    //    ep_duration = QDateTime::fromTime_t(seconds).toUTC().toString(seconds < 3600 ? "mm:ss" : "hh:mm:ss");
                                                                                    //}
                                                                                }

                                                                                if (playlist_item) {
                                                                                    QJsonObject videoCountShortText = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "videoCountShortText", &ok);
                                                                                    if (!videoCountShortText.isEmpty())
                                                                                        ep_ext = jsonutils::getJsonValue<QJsonValue::Type::String>(videoCountShortText, "simpleText", &ok);
                                                                                }

                                                                                QString ep_data = ep_title + " " + ep_ext;
                                                                                
                                                                                if (!playlist_id.isEmpty())
                                                                                    video_url.append("&list=").append(playlist_id);
                                                                                episodes.push_back(QPair(ep_data, video_url));
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    parseContinuation(json, continuation);
}

void YoutubeParser::parseContinuationVideosAndCont(const QString& json, QList<QPair<QString, QString>>& episodes, QString& continuation,
    QString playlist_id) {
    bool ok = false;
    QJsonDocument json_doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject json_object = json_doc.object();
    bool reload_continuation = false;
    if (!json_object.isEmpty()) {
        QJsonArray onResponseReceivedActions = jsonutils::getJsonValue<QJsonValue::Type::Array>(json_object, "onResponseReceivedActions", &ok);
        if (!onResponseReceivedActions.isEmpty()) {
            for (int id = 0; id < onResponseReceivedActions.size(); ++id) {
                QJsonObject action = jsonutils::getJsonValue<QJsonValue::Type::Object>(onResponseReceivedActions, id, &ok);
                if (!action.isEmpty()) {
                    //QJsonObject reloadContinuationItemsCommand = jsonutils::getJsonValue<QJsonValue::Type::Object>(action, "reloadContinuationItemsCommand", &ok);
                    //if (!reloadContinuationItemsCommand.isEmpty()) {
                    //    reload_continuation = true;
                    //    break;
                    //}
                    QJsonObject appendContinuationItemsAction = jsonutils::getJsonValue<QJsonValue::Type::Object>(action, "appendContinuationItemsAction", &ok);
                    if (!appendContinuationItemsAction.isEmpty()) {
                        QJsonArray continuationItems = jsonutils::getJsonValue<QJsonValue::Type::Array>(appendContinuationItemsAction, "continuationItems", &ok);
                        if (!continuationItems.isEmpty()) {
                            for (int id = 0; id < continuationItems.size(); ++id) {
                                QJsonObject item = jsonutils::getJsonValue<QJsonValue::Type::Object>(continuationItems, id, &ok);
                                if (!item.isEmpty()) {
                                    
                                    QJsonObject renderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "playlistVideoRenderer", &ok);
                                    if (renderer.isEmpty()) {
                                        QJsonObject richItemRenderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(item, "richItemRenderer", &ok);
                                        if (!richItemRenderer.isEmpty()) {
                                            QJsonObject content = jsonutils::getJsonValue<QJsonValue::Type::Object>(richItemRenderer, "content", &ok);
                                            if (!content.isEmpty())
                                                renderer = jsonutils::getJsonValue<QJsonValue::Type::Object>(content, "videoRenderer", &ok);
                                        }
                                    }
                                    
                                    if (!renderer.isEmpty()) {
                                            QString videoId = jsonutils::getJsonValue<QJsonValue::Type::String>(renderer, "videoId", &ok);

                                            if (!videoId.isEmpty()) {
                                                QString ep_title;
                                                QJsonObject title = jsonutils::getJsonValue<QJsonValue::Type::Object>(renderer, "title", &ok);
                                                if (!title.isEmpty()) {
                                                    QJsonArray runs = jsonutils::getJsonValue<QJsonValue::Type::Array>(title, "runs", &ok);
                                                    if (!runs.isEmpty()) {
                                                        for (int id = 0; id < runs.size(); ++id) {
                                                            QJsonObject run = jsonutils::getJsonValue<QJsonValue::Type::Object>(runs, id, &ok);
                                                            ep_title = normalizeTitle(jsonutils::getJsonValue<QJsonValue::Type::String>(run, "text", &ok), 40);
                                                            if (!ep_title.isEmpty())
                                                                break;
                                                        }
                                                    }
                                                }

                                                QString ep_duration;
                                                QJsonObject lengthText = jsonutils::getJsonValue<QJsonValue::Type::Object>(renderer, "lengthText", &ok);
                                                if (!lengthText.isEmpty()) {
                                                    ep_duration = jsonutils::getJsonValue<QJsonValue::Type::String>(lengthText, "simpleText", &ok);
                                                    //if (!simpleText.isEmpty()) {
                                                    //    int mins = simpleText.split(":").first().toInt();
                                                    //    int secs = simpleText.split(":").last().toInt();
                                                    //    int seconds = mins * 60 + secs;
                                                    //    ep_duration = QDateTime::fromTime_t(seconds).toUTC().toString(seconds < 3600 ? "mm:ss" : "hh:mm:ss");
                                                    //}
                                                }

                                                QString ep_data = ep_title + " " + ep_duration;
                                                QString video_url = YT_WATCH + videoId;
                                                if (!playlist_id.isEmpty())
                                                    video_url.append("&list=").append(playlist_id);
                                                episodes.push_back(QPair(ep_data, video_url));
                                            }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    parseContinuation(json, continuation);
}

void YoutubeParser::getPlaylist(const QString& playlist_id,
    QList<QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>>& playlists_data, VideoInfo& video_info, Abortable* abortable) {
    QString response;
    QString continuation;
    QList<QPair<QString, QString>> episodes;

    if (requestBrowse(QString("\"browseId\": \"VL%1\"").arg(playlist_id), response, abortable)) {
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("video_playlist_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(response.toUtf8());
            file.close();
        }
#endif
        parseVideosAndCont(response, episodes, continuation, playlist_id);

        int i = 0;
        QString prev_continuation;
        bool limit_cycles_exceeded = false;
        while (!continuation.isEmpty()
            && continuation != prev_continuation
            && !limit_cycles_exceeded) {
            i++;
            if (i > PLAYLIST_MAXIMUM_CYCLES)
                limit_cycles_exceeded = true;

            prev_continuation = continuation;

            if (requestBrowse(QString("\"continuation\": \"%1\"").arg(continuation), response, abortable)) {
#if _DEBUG
                unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
                QFile   file("video_playlist_continuation_" + QString::number(ms) + ".json");
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0);
                    file.write(response.toUtf8());
                    file.close();
                }
#endif
                parseContinuationVideosAndCont(response, episodes, continuation, playlist_id);
            }
        }

        if (!episodes.isEmpty()) {
            video_info.playlist_kind = PlaylistKind::kPlaylist;
            YtContentKind content_kind = YtContentKind::kPLAYLIST;
            if (!limit_cycles_exceeded) {
                video_info.playlist.push_back(QPair((int)content_kind, episodes));
                QList<QPair<QString, QList<QPair<QString, QString>>>> data;
                data.push_back(QPair(playlist_id, episodes));
                playlists_data.push_back(QPair(content_kind, data));
#if _DEBUG
                unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
                QFile   file("episodes_" + QString::number(ms) + ".txt");
                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0);
                    for (const auto& ep : episodes) {
                        file.write(ep.first.toUtf8() + " : " + ep.second.toUtf8());
                        file.write("\n");
                    }
                    file.close();
                }
#endif
            }
            else {
                video_info.playlist_kind = PlaylistKind::kEmptyPlaylist;
                content_kind = YtContentKind::kEMPTYPLAYLIST;
                episodes.clear();
                episodes.push_back(QPair(QString(), QString(YT_PLAYLIST) + playlist_id));

                video_info.playlist.push_back(QPair((int)content_kind, episodes));
                QList<QPair<QString, QList<QPair<QString, QString>>>> data;
                data.push_back(QPair(playlist_id, episodes));
                playlists_data.push_back(QPair(content_kind, data));
            }
        }
    }
}

void YoutubeParser::getChannel(const YtContentKind& content_kind, const QString& channel_id,
    QList<QPair<YtContentKind, QList<QPair<QString, QList<QPair<QString, QString>>>>>>& channels_data, VideoInfo& video_info, Abortable* abortable) {

    QString response;
    QString continuation;
    QList<QPair<QString, QString>> episodes;

    QList<YtContentKind> kinds = { YtContentKind::kVIDEOS, YtContentKind::kSHORTS, YtContentKind::kPLAYLIST, YtContentKind::kCOMMUNITY /*, YtContentKind::kCHANNEL*/};
    if (content_kind == YtContentKind::kSHORTS)
        kinds = { content_kind };

    for (auto& content_kind : kinds) {
        QString content_param = getContentParam(content_kind);

        if (requestBrowse(QString("\"browseId\": \"%1\",\"params\": \"%2\"").arg(channel_id).arg(content_param), response, abortable)) {
#if _DEBUG
            unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
            QFile   file("video_channel_" + QString::number(ms) + ".json");
            if (file.open(QIODevice::ReadWrite)) {
                file.resize(0);
                file.write(response.toUtf8());
                file.close();
            }
#endif
            parseVideosAndCont(response, episodes, continuation);

            int i = 0;
            QString prev_continuation;
            bool limit_cycles_exceeded = false;
            while (!continuation.isEmpty()
                && continuation != prev_continuation
                && !limit_cycles_exceeded) {
                i++;
                if (i > PLAYLIST_MAXIMUM_CYCLES)
                    limit_cycles_exceeded = true;

                prev_continuation = continuation;

                if (requestBrowse(QString("\"continuation\": \"%1\",\"params\": \"%2\"").arg(continuation).arg(content_param), response, abortable)) {
#if _DEBUG
                    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
                    QFile   file("video_channel_continuation_" + QString::number(ms) + ".json");
                    if (file.open(QIODevice::ReadWrite)) {
                        file.resize(0);
                        file.write(response.toUtf8());
                        file.close();
                    }
#endif
                    parseContinuationVideosAndCont(response, episodes, continuation);
                }
            }

            if (!episodes.isEmpty()) {
                if (!limit_cycles_exceeded) {
                    video_info.playlist_kind = PlaylistKind::kPlaylist;
                    video_info.playlist.push_back(QPair((int)content_kind, episodes));
                    QList<QPair<QString, QList<QPair<QString, QString>>>> episodes_data;
                    episodes_data.push_back(QPair(channel_id, episodes));
                    
                    bool chunnel_exists = false;
                    for (const auto& data : channels_data) {
                        if (data.first == content_kind) {
                            for (const auto& data_ep : data.second) {
                                if (data_ep.first == channel_id) {
                                    chunnel_exists = true;
                                    break;
                                }
                            }
                            if (chunnel_exists)
                                break;
                        }
                    }
                    if (!chunnel_exists)
                        channels_data.push_back(QPair(content_kind, episodes_data));
#if _DEBUG
                    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
                    QFile   file("episodes_" + QString::number(ms) + ".txt");
                    if (file.open(QIODevice::ReadWrite)) {
                        file.resize(0);
                        for (const auto& ep : episodes) {
                            file.write(ep.first.toUtf8() + " : " + ep.second.toUtf8());
                            file.write("\n");
                        }
                        file.close();
                    }
#endif
                    episodes.clear();
                }
                else {
                    video_info.playlist_kind = PlaylistKind::kEmptyPlaylist;
                    episodes.clear();
                    episodes.push_back(QPair(QString(), QString(YT_CHANNEL) + channel_id));

                    if (content_kind == YtContentKind::kVIDEOS)
                        content_kind = YtContentKind::kEMPTYVIDEOS;
                    if (content_kind == YtContentKind::kSHORTS)
                        content_kind = YtContentKind::kEMPTYSHORTS;
                    if (content_kind == YtContentKind::kCHANNEL)
                        content_kind = YtContentKind::kEMPTYCHANNEL;

                    video_info.playlist.push_back(QPair((int)content_kind, episodes));
                    QList<QPair<QString, QList<QPair<QString, QString>>>> data;
                    data.push_back(QPair(channel_id, episodes));

                    channels_data.push_back(QPair(content_kind, data));
                }
            }
        }
    }
}
