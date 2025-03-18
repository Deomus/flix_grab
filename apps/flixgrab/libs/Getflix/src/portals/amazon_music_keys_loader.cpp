#include "amazon_music_keys_loader.h"
#include <decrypsis.h>
#include <ctime>


AmazonMusicKeysLoader::~AmazonMusicKeysLoader() {
	DecryptCloseSession(cdm_session_.data(), cdm_session_.size(), (SessionMessageCallback*)this);
}

void AmazonMusicKeysLoader::initCDMSession(std::vector<uint8_t> init_data, QUrl widevine2LicenseQuery, QPointer<WebContent>	sender, int* host_primise_id) {
	
	sender_ = sender;
	QObject::connect(sender_, &WebContent::finished, this, [=]() {
		AmazonMusicKeysLoader::onLoadResponseFromPRS(sender_);
		}
	, Qt::QueuedConnection);	
	
	widevine2LicenseQuery_ = widevine2LicenseQuery;

	InstallDecrypsis();

	if (host_primise_id != nullptr)
		host_primise_id_ = *host_primise_id;

	DecryptCreateSessionAndGenerateRequest(init_data.data(), init_data.size(), (SessionMessageCallback*)this, true, &host_primise_id_); 
	if(host_primise_id != nullptr)
		*host_primise_id = host_primise_id_;

	//cdm_initializing_ = true;
}

//void AmazonMusicKeysLoader::initCDMSession(AbstractDecryptService* cdmSrv)
//{
//	Q_UNUSED(cdmSrv);
//	//DecryptReCreateService(cdmSrv);
//}

void AmazonMusicKeysLoader::OnSessionMessage(const char* session_id, uint32_t session_id_size, uint32_t message_type, const char* message, uint32_t message_size) {
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	//qDebug() << "OnSessionMessage: " << std::ctime(&end_time);

	sender_->reset();
	
	cdm_session_ = QByteArray(session_id, session_id_size);

	QByteArray qbChallenge(message, (int)message_size);
	qDebug() << "AmazonMusicKeysLoader OnSessionMessage " << session_id << "-> size: " << message_size << " type:" << message_type << " >> " << widevine2LicenseQuery_.toString();
	
	auto content_type = emit getPostData(qbChallenge);
	if (content_type.isEmpty())
		content_type = "application/octet-stream";
	qDebug() << "post " << content_type << ", " << qbChallenge;
	sender_->post(widevine2LicenseQuery_, qbChallenge, content_type.toLocal8Bit());
}

void AmazonMusicKeysLoader::onLoadResponseFromPRS(const WebContent* content)
{
	try {
		content->raw_data();
		qDebug() << "got response from PRS: (" << content->raw_data().size() << "): "; // << license.toUtf8();
		const auto& r = content->raw_data();
		//qDebug() << r.size() << "): " << r.constData();

		const QByteArray license = emit extractLicense(r);

		DecryptUpdateSession(cdm_session_.constData(), cdm_session_.size(), license.constData(), license.size(), host_primise_id_);

		//emit keysLoaded();
	}
	catch (...) {
		qDebug() << "error loading From PRS.";
	}
}
void AmazonMusicKeysLoader::OnResolveNewSessionPromise(uint32_t promise_id, const char* session_id, uint32_t session_id_size) {
	Q_UNUSED(promise_id);
	Q_UNUSED(session_id);
	Q_UNUSED(session_id_size);
	qDebug() << "interceptor: OnResolveNewSessionPromise ...";
}

void AmazonMusicKeysLoader::OnSessionKeysChange()
{
	qDebug() << "got keys.";
	emit keysLoaded();
}

void AmazonMusicKeysLoader::OnRejectPromise()
{
	qDebug() << "License request rejected.";
}
