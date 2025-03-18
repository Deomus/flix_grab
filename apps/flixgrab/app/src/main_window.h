#pragma once 

#include <QtGui>
#include "ui_main_window.h"

#include "translator.h"
#include "content_controller.h"
#include "menu_submenu.h"
#include "submenu_widget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //This function can be called before main loop is executed;
    bool                        checkMandatoryUpdate();

private slots:
    void                        createTitle();
    void                        createSettings();
    void                        createUserSettings();
    void                        createActivateSettings();
    void                        createDownloads();
    void                        createFilters();
    void                        createSorts();
    void                        createProxy();
    void                        createGetKey();
    void                        createHelp();
    void                        createOtherApps();

    void                        createSocialLinks();

private:
    SubmenuWidget*              createFolderOption(QMenu* menu);
    SubmenuWidget*              createDeleteAccountWidget(QMenu* menu);
    SubmenuWidget*              createUpdateWidget(QMenu* menu);
#if PURCHASING_BACKEND == VENDING_BACKEND
    SubmenuWidget*              createPurhcaseLicenseWidget(QMenu* menu);
    SubmenuWidget*              createCheckLicenseWidget(QMenu* menu);
    SubmenuWidget*              createDeleteLicenseWidget(QMenu* menu);
    MenuSubmenu*                createLicenseMenu(QMenu* menu);
#endif
    MenuSubmenu*                createPostDownloadOption(QMenu* menu);
    MenuSubmenu*                createLanguageMenu(QMenu* menu);
    MenuSubmenu*                createDownloadThreadsOption(QMenu* menu);
    MenuSubmenu*                createEncodingSpeedOption(QMenu* menu);
    MenuSubmenu*                createVideoSelectorOption(QMenu* menu);
    MenuSubmenu*                createAudioSelectorOption(QMenu* menu);
    MenuSubmenu*                createImageSelectorOption(QMenu* menu);
    MenuSubmenu*                createSaveAsOption(QMenu* menu);
    MenuSubmenu*                createAudioTypeOption(QMenu* menu);
    MenuSubmenu*                createAudioLanguageSelectorOption(QMenu* menu);
    MenuSubmenu*                createSubtitleLanguageOption(QMenu* menu);
    MenuSubmenu*                createSaveSubtitlesAsOption(QMenu* menu);

private:
    bool                        checkDownloadThreadsTrack(const int data);
    bool                        checkEncodingSpeedTrack(const int data);
    bool                        checkVideoStreamTrack(const int data);
    bool                        checkAudioStreamTrack(const int data);
    bool                        checkImageStreamTrack(const int data);
    bool                        checkSaveAsTrack(const int data);
    bool                        checkAudioTypeTrack(const int data);
    bool                        checkAudioLanguageTrack(const QAction* action, const QList<QAction*>& subactions);
    bool                        checkSubtitleLanguageTrack(const QString& data);
    bool                        checkSaveSubtitlesAsTrack(const int data);

private:
    void                        createShowOutputTool();
    void                        createSaveListTool();
    void                        createLoadListTool();
    void                        createSelectAllTool();
    void                        createRemoveAllTool();

private:
    void                        createGetflixHandlers();
    void                        saveList();

private:
    void                        setStartLanguage();
    QString                     presetTrackDesk(const EPreset preset);
    void                        switchLanguage(QString new_lang, bool user_clicked = false);
    void                        createProductLimitations();
    void                        createUpdater();
    void                        createSystemTray();
    QNetworkProxy::ProxyType    getProxyTypeByIndex(int index);
    bool                        checkProxy(int& retry_load_interval);
    //void                        setProxy();
    void                        deleteAccounts();
    void                        cleanupProgramData();
    void                        setHighQualitySettings();
    void                        setLowQualitySettings();
//start values
private:
    int                         user_download_threads_ = 1;
    EEncodingSelector           user_encoding_speed_ = kEncodingSelector_Medium;
    EVideoSelector              user_video_selector_ = kVideoSelector_MinimumSize;
    EAudioSelector              user_audio_selector_ = kAudioSelector_MinimumSize;
    EImageSelector              user_image_selector_ = kImageSelector_MinimumSize;
    EAudioTypeOption            user_audio_type_ = kAudioType_Original;
    EPreset                     user_save_as_ = DEFAULTPRESET;
    QList<QString>              user_subtitles_language_ = { settings::kSubtitleLanguageDefault };
    QList<int>                  user_save_subtitles_as_ = { kSaveSubtitlesAs_Original };

private:
    //void                        create(WId window, bool initializeWindow, bool destroyOldWindow) override;
    bool                        eventFilter(QObject* o, QEvent* e);
    bool                        event(QEvent *event) override;
    void                        closeEvent(QCloseEvent *event) override;
    Q_INVOKABLE void            quitDeferred();
//
//private:
//    void                        showInGraphicalShell(QWidget *parent, const QString &pathIn);

private:
    Ui::MainWindow              ui_;
    Tutorial*                   tutorial_ = nullptr;

    lang::CTranslator*          translator_ = nullptr;

    Getflix*                    getflix_ = nullptr;
    ContentController*          controller_ = nullptr;

private:
    MenuSubmenu*                language_submenu_ = nullptr;
    MenuSubmenu*                license_submenu_ = nullptr;
    SubmenuWidget*              check_license_widget_ = nullptr;
    SubmenuWidget*              delete_license_widget_ = nullptr;
    SubmenuWidget*              purhcase_license_widget_ = nullptr;
    SubmenuWidget*              update_widget_ = nullptr;
    SubmenuWidget*              delete_account_widget_ = nullptr;
    QSettings                   settings_;
    QSystemTrayIcon             tray_icon_;
    bool                        check_update_clicked_ = false;

    enum ETrayActivationReason {
        kTrayActivation_Unknown = 0,
        kTrayActivation_Update,
        kTrayActivation_Error,
    };

    ETrayActivationReason       tray_reason_ = kTrayActivation_Unknown;
    int orig_width_;

    QString scrollAreaLeftPanelStyleSheet = "QScrollArea{background-color: #3D404F;} QScrollBar:vertical{border: none;background-color: #3D404F;width: 7px;margin: 0 0 0 0;}\
    QScrollBar:vertical:hover{border: none;background-color: #3D404F;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:vertical{border-radius: 3px;border-color: #3D404F;border-width: 1px; border-style: solid; background-color: #3D404F; min-height: 30px;}\
    QScrollBar::handle:vertical:hover{border-radius: 3px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-height: 30px;}\
    QScrollBar::add-line:vertical{width: 0px; height: 0px;}\
    QScrollBar::sub-line:vertical{width: 0px; height: 0px;}\
    QScrollBar::add-page:vertical{background-color: transparent;}\
    QScrollBar::sub-page:vertical{background-color: transparent;}";

    QString QScrollBarStyleSheetHoverLeft = "QScrollBar:vertical{border: none;background-color: #3D404F;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:vertical{border-radius: 3px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-height: 30px;}";

    QString QScrollBarStyleSheetLeft = "QScrollBar:vertical{border: none;background-color: #3D404F;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:vertical{border-radius: 3px;border-color: #3D404F;border-width: 1px; border-style: solid; background-color: #3D404F; min-height: 30px;}";

    QString QScrollBarStyleSheetHover = "QScrollBar:vertical{border: none;background-color: #353945;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:vertical{border-radius: 3px;border-color: #39A5F0;border-width: 1px; border-style: solid; background-color: #39A5F0; min-height: 30px;}";

    QString QScrollBarStyleSheet = "QScrollBar:vertical{border: none;background-color: #353945;width: 7px;margin: 0 0 0 0;}\
    QScrollBar::handle:vertical{border-radius: 3px;border-color: #353945;border-width: 1px; border-style: solid; background-color: #353945; min-height: 30px;}";

protected:
    void changeEvent(QEvent* event) override {
        if (event->type() == QEvent::WindowStateChange) {
            if (!isMaximized()) {
                if (ui_.leftPanel->isVisible()) {
                    QSize sizeBeforeMaximize = size();
                    this->resize(sizeBeforeMaximize.width() + ui_.leftPanel->width(), sizeBeforeMaximize.height());
                }
            }
        }

        QMainWindow::changeEvent(event);
    }
};

