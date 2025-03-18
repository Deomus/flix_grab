#include "dailymotion_parser.h"

#include "get_utils.h"


DailymotionParser::DailymotionParser(QObject* parent /*= 0*/) : QObject(parent) {
	qRegisterMetaType<VideoInfo>();
}

Status DailymotionParser::getTracksData(const QString& data, TracksInfo& tracks_info, VideoInfo& video_info, const QString& url) {
	Status status;

	QString html = data;
	QString referer = "https%3A%2F%2Fwww.dailymotion.com%2Fsignin%3Furlback%3D%252Ffr";

	QRegularExpression regexp("\"og:video:url\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
	QRegularExpressionMatch match = regexp.match(html);

	QString url_part;
	if (match.hasMatch()) {
		url_part = match.captured(1).replace("&amp;", "&");
	}
	else {
		regexp = QRegularExpression("\"og:video:secure_url\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
		match = regexp.match(html);
		if (match.hasMatch()) {
			url_part = match.captured(1).replace("&amp;", "&");
		}
		else {
			regexp = QRegularExpression("\"twitter:player\"\\s*value=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
			match = regexp.match(html);
			if (match.hasMatch()) {
				url_part = match.captured(1).replace("&amp;", "&");
			}
		}
	}

	if (!url_part.isEmpty()) {
		QString url = url_part + "&source=" + referer;
		if (!url_part.contains("?"))
			url = url_part + "?source=" + referer;
		downloadToString(url, html);
	}

	if (!html.isEmpty()) {
		//regexp = QRegularExpression("\"metadata_template_url\":\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
		//match = regexp.match(html);

		QString daily_player;
		//if (match.hasMatch()) {
		QString video_id = url.split("/").last();
		if (!video_id.isEmpty()) {
			//QString embedder = QUrl::toPercentEncoding(url);
			QString video_url = "https://www.dailymotion.com/player/metadata/video/" + video_id;//match.captured(1).replace("\\\\", "").replace("\\", "").replace(":videoId", video_id).replace(":embedder", embedder).replace(":referer", referer);
			downloadToString(video_url, daily_player);
		}
		//}

		if (!daily_player.isEmpty()) {
			//check geo-restricted
			if (daily_player.contains("geo-restricted")) {
				status.update(EStatusCode::VideoBlockedInLocation, "This video is unavailable in your location.");
				return status;
			}

			//get video duration if empty
			if (video_info.duration == 0) {
				regexp = QRegularExpression("\"duration\":([^,]+)[,]", QRegularExpression::PatternOption::CaseInsensitiveOption);
				match = regexp.match(daily_player);
				if (match.hasMatch())
					video_info.duration = match.captured(1).toInt();
			}

			//get video data
			regexp = QRegularExpression("\"qualities\":([^\]]+)[\]]", QRegularExpression::PatternOption::CaseInsensitiveOption);
			match = regexp.match(daily_player);
			if (match.hasMatch()) {
				QString video_block = match.captured(1);

				regexp = QRegularExpression("\"url\":\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
				match = regexp.match(video_block);
				if (match.hasMatch()) {
					QString url = match.captured(1).replace("\\/", "/");

					QString data;
					if (downloadToString(url, data) && !data.isEmpty()) {

						QList<QString> video_data = data.split("\n");
						video_data.removeAll(QString(""));

						if (!video_data.isEmpty()) {
							QList<int> video_name;
							bool is_url = false;

							for (QList<QString>::iterator it = video_data.begin(); it != video_data.end(); it++) {

								if (it->contains("#EXT-X-STREAM-INF")) {
									int name = it->split("NAME=\"").last().split("\"").first().toInt();
									if (!video_name.contains(name)) {
										VideoTrackInfo video_track;

										//get video resolution
										regexp = QRegularExpression("RESOLUTION=([^,]+)[,]", QRegularExpression::PatternOption::CaseInsensitiveOption);
										match = regexp.match(*it);
										if (match.hasMatch()) {
											QString resolution = match.captured(1);
											if (!resolution.isEmpty()) {
												video_track.width = resolution.split("x").first().toInt();
												video_track.height = resolution.split("x").last().toInt();
											}
										}

										//get video bitrate
										regexp = QRegularExpression("BANDWIDTH=([^,]+)[,]", QRegularExpression::PatternOption::CaseInsensitiveOption);
										match = regexp.match(*it);
										if (match.hasMatch())
											video_track.bitrate = match.captured(1).toInt();

										regexp = QRegularExpression("PROGRESSIVE-URI=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
										match = regexp.match(*it);
										if (match.hasMatch())
											video_track.urls.push_back(match.captured(1).split(".mp4").first() + ".mp4");
										else
											is_url = true;

										normalizeTrack(video_track);
										tracks_info.calculateTrackSize(video_track, video_info);
										normalizeVideoTrack(video_track);


										tracks_info.video_tracks.append(video_track);

										video_name << name;
									}

									continue;
								}
								if (is_url) {
									QString url = *it;
									url = url.split(".m3u8").first() + ".m3u8";
									tracks_info.video_tracks.back().urls.push_back(url);

									//QString data_video;
									//if (downloadToString(*it, data_video)
									//    && !data_video.isEmpty()) {

									//    QList<QString> m3u8_data_ts = data_video.split("\n");
									//    m3u8_data_ts.removeAll(QString(""));

									//    foreach(QString str_vid, m3u8_data_ts) {
									//        if (str_vid.contains(".ts")) {
									//            QStringList data = str_vid.split("/");
									//            data.removeAll(QString(""));
									//            if (data.size() > 0) {
									//                tracks_info.video_tracks.back().urls.push_back(it->left(it->lastIndexOf(data[0]) - 1) + str_vid);
									//                tracks_info.video_tracks.back().is_chunked = true;
									//            }
									//        }
									//    }
									//}
									//else {
									//    video_name.removeLast();
									//    tracks_info.video_tracks.removeLast();
									//}
									is_url = false;
								}
							}
						}

						//QRegularExpression re_url("include=(.*?)\"}]", QRegularExpression::DotMatchesEverythingOption);
						//QRegularExpressionMatchIterator i = re_url.globalMatch(video_block);

						//QList<QString> video_urls;
						//while (i.hasNext()) {
						//    QRegularExpressionMatch match_url = i.next();
						//    QString video_section = match_url.captured(1);

						//    //get video url
						//    if (!video_section.isEmpty()) {
						//        QString url = video_section.split("\"url\":\"").last();
						//        url.replace("\\/", "/");

						//        if (!video_urls.contains(url)) {
						//            VideoTrackInfo video_track;

						//            //get video size
						//            QString geometry = url.split("H264-").last();
						//            geometry = geometry.split("/video").first();
						//            video_track.width = geometry.split("x").first().toInt();
						//            QString height = geometry.split("x").last();
						//            height = height.split("-").first();
						//            video_track.height = height.toInt();

						//            if (!video_data.isEmpty()) {
						//                QString keyword = "NAME=\"" + video_section.split("\"}").first() + "\"";
						//                for (QList<QString>::iterator it = video_data.begin(); it != video_data.end(); it++) {

						//                    if (it->contains(keyword)
						//                        || it->contains(QString::number(video_track.width) + "x")
						//                        || it->contains("NAME=\"" + QString::number(video_track.height)))
						//                    {
						//                        QString bitrate = it->split(",CODECS").first();
						//                        bitrate = bitrate.split("BANDWIDTH=").last();
						//                        video_track.bitrate = bitrate.toInt();
						//                        break;
						//                    }
						//                }
						//            }

						//            video_track.urls.append(url);

						//            //bool track_exists = false;
						//            //for (auto it : tracks_info.video_tracks) {
						//            //    if (video_track.height == it.height) {
						//            //        track_exists = true;
						//            //        break;
						//            //    }
						//            //}
						//            //if (!track_exists) {
						//                  normalizeTrack(video_track);
						//                  normalizeVideoTrack(video_track);
						//                  tracks_info.video_tracks.append(video_track);
									  //}
							//        }
							//        video_urls << url;
							//    }
							//}
					}
				}

				QList<int> bitrates;
				for (int i = 0; i < tracks_info.video_tracks.size(); ++i) {
					if (!tracks_info.video_tracks[i].urls.isEmpty()) {

						int height = tracks_info.video_tracks[i].height;

						int bitrate = 0;
						if (height > 0
							&& height < 240)
							bitrate = 12000;
						else if (height < 480)
							bitrate = 64000;
						else if (height < 1080)
							bitrate = 128000;
						else if (height >= 1080)
							bitrate = 160000;

						if (!bitrates.contains(bitrate)) {

							AudioTrackInfo audio_track;
							audio_track.from_video_stream = true;
							audio_track.bitrate = bitrate;
							audio_track.urls.append(tracks_info.video_tracks[i].urls[0]);

							normalizeTrack(audio_track);
							normalizeAudioTrack(audio_track);
							tracks_info.audio_tracks.append(audio_track);

							bitrates << bitrate;
						}
					}
				}
			}
		}
	}

	qDebug() << "Parsed tracks: videos " << tracks_info.video_tracks.size() << " audios " << tracks_info.audio_tracks.size() << " images " << tracks_info.image_tracks.size();

	return status;
}

Status DailymotionParser::parsePage(const QString& html, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) {
	Status status;

	//get title
	QString title;
	QRegularExpression regexp("\"og:title\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
	QRegularExpressionMatch match = regexp.match(html);
	if (match.hasMatch()) {
		title = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
		title = title.split("\"").first();
	}
	//get title for embed video
	if (title.isEmpty()) {
		regexp = QRegularExpression("<title>([^<]+)[<]", QRegularExpression::PatternOption::CaseInsensitiveOption);
		match = regexp.match(html);
		if (match.hasMatch())
			title = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();
	}

	//obtain basic info from web page
	if (!title.isEmpty()) {
		title.remove("Dailymotion Video Player - ");
		title.remove(" - video dailymotion", Qt::CaseInsensitive);

		video_info.video_title = normalizeTitle(title);

		//get thumbnail url
		regexp = QRegularExpression("\"og:image\"\\s*content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
		match = regexp.match(html);
		if (match.hasMatch())
			video_info.artwork_url = QTextDocumentFragment::fromHtml(match.captured(1)).toPlainText();


		//get thumbnail for embed video
		if (video_info.artwork_url.isEmpty()) {
			regexp = QRegularExpression("\"posters\":([^}]+)[}]", QRegularExpression::PatternOption::CaseInsensitiveOption);
			match = regexp.match(html);
			if (match.hasMatch()) {
				QString poster = match.captured(1).split(",").last();
				video_info.artwork_url = poster.split("\":\"").last().split("\"").first().replace("\\/", "/");
			}
		}

		video_info.metadata_artwork_url = video_info.artwork_url;

		//get duration in seconds
		regexp = QRegularExpression("\"video:duration\" content=\"([^\"]+)[\"]", QRegularExpression::PatternOption::CaseInsensitiveOption);
		match = regexp.match(html);
		if (match.hasMatch())
			video_info.duration = match.captured(1).toInt();

		status = getTracksData(html, tracks_info, video_info, url);
		if (status.ok() && tracks_info.video_tracks.isEmpty())
			status.update(EStatusCode::ParseError, "Empty tracks info.");
	}
	else {
		status.update(EStatusCode::ParseError, "Could not retrieve video title.");
	}
	return status;

}

Status DailymotionParser::parseHTML(const QString& html, VideoInfo& video_info, TracksInfo& tracks_info, const QString& url) {
	Status status;

	//Check for login required
	//QRegExp expression = QRegExp("<div id=\"watch7-player-age-gate-content\">.*<button.*href=\"([^\"]+)\"");
	//expression.setMinimal(true);
	//if (expression.indexIn(html) != -1)
	//    status.update(Status::RegistrationFailed, "Login required.");
	//else {
	status = parsePage(html, video_info, tracks_info, url);
	//}

	return status;
}