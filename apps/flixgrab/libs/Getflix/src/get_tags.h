#pragma once

#include <QtCore>

#include <regex>

#include "get_utils.h"
#include "video_info.h"

class TagsInfo : public QObject {
    Q_OBJECT

public:
    bool GetTagsInfo(const QString& media_file, VideoInfo& video_info) {
        bool ok = false;

        std::string filename;
        std::string duration;
        std::string fingerprint;

        if (runFpcalc(media_file, &filename, &duration, &fingerprint)) {
            const QString acoustid_json = downloadAcoustidInfo(duration, fingerprint);
            if (!acoustid_json.isEmpty()) {
                QString artist;
                QString title;
                QString recording_id;
                if (parseAcoustidJson(acoustid_json, artist, title, recording_id)
                    && !recording_id.isEmpty()) {
                    video_info.metadata_artist = artist;
                    video_info.metadata_title = title;
                    const QString musicbrainz_json = downloadMusicbrainzInfo(recording_id);
                    if (!musicbrainz_json.isEmpty()) {
                        QString release_date;
                        QString thumbnail_url;
                        QString album;
                        QString title;
                        if (parseMusicbrainzJson(musicbrainz_json, release_date, thumbnail_url, album, title)) {
                            if (release_date.split("-").first().toInt() > 0)
                                video_info.metadata_year = release_date.split("-").first();
                            if (!thumbnail_url.isEmpty())
                                video_info.metadata_artwork_url = thumbnail_url;
                            if (!album.isEmpty())
                                video_info.metadata_album = album;
                            if (!title.isEmpty()
                                && video_info.metadata_title.isEmpty())
                                video_info.metadata_title = title;
                            ok = true;
                        }
                    }
                }
            }
        }

        return ok;
    }

private:
    bool runFpcalc(const QString& target_file, std::string* filename, std::string* duration, std::string* fingerprint) {
        std::string fpcalc_path = QCoreApplication::applicationDirPath().toStdString() + "/fpcalc.exe";

        QProcess fpcalc_proc;
        fpcalc_proc.setProgram(QString::fromStdString(fpcalc_path));
        fpcalc_proc.setArguments(QStringList() << target_file);
        fpcalc_proc.start();
        fpcalc_proc.waitForFinished();

        QString qoutput = fpcalc_proc.readAllStandardOutput();
        std::string output = qoutput.toStdString();

        std::regex r("FILE=(.*)\r\nDURATION=(.*)\r\nFINGERPRINT=(.*)\r\n");
        std::smatch m;

        if (std::regex_search(output, m, r)) {
            filename->assign(m[1]);
            duration->assign(m[2]);
            fingerprint->assign(m[3]);
            return true;
        }

        return false;
    }

    QString downloadAcoustidInfo(const std::string& duration, const std::string& fingerprint) {
        QUrlQuery acoustidParams;
        acoustidParams.addQueryItem("duration", QString::fromStdString(duration));
        acoustidParams.addQueryItem("fingerprint", QString::fromStdString(fingerprint));

        QUrl acoustidResource("http://api.acoustid.org/v2/lookup?client=hTdG9OHQyL&meta=recordings");

        QNetworkRequest acoustidRequest(acoustidResource);
        acoustidRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QNetworkAccessManager nam;
        QNetworkReply* reply = nam.post(acoustidRequest, acoustidParams.query(QUrl::FullyEncoded).toUtf8());

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        const auto& response = reply->readAll();
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("acoustid_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(response);
            file.close();
        }
#endif

        return response;
    }

    bool parseAcoustidJson(const QString& info, QString& artist, QString& title, QString& recording_id) {
        bool ok = false;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(info.toLocal8Bit(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Failed to parse video info json. Reason: " << parseError.errorString();
            return ok;
        }
        if (doc.isObject()) {
            QJsonObject info_json_object = doc.object();

            if (info_json_object["status"].toString() != "ok") {
                qDebug() << "Failed to parse acoustid json status";
                return ok;
            }

            const auto results = info_json_object["results"].toArray();
            for (const auto result : results) {
                const auto  recordings = result.toObject()["recordings"].toArray();
                for (const auto record : recordings) {
                    const auto  artists = record.toObject()["artists"].toArray();
                    if (!artists.empty())
                        artist = artists[0].toObject()["name"].toString();
                    title = record.toObject()["title"].toString();
                    recording_id = record.toObject()["id"].toString();
                    if (!title.isEmpty()
                        && !recording_id.isEmpty()) {
                        ok = true;
                        break;
                    }
                }

                if (ok)
                    break;
            }
        }

        return ok;
    }

    QString downloadMusicbrainzInfo(const QString& recording_id) {
        QString formatUrl = QString("http://musicbrainz.org/ws/2/recording/%1?inc=releases&fmt=json").arg(recording_id);

        QUrl musicbrainzResource(formatUrl);

        QNetworkRequest musicbrainzRequest(musicbrainzResource);
        musicbrainzRequest.setHeader(QNetworkRequest::UserAgentHeader, "fingerprint-search");

        QNetworkAccessManager nam;
        QNetworkReply* reply = nam.get(musicbrainzRequest);

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        const auto& response = reply->readAll();
#if _DEBUG
        unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
        QFile   file("musicbrainz_" + QString::number(ms) + ".json");
        if (file.open(QIODevice::ReadWrite)) {
            file.resize(0);
            file.write(response);
            file.close();
        }
#endif

        return response;
    }

    bool parseMusicbrainzJson(const QString& info, QString& release_date, QString& thumbnail_url, QString& album, QString& title) {
        bool ok = false;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(info.toLocal8Bit(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "Failed to parse video info json. Reason: " << parseError.errorString();
            return ok;
        }

        if (doc.isObject()) {
            QJsonObject info_json_object = doc.object();
            const auto releases = info_json_object["releases"].toArray();
            for (const auto release : releases) {
                QString status = release.toObject()["status"].toString().toLower();
                if (status == "official")
                    album = release.toObject()["title"].toString();

                release_date = release.toObject()["date"].toString();

                QString thumbnail_id = release.toObject()["id"].toString();
                if (!thumbnail_id.isEmpty()) {
                    thumbnail_url = QString("http://coverartarchive.org/release/%1/front").arg(thumbnail_id);
                    long track_size = getTrackSize(thumbnail_url);

                    if (track_size < 500
                        || track_size > 2097152)
                        thumbnail_url.clear();
                }

                if (!thumbnail_url.isEmpty()
                    && status == "official") {
                    ok = true;
                    break;
                }
            }

            title = info_json_object["title"].toString();
        }

        return ok;
    }
};
