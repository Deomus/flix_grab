#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWebEngineWidgets>
#include <QtWebEngineCore>

#include "getflix2.h"
#include "gethulu.h"

#include "ui_main_window.h"
#include "ui_main_dialog.h"


class BrowserDialog;


class MainWindow : public QDialog//QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
 


private slots:

    void    handleVideo();


    void    updateTracksInfo(const TracksInfo&);
    void    updateVideoInfo(const VideoInfo&);

    void    handleError(QString error);
    
    void    processClicked();
    void    downloadClicked();
    void    cancelClicked();

    void    handleOpenFolder();

    void    decryptStarted();
    void    remuxStarted();
    void    decryptFinished();

    void    handleRegistration(QWidget*);
   
    
private:
    Ui::MainDialog                          ui_;

    Getflix2*                               netflix_;
    VideoHandle*                            handle_;

    bool                                    is_downloading_;
    bool                                    is_decryptor_ready_;
     
  //  DecryptEngine                       engine_;
   

};


#endif // MAINWINDOW_H
