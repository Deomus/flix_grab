#pragma once

#include <QtCore>
#include <QtWidgets>

#include "ui_alert_dialog.h"
#include "portals.h"

enum class EAlert {
    kAlert_Unknown = 0,
    kAlert_LimitHdAnd5_1,
    kAlert_LimitAudioTracks,
    kAlert_LimitAudioLanguages,
    kAlert_LimitAudioType,
    kAlert_LimitDownloadSpeed,
    kAlert_VideoLimitDuration,
    kAlert_AudioLimitDuration,
    kAlert_LimitThreads,
    kAlert_LimitEncodingSpeed,
    kAlert_LimitPresets,
    kAlert_LimitSubtitle,
    kAlert_LimitSaveSubtitlesAs,
    kAlert_LimitVideo,
    kAlert_LimitAudio,
    kAlert_LimitImage,
    kAlert_LimitPlaylist,
    kAlert_Image,
    kAlert_GetProductsData,
    kAlert_DownloadUpdate,
    kAlert_MandatoryUpdate,
    kAlert_DeleteAccounts,
    kAlert_UnexpectedExit,
    kAlert_UnexpectedExitOnDownload,
    kAlert_UnexpectedExitOnParse,
    kAlert_DeleteItems,
    kAlert_LicenceActivated,
    kAlert_LicenceDeactivated,
    kAlert_LicenseNetworkError,
    kAlert_License,
    kAlert_DeleteLicense,
    kAlert_TrialStarted,
    kAlert_TrialFinished,
    kAlert_ShutdownPC,
};

Q_DECLARE_METATYPE(EAlert)

class AlertDialog : public QDialog {
    Q_OBJECT
public:
    explicit AlertDialog(const EAlert type, const EPortal& portal = EPortal::kUnknown, const QString& main_text = "", QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    EAlert                  type() const { return type_; }

public:
    static int              execDialog(const EAlert type, const EPortal& portal = EPortal::kUnknown, const QString& main_text = "", QWidget* parent = QApplication::activeWindow());

signals:
    void                    countdownEnded(const EAlert&);

private slots:
    void                    onTimerTimeout();

private:
    void                    setType(const EAlert type, const EPortal& portal = EPortal::kUnknown, const QString& main_text = "");
    void                    startTimer(unsigned long timeout = 60);
    void                    updateMessage();

private:
    EAlert                  type_;
    Ui::AlertDialog         ui_;
    unsigned long           timeout_ = 60;
};