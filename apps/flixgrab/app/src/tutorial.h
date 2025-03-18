#pragma once

#include <QtCore>
#include <QtWidgets>


#include <common_config.h>
#include <product_info.h>

#include "ui_tutorial.h"


class Tutorial : public QWidget {
    Q_OBJECT
public:
    Tutorial(QWidget* parent = nullptr) : QWidget(parent) {
        ui_.setupUi(this);
        ui_.wgtScroll->setVisible(false);

        QObject::connect(ui_.chbDontShow, &QCheckBox::toggled, [=](bool checked) {
            emit(tutorialChecked(checked));
        });

        QObject::connect(ui_.btnNetflix, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.netflix.com/"));
        });

        QObject::connect(ui_.btnDisney, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.disneyplus.com/"));
        });
        
        QObject::connect(ui_.btnHulu, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.hulu.com/"));
        });

        QObject::connect(ui_.btnHBO, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://play.hbomax.com/"));
        });

        QObject::connect(ui_.btnYouTube, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.youtube.com/"));
        });

        QObject::connect(ui_.btnPornhub, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://pornhub.com/"));
        });

        QObject::connect(ui_.btnFacebook, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.facebook.com/"));
        });

        QObject::connect(ui_.btnAmazon, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.amazon.com/"));
        });

        QObject::connect(ui_.btnAmazonMusic, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://music.amazon.com/"));
        });

        QObject::connect(ui_.btnInstagram, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.instagram.com/"));
        });

        QObject::connect(ui_.btnXVideos, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.xvideos.com/"));
        });

        QObject::connect(ui_.btnTwitter, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.twitter.com/"));
        });

        QObject::connect(ui_.btnTwitch, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.twitch.tv/"));
        });

        QObject::connect(ui_.btnVimeo, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.vimeo.com/"));
        });

        QObject::connect(ui_.btnDailymotion, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://www.dailymotion.com/"));
        });

        QObject::connect(ui_.btnSpotify, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://open.spotify.com/"));
        });

        QObject::connect(ui_.btnAppleMusic, &QPushButton::clicked, []() {
            QDesktopServices::openUrl(QUrl("https://music.apple.com/"));
        });
        

        QObject::connect(ui_.btnLeftArrow, &QPushButton::clicked, [=]() {
            int new_value = ui_.scrollAreaPortals->horizontalScrollBar()->value() - ui_.btnNetflix->width() - ui_.wgtScrollArea->layout()->spacing();
            ui_.scrollAreaPortals->horizontalScrollBar()->setValue(new_value);
        });

        QObject::connect(ui_.btnRightArrow, &QPushButton::clicked, [=]() {
            int new_value = ui_.scrollAreaPortals->horizontalScrollBar()->value() + ui_.btnNetflix->width() + ui_.wgtScrollArea->layout()->spacing();
            ui_.scrollAreaPortals->horizontalScrollBar()->setValue(new_value);
        });
    }

    void                setInvisibleMode() {
        ui_.wgtCaption->setVisible(false);
        ui_.wgtDesk->setVisible(false);
        ui_.wgtLine->setVisible(false);
        ui_.wgtSteps->setVisible(false);
        ui_.chbDontShow->setVisible(false);

        ui_.wgtLayout->setProperty("invisible", true);
        ui_.wgtLayout->style()->polish(ui_.wgtLayout);
        ui_.wgtLayout->update();
    }

    void                showPortalsCarousel(bool show = false) {
        ui_.lblBrowser->setVisible(!show);
        ui_.wgtScroll->setVisible(show);
        QString product_name = PRODUCT_NAME;

        QString service_unavailable = tr("Service is unavailable now");
        
        if (product_name == "FlixGrab"
            || product_name == "FlixGrabMS") {
            //disable audio services
            ui_.btnSpotify->setVisible(!show);
            ui_.btnAppleMusic->setVisible(!show);
            ui_.btnAmazonMusic->setVisible(!show);
            
            //disable 18+ content
            ui_.btnPornhub->setVisible(!show);
            ui_.btnXVideos->setVisible(!show);
            
            //disable youtube for microsoft
            if (product_name == "FlixGrabMS"
#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
                || product_name == "FlixGrabMusic"
#endif
                )
                ui_.btnYouTube->setVisible(!show);

            if (!NETFLIXPORTALENABLED) {
                ui_.btnNetflix->setProperty("state", "disabled");
                ui_.btnNetflix->style()->polish(ui_.btnNetflix);
                ui_.btnNetflix->update();
                ui_.btnNetflix->setToolTip(service_unavailable);
            }
            
            if (!AMAZONPORTALENABLED) {
                ui_.btnAmazon->setProperty("state", "disabled");
                ui_.btnAmazon->style()->polish(ui_.btnAmazon);
                ui_.btnAmazon->update();
                ui_.btnAmazon->setToolTip(service_unavailable);
            }

            if (!DISNEYPLUSPORTALENABLED) {
                ui_.btnDisney->setProperty("state", "disabled");
                ui_.btnDisney->style()->polish(ui_.btnDisney);
                ui_.btnDisney->update();
                ui_.btnDisney->setToolTip(service_unavailable);
            }

            if (!HBOPORTALENABLED) {
                ui_.btnHBO->setProperty("state", "disabled");
                ui_.btnHBO->style()->polish(ui_.btnHBO);
                ui_.btnHBO->update();
                ui_.btnHBO->setToolTip(service_unavailable);
            }

            if (!HULUPORTALENABLED) {
                ui_.btnHulu->setProperty("state", "disabled");
                ui_.btnHulu->style()->polish(ui_.btnHulu);
                ui_.btnHulu->update();
                ui_.btnHulu->setToolTip(service_unavailable);
            }

            if (!YOUTUBEPORTALENABLED) {
                ui_.btnYouTube->setProperty("state", "disabled");
                ui_.btnYouTube->style()->polish(ui_.btnYouTube);
                ui_.btnYouTube->update();
                ui_.btnYouTube->setToolTip(service_unavailable);
            }

            if (!INSTAGRAMPORTALENABLED) {
                ui_.btnInstagram->setProperty("state", "disabled");
                ui_.btnInstagram->style()->polish(ui_.btnInstagram);
                ui_.btnInstagram->update();
                ui_.btnInstagram->setToolTip(service_unavailable);
            }

            if (!DAILYMOTIONPORTALENABLED) {
                ui_.btnDailymotion->setProperty("state", "disabled");
                ui_.btnDailymotion->style()->polish(ui_.btnDailymotion);
                ui_.btnDailymotion->update();
                ui_.btnDailymotion->setToolTip(service_unavailable);
            }

            if (!FACEBOOKPORTALENABLED) {
                ui_.btnFacebook->setProperty("state", "disabled");
                ui_.btnFacebook->style()->polish(ui_.btnFacebook);
                ui_.btnFacebook->update();
                ui_.btnFacebook->setToolTip(service_unavailable);
            }

            if (!VIMEOPORTALENABLED) {
                ui_.btnVimeo->setProperty("state", "disabled");
                ui_.btnVimeo->style()->polish(ui_.btnVimeo);
                ui_.btnVimeo->update();
                ui_.btnVimeo->setToolTip(service_unavailable);
            }

            if (!XVIDEOSPORTALENABLED) {
                ui_.btnXVideos->setProperty("state", "disabled");
                ui_.btnXVideos->style()->polish(ui_.btnXVideos);
                ui_.btnXVideos->update();
                ui_.btnXVideos->setToolTip(service_unavailable);
            }

            if (!PORNHUBPORTALENABLED) {
                ui_.btnPornhub->setProperty("state", "disabled");
                ui_.btnPornhub->style()->polish(ui_.btnPornhub);
                ui_.btnPornhub->update();
                ui_.btnPornhub->setToolTip(service_unavailable);
            }

            if (!TWITCHPORTALENABLED) {
                ui_.btnTwitch->setProperty("state", "disabled");
                ui_.btnTwitch->style()->polish(ui_.btnTwitch);
                ui_.btnTwitch->update();
                ui_.btnTwitch->setToolTip(service_unavailable);
            }

            if (!TWITTERPORTALENABLED) {
                ui_.btnTwitter->setProperty("state", "disabled");
                ui_.btnTwitter->style()->polish(ui_.btnTwitter);
                ui_.btnTwitter->update();
                ui_.btnTwitter->setToolTip(service_unavailable);
            }
        }
        else if (product_name == "FlixGrabMusic") {
            ui_.btnAppleMusic->setVisible(!show);

            //disable not supported video services
            ui_.btnAmazon->setVisible(!show);
            ui_.btnDisney->setVisible(!show);
            ui_.btnHBO->setVisible(!show);
            ui_.btnHulu->setVisible(!show);
            ui_.btnInstagram->setVisible(!show);
            ui_.btnNetflix->setVisible(!show);
            ui_.btnTwitch->setVisible(!show);
            ui_.btnTwitter->setVisible(!show);
            ui_.btnXVideos->setVisible(!show);
            ui_.btnPornhub->setVisible(!show);

            if (!SPOTIFYPORTALENABLED) {
                ui_.btnSpotify->setProperty("state", "disabled");
                ui_.btnSpotify->style()->polish(ui_.btnSpotify);
                ui_.btnSpotify->update();
                ui_.btnSpotify->setToolTip(service_unavailable);
            }

            if (!YOUTUBEPORTALENABLED) {
                ui_.btnYouTube->setProperty("state", "disabled");
                ui_.btnYouTube->style()->polish(ui_.btnYouTube);
                ui_.btnYouTube->update();
                ui_.btnYouTube->setToolTip(service_unavailable);
            }

            if (!AMAZONMUSICPORTALENABLED) {
                ui_.btnAmazonMusic->setProperty("state", "disabled");
                ui_.btnAmazonMusic->style()->polish(ui_.btnAmazonMusic);
                ui_.btnAmazonMusic->update();
                ui_.btnAmazonMusic->setToolTip(service_unavailable);
            }

            if (!FACEBOOKPORTALENABLED) {
                ui_.btnFacebook->setProperty("state", "disabled");
                ui_.btnFacebook->style()->polish(ui_.btnFacebook);
                ui_.btnFacebook->update();
                ui_.btnFacebook->setToolTip(service_unavailable);
            }

            if (!VIMEOPORTALENABLED) {
                ui_.btnVimeo->setProperty("state", "disabled");
                ui_.btnVimeo->style()->polish(ui_.btnVimeo);
                ui_.btnVimeo->update();
                ui_.btnVimeo->setToolTip(service_unavailable);
            }

            if (!DAILYMOTIONPORTALENABLED) {
                ui_.btnDailymotion->setProperty("state", "disabled");
                ui_.btnDailymotion->style()->polish(ui_.btnDailymotion);
                ui_.btnDailymotion->update();
                ui_.btnDailymotion->setToolTip(service_unavailable);
            }

            if (!APPLEMUSICPORTALENABLED) {
                ui_.btnAppleMusic->setProperty("state", "disabled");
                ui_.btnAppleMusic->style()->polish(ui_.btnAppleMusic);
                ui_.btnAppleMusic->update();
                ui_.btnAppleMusic->setToolTip(service_unavailable);
            }
        }
    }

    void                setStep1Text(const QString& text) {
        ui_.lblDesk1->setText(text);
    }

    void                translate() {
        ui_.retranslateUi(this);
    }

signals:
    void                tutorialChecked(bool checked);

private:
    Ui::Tutorial            ui_;
};
