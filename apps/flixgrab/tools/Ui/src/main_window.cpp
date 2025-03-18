#include "main_window.h"
#include "ui_main_window.h"

#include <iostream>

#include <QtWebEngine>
//#include <QWebEngineSettings>



MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)//QMainWindow(parent)
    , is_downloading_(false)
    , is_decryptor_ready_(false)
    , handle_(NULL)
    
{
  
    //netflix_ = new Getflix(this);
    netflix_ = new Getflix2();

    ui_.setupUi(this);

    QString default_path = QDir(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)).filePath("movie_decrypted.mp4");
    ui_.lbPath->setText(default_path);
    

#ifdef _DEBUG
ui_.edUrl->setText("https://www.netflix.com/title/80125726");
//    ui_.edUrl->setText("https://www.netflix.com/watch/80041529");
#endif

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
        
    QObject::connect(ui_.edUrl, &QLineEdit::returnPressed, this, &MainWindow::processClicked);
    QObject::connect(ui_.pbAnalyze, &QPushButton::released, this, &MainWindow::processClicked);
    QObject::connect(ui_.pbLogout, &QPushButton::released, netflix_, &Getflix2::signOut);

    QObject::connect(ui_.pbDownload, &QPushButton::released, this, &MainWindow::downloadClicked);
    QObject::connect(ui_.pbCancel, &QPushButton::released, this, &MainWindow::cancelClicked);
    QObject::connect(ui_.pbFolder, &QPushButton::released, this, &MainWindow::handleOpenFolder);

    QObject::connect(netflix_, &Getflix2::registrationRequired, this, &MainWindow::handleRegistration);

    QObject::connect(netflix_, &Getflix2::error, this, &MainWindow::handleError, Qt::DirectConnection);
     

    ui_.pbDownload->setEnabled(false);
}


MainWindow::~MainWindow()
{
   
}


void MainWindow::updateTracksInfo(const TracksInfo& tracks_info)
{
    ui_.cbVideo->clear();
    for (auto track : tracks_info.video_tracks) {

        QString text = QString("%1x%2 @ %3 Kbps: %4 Mb %5").arg(track.width).arg(track.height).arg(track.bitrate).arg(track.size / 1000000).arg(track.is_encrypted ? "enc" : "clear");
        ui_.cbVideo->addItem(text);
    }

    ui_.cbAudio->clear();
    for (auto track : tracks_info.audio_tracks) {

        QString text = QString("%1 %2chn @ %3 Kbps: %4 Mb %5").arg(track.language).arg(track.channels).arg(track.bitrate).arg(track.size / 1000000).arg(track.is_encrypted ? "enc" : "clear");
        ui_.cbAudio->addItem(text);
    }

    ui_.cbVideo->setCurrentIndex(tracks_info.default_video_track);
    ui_.cbAudio->setCurrentIndex(tracks_info.default_audio_track);
      

    is_decryptor_ready_ = true;

    if ( !is_downloading_ )
        ui_.pbDownload->setEnabled(true);
}

QString makeFilename(const QString& name) {
    const char forbidden[] = { '<', '>', ':', '\"', '/', '\\', '|', '?', '*' };
    const char* replace[] =   { "(", ")", " -", "\'", "[", "]", " ", " ", " " };

    QByteArray bytes = name.toLocal8Bit();
    for (int i = 0; i < sizeof(forbidden) / sizeof(QChar); ++i) {
        bytes.replace(forbidden[i], replace[i]);
    }
    return QString::fromUtf8(bytes);
}

void MainWindow::updateVideoInfo(const VideoInfo& info)
{
    QFileInfo file_info(ui_.lbPath->text());

    QString file_name = makeFilename(info.video_title);

    QString new_path = file_info.dir().filePath(QString("%1.mp4").arg(file_name));

    ui_.lbPath->setText(new_path);
}

void MainWindow::processClicked()
{
   // if (handle_)
     //   handle_->deleteLater();
      
  //  handle_ = netflix_->add("https://www.netflix.com/watch/errr");
    handle_ = netflix_->add(ui_.edUrl->text());


    
    QObject::connect(handle_, &VideoHandle::analyzeCompleted, this, &MainWindow::handleVideo);
    QObject::connect(handle_, &VideoHandle::error, this, &MainWindow::handleError);

    ui_.progressBar->setAlignment(Qt::AlignCenter);
    QObject::connect(handle_, &VideoHandle::progressChanged, ui_.progressBar, &QProgressBar::setValue);

    QObject::connect(handle_, &VideoHandle::downloadStarted, this, &MainWindow::decryptStarted);
    QObject::connect(handle_, &VideoHandle::downloadFinished, this, &MainWindow::decryptFinished);
    


}

void MainWindow::handleError(QString error)
{
    qDebug() << "Getflix error: " << error;
}

void MainWindow::downloadClicked()
{
    handle_->setAudioId(ui_.cbAudio->currentIndex());
    handle_->setVideoId(ui_.cbVideo->currentIndex());
    handle_->setOutput(ui_.lbPath->text());

    handle_->download();

    ui_.pbDownload->setEnabled(false);
    is_downloading_ = true;
    is_decryptor_ready_ = false;
}

void MainWindow::cancelClicked()
{
    handle_->cancel();
    is_downloading_ = false;
    ui_.cbVideo->clear();
    ui_.cbAudio->clear();

    ui_.progressBar->setValue(0);
    ui_.progressBar->setFormat("Cancelled.");
}

void MainWindow::handleOpenFolder()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Movie"), ui_.lbPath->text(), tr("Movies (*.mp4)"));
    if ( !filename.isEmpty() )
        ui_.lbPath->setText(filename);
}

void MainWindow::decryptStarted()
{
    ui_.progressBar->setFormat("Decrypting %p%");
}

void MainWindow::remuxStarted()
{
    ui_.progressBar->setValue(0);
    ui_.progressBar->setFormat("Remuxing %p%");
}

void MainWindow::decryptFinished()
{
    ui_.progressBar->setFormat("Completed!");
    is_downloading_ = false;

    if (is_decryptor_ready_)
        ui_.pbDownload->setEnabled(true);
}

void MainWindow::handleVideo()
{
    updateVideoInfo(handle_->videoInfo());
    updateTracksInfo(handle_->tracksInfo());
    
}


void MainWindow::handleRegistration(QWidget*)
{
    qDebug() << "Registration requested";
}
