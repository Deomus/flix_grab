#include "main_window.h"

#include <iostream>

#include <QWebEngineView>
#include <QScreen>

#include <company_info.h>
#include <common_config.h>
#include <product_config.h>
#include <languages.h>
#include <error_codes.h>
#include <presets.h>
#include <limitations.h>

#include "utils.h"
#include "get_utils.h"
#include "app_update.h"

#include "submenu_widget.h"
#include "settings.h"

#include "product_controller.h"
#include "alert_dialog.h"
#include "tutorial.h"

#define MAINWINDOWMINIMUMHEIGHT           (445)


inline QString          autosaveFilePath() {
    return QDir(QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation)).filePath("autosave.dat");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setMouseTracking(true);

    settings::createDefaults();

    ui_.setupUi(this);
    //translator
    QApplication::translate("SupportLanguages", "English", "Here you need to write how the name of your language is written in your language.");
    translator_ = new lang::CTranslator(QStringList() << "language_%1.qm");
    //setProxy();

    QString product_name = PRODUCT_NAME;

    if (product_name == "NetflixDownloader")
        ui_.btnVideoGuides->hide();

    if (product_name == "NetflixDownloader") {
        ui_.btnFAQ->hide();
        ui_.btnBlog->hide();
        ui_.btnYouTube->hide();
        ui_.btnFacebook->hide();
        ui_.btnTwitter->hide();
        ui_.btnReddit->hide();
    }

    if (settings_.value(settings::kDeleteAccounts).toBool()) {
        deleteAccounts();
        settings_.setValue(settings::kDeleteAccounts, 0);
    }

    getflix_ = new Getflix(this);

    createActivateSettings();
    createUserSettings();

    //Content Resize And Scroll sensitivity
    ui_.scrollAreaWidgetContents->setSensitivity(QSize(ITEM_MARGIN_WIDTH, ITEM_MARGIN_HIGTH));
   
    //Connect buttons
//    QObject::connect(ui_.btnReSort, &QPushButton::released, this, &MainWindow::updateSorting);

    orig_width_ = this->geometry().width();

    QObject::connect(ui_.btnLeftPanel, &QPushButton::clicked, [=]() {
        ui_.scrollAreaLeftPanel->setStyleSheet(scrollAreaLeftPanelStyleSheet);
        if (!this->isMaximized()) {
            if (ui_.leftPanel->isVisible()) {
                settings_.setValue(settings::kLeftPanelVisible, 0);
                QSize widget_size = ui_.scrollAreaWidgetContents->sizeHint();
                int width_right_panel_content = widget_size.width();
                this->setMinimumWidth(orig_width_);
                this->resize(width_right_panel_content, this->height());
            }
            else {
                settings_.setValue(settings::kLeftPanelVisible, 1);

                QSize widget_size = ui_.scrollAreaWidgetContents->sizeHint();
                int width_right_panel_content = widget_size.width();
                int size_with_left_panel = orig_width_ + ui_.leftPanel->width();
                if (this->geometry().width() <= size_with_left_panel) {
                    this->setMinimumWidth(size_with_left_panel);
                    this->resize(width_right_panel_content + ui_.leftPanel->width(), this->height());
                }
            }
        }
        else {
            if (ui_.leftPanel->isVisible())
                settings_.setValue(settings::kLeftPanelVisible, 0);
            else 
                settings_.setValue(settings::kLeftPanelVisible, 1);
        }

        ui_.leftPanel->setVisible(settings_.value(settings::kLeftPanelVisible).toBool());
    });

    bool left_panel_visible = settings_.value(settings::kLeftPanelVisible).toBool();
    ui_.leftPanel->setVisible(left_panel_visible);
    ui_.scrollAreaLeftPanel->installEventFilter(this);
    ui_.scrollAreaLeftPanel->verticalScrollBar()->installEventFilter(this);

    ui_.scrollAreaLeftPanel->setStyleSheet(scrollAreaLeftPanelStyleSheet);
    ui_.scrollArea->setStyleSheet(scrollAreaLeftPanelStyleSheet);
    ui_.btnLeftPanel->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));

    tutorial_ = new Tutorial(ui_.scrollArea->viewport());
    if (!settings_.value(settings::kTutorialVisible).toBool())
        tutorial_->setInvisibleMode();

    tutorial_->showPortalsCarousel(product_name == "FlixGrab"
        || product_name == "FlixGrabMS"
        || product_name == "FlixGrabMusic");

    QObject::connect(tutorial_, &Tutorial::tutorialChecked, [=](bool checked) {
        settings_.setValue(settings::kTutorialVisible, !checked);
        if (checked)
            tutorial_->setInvisibleMode();
        });
    if (product_name == "FreeInstagramDownload")
        tutorial_->setStep1Text(tr("Copy a link from your browser"));
    if (DEFAULTPRESET == kPreset_MP3Original)
        tutorial_->setStep1Text(tr("Copy an audio link from your browser"));

    //setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    controller_ = new ContentController(getflix_, ui_.scrollArea, ui_.scrollAreaWidgetContents, tutorial_, this);

    QObject::connect(ui_.btnPaste, &QPushButton::clicked, controller_, &ContentController::pasteFromClipboard);
    ui_.btnPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));

    QObject::connect(ui_.btnDownload, &QPushButton::clicked, controller_, &ContentController::downloadAll);
    ui_.btnDownload->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));

    QObject::connect(ui_.btnPause, &QPushButton::clicked, controller_, &ContentController::pauseAll);
    ui_.btnPause->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));

    ui_.btnDownloads->setVisible(false);
    ui_.btnFilters->setVisible(false);
    ui_.btnFreeKey->setVisible(false);
    ui_.btnHelp->setVisible(false);
    ui_.btnOtherApps->setVisible(false);
    ui_.btnProxy->setVisible(false);
    ui_.btnSort->setVisible(false);

    ui_.wgtDownloads->setVisible(false);
    ui_.wgtFilters->setVisible(false);
    ui_.wgtFreeKey->setVisible(false);
    ui_.wgtHelp->setVisible(false);
    ui_.wgtOtherApps->setVisible(false);
    ui_.wgtProxy->setVisible(false);
    ui_.wgtSort->setVisible(false);
    ui_.wgtSocial->setVisible(false);

    //Create all other parts of ui;
    createTitle();
    createGetflixHandlers();
    createProductLimitations();
    createSettings();
    createDownloads();
    createFilters();
    createSorts();
    createProxy();
    createHelp();

    if (QString(PRODUCT_NAME) != "NetflixDownloader") {
#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
        ui_.btnFreeKey->setVisible(false);
        ui_.wgtFreeKey->setVisible(false);
#else
        createGetKey();
#endif
        createOtherApps();
    }

    createSocialLinks();
    createSystemTray();
    createUpdater();

    cleanupProgramData();

    //m_pModel->setupGeometry(QSize(214, 194), QSize(3, 3));

    //Try to resize are to content size;
    ui_.leftPanel->adjustSize();
    if (left_panel_visible)
        this->setMinimumWidth(orig_width_ + ui_.leftPanel->width());
    ui_.scrollArea->adjustSize();
    ui_.scrollArea->installEventFilter(this);
    ui_.scrollArea->verticalScrollBar()->installEventFilter(this);
    this->adjustSize();
    this->setMinimumHeight(MAINWINDOWMINIMUMHEIGHT);
     
    //Load Autosave Item List;
    if (QFileInfo::exists(autosaveFilePath()))
        controller_->loadList(autosaveFilePath());

    controller_->setSorting(kContentSorting_DateAdded);
}

MainWindow::~MainWindow() {
    qDebug() << "bye mainwindow";
    if (getflix_)
        delete getflix_;
}

void MainWindow::createTitle() {
    QString mode;
    if (ProductController::instance()->isTrial())
        mode = " " + tr("TRIAL");
    else if (ProductController::instance()->isActivated())
        mode = " " + tr("PREMIUM");

    QString beta;
#ifdef BETA
    beta = BETA;
#endif

    this->setWindowTitle(QString("%1%2%3  v.%4").arg(QApplication::applicationName()).arg(beta).arg(mode).arg(QApplication::applicationVersion()));
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    // Duplicate code
    if (event->type() == QEvent::Show
        && (obj == ui_.btnSettings->menu() || obj == ui_.btnUser->menu())) {
        QPushButton* button = ui_.btnSettings;

        if (obj == ui_.btnSettings->menu())
            QPushButton* button = ui_.btnSettings;
        else if (obj == ui_.btnUser->menu())
            QPushButton* button = ui_.btnUser;
        else 
            return false;

        button->menu()->adjustSize();

        auto const rec = QGuiApplication::primaryScreen()->geometry();
        auto const height = rec.height();
        auto const width = rec.width();

        QPoint pos_menu_lower_left_corner = button->mapToGlobal(QPoint(0, button->height()))
            + QPoint(button->menu()->width(), button->menu()->height());

        QPoint pos = button->mapToGlobal(QPoint(0, button->height()));
        if (width < pos_menu_lower_left_corner.x()
            && height > pos_menu_lower_left_corner.y()) {

            pos = button->mapToGlobal(QPoint(button->width(), button->height()))
                - QPoint(button->menu()->width(), 0);
        }
        else if (width < pos_menu_lower_left_corner.x()
            && height < pos_menu_lower_left_corner.y()) {

            QPoint pos_2 = button->mapToGlobal(QPoint(button->width(), 0))
                - QPoint(button->menu()->width(), button->menu()->height());

            if (pos_2.y() > 0)
                pos = pos_2;
            else if (width < pos_menu_lower_left_corner.x())
                pos = button->mapToGlobal(QPoint(button->width(), button->height()))
                - QPoint(button->menu()->width(), 0);
        }
        else if (width > pos_menu_lower_left_corner.x()
            && height < pos_menu_lower_left_corner.y()) {
            QPoint pos_2 = button->mapToGlobal(QPoint(0, 0))
                - QPoint(0, button->menu()->height());

            if (pos_2.y() > 0)
                pos = pos_2;
            else if (width < pos_menu_lower_left_corner.x())
                pos = button->mapToGlobal(QPoint(button->width(), button->height()))
                - QPoint(button->menu()->width(), 0);
        }

        button->menu()->move(pos);
    }
    //else if (event->type() == QEvent::MetaCall
    //    && (obj == ui_.scrollAreaLeftPanel
    //        || obj == ui_.scrollArea)) {
    //    QScrollArea* scroll_area = qobject_cast<QScrollArea*>(obj);;
    //    QScrollBar* scroll_v = scroll_area->verticalScrollBar();
    //    QScrollBar* scroll_h = scroll_area->horizontalScrollBar();
    //    int a = scroll_area->widget()->size().height() / 8;
    //    
    //    if (obj == ui_.scrollArea) {
    //        a = ui_.scrollAreaWidgetContents->get_geom_size_().height() / 3;
    //    }
    //    else {
    //        scroll_area->setStyleSheet(scrollAreaLeftPanelStyleSheet);
    //        scroll_area->style()->unpolish(scroll_area);
    //        scroll_area->style()->polish(scroll_area);
    //    }

    //    if (scroll_v->maximumHeight() != a)
    //        scroll_v->setMaximumHeight(a);
    //    if (scroll_h->maximumWidth() != a)
    //        scroll_h->setMaximumWidth(a);
    //}

    else if (event->type() == QEvent::Enter
        && (obj == ui_.scrollAreaLeftPanel->verticalScrollBar()
            || obj == ui_.scrollArea->verticalScrollBar())) {

        QScrollBar* scroll_bar = qobject_cast<QScrollBar*>(obj);
        if (obj == ui_.scrollArea->verticalScrollBar())
            scroll_bar->setStyleSheet(QScrollBarStyleSheetHover);
        else 
            scroll_bar->setStyleSheet(QScrollBarStyleSheetHoverLeft);
    }
    else if (event->type() == QEvent::Leave
        && (obj == ui_.scrollAreaLeftPanel->verticalScrollBar()
            || obj == ui_.scrollArea->verticalScrollBar())) {

        QScrollBar* scroll_bar = qobject_cast<QScrollBar*>(obj);
        if (obj == ui_.scrollArea->verticalScrollBar())
            scroll_bar->setStyleSheet(QScrollBarStyleSheet);
        else 
            scroll_bar->setStyleSheet(QScrollBarStyleSheetLeft);
    }

    return false;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    //Save Autosave Item List;
    if (!controller_->saveList(autosaveFilePath(), false))
        if (AlertDialog::execDialog(EAlert::kAlert_UnexpectedExit, EPortal::kUnknown, "", this) == QDialog::Accepted)
            saveList();
    
    if (getflix_->isDownloading()) {
        if (AlertDialog::execDialog(EAlert::kAlert_UnexpectedExitOnDownload, EPortal::kUnknown, "", this) == QDialog::Accepted)
            getflix_->saveOutputOnEsit();

        event->accept();
    }
    else {
        QMainWindow::closeEvent(event);
    }

    checkMandatoryUpdate();
}

bool MainWindow::event(QEvent * event) {

    //Ensure that window created and has HWND and can be initialized by Windows Store;
    if (event->type() == QEvent::ShowToParent) {
        ProductController::instance()->initializeUi(this);
    }

    return QMainWindow::event(event);
}

void MainWindow::createSettings() {
    QString  menuStyle(
        "QMenu{"
        "font-family: \"Tahoma\";"
        "font-size: 9pt;"
        "border: none;"
        "}"
        "QMenu::separator{"
        "background-color: #858A9B;"
        "height: 1px;"
        "}"
        "QMenu::separator:hover{"
        "background-color: #858A9B;"
        "}"
    );

    TransMenu* menu = new TransMenu(ui_.btnSettings);
    menu->setStyleSheet(menuStyle);
    menu->installEventFilter(this);
    ui_.btnSettings->setMenu(menu);

    QList<SubmenuWidget*> sub_widgets;
    QList<MenuSubmenu*> widgets;

    sub_widgets.push_back(createFolderOption(menu));
    menu->addAction(sub_widgets.last()->action());

    widgets.push_back(createPostDownloadOption(menu));
    menu->addAction(widgets.last()->action());
    widgets.last()->setSignalButton(menu);

    //Download Threads Action
    if (THREADSSELECTORENABLED) {
        widgets.push_back(createDownloadThreadsOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Encoding Speed
    if (ENCODINGSELECTORENABLED) {
        widgets.push_back(createEncodingSpeedOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Video Quality Action
    if (VIDEOSELECTORENABLED) {
        widgets.push_back(createVideoSelectorOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Audio Quality Action
    if (AUDIOSELECTORENABLED) {
        widgets.push_back(createAudioSelectorOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Image Quality Action
    if (IMAGESELECTORENABLED) {
        widgets.push_back(createImageSelectorOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //SaveAs Quality Action
    if (SAVEASSELECTORENABLED) {
        widgets.push_back(createSaveAsOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Audio Type Action
    if (AUDIOTYPESELECTORENABLED) {
        widgets.push_back(createAudioTypeOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Audio Language Action
    if (AUDIOLANGSELECTORENABLED) {
        widgets.push_back(createAudioLanguageSelectorOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    //Subtitle Language Action
    if (SUBTITLESELECTORENABLED) {
        widgets.push_back(createSubtitleLanguageOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);

        widgets.push_back(createSaveSubtitlesAsOption(menu));
        menu->addAction(widgets.last()->action());
        widgets.last()->setSignalButton(menu);
    }

    int min_width = qMax(SubmenuWidget::getNormalizeNameWidth(sub_widgets), MenuSubmenu::getNormalizeNameWidth(widgets));
    SubmenuWidget::normalizeNameWidth(sub_widgets, min_width);
    MenuSubmenu::normalizeNameWidth(widgets, min_width);
}

MenuSubmenu* MainWindow::createPostDownloadOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Post download action"), tr("Unknown"), true, menu);

    QMap<QString, int> items;

    items.insert(tr("Do nothing"), kDoNothing);
    items.insert(tr("Play sound"), kPlaySound);
    items.insert(tr("Shutdown computer"), kShutdownPC);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, false);

    //set start value
    QList<int> selected_items;
    QList<int> postdownloadaction_items = settings_.value(settings::kPostDownloadAction).value<QList<int>>();
    for (auto it : postdownloadaction_items) {
        if (it != kShutdownPC)
            selected_items << it;
    }
    if (selected_items.isEmpty())
        selected_items << kDoNothing;
    settings_.setValue(settings::kPostDownloadAction, QVariant::fromValue(selected_items));
    for (auto it : submenu->subactions()) {
        if (selected_items.contains(it->data().toInt()))
            it->setChecked(true);
    }

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        QString menu_text;

        QList<int> selected_items = settings_.value(settings::kPostDownloadAction).value<QList<int>>();
        for (auto it : submenu->subactions()) {
            if (selected_items.contains(it->data().toInt()))
                menu_text.append(it->text()).append(", ");
        }

        menu_text = menu_text.mid(0, menu_text.count() - 2);
        submenu->setValue(menu_text);
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        QList<int> selected_items = settings_.value(settings::kPostDownloadAction).value<QList<int>>();
        if (!selected_items.isEmpty()) {
            for (auto it : submenu->subactions())
                it->setChecked(selected_items.contains(it->data().toInt()));
        }
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (a->data().toInt() != kDoNothing) {
                for (auto it : submenu->subactions()) {
                    if (it->data().toInt() == kDoNothing)
                        it->setChecked(false);
                }

                a->setChecked(true);
            }
            else {
                for (auto it : submenu->subactions())
                    it->setChecked(false);
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        QList<int> selected_items;
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                selected_items << it->data().toInt();
        }

        if (selected_items.isEmpty())
            selected_items << kDoNothing;

        settings_.setValue(settings::kPostDownloadAction, QVariant::fromValue(selected_items));
    });

    return submenu;
}

SubmenuWidget* MainWindow::createFolderOption(QMenu* menu) {
    SubmenuWidget* widget = new SubmenuWidget(tr("Output folder"), settings_.value(settings::kOutputFolder).toString(), menu);
    
    QObject::connect(widget, &SubmenuWidget::triggered, [=]() {
        QString path = settings_.value(settings::kOutputFolder).toString();

        path = QFileDialog::getExistingDirectory(this, tr("Save video"), path);

        if (!path.isEmpty()) {
            settings_.setValue(settings::kOutputFolder, path);
            widget->setValue(path);
            controller_->applySettings();
            /*if (SubmenuWidget* w = qobject_cast<SubmenuWidget*>(sender())) {
                w->setValue(path);
            }*/
        }
    });

    return widget;
}

MenuSubmenu* MainWindow::createDownloadThreadsOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Maximum active downloads"), "1", true, menu);

    QMap<QString, int> items;
    items.insert("1", 1);
    items.insert("2", 2);
    items.insert("3 (" + tr("recommended") + ")", 3);
    items.insert("4", 4);
    items.insert("5", 5);
    items.insert("6 (" + tr("may cause crash") + ")", 6);
    items.insert("7 (" + tr("may cause crash") + ")", 7);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() > t2->data().toInt());
    });

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kDownloadThreads).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kDownloadThreads).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkDownloadThreadsTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitThreads);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked()
                && it->data().toInt() != settings_.value(settings::kDownloadThreads).toInt()) {

                settings_.setValue(settings::kDownloadThreads, it->data());
                getflix_->setDownloadThreads(it->data().toInt());
            }
        }
    });

    getflix_->setDownloadThreads(settings_.value(settings::kDownloadThreads).toInt());

    return submenu;
}

MenuSubmenu* MainWindow::createEncodingSpeedOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Encoding speed"), tr("Unknown"), true, menu);
    //"ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower", "veryslow", "placebo"
    QMap<QString, int> items;
    items.insert(tr("2 Pass (error correction)"), kEncodingSelector_2_pass);
    items.insert(tr("Slow"), kEncodingSelector_Slow);
    items.insert(tr("Medium"), kEncodingSelector_Medium);
    items.insert(tr("Faster"), kEncodingSelector_Faster);
    items.insert(tr("Veryfast"), kEncodingSelector_Veryfast);
    items.insert(tr("Superfast") + " (" + tr("recommended") + ")", kEncodingSelector_Superfast);
    items.insert(tr("Ultrafast"), kEncodingSelector_Ultrafast);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kEncodingSpeed).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kEncodingSpeed).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkEncodingSpeedTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitEncodingSpeed);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked() 
                && it->data().toInt() != settings_.value(settings::kEncodingSpeed).toInt()) {

                settings_.setValue(settings::kEncodingSpeed, it->data());
                controller_->applySettings();
            }
        }
    });

    return submenu;
}

MenuSubmenu* MainWindow::createVideoSelectorOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Video quality"), tr("Unknown"), true, menu);

    QMap<QString, int> items;
    items.insert(tr("Minimum size"), kVideoSelector_MinimumSize);
    items.insert(tr("Default"), kVideoSelector_Default);
    items.insert(tr("High quality"), kVideoSelector_HighQuality);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kVideoStream).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kVideoStream).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkVideoStreamTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitHdAnd5_1);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked()
                && it->data().toInt() != settings_.value(settings::kVideoStream).toInt()) {

                settings_.setValue(settings::kVideoStream, it->data());
                controller_->applySettings();
            }
        }
    });

    return submenu;
}

MenuSubmenu* MainWindow::createAudioSelectorOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Audio quality"), tr("Unknown"), true, menu);

    QMap<QString, int> items;
    items.insert(tr("Minimum size"), kAudioSelector_MinimumSize);
    items.insert(tr("Default"), kAudioSelector_Default);
    items.insert(tr("High quality"), kAudioSelector_HighQuality);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kAudioStream).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kAudioStream).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkAudioStreamTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitHdAnd5_1);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked()
                && it->data().toInt() != settings_.value(settings::kAudioStream).toInt()) {

                settings_.setValue(settings::kAudioStream, it->data());
                controller_->applySettings();
            }
        }
    });

    return submenu;
}

MenuSubmenu* MainWindow::createImageSelectorOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Image quality"), tr("Unknown"), true, menu);

    QMap<QString, int> items;
    items.insert(tr("Minimum size"), kImageSelector_MinimumSize);
    items.insert(tr("Default"), kImageSelector_Default);
    items.insert(tr("High quality"), kImageSelector_HighQuality);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kImageStream).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kImageStream).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkImageStreamTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_Image);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked()
                && it->data().toInt() != settings_.value(settings::kImageStream).toInt()) {

                settings_.setValue(settings::kImageStream, it->data());
                controller_->applySettings();
            }
        }
    });

    return submenu;

}

QString MainWindow::presetTrackDesk(const EPreset preset) {  //todo move to video_handle
    QString desk;

    switch (preset) {
        case kPreset_OriginalVideo:
            desk = tr("Original video");
            break;
        case kPreset_MP3Original:
            desk = "MP3 " + tr("original quality");
            break;
        case kPreset_MP3LameInsane:
            desk = "MP3 " + tr("high quality") + " (320 Kbps)";
            break;
        case kPreset_MP3LameExtreme:
            desk = "MP3 " + tr("high quality") + " (~230 Kbps)";
            break;
        case kPreset_MP3LameStandard:
            desk = "MP3 " + tr("normal quality") + " (~190 Kbps)";
            break;
        case kPreset_MP3LameR3mix:
            desk = "MP3 " + tr("normal quality") + " (~160 Kbps)";
            break;
        case kPreset_MP3LowQuality:
            desk = "MP3 " + tr("low quality") + " (32 Kbps)";
            break;
        case kPreset_OriginalAudio:
            desk = tr("Original audio (no conversion)");
            break;
    }

    return desk;
}

MenuSubmenu* MainWindow::createSaveAsOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Save as"), tr("Unknown"), true, menu);

    QMap<QString, int> items;
    if (VIDEOCONTENTEXIST)
        items.insert(presetTrackDesk(kPreset_OriginalVideo), kPreset_OriginalVideo);
    items.insert(presetTrackDesk(kPreset_MP3Original), kPreset_MP3Original);
    items.insert(presetTrackDesk(kPreset_MP3LameInsane), kPreset_MP3LameInsane);
    items.insert(presetTrackDesk(kPreset_MP3LameExtreme), kPreset_MP3LameExtreme);
    items.insert(presetTrackDesk(kPreset_MP3LameStandard), kPreset_MP3LameStandard);
    items.insert(presetTrackDesk(kPreset_MP3LameR3mix), kPreset_MP3LameR3mix);
    if (forcedConversionToLowQualityEnabled())
        items.insert(presetTrackDesk(kPreset_MP3LowQuality), kPreset_MP3LowQuality);
    items.insert(presetTrackDesk(kPreset_OriginalAudio), kPreset_OriginalAudio);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_front(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    if (VIDEOCONTENTEXIST
        && DEFAULTPRESET == kPreset_MP3Original) {
        actions.push_back(actions.takeFirst());
    }

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kSaveAs).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kSaveAs).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkSaveAsTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitPresets);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked()
                && it->data().toInt() != settings_.value(settings::kSaveAs).toInt()) {

                settings_.setValue(settings::kSaveAs, it->data());
                controller_->applySettings();
            }
        }
    });

    return submenu;
}

MenuSubmenu* MainWindow::createAudioTypeOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Default audio type"), tr("Unknown"), true, menu);

    QMap<QString, bool> items;
    items.insert(tr("Original"), kAudioType_Original);
    items.insert(tr("Audio for the blind"), kAudioType_AudioForBlind);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_front(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, true);

    //set start value
    for (auto it : submenu->subactions())
        it->setChecked(it->data().toInt() == settings_.value(settings::kAudioType).toInt());

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                submenu->setValue(it->text());
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        for (auto it : submenu->subactions())
            it->setChecked(it->data().toInt() == settings_.value(settings::kAudioType).toInt());
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkAudioTypeTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitAudioType);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        for (auto it : submenu->subactions()) {
            if (it->isChecked()
                && it->data().toInt() != settings_.value(settings::kAudioType).toInt()) {

                settings_.setValue(settings::kAudioType, it->data().toInt());
                controller_->applySettings();
            }
        }
    });

    return submenu;
}

MenuSubmenu* MainWindow::createAudioLanguageSelectorOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Default audio language"), kLanguagesAndCodes[0][0], true, menu);
    
    QList<QAction*> actions;
    for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[0]); ++i) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(kLanguagesAndCodes[i][0]);
        menu_checkbox->requestAction()->setData(kLanguagesAndCodes[i][1]);
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }
    
    bool disable_multiselect = false;
    QString product_name = PRODUCT_NAME;
    if (product_name == "FreeYouTubeDownload"
        || product_name == "FreeYouTubeToMP3Converter")
        disable_multiselect = true;

    submenu->createSubmenu(actions, disable_multiselect);

    //set start value
    QList<QString> selected_languages = settings_.value(settings::kAudioLanguage).value<QList<QString>>();
    for (auto it : submenu->subactions()) {
        if (selected_languages.contains(it->data().toString()))
            it->setChecked(true);
    }

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        QString menu_text;
        int checked_items = 0;
        for (auto it : submenu->subactions()) {
            if (it->isChecked()) {
                menu_text.append(it->text()).append(", ");
                checked_items++;
            }
        }

        if (checked_items > 3)
            menu_text = QString::number(checked_items) + "x " + tr("languages");
        else
            menu_text = menu_text.mid(0, menu_text.count() - 2);

        submenu->setValue(menu_text);
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {});

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            auto acts = submenu->subactions(); 

            if (a->data().toString() == kLanguagesAndCodes[0][1]) { //none case
                for (auto it : acts)
                    it->setChecked(false);
                a->setChecked(true);
            }
            else if (checkAudioLanguageTrack(a, acts)) {
                for (auto it : acts)
                    it->setChecked(false);
                a->setChecked(true);

                AlertDialog::execDialog(EAlert::kAlert_LimitAudioLanguages);
            }
            else {
                for (auto it : acts) {
                    if (it->data().toString() == kLanguagesAndCodes[0][1]) //none case
                        it->setChecked(false);
                }
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        QList<QString> selected_languages;
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                selected_languages << it->data().toString();
        }

        if (selected_languages.isEmpty()) {
            QString default_lang = "en-us";
            for (auto it : submenu->subactions()) {
                if (it->data().toString() == default_lang)
                    it->setChecked(true);
            }
            selected_languages << default_lang;
        }

        settings_.setValue(settings::kAudioLanguage, QVariant::fromValue(selected_languages));

        controller_->applySettings();
    });

    return submenu;
}

MenuSubmenu* MainWindow::createSubtitleLanguageOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Default subtitle language"), kLanguagesAndCodes[0][0], true, menu);

    QList<QAction*> actions;

    for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[0]); ++i) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(kLanguagesAndCodes[i][0]);
        menu_checkbox->requestAction()->setData(kLanguagesAndCodes[i][1]);
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    submenu->createSubmenu(actions, false);
    
    //set start value
    QList<QString> selected_languages = settings_.value(settings::kSubtitleLanguage).value<QList<QString>>();
    for (auto it : submenu->subactions()) {
        if (selected_languages.contains(it->data().toString()))
            it->setChecked(true);
    }

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        QString menu_text;

        int checked_items = 0;
        for (auto it : submenu->subactions()) {
            if (it->isChecked()) {
                menu_text.append(it->text()).append(", ");
                checked_items++;
            }
        }

        if (checked_items > 3)
            menu_text = QString::number(checked_items) + "x " + tr("languages");
        else
            menu_text = menu_text.mid(0, menu_text.count() - 2);

        submenu->setValue(menu_text);
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        QList<QString> selected_languages = settings_.value(settings::kSubtitleLanguage).value<QList<QString>>();
        if (!selected_languages.isEmpty()) {
            for (auto it : submenu->subactions())
                it->setChecked(selected_languages.contains(it->data().toString()));
        }
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (a->data().toString() == settings::kSubtitleLanguageDefault) {
                for (auto it : submenu->subactions())
                    it->setChecked(false);
                a->setChecked(true);
            }
            else if (checkSubtitleLanguageTrack(a->data().toString())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitSubtitle);
            }
            else {
                for (auto it : submenu->subactions())
                    if (it->data().toString() == settings::kSubtitleLanguageDefault)
                        it->setChecked(false);
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        QList<QString> selected_languages;
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                selected_languages << it->data().toString();
        }

        if (selected_languages.isEmpty())
            selected_languages << settings::kSubtitleLanguageDefault; //no subtitles

        settings_.setValue(settings::kSubtitleLanguage, QVariant::fromValue(selected_languages));
        controller_->applySettings();
    });

    return submenu;
}

MenuSubmenu* MainWindow::createSaveSubtitlesAsOption(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Save subtitles as"), tr("Unknown"), true, menu);

    QMap<QString, int> items;
    items.insert(tr("Embedded"), kSaveSubtitlesAs_Embedded);
    items.insert(tr("External SRT"), kSaveSubtitlesAs_SRT);
    items.insert(tr("External original"), kSaveSubtitlesAs_Original);

    QList<QAction*> actions;
    for (auto it : items.keys()) {
        MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
        menu_checkbox->setName(it);
        menu_checkbox->requestAction()->setData(items.value(it));
        menu_checkbox->requestAction()->setCheckable(true);

        actions.push_back(menu_checkbox->requestAction());
    }

    std::sort(actions.begin(), actions.end(), [](const QAction* t1, const QAction* t2)->bool {
        return (t1->data().toInt() < t2->data().toInt());
    });

    submenu->createSubmenu(actions, false);

    //set start value
    QList<int> selected_items = settings_.value(settings::kSaveSubtitlesAs).value<QList<int>>();
    for (auto it : submenu->subactions()) {
        if (selected_items.contains(it->data().toInt()))
            it->setChecked(true);
    }

    //set connections
    QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
        QString menu_text;

        QList<int> selected_items = settings_.value(settings::kSaveSubtitlesAs).value<QList<int>>();
        for (auto it : submenu->subactions()) {
            if (selected_items.contains(it->data().toInt()))
                menu_text.append(it->text()).append(", ");
        }

        menu_text = menu_text.mid(0, menu_text.count() - 2);
        submenu->setValue(menu_text);
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
        QList<int> selected_items = settings_.value(settings::kSaveSubtitlesAs).value<QList<int>>();
        if (!selected_items.isEmpty()) {
            for (auto it : submenu->subactions())
                it->setChecked(selected_items.contains(it->data().toInt()));
        }
    });

    QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
        if (a->isChecked()) {
            if (checkSaveSubtitlesAsTrack(a->data().toInt())) {
                a->setChecked(false);
                AlertDialog::execDialog(EAlert::kAlert_LimitSaveSubtitlesAs);
            }
            else {
                a->setChecked(true);
            }
        }
    });

    QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
        QList<int> selected_items;
        for (auto it : submenu->subactions()) {
            if (it->isChecked())
                selected_items << it->data().toInt();
        }

        if (selected_items.isEmpty())
            selected_items << kSaveSubtitlesAs_Original; //no subtitles

        settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(selected_items));
        controller_->applySettings();
    });

    return submenu;
}


//////////////////////////////////////////////////////////////////////////
//USER SETTINGS
void MainWindow::createUserSettings() {
    QString  menuStyle(
        "QMenu{"
            "font-family: \"Tahoma\";"
            "font-size: 9pt;"
            "border: none;"
        "}"
        "QMenu::separator{"
            "background-color: #858A9B;"
            "height: 1px;"
        "}"
        "QMenu::separator:hover{"
            "background-color: #858A9B;"
        "}"
    );

    TransMenu* menu = new TransMenu(ui_.btnUser);
    menu->setStyleSheet(menuStyle);
    menu->installEventFilter(this);
    ui_.btnUser->setMenu(menu);

    QList<SubmenuWidget*> sub_widgets;
    QList<MenuSubmenu*> widgets;

    language_submenu_ = createLanguageMenu(menu);
    widgets.push_back(language_submenu_);
    menu->addAction(widgets.last()->action());
    widgets.last()->setSignalButton(menu);
    setStartLanguage();

#if PURCHASING_BACKEND == VENDING_BACKEND
    QDateTime expiration = ProductController::instance()->expirationDate();

    if (ProductController::instance()->isActivated()) {
        if (expiration.isValid()
            && expiration >= QDateTime::currentDateTimeUtc()) {
            license_submenu_ = createLicenseMenu(menu);
            widgets.push_back(license_submenu_);
            menu->addAction(widgets.last()->action());
            widgets.last()->setSignalButton(menu);
        }
        else {
            ProductController::instance()->deactivate();
        }
    }
    else {
        if (expiration.isValid()
            && expiration >= QDateTime::currentDateTimeUtc()) {

            sub_widgets.push_back(createCheckLicenseWidget(menu));
            menu->addAction(sub_widgets.last()->action());

            sub_widgets.push_back(createDeleteLicenseWidget(menu));
            menu->addAction(sub_widgets.last()->action());
        }
        else {
            sub_widgets.push_back(createPurhcaseLicenseWidget(menu));
            menu->addAction(sub_widgets.last()->action());
        }
    }
#endif

    sub_widgets.push_back(createUpdateWidget(menu));
    menu->addAction(sub_widgets.last()->action());

    if (ACCOUNTMENUENABLED) {
        sub_widgets.push_back(createDeleteAccountWidget(menu));
        menu->addAction(sub_widgets.last()->action());
    }

    int min_width = qMax(SubmenuWidget::getNormalizeNameWidth(sub_widgets), MenuSubmenu::getNormalizeNameWidth(widgets));
    SubmenuWidget::normalizeNameWidth(sub_widgets, min_width);
    MenuSubmenu::normalizeNameWidth(widgets, min_width);
}

void MainWindow::createShowOutputTool() {
    QObject::connect(ui_.btnShowOutput, &QPushButton::clicked, [=]() {
        QString path = settings_.value(settings::kOutputFolder).toString();
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    });

    ui_.btnShowOutput->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
}

void MainWindow::saveList() {
    QString path = settings_.value(settings::kListPath).toString();
    path = QFileDialog::getSaveFileName(this
        , tr("Save list"), path
        , tr("Text files") + " (*.txt *.lst *.list)");
    settings_.setValue(settings::kListPath, path);
    controller_->saveList(path);
}

void MainWindow::createSaveListTool() {
    QObject::connect(ui_.btnSaveList, &QPushButton::clicked, this, &MainWindow::saveList);
    ui_.btnSaveList->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
}

void MainWindow::createLoadListTool() {
    QObject::connect(ui_.btnLoadList, &QPushButton::clicked, [=]() {
        QString path = settings_.value(settings::kListPath).toString();
        path = QFileDialog::getOpenFileName(this
                                            , tr("Load list"), path
                                            , tr("Text files") + " (*.txt *.lst *.list)");
        settings_.setValue(settings::kListPath, path);
        controller_->loadList(path);
    });

    ui_.btnLoadList->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
}

void MainWindow::createSelectAllTool() {
    QObject::connect(ui_.btnSelectAll, &QPushButton::clicked, [=]() {
        controller_->selectAllVisibles();
    });

    ui_.btnSelectAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
}

void MainWindow::createRemoveAllTool() {
    QObject::connect(ui_.btnRemoveAll, &QPushButton::clicked, [=]() {
        controller_->removeAllVisibles();
    });

    ui_.btnRemoveAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
}

SubmenuWidget* MainWindow::createDeleteAccountWidget(QMenu* menu) {
    delete_account_widget_ = new SubmenuWidget(tr("Delete accounts data"), "", menu);

    QObject::connect(delete_account_widget_, &SubmenuWidget::triggered, [=]() {
        if (AlertDialog::execDialog(EAlert::kAlert_DeleteAccounts) == QDialog::Accepted) {
            settings_.setValue(settings::kDeleteAccounts, 1);
            QMetaObject::invokeMethod(this, "quitDeferred", Qt::QueuedConnection);
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        }
    });

    return delete_account_widget_;
}

SubmenuWidget* MainWindow::createUpdateWidget(QMenu* menu) {
    update_widget_ = new SubmenuWidget(tr("Check for updates"), "", menu);

    QObject::connect(update_widget_, &SubmenuWidget::triggered, [=]() {
        if (tray_reason_ == kTrayActivation_Update) {
            ProductController::instance()->installUpdate();
        }
#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
        else {
            tray_reason_ = kTrayActivation_Update;
            tray_icon_.showMessage(
                QApplication::applicationName(),
                tr("No updates available"),
                QSystemTrayIcon::Information, 10000);
        }
#endif
#if PURCHASING_BACKEND == VENDING_BACKEND
        else if (!check_update_clicked_) {
            check_update_clicked_ = true;

            update_widget_->setName(tr("Checking update..."));

            ProductController::instance()->downloadProductData();
        }
#endif
    });

    QObject::connect(ProductController::instance(), &ProductController::updateAvailable, [=](bool available) {
        QString text = tr("Check for updates");
        if (available) {
            text = tr("Update is available");
            tray_icon_.showMessage(
                QApplication::applicationName(),
                text,
                QSystemTrayIcon::Information, 10000);
        }
        else {
            if (check_update_clicked_) {
                check_update_clicked_ = false;
                tray_icon_.showMessage(
                    QApplication::applicationName(),
                    tr("No updates available"),
                    QSystemTrayIcon::Information, 10000);
            }
        }

        update_widget_->setName(text);
    });

    QObject::connect(ProductController::instance(), &ProductController::updateProgress, [=](float progress) {
        check_update_clicked_ = true;
        update_widget_->setName(tr("Update progress") + QString(" %1").arg((int)progress) + "%");
    });

    QObject::connect(ProductController::instance(), &ProductController::updateDownloaded, [=]() {
        check_update_clicked_ = false;
        tray_reason_ = kTrayActivation_Update;
        update_widget_->setName(tr("Install update")); //CHECK
        tray_icon_.showMessage(
            QApplication::applicationName(),
            tr("Update is downloaded"),
            QSystemTrayIcon::Information, 10000);
    });

    return update_widget_;
}
#if PURCHASING_BACKEND == VENDING_BACKEND

SubmenuWidget* MainWindow::createPurhcaseLicenseWidget(QMenu* menu) {
    purhcase_license_widget_ = new SubmenuWidget(tr("Purchase license key"), "", menu);

    QObject::connect(purhcase_license_widget_, &SubmenuWidget::triggered, [=]() {
        ProductController::instance()->execPurchase();
    });

    return purhcase_license_widget_;
}

SubmenuWidget* MainWindow::createCheckLicenseWidget(QMenu* menu) {
    check_license_widget_ = new SubmenuWidget(tr("Check license"), "", menu);
    check_license_widget_->setTextStylesheet("QLabel {color: #D42B18;}");
        
    QObject::connect(check_license_widget_, &SubmenuWidget::triggered, [=]() {
        check_license_widget_->setEnabled(false);
        check_license_widget_->setName(tr("Checking license..."));
        ProductController::instance()->verify(true);
    });
    
    return check_license_widget_;
}

SubmenuWidget* MainWindow::createDeleteLicenseWidget(QMenu* menu) {
    delete_license_widget_ = new SubmenuWidget(tr("Delete license"), "", menu);

    QObject::connect(delete_license_widget_, &SubmenuWidget::triggered, [=]() {
        AlertDialog::execDialog(EAlert::kAlert_DeleteLicense);
    });

    return delete_license_widget_;
}

MenuSubmenu* MainWindow::createLicenseMenu(QMenu* menu) {
    QString license_status;
    QDateTime expiration = ProductController::instance()->expirationDate();
    QList<QAction*> actions;
    if (expiration < QDateTime::currentDateTimeUtc())
        license_status = tr("Expired");
    else if (ProductController::instance()->isActivated())
        license_status = tr("Active");

    if (license_status.isEmpty()) {
        license_status = tr("No license");
    }
    else {
        QMap<QString, QString> items;
        items.insert(tr("Valid to"), QLocale("en_EN").toString(expiration, "MMMM dd yyyy"));
        items.insert(tr("Email"), ProductController::instance()->registeredEmail());
        items.insert(tr("Key"), ProductController::instance()->registeredKey());

        QList<SubmenuWidget*> widgets;

        for (auto it : items.keys()) {
            SubmenuWidget* widget = new SubmenuWidget(it, items.value(it), this);
            QObject::connect(widget, &SubmenuWidget::triggered, [=]() {
                QApplication::clipboard()->setText(items.value(it));
                tray_icon_.showMessage(QApplication::applicationName(), it + " " + tr("copied to clipboard"), QSystemTrayIcon::Information, 10000);
                });

            widgets.push_back(widget);
            actions.push_back(widget->action());
        }

        if (license_status == tr("Active")) {
            SubmenuWidget* widget = new SubmenuWidget(tr("Delete license"), "", this);

            QObject::connect(widget, &SubmenuWidget::triggered, [=]() {
                AlertDialog::execDialog(EAlert::kAlert_DeleteLicense);
            });

            widgets.push_back(widget);
            actions.push_back(widget->action());
        }
        else if (license_status == tr("Expired")) {
            SubmenuWidget* widget = new SubmenuWidget(tr("Renew license"), "", this);
            //widget->setTextStyleSheet("QLabel {border:none; color: #FAD00F; padding: 6 11 6 11; font: normal 9pt \"Tahoma\";}");

            QObject::connect(widget, &SubmenuWidget::triggered, [=]() {
                ProductController::instance()->execPurchase();
                });

            widgets.push_back(widget);
            actions.push_back(widget->action());
        }

        int min_width = SubmenuWidget::getNormalizeNameWidth(widgets);
        SubmenuWidget::normalizeNameWidth(widgets, min_width);
    }

    MenuSubmenu* submenu = new MenuSubmenu(tr("License"), license_status, true, menu);
    if (!actions.isEmpty())
        submenu->createSubmenu(actions, true);


    return submenu;
}
#endif

void MainWindow::switchLanguage(QString new_lang, bool user_clicked) {
    if (new_lang.isEmpty())
        new_lang = "en";

    if (translator_
        && !translator_->switchLang(new_lang))
        qDebug() << "translate error new_lang: " << new_lang;

    ui_.retranslateUi(this);
    if (language_submenu_)
        language_submenu_->setName(tr("Language"));

#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
    if (!ProductController::instance()->isActivated())
        ui_.btnActivateKey->setText(tr("Get Premium"));
#endif

    if (user_clicked) {
        if (tutorial_)
            tutorial_->translate();

        createTitle();

        if (license_submenu_)
            license_submenu_->setName(tr("License"));
        
        if (check_license_widget_)
            check_license_widget_->setName(tr("Check license"));

        if (delete_license_widget_)
            delete_license_widget_->setName(tr("Delete license"));
        
        if (update_widget_)
            update_widget_->setName(tr("Check for updates"));
        
        if (delete_account_widget_)
            delete_account_widget_->setName(tr("Delete accounts data"));

        if (purhcase_license_widget_)
            purhcase_license_widget_->setName(tr("Purchase license key"));

        delete ui_.btnSettings->menu();
        createSettings();
    }
}

void MainWindow::setStartLanguage() {
    //set start value
    if (language_submenu_) {
        bool language_found = false;
        for (auto it : language_submenu_->subactions()) {
            if (it->data().toString() == settings_.value(settings::kLanguage).toString()) {
                language_found = true;
                it->setChecked(true);
                switchLanguage(lang::getShortLangByFullLangName(it->data().toString()));
            }
        }
        if (!language_found) {
            settings_.setValue(settings::kLanguage, lang::getFullLangByShortName(QString("en")));
            for (auto it : language_submenu_->subactions()) {
                if (it->data().toString() == settings_.value(settings::kLanguage).toString()) {
                    it->setChecked(true);
                    switchLanguage(lang::getShortLangByFullLangName(it->data().toString()));
                }
            }
        }
    }
}

MenuSubmenu* MainWindow::createLanguageMenu(QMenu* menu) {
    MenuSubmenu* submenu = new MenuSubmenu(tr("Language"), tr("Unknown"), true, menu);
    
    if (translator_) {
        QList<QAction*> actions;
        QStringList::const_iterator it;
        QStringList available_langs = translator_->supportedLangs();
        for (it = available_langs.begin(); it != available_langs.end(); it++) {
            QString lang = translator_->fullLangName(*it);
            if (!lang.isEmpty()
                && lang != "DUITN") {

                MenuCheckbox* menu_checkbox = new MenuCheckbox(this);
                menu_checkbox->setName(lang);
                menu_checkbox->requestAction()->setData(lang::getFullLangByShortName(*it));
                menu_checkbox->requestAction()->setCheckable(true);
                actions.push_back(menu_checkbox->requestAction());
            }
        }

        QObject::connect(submenu, &MenuSubmenu::aboutToShow, [=]() {
            for (auto it : actions) {
                if (it->isChecked())
                    submenu->setValue(it->text());
            }
        });

        QObject::connect(submenu, &MenuSubmenu::submenuAboutToShow, [=]() {
            for (auto it : actions)
                it->setChecked(it->data().toString() == settings_.value(settings::kLanguage).toString());
        });

        QObject::connect(submenu, &MenuSubmenu::subactionTriggered, [=](QAction* a) {
            a->setChecked(true);
        });

        QObject::connect(submenu, &MenuSubmenu::submenuAboutToHide, [=]() {
            for (auto it : actions) {
                if (it->isChecked()
                    && it->data().toString() != settings_.value(settings::kLanguage).toString()) {

                    settings_.setValue(settings::kLanguage, it->data().toString());
                    switchLanguage(lang::getShortLangByFullLangName(it->data().toString()), true);

                    controller_->translateItem();
                }
            }
        });

        if (!actions.isEmpty())
            submenu->createSubmenu(actions, true);
    }

    return submenu;
}

void MainWindow::setHighQualitySettings() {
    settings_.setValue(settings::kEncodingSpeed, kEncodingSelector_Ultrafast);
    settings_.setValue(settings::kVideoStream, kVideoSelector_HighQuality);
    settings_.setValue(settings::kAudioStream, kAudioSelector_HighQuality);
    settings_.setValue(settings::kImageStream, kImageSelector_HighQuality);
    settings_.setValue(settings::kSaveAs, DEFAULTPRESET);
    settings_.setValue(settings::kAudioType, kAudioType_Original);
    
    if (ProductController::instance()->isActivated()
        || !contentLengthLimitEnabled()) {
        settings_.setValue(settings::kDownloadThreads, 3);
        QList<int> selected_items({ kSaveSubtitlesAs_Embedded });
        settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(selected_items));
    }
    else {
        settings_.setValue(settings::kDownloadThreads, 1);

        QList<QString> selected_subtitle_languages({ settings::kSubtitleLanguageDefault });
        settings_.setValue(settings::kSubtitleLanguage, QVariant::fromValue(selected_subtitle_languages));

        QList<int> selected_items({ kSaveSubtitlesAs_Original });
        settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(selected_items));
    }
}

void MainWindow::setLowQualitySettings() {
    settings_.setValue(settings::kDownloadThreads, 1);
    settings_.setValue(settings::kEncodingSpeed, kEncodingSelector_Medium);
    settings_.setValue(settings::kVideoStream, kVideoSelector_MinimumSize);
    settings_.setValue(settings::kAudioStream, kAudioSelector_MinimumSize);
    settings_.setValue(settings::kImageStream, kImageSelector_MinimumSize);
    settings_.setValue(settings::kAudioType, kAudioType_Original);
    if (forcedConversionToLowQualityEnabled())
        settings_.setValue(settings::kSaveAs, kPreset_MP3LowQuality);

    QList<QString> selected_subtitle_languages({ settings::kSubtitleLanguageDefault });
    settings_.setValue(settings::kSubtitleLanguage, QVariant::fromValue(selected_subtitle_languages));

    QList<int> selected_items({ kSaveSubtitlesAs_Original });
    settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(selected_items));
}

void MainWindow::createActivateSettings() {
    //remember user choise
    user_download_threads_ = settings_.value(settings::kDownloadThreads).toInt();
    user_encoding_speed_ = EEncodingSelector(settings_.value(settings::kEncodingSpeed).toInt());
    user_video_selector_ = EVideoSelector(settings_.value(settings::kVideoStream).toInt());
    user_audio_selector_ = EAudioSelector(settings_.value(settings::kAudioStream).toInt());
    user_image_selector_ = EImageSelector(settings_.value(settings::kImageStream).toInt());
    user_audio_type_ = EAudioTypeOption(settings_.value(settings::kAudioType).toInt());
    user_save_as_ = EPreset(settings_.value(settings::kSaveAs).toInt());
    user_subtitles_language_ = settings_.value(settings::kSubtitleLanguage).value<QList<QString>>();
    user_save_subtitles_as_ = settings_.value(settings::kSaveSubtitlesAs).value<QList<int>>();

    QObject::connect(ui_.btnActivateKey, &QPushButton::clicked, [=]() {
        if (!ProductController::instance()->isActivated()
            || ProductController::instance()->isTrial())
            ProductController::instance()->execActivation();
    });

    QObject::connect(ProductController::instance(), &ProductController::licenseChanged, [=](bool forced) {
        if (check_license_widget_)
            check_license_widget_->setEnabled(true);

        getflix_->setActivated(ProductController::instance()->isActivated());

        if (ProductController::instance()->isActivated()
            || contentLengthLimitEnabled()) {

            ui_.btnActivateKey->setVisible(false);
            ui_.btnUser->setProperty("state", "premium");
            ui_.btnUser->style()->polish(ui_.btnUser);
            ui_.btnUser->update();
            
            if (forced) {   //restore user choise
                settings_.setValue(settings::kDownloadThreads, user_download_threads_);
                settings_.setValue(settings::kEncodingSpeed, user_encoding_speed_);
                settings_.setValue(settings::kVideoStream, user_video_selector_);
                settings_.setValue(settings::kAudioStream, user_audio_selector_);
                settings_.setValue(settings::kImageStream, user_image_selector_);
                settings_.setValue(settings::kAudioType, user_audio_type_);
                settings_.setValue(settings::kSaveAs, user_save_as_);
                settings_.setValue(settings::kSubtitleLanguage, QVariant::fromValue(user_subtitles_language_));
                settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(user_save_subtitles_as_));
            }
            else {   //set premium settings
                setHighQualitySettings();
            }
        }
        else {
            ui_.btnActivateKey->setVisible(true);
            ui_.btnUser->setProperty("state", "free");
#if PURCHASING_BACKEND == VENDING_BACKEND
            QDateTime expiration = ProductController::instance()->expirationDate();
            if (expiration.isValid()
                && expiration >= QDateTime::currentDateTimeUtc())
                ui_.btnUser->setProperty("state", "check");
#endif
            ui_.btnUser->style()->polish(ui_.btnUser);
            ui_.btnUser->update();

            if (contentLengthLimitEnabled()) {
                if (forced) {   //restore user choise
                    settings_.setValue(settings::kDownloadThreads, user_download_threads_);
                    settings_.setValue(settings::kEncodingSpeed, user_encoding_speed_);
                    settings_.setValue(settings::kVideoStream, user_video_selector_);
                    settings_.setValue(settings::kAudioStream, user_audio_selector_);
                    settings_.setValue(settings::kImageStream, user_image_selector_);
                    settings_.setValue(settings::kAudioType, user_audio_type_);
                    settings_.setValue(settings::kSaveAs, user_save_as_);
                    settings_.setValue(settings::kSubtitleLanguage, QVariant::fromValue(user_subtitles_language_));
                    settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(user_save_subtitles_as_));
                }
                else {   //set premium settings
                    setHighQualitySettings();
                }
            }
            else {
                setLowQualitySettings();
            }
        }

        delete ui_.btnUser->menu();
        createUserSettings();

        delete ui_.btnSettings->menu();
        createSettings();

        controller_->applySettings();
        createTitle();
    });

    if (!ProductController::instance()->isActivated()) {
        if (contentLengthLimitEnabled()) {
            settings_.setValue(settings::kDownloadThreads, 1);
            settings_.setValue(settings::kEncodingSpeed, user_encoding_speed_);
            settings_.setValue(settings::kVideoStream, user_video_selector_);
            settings_.setValue(settings::kAudioStream, user_audio_selector_);
            settings_.setValue(settings::kImageStream, user_image_selector_);
            settings_.setValue(settings::kAudioType, user_audio_type_);
            settings_.setValue(settings::kSaveAs, user_save_as_);
            settings_.setValue(settings::kSubtitleLanguage, QVariant::fromValue(user_subtitles_language_));
            settings_.setValue(settings::kSaveSubtitlesAs, QVariant::fromValue(user_save_subtitles_as_));
        }
        else {
            setLowQualitySettings();
        }

        ui_.btnActivateKey->setVisible(true);
#if PURCHASING_BACKEND == VENDING_BACKEND
        QDateTime expiration = ProductController::instance()->expirationDate();
        if (expiration.isValid()
            && expiration >= QDateTime::currentDateTimeUtc()) {
            ui_.btnActivateKey->setVisible(false);
            ui_.btnUser->setProperty("state", "check");
        }
#endif
#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
        ui_.btnActivateKey->setText(tr("Get Premium"));
#endif
    }
    else {
        ui_.btnActivateKey->setVisible(false);
        ui_.btnUser->setProperty("state", "premium");
    }

    ui_.btnUser->style()->polish(ui_.btnUser);
    ui_.btnUser->update();
}

void MainWindow::createDownloads() {
    QObject::connect(ui_.btnDownloads, &QPushButton::clicked, [=]() {
        if (ui_.wgtDownloads->isVisible()) {
            settings_.setValue(settings::kDownloadsPanelVisible, 0);
            ui_.btnDownloads->setProperty("list", "closed");
        }
        else {
            settings_.setValue(settings::kDownloadsPanelVisible, 1);

            ui_.btnDownloads->setProperty("list", "opened");

            ui_.wgtDownloads->setProperty("list", "opened");
            ui_.wgtDownloads->style()->polish(ui_.wgtDownloads);
            ui_.wgtDownloads->update();
        }

        ui_.btnDownloads->style()->polish(ui_.btnDownloads);
        ui_.btnDownloads->update();
        ui_.wgtDownloads->setVisible(settings_.value(settings::kDownloadsPanelVisible).toBool());
    });

    createShowOutputTool();
    createSelectAllTool();
    createRemoveAllTool();
    createSaveListTool();
    createLoadListTool();

    ui_.wgtDownloads->setVisible(settings_.value(settings::kDownloadsPanelVisible).toBool());
    if (settings_.value(settings::kDownloadsPanelVisible).toBool()) {
        ui_.wgtDownloads->setProperty("list", "opened");
        ui_.wgtDownloads->style()->polish(ui_.wgtDownloads);
        ui_.wgtDownloads->update();

        ui_.btnDownloads->setProperty("list", "opened");
        ui_.btnDownloads->style()->polish(ui_.btnDownloads);
        ui_.btnDownloads->update();
    }

    ui_.btnDownloads->setVisible(true);
}

void MainWindow::createFilters() {
    QObject::connect(ui_.btnFilters, &QPushButton::clicked, [=]() {
        if (ui_.wgtFilters->isVisible()) {
            settings_.setValue(settings::kFiltersPanelVisible, 0);
            ui_.btnFilters->setProperty("list", "closed");
        }
        else {
            settings_.setValue(settings::kFiltersPanelVisible, 1);
            
            ui_.btnFilters->setProperty("list", "opened");

            ui_.wgtFilters->setProperty("list", "opened");
            ui_.wgtFilters->style()->polish(ui_.wgtFilters);
            ui_.wgtFilters->update();
        }
        ui_.btnFilters->style()->polish(ui_.btnFilters);
        ui_.btnFilters->update();
        ui_.wgtFilters->setVisible(settings_.value(settings::kFiltersPanelVisible).toBool());
    });

    QButtonGroup* filter_group = new QButtonGroup(ui_.wgtFilters);

    filter_group->addButton(ui_.btnFltAll, kContentFilter_All);
    filter_group->addButton(ui_.btnFltSettingUp, kContentFilter_SettingUp);
    filter_group->addButton(ui_.btnFltDownloading, kContentFilter_Downloading);
    filter_group->addButton(ui_.btnFltPaused, kContentFilter_Paused);
    filter_group->addButton(ui_.btnFltCompleted, kContentFilter_Completed);
    filter_group->addButton(ui_.btnFltFailed, kContentFilter_Failed);
        
    filter_group->setExclusive(true);
    
    QObject::connect(filter_group, &QButtonGroup::idClicked
        , [=](int id) {
        this->controller_->setFilter(static_cast<EContentFilter>(id));
    });
     
    QObject::connect(controller_, &ContentController::filterSizeChanged
        , [=](EContentFilter filter, int size) {
        if (auto button = qobject_cast<NotificationButton*>(filter_group->button(filter))) {
            if (size > 0) 
                button->setNotificationText(QString::number(size));
            else
                button->hideNotification();
        }
    });

    QObject::connect(controller_, &ContentController::filterAboutToSwitch
        , [=](EContentFilter filter) {
        if (auto button = qobject_cast<NotificationButton*>(filter_group->button(filter))) {
            button->click();
        }
    });

    QObject::connect(controller_, &ContentController::filterChanged
        , [=](EContentFilter filter) {
             
        if (filter_group->checkedId() != filter) {
            if (auto button = qobject_cast<NotificationButton*>(filter_group->button(filter))) {
                button->click();
            }
        }
    });

    ui_.btnFltAll->click();

    ui_.wgtFilters->setVisible(settings_.value(settings::kFiltersPanelVisible).toBool());
    if (settings_.value(settings::kFiltersPanelVisible).toBool()) {
        ui_.wgtFilters->setProperty("list", "opened");
        ui_.wgtFilters->style()->polish(ui_.wgtFilters);
        ui_.wgtFilters->update();

        ui_.btnFilters->setProperty("list", "opened");
        ui_.btnFilters->style()->polish(ui_.btnFilters);
        ui_.btnFilters->update();
    }

    ui_.btnFilters->setVisible(true);
}

void MainWindow::createSorts() {
    QObject::connect(ui_.btnSort, &QPushButton::clicked, [=]() {
        if (ui_.wgtSort->isVisible()) {
            settings_.setValue(settings::kSortPanelVisible, 0);
            ui_.btnSort->setProperty("list", "closed");
        }
        else {
            settings_.setValue(settings::kSortPanelVisible, 1);

            ui_.btnSort->setProperty("list", "opened");

            ui_.wgtSort->setProperty("list", "opened");
            ui_.wgtSort->style()->polish(ui_.wgtSort);
            ui_.wgtSort->update();
        }
        ui_.btnSort->style()->polish(ui_.btnSort);
        ui_.btnSort->update();
        ui_.wgtSort->setVisible(settings_.value(settings::kSortPanelVisible).toBool());
    });
    
    QButtonGroup* sort_group = new QButtonGroup(ui_.wgtSort);
    sort_group->addButton(ui_.btnSortDateAdded, kContentSorting_DateAdded);
    sort_group->addButton(ui_.btnSortVideoTitle, kContentSorting_VideoTitle);
    sort_group->addButton(ui_.btnSortVideoSize, kContentSorting_VideoSize);
    sort_group->addButton(ui_.btnSortVideoDuration, kContentSorting_VideoDuration);
    if (DEFAULTPRESET == kPreset_OriginalVideo
        || DEFAULTPRESET == kPreset_JPG)
        sort_group->addButton(ui_.btnSortVideoResolution, kContentSorting_VideoResolution);
    else
        ui_.btnSortVideoResolution->setVisible(false);
    QString prod_name = PRODUCT_NAME;
    if (prod_name == "FlixGrab"
        || prod_name == "FlixGrabMS"
        || prod_name == "FlixGrabMusic")
        sort_group->addButton(ui_.btnSortPortalName, kContentSorting_PortalName);
    else
        ui_.btnSortPortalName->setVisible(false);

    sort_group->addButton(ui_.btnSortoutputMediaPath, kContentSorting_outputMediaPath);
    ui_.btnSortoutputMediaPath->setVisible(false);

    sort_group->setExclusive(true);
    
    QObject::connect(sort_group, &QButtonGroup::idClicked
        , [=](int id) {
        this->controller_->setSorting(static_cast<EContentSorting>(id));
    });

    ui_.wgtSort->setVisible(settings_.value(settings::kSortPanelVisible).toBool());
    if (settings_.value(settings::kSortPanelVisible).toBool()) {
        ui_.wgtSort->setProperty("list", "opened");
        ui_.wgtSort->style()->polish(ui_.wgtSort);
        ui_.wgtSort->update();

        ui_.btnSort->setProperty("list", "opened");
        ui_.btnSort->style()->polish(ui_.btnSort);
        ui_.btnSort->update();
    }

    ui_.btnSort->setVisible(true);
}

bool MainWindow::checkProxy(int& retry_load_interval) {
    bool res = false;
    QString data;
    QList<QPair<QByteArray, QByteArray>>  headers;
    headers.append(qMakePair(QByteArray("User-Agent"), QByteArray(USER_AGENT)));

    QElapsedTimer timer;
    timer.start();

    if (downloadToString(testProxyUrl(PRODUCT_NAME), data)) {//check https://www.netflix.com/ through proxy
        QString prod_name = PRODUCT_NAME;
        if (prod_name == "FlixGrab"
            || prod_name == "FlixGrabMS"
            || prod_name == "FreeNetflixDownload"
            || prod_name == "VideoDownloaderForNF"
            || prod_name == "NetflixDownloader"
            || (!data.isEmpty() && !data.contains("302 Found"))) {
            res = true;
            retry_load_interval = timer.elapsed() * 3;
        }
    }

    return res;
}

void MainWindow::createProxy() {
    //proxy panel visible
    QObject::connect(ui_.btnProxy, &QPushButton::clicked, [=]() {
        if (ui_.wgtProxy->isVisible()) {
            settings_.setValue(settings::kProxyPanelVisible, 0);
            ui_.btnProxy->setProperty("list", "closed");
        }
        else {
            settings_.setValue(settings::kProxyPanelVisible, 1);
            ui_.btnProxy->setProperty("list", "opened");
            ui_.wgtProxy->setProperty("list", "opened");
            ui_.wgtProxy->style()->polish(ui_.wgtProxy);
            ui_.wgtProxy->update();
        }
        ui_.btnProxy->style()->polish(ui_.btnProxy);
        ui_.btnProxy->update();
        ui_.wgtProxy->setVisible(settings_.value(settings::kProxyPanelVisible).toBool());
    });

    ui_.wgtProxy->setVisible(settings_.value(settings::kProxyPanelVisible).toBool());
    if (settings_.value(settings::kProxyPanelVisible).toBool()) {
        ui_.wgtProxy->setProperty("list", "opened");
        ui_.wgtProxy->style()->polish(ui_.wgtProxy);
        ui_.wgtProxy->update();

        ui_.btnProxy->setProperty("list", "opened");
        ui_.btnProxy->style()->polish(ui_.btnProxy);
        ui_.btnProxy->update();
    }

    //set value on start
    ui_.edPassword->setEchoMode(QLineEdit::Password);
    ui_.cmbProxyType->setCurrentIndex(settings_.value(settings::kProxyType).toInt());
    ui_.edAddress->setText(settings_.value(settings::kProxyAddress).toString());
    ui_.edPort->setText(settings_.value(settings::kProxyPort).toString());
    ui_.cmbProxyType->setCurrentIndex(settings_.value(settings::kProxyType).toInt());
    
    QPair<QString, QString> cred = settings_.value(settings::kProxyCred).value<QPair<QString, QString>>();
    ui_.edLogin->setText(cred.first);
    ui_.edPassword->setText(cred.second);

    if (ui_.cmbProxyType->currentIndex() > 1) {
        ui_.btnCheckProxy->setEnabled(true);
        ui_.btnCheckProxy->setProperty("state", "enabled");
        ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
        ui_.btnCheckProxy->update();

        ui_.wgtLblCred->setProperty("state", "enabled");
        ui_.wgtLblCred->style()->polish(ui_.wgtLblCred);
        ui_.wgtLblCred->update();

        ui_.wgtLblAuth->setProperty("state", "enabled");
        ui_.wgtLblAuth->style()->polish(ui_.wgtLblAuth);
        ui_.wgtLblAuth->update();

        ui_.wgtEnableProxy->setProperty("state", "enabled");
        ui_.wgtEnableProxy->style()->polish(ui_.wgtEnableProxy);
        ui_.wgtEnableProxy->update();

        ui_.wgtEnableAuth->setProperty("state", "enabled");
        ui_.wgtEnableAuth->style()->polish(ui_.wgtEnableAuth);
        ui_.wgtEnableAuth->update();

        ui_.wgtProxyStatus->setProperty("state", "enabled");
        ui_.wgtProxyStatus->style()->polish(ui_.wgtProxyStatus);
        ui_.wgtProxyStatus->update();
        
        if (!ui_.edAddress->text().isEmpty()
            && !ui_.edPort->text().isEmpty()) {

            if ((ui_.edLogin->text().isEmpty() && ui_.edPassword->text().isEmpty())
                || (!ui_.edLogin->text().isEmpty() && !ui_.edPassword->text().isEmpty())) {
                QNetworkProxy::setApplicationProxy(QNetworkProxy(getProxyTypeByIndex(ui_.cmbProxyType->currentIndex()), ui_.edAddress->text(), ui_.edPort->text().toInt(), ui_.edLogin->text(), ui_.edPassword->text()));
                getflix_->setRetryLoadInterval(settings_.value(settings::kProxyRetryLoadInterval).toInt());
            }
        }
    }
    else {
        if (ui_.cmbProxyType->currentIndex() == 1) {
            ui_.btnCheckProxy->setEnabled(true);
            ui_.btnCheckProxy->setProperty("state", "active");

            ui_.wgtProxyStatus->setProperty("state", "enabled");
            ui_.wgtProxyStatus->style()->polish(ui_.wgtProxyStatus);
            ui_.wgtProxyStatus->update();

            getflix_->setRetryLoadInterval(settings_.value(settings::kProxyRetryLoadInterval).toInt());
        }
        else {
            ui_.btnCheckProxy->setEnabled(false);
            ui_.btnCheckProxy->setProperty("state", "disabled");
            settings_.setValue(settings::kProxyRetryLoadInterval, 0);
        }
        ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
        ui_.btnCheckProxy->update();

        ui_.edAddress->setEnabled(false);
        ui_.edPort->setEnabled(false);
        ui_.edLogin->setEnabled(false);
        ui_.edPassword->setEnabled(false);
    }

    //set cmbProxyType view
    ui_.cmbProxyType->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    QListView * listView = new QListView(ui_.cmbProxyType);
    listView->setCursor(Qt::PointingHandCursor);
    listView->setStyleSheet("QListView::item {\
                                padding-left: 3px;\
                                border: none;\
                            }\
                            QListView::item:selected {\
                                background: #4E5164;\
                            }\
                            QListView::item:hover {\
                                background: #4E5164;\
                            }\
                        ");

    ui_.cmbProxyType->setView(listView);
    ui_.cmbProxyType->setStyleSheet("QComboBox {\
                                        border: none;\
                                        padding: 3 6 6 7;\
                                        text-align: left;\
                                        background-color: #292C35;\
                                    }\
                                    QComboBox:on {\
                                        border-right: none;\
                                        border-top: 2px solid #292C35;\
                                        border-left: none;\
                                        border-right: none;\
                                        border-bottom: 2px solid #3A3D46;\
                                    }\
                                    QComboBox::drop-down {\
                                        border: none;\
                                    }\
                                    QComboBox::down-arrow {\
                                        subcontrol-origin: padding;\
                                        subcontrol-position: right;\
                                        padding-right:7px;\
                                        border-style: solid;\
                                        image: url(:/img/arrow_down.png);\
                                    }\
                                    QComboBox::down-arrow:on {\
                                        image: url(:/img/arrow_up.png);\
                                    }\
                                    QComboBox QAbstractItemView {\
                                        border: none;\
                                        outline: 0px;\
                                        color: rgba(255, 255, 255, 250);\
                                        background-color: #292C35;\
                                    }\
                                    QComboBox QAbstractItemView::item {\
                                        min-height: 30px;\
                                    }\
                                ");

    QObject::connect(ui_.cmbProxyType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int id) {
        ui_.edAddress->setEnabled(id > 1);
        ui_.edPort->setEnabled(id > 1);
        ui_.edLogin->setEnabled(id > 1);
        ui_.edPassword->setEnabled(id > 1);

        if (id > 1) {
            ui_.edAddress->setFocus();

            ui_.btnCheckProxy->setEnabled(true);

            if ((!ui_.edAddress->text().isEmpty()
                && !ui_.edPort->text().isEmpty()
                && ui_.edLogin->text().isEmpty()
                && ui_.edPassword->text().isEmpty())
                ||
                (!ui_.edAddress->text().isEmpty()
                && !ui_.edPort->text().isEmpty()
                && !ui_.edLogin->text().isEmpty()
                && !ui_.edPassword->text().isEmpty())) {

                ui_.btnCheckProxy->setProperty("state", "active");
            }
            else {
                ui_.btnCheckProxy->setProperty("state", "enabled");
            }
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();

            ui_.wgtLblCred->setProperty("state", "enabled");
            ui_.wgtLblCred->style()->polish(ui_.wgtLblCred);
            ui_.wgtLblCred->update();

            ui_.wgtLblAuth->setProperty("state", "enabled");
            ui_.wgtLblAuth->style()->polish(ui_.wgtLblAuth);
            ui_.wgtLblAuth->update();

            ui_.wgtEnableProxy->setProperty("state", "enabled");
            ui_.wgtEnableProxy->style()->polish(ui_.wgtEnableProxy);
            ui_.wgtEnableProxy->update();

            ui_.wgtEnableAuth->setProperty("state", "enabled");
            ui_.wgtEnableAuth->style()->polish(ui_.wgtEnableAuth);
            ui_.wgtEnableAuth->update();
            
            ui_.wgtProxyStatus->setProperty("state", "enabled");
            ui_.wgtProxyStatus->style()->polish(ui_.wgtProxyStatus);
            ui_.wgtProxyStatus->update();
        }
        else {
            if (id == 1) {
                ui_.edAddress->setText("");
                ui_.edPort->setText("");
                ui_.edLogin->setText("");
                ui_.edPassword->setText("");

                ui_.btnCheckProxy->setEnabled(true);
                ui_.btnCheckProxy->setProperty("state", "active");
            }
            else {
                ui_.btnCheckProxy->setEnabled(false);
                ui_.btnCheckProxy->setProperty("state", "disabled");
                settings_.setValue(settings::kProxyRetryLoadInterval, 0);
                getflix_->setRetryLoadInterval(settings_.value(settings::kProxyRetryLoadInterval).toInt());
            }
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();

            ui_.wgtLblCred->setProperty("state", "disabled");
            ui_.wgtLblCred->style()->polish(ui_.wgtLblCred);
            ui_.wgtLblCred->update();

            ui_.wgtLblAuth->setProperty("state", "disabled");
            ui_.wgtLblAuth->style()->polish(ui_.wgtLblAuth);
            ui_.wgtLblAuth->update();

            ui_.wgtEnableProxy->setProperty("state", "disabled");
            ui_.wgtEnableProxy->style()->polish(ui_.wgtEnableProxy);
            ui_.wgtEnableProxy->update();

            ui_.wgtEnableAuth->setProperty("state", "disabled");
            ui_.wgtEnableAuth->style()->polish(ui_.wgtEnableAuth);
            ui_.wgtEnableAuth->update();

            if (id == 1)
                ui_.wgtProxyStatus->setProperty("state", "enabled");
            else
                ui_.wgtProxyStatus->setProperty("state", "disabled");

            ui_.wgtProxyStatus->style()->polish(ui_.wgtProxyStatus);
            ui_.wgtProxyStatus->update();

            ui_.lblStatus->setText(tr("Unknown"));
            ui_.lblStatus->setProperty("state", "unknown");
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();
        }

        settings_.setValue(settings::kProxyType, id);
    });

    QObject::connect(ui_.btnCheckProxy, &QPushButton::clicked, [=]() {
        if (ui_.edAddress->isEnabled()
            && ui_.edAddress->text().isEmpty()) {

            ui_.lblStatus->setText(tr("Add address"));
            ui_.lblStatus->setProperty("state", "warning");
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();

            ui_.edAddress->setFocus();
        }
        else if (ui_.edPort->isEnabled()
            && ui_.edPort->text().isEmpty()) {

            ui_.lblStatus->setText(tr("Add port"));
            ui_.lblStatus->setProperty("state", "warning");
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();

            ui_.edPort->setFocus();
        }
        else if (ui_.edLogin->isEnabled()
            && !ui_.edLogin->text().isEmpty()
            && ui_.edPassword->text().isEmpty()) {

            ui_.lblStatus->setText(tr("Add password"));
            ui_.lblStatus->setProperty("state", "warning");
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();

            ui_.edPassword->setFocus();
        }
        else if (ui_.edLogin->isEnabled()
            && !ui_.edPassword->text().isEmpty()
            && ui_.edLogin->text().isEmpty()) {

            ui_.lblStatus->setText(tr("Add login"));
            ui_.lblStatus->setProperty("state", "warning");
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();

            ui_.edLogin->setFocus();
        }
        else {
            ui_.cmbProxyType->setEnabled(false);
            ui_.btnCheckProxy->setEnabled(false);
            ui_.btnCheckProxy->setProperty("state", "disabled");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
            ui_.edAddress->setEnabled(false);
            ui_.edPort->setEnabled(false);
            ui_.edLogin->setEnabled(false);
            ui_.edPassword->setEnabled(false);

            ui_.lblStatus->setText(tr("Connecting..."));
            ui_.lblStatus->setProperty("state", "unknown");
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();

            int proxy_retry_load_interval = 0;
            bool check = checkProxy(proxy_retry_load_interval);
            if (check) {
                QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::NoProxy, ui_.edAddress->text(), ui_.edPort->text().toInt(), ui_.edLogin->text(), ui_.edPassword->text()));
                settings_.setValue(settings::kProxyAddress, ui_.edAddress->text());
                settings_.setValue(settings::kProxyPort, ui_.edPort->text());
                settings_.setValue(settings::kProxyCred, QVariant::fromValue(qMakePair(ui_.edLogin->text(), ui_.edPassword->text())));
                settings_.setValue(settings::kProxyRetryLoadInterval, proxy_retry_load_interval);

                ui_.lblStatus->setText(tr("Connected"));
                ui_.lblStatus->setProperty("state", "connected");
            }
            else {
                ui_.lblStatus->setText(tr("No connection"));
                ui_.lblStatus->setProperty("state", "noconnection");
                settings_.setValue(settings::kProxyRetryLoadInterval, 0);
            }
            getflix_->setRetryLoadInterval(settings_.value(settings::kProxyRetryLoadInterval).toInt());
            ui_.lblStatus->style()->polish(ui_.lblStatus);
            ui_.lblStatus->update();

            ui_.cmbProxyType->setEnabled(true);
            ui_.btnCheckProxy->setEnabled(true);

            if (ui_.cmbProxyType->currentIndex() > 1) {
                ui_.edAddress->setEnabled(true);
                ui_.edPort->setEnabled(true);
                ui_.edLogin->setEnabled(true);
                ui_.edPassword->setEnabled(true);
            }
            else if (ui_.cmbProxyType->currentIndex() == 1) {
                ui_.btnCheckProxy->setProperty("state", "active");
                ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
                ui_.btnCheckProxy->update();
            }
        }
    });

    QObject::connect(ui_.edAddress, &QLineEdit::textChanged, [=]() {
        style()->polish(ui_.edAddress);
        ui_.lblStatus->setText(tr("Unknown"));
        ui_.lblStatus->setProperty("state", "unknown");
        ui_.lblStatus->style()->polish(ui_.lblStatus);
        ui_.lblStatus->update();
        
        if ((!ui_.edAddress->text().isEmpty()
            && !ui_.edPort->text().isEmpty()
            && ui_.edLogin->text().isEmpty()
            && ui_.edPassword->text().isEmpty()) 
            ||
            (!ui_.edAddress->text().isEmpty()
            && !ui_.edPort->text().isEmpty()
            && !ui_.edLogin->text().isEmpty()
            && !ui_.edPassword->text().isEmpty())) {

            ui_.btnCheckProxy->setProperty("state", "active");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
        else {
            ui_.btnCheckProxy->setProperty("state", "enabled");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
    });
    QObject::connect(ui_.edAddress, &QLineEdit::editingFinished, [=]() {
        ui_.edAddress->setText(ui_.edAddress->text().replace(" ", ""));

        if (!ui_.edAddress->text().isEmpty()) {
            if (ui_.edLogin->text().isEmpty()
                && ui_.edPassword->text().isEmpty()) {

                if (ui_.edPort->text().isEmpty())
                    ui_.edPort->setFocus();
            }
            else if (!ui_.edLogin->text().isEmpty())
                ui_.edPassword->setFocus();
            else if (!ui_.edPassword->text().isEmpty())
                ui_.edLogin->setFocus();
        }
        else {
            ui_.btnCheckProxy->setProperty("state", "enabled");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
    });

    QObject::connect(ui_.edPort, &QLineEdit::textChanged, [=]() {
        style()->polish(ui_.edPort);
        ui_.lblStatus->setText(tr("Unknown"));
        ui_.lblStatus->setProperty("state", "unknown");
        ui_.lblStatus->style()->polish(ui_.lblStatus);
        ui_.lblStatus->update();
        
        if ((!ui_.edPort->text().isEmpty()
            && !ui_.edAddress->text().isEmpty()
            && ui_.edLogin->text().isEmpty()
            && ui_.edPassword->text().isEmpty())
            ||
            (!ui_.edPort->text().isEmpty()
            && !ui_.edAddress->text().isEmpty()
            && !ui_.edLogin->text().isEmpty()
            && !ui_.edPassword->text().isEmpty())) {

            ui_.btnCheckProxy->setProperty("state", "active");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
        else {
            ui_.btnCheckProxy->setProperty("state", "enabled");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
    });
    QObject::connect(ui_.edPort, &QLineEdit::editingFinished, [=]() {
        ui_.edPort->setText(ui_.edPort->text().replace(" ", ""));

        if (!ui_.edPort->text().isEmpty()) {
            if (ui_.edLogin->text().isEmpty()
                && ui_.edPassword->text().isEmpty()) {

                if (ui_.edAddress->text().isEmpty())
                    ui_.edAddress->setFocus();
            }
            else if (!ui_.edLogin->text().isEmpty())
                ui_.edPassword->setFocus();
            else if (!ui_.edPassword->text().isEmpty())
                ui_.edLogin->setFocus();
        }
    });

    QObject::connect(ui_.edLogin, &QLineEdit::textChanged, [=]() {
        style()->polish(ui_.edLogin);
        ui_.lblStatus->setText(tr("Unknown"));
        ui_.lblStatus->setProperty("state", "unknown");
        ui_.lblStatus->style()->polish(ui_.lblStatus);
        ui_.lblStatus->update();

        if (!ui_.edAddress->text().isEmpty()
            && !ui_.edPort->text().isEmpty()) {

            if ((!ui_.edLogin->text().isEmpty()
                && !ui_.edPassword->text().isEmpty())
                ||
                (ui_.edLogin->text().isEmpty()
                && ui_.edPassword->text().isEmpty())) {

                ui_.btnCheckProxy->setProperty("state", "active");
                ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
                ui_.btnCheckProxy->update();
            }
            else {
                ui_.btnCheckProxy->setProperty("state", "enabled");
                ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
                ui_.btnCheckProxy->update();
            }
        }
        else {
            ui_.btnCheckProxy->setProperty("state", "enabled");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
    });
    QObject::connect(ui_.edLogin, &QLineEdit::editingFinished, [=]() {
        ui_.edLogin->setText(ui_.edLogin->text().replace(" ", ""));

        if (!ui_.edLogin->text().isEmpty()) {
            if (ui_.edAddress->text().isEmpty())
                ui_.edAddress->setFocus();
            else if (ui_.edPort->text().isEmpty())
                ui_.edPort->setFocus();
            else if (ui_.edPassword->text().isEmpty())
                ui_.edPassword->setFocus();
        }
    });

    QObject::connect(ui_.edPassword, &QLineEdit::textChanged, [=]() {
        style()->polish(ui_.edPassword);
        ui_.lblStatus->setText(tr("Unknown"));
        ui_.lblStatus->setProperty("state", "unknown");
        ui_.lblStatus->style()->polish(ui_.lblStatus);
        ui_.lblStatus->update();

        if (!ui_.edAddress->text().isEmpty()
            && !ui_.edPort->text().isEmpty()) {

            if ((!ui_.edPassword->text().isEmpty()
                && !ui_.edLogin->text().isEmpty())
                ||
                (ui_.edPassword->text().isEmpty()
                && ui_.edLogin->text().isEmpty())) {

                ui_.btnCheckProxy->setProperty("state", "active");
                ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
                ui_.btnCheckProxy->update();
            }
            else {
                ui_.btnCheckProxy->setProperty("state", "enabled");
                ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
                ui_.btnCheckProxy->update();
            }
        }
        else {
            ui_.btnCheckProxy->setProperty("state", "enabled");
            ui_.btnCheckProxy->style()->polish(ui_.btnCheckProxy);
            ui_.btnCheckProxy->update();
        }
    });
    QObject::connect(ui_.edPassword, &QLineEdit::editingFinished, [=]() {
        ui_.edPassword->setText(ui_.edPassword->text().replace(" ", ""));

        if (!ui_.edPassword->text().isEmpty()) {
            if (ui_.edAddress->text().isEmpty())
                ui_.edAddress->setFocus();
            else if (ui_.edPort->text().isEmpty())
                ui_.edPort->setFocus();
            else if (ui_.edLogin->text().isEmpty())
                ui_.edLogin->setFocus();
        }
    });

    ui_.btnProxy->setVisible(true);
}

void MainWindow::createGetKey() {
    QObject::connect(ui_.btnFreeKey, &QPushButton::clicked, [=]() {
        if (ui_.wgtFreeKey->isVisible()) {
            ui_.wgtFreeKey->setVisible(0);
            ui_.btnFreeKey->setProperty("list", "closed");
        }
        else {
            ui_.wgtFreeKey->setVisible(1);
            ui_.wgtFreeKey->setProperty("list", "opened");
            ui_.wgtFreeKey->style()->polish(ui_.wgtFreeKey);
            ui_.wgtFreeKey->update();

            ui_.btnFreeKey->setProperty("list", "opened");
        }
        ui_.btnFreeKey->style()->polish(ui_.btnFreeKey);
        ui_.btnFreeKey->update();
    });

    QObject::connect(ui_.btnGetFreeKeyUser, &QPushButton::clicked, []() {
        QString company_url = COMPANY_URL;
        QDesktopServices::openUrl(QUrl(company_url + "/freekey/"));
    });

    QObject::connect(ui_.btnGetFreeKeyBusiness, &QPushButton::clicked, []() {
        QString company_url = COMPANY_URL;
        QDesktopServices::openUrl(QUrl(company_url + "/business/"));
    });

    ui_.wgtFreeKey->setVisible(0);
    ui_.btnFreeKey->setVisible(true);
}


void MainWindow::createHelp() {
    QObject::connect(ui_.btnHelp, &QPushButton::clicked, [=]() {
        if (ui_.wgtHelp->isVisible()) {
            ui_.wgtHelp->setVisible(0);
            ui_.btnHelp->setProperty("list", "closed");
        }
        else {
            ui_.wgtHelp->setVisible(1);
            ui_.wgtHelp->setProperty("list", "opened");
            ui_.wgtHelp->style()->polish(ui_.wgtHelp);
            ui_.wgtHelp->update();

            ui_.btnHelp->setProperty("list", "opened");
        }
        ui_.btnHelp->style()->polish(ui_.btnHelp);
        ui_.btnHelp->update();
    });

    QObject::connect(ui_.btnFAQ, &QPushButton::clicked, []() {
        QString faq_url = FAQ_URL;
        QDesktopServices::openUrl(QUrl(faq_url));
    });

    QObject::connect(ui_.btnVideoGuides, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(portalVideoGuideUrl(PRODUCT_NAME));
    });

    QObject::connect(ui_.btnBlog, &QPushButton::clicked, []() {
        QString company_domain = COMPANY_DOMAIN;
        QDesktopServices::openUrl(QUrl("https://" + company_domain + "/blog/"));
    });

    QObject::connect(ui_.btnBugReport, &QPushButton::clicked, []() {
        QString bugreport_url = BUGREPORT_URL;
        QDesktopServices::openUrl(QUrl(bugreport_url));
    });

    ui_.wgtHelp->setVisible(0);
    ui_.btnHelp->setVisible(true);
}

void MainWindow::createOtherApps() {
    QObject::connect(ui_.btnOtherApps, &QPushButton::clicked, [=]() {
        if (ui_.wgtOtherApps->isVisible()) {
            ui_.wgtOtherApps->setVisible(0);
            ui_.btnOtherApps->setProperty("list", "closed");
        }
        else {
            ui_.wgtOtherApps->setVisible(1);
            ui_.wgtOtherApps->setProperty("list", "opened");
            ui_.wgtOtherApps->style()->polish(ui_.wgtOtherApps);
            ui_.wgtOtherApps->update();

            ui_.btnOtherApps->setProperty("list", "opened");
        }
        ui_.btnOtherApps->style()->polish(ui_.btnOtherApps);
        ui_.btnOtherApps->update();
    });

    QSettings       settings(COMPANY_SETTINGS_PATH, QSettings::NativeFormat);
    QString read_path = COMPANY_NAME;
#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
    read_path.append("Store");
#endif
    QVariantMap     products_data_ = settings.value(read_path).toMap();

#if PURCHASING_BACKEND == VENDING_BACKEND
    if (products_data_.isEmpty()) {
        QString company_url = COMPANY_URL;

        products_data_.insert("FlixGrab", company_url + "/product/flixgrab/");
        products_data_.insert("FlixGrabMusic", company_url + "/product/flixgrab-music/");
        products_data_.insert("FreeAppleMusicDownload", company_url + "/product/free-apple-music-download/");
        products_data_.insert("FreeAmazonMusicDownload", company_url + "/product/free-amazon-music-download/");
        products_data_.insert("FreeAmazonPrimeDownload", company_url + "/product/free-amazon-prime-download/");
        products_data_.insert("FreeDailymotionDownload", company_url + "/product/free-dailymotion-download/");
        products_data_.insert("FreeDisneyPlusDownload", company_url + "/product/free-disney-plus-download/");
        products_data_.insert("FreeFacebookVideoDownload", company_url + "/product/free-facebook-video-download/");
        products_data_.insert("FreeHBODownload", company_url + "/product/free-hbo-download/");
        products_data_.insert("FreeHuluDownload", company_url + "/product/free-hulu-download/");
        products_data_.insert("FreeInstagramDownload", company_url + "/product/free-instagram-download/");
        products_data_.insert("FreeNetflixDownload", company_url + "/product/free-netflix-download/");
        products_data_.insert("FreePornhubDownload", company_url + "/product/free-pornhub-download/");
        products_data_.insert("FreeSpotifyDownload", company_url + "/product/free-spotify-download/");
        products_data_.insert("FreeTwitchDownload", company_url + "/product/free-twitch-download/");
        products_data_.insert("FreeTwitterDownload", company_url + "/product/free-twitter-download/");
        products_data_.insert("FreeVimeoDownload", company_url + "/product/free-vimeo-download/");
        products_data_.insert("FreeXVideosDownload", company_url + "/product/free-xvideos-download/");
        products_data_.insert("FreeYouTubeDownload", company_url + "/product/free-youtube-download/");
        products_data_.insert("FreeYouTubeToMP3Converter", company_url + "/product/free-youtube-to-mp3-converter/");
    }
#endif
    bool otherAppsContent = false;

    for (QVariantMap::const_iterator iter = products_data_.begin(); iter != products_data_.end(); ++iter) {
        QString prod_name = PRODUCT_NAME;
        if (iter.key() != prod_name
            && !iter.key().contains("Donation")
            && iter.key() != "NetflixDownloader"
            && iter.key() != "FlixGrabTest") {
            QString app_name = iter.key();

            if (app_name != "FlixGrab"
                && app_name != "FlixGrabMS"
                && app_name != "FlixGrabMusic") {
                QStringList parts;
                QRegularExpression regexp("((?:^|[A-Z])[^A-Z]*)");
                QRegularExpressionMatchIterator match = regexp.globalMatch(iter.key());
                while (match.hasNext()) {
                    QString part = match.next().captured(1);
                    if (!part.isEmpty())
                        parts << part;
                }

                if (!parts.isEmpty()) {
                    app_name = parts.join(" ");
                    app_name.replace("M P3", "MP3");
                    app_name.replace("H B O", "HBO");
                    app_name.replace("X Videos", "XVideos");
                    app_name.replace("You Tube", "YouTube");
                    //wordwrap
                    if (parts.size() > 2) {
                        QString last_part = app_name.section(" ", -1);
                        //MP3 Converter exception
                        if (last_part == "Converter")
                            app_name = app_name.split("To").first() + "To" + "\n" + app_name.split("To").last().trimmed();
                        else
                            app_name = app_name.split(" " + last_part).first() + "\n" + last_part;
                    }
                }
            }

            QPushButton* button = new QPushButton();
            button->setText(app_name);
            button->setIconSize(QSize(28, 24));
            QString icon_path = ":/apps/all_apps/" + iter.key() + ".png";
            if (!QFile::exists(icon_path))
                icon_path = ":/apps/all_apps/NewApp.png";
            button->setIcon(QIcon(icon_path));

            QUrl product_url = iter.value().toUrl();
            QObject::connect(button, &QPushButton::clicked, [=]() {
                QDesktopServices::openUrl(product_url);
            });

            ui_.wgtOtherApps->layout()->addWidget(button);
            otherAppsContent = true;
        }
    }

    ui_.wgtOtherApps->setVisible(0);
    ui_.btnOtherApps->setVisible(otherAppsContent);
}

void MainWindow::createSocialLinks() {
    ui_.btnFGA->setText(COMPANY_DOMAIN);

#if PURCHASING_BACKEND == WINDOWS_STORE_BACKEND
    ui_.btnFGA->setVisible(false);
#endif

    QObject::connect(ui_.btnFGA, &QPushButton::clicked, []() {
        QString company_url = COMPANY_URL;
        QDesktopServices::openUrl(QUrl(company_url));
    });
    
    QObject::connect(ui_.btnYouTube, &QPushButton::clicked, []() {
        QString company_url = COMPANY_URL;
        QDesktopServices::openUrl(QUrl(company_url + "/youtubechannel/"));
    });

    QObject::connect(ui_.btnFacebook, &QPushButton::clicked, []() {
        QString company_name = COMPANY_NAME;
        QDesktopServices::openUrl(QUrl("https://www.facebook.com/" + company_name));
    });

    QObject::connect(ui_.btnTwitter, &QPushButton::clicked, []() {
        QString company_name = COMPANY_NAME;
        QDesktopServices::openUrl(QUrl("https://twitter.com/" + company_name));
    });

    QObject::connect(ui_.btnReddit, &QPushButton::clicked, []() {
        QString company_name = COMPANY_NAME;
        QDesktopServices::openUrl(QUrl("https://www.reddit.com/r/" + company_name));
    });
    
    ui_.wgtSocial->setVisible(true);
}

void MainWindow::createGetflixHandlers() {
    Q_ASSERT(getflix_);

    QObject::connect(getflix_, &Getflix::loadPlaylistItem, [=](const QString& item) {
        controller_->loadPlayList(item);
    });

    QObject::connect(getflix_, &Getflix::finisheditems, [=]() {
        bool enable_sound = false;
        bool enable_shutdown = false;
        QList<int> selected_items = settings_.value(settings::kPostDownloadAction).value<QList<int>>();
        for (auto it : selected_items) {
            if (it == kPlaySound)
                enable_sound = true;
            if (it == kShutdownPC)
                enable_shutdown = true;
        }
        if (enable_sound)
            playSound(QUrl::fromLocalFile("finish.wav"));
        if (enable_shutdown) {
            AlertDialog dialog(EAlert::kAlert_ShutdownPC);
            
            QObject::connect(&dialog, &AlertDialog::countdownEnded, [=](const EAlert& type) {
                if (type == EAlert::kAlert_ShutdownPC) //Shutdown computer;
                    shutdownPC();
            });

            dialog.exec();
        }
    });

    QObject::connect(getflix_, &Getflix::saveSettings, [=](const QString& data) {
        QString key = data.split(":").first();
        QString value = data.split(":").last();
        settings_.setValue(key, value);
    });

    QObject::connect(getflix_, &Getflix::loadSettings, [=](const QString& key) {
        QString data = key + ":" + settings_.value(key).toString();
        getflix_->setSetting(data);
    });

    QObject::connect(getflix_, &Getflix::loadAudioLanguages, [=]() {
        QList<QString> selected_languages = settings_.value(settings::kAudioLanguage).value<QList<QString>>();
        getflix_->setAudioLanguages(selected_languages);
    });
}

void MainWindow::createProductLimitations() {
    Q_ASSERT(getflix_);

    if (ProductController::instance()->isActivated())
        getflix_->setActivated(ProductController::instance()->isActivated());
#if PURCHASING_BACKEND == VENDING_BACKEND
    else
        getflix_->setActivated(ProductController::instance()->isFirstTry());
#endif

    QObject::connect(getflix_, &Getflix::aboutToParsingPlaylist, [=](const EPortal& portal) {
        if (!ProductController::instance()->isActivated())
            AlertDialog::execDialog(EAlert::kAlert_LimitPlaylist, portal);
    });

    QObject::connect(getflix_, &Getflix::finished, [=](VideoHandle* video) {
#if PURCHASING_BACKEND == VENDING_BACKEND
        if (ProductController::instance()->isFirstTry()) {
            ProductController::instance()->firstTryEnded();
            getflix_->setActivated(false);
            AlertDialog::execDialog(EAlert::kAlert_LimitVideo, video->portal());
        }
#endif
    });

    QObject::connect(getflix_, &Getflix::aboutToDownload, [=](VideoHandle* video) {
        video->setEncodingSelector(settings_.value(settings::kEncodingSpeed).toInt());

        if (loadingSlowdownEnabled())
            video->setSlowdownFactor(ProductController::instance()->isActivated());

        if (contentLengthLimitEnabled()) {
            video->setDownloadPermission(true);

            if (!ProductController::instance()->isActivated()) {
                ProductController::instance()->setActivationDialogClicked(false);

                EAlert alert_type = EAlert::kAlert_VideoLimitDuration;

                if (!video->saveAsIds().isEmpty()) {
                    if (video->saveAsIds()[0] != kPreset_OriginalVideo)
                        alert_type = EAlert::kAlert_AudioLimitDuration;
                }
                else if (DEFAULTPRESET == kPreset_MP3Original) {
                    alert_type = EAlert::kAlert_AudioLimitDuration;
                }

                if (video->portal() == EPortal::kInstagram)
                    alert_type = EAlert::kAlert_LimitVideo;

                if (AlertDialog::execDialog(alert_type, video->portal()) == QDialog::Accepted
                    || ProductController::instance()->activationDialogClicked()) {
                    video->setDownloadPermission(false);
                }
            }

            video->setLimitDuration(ProductController::instance()->isActivated());
        }
        else if (video->isPending()) {
            if (!ProductController::instance()->isActivated())
                AlertDialog::execDialog(EAlert::kAlert_LimitThreads);
        }
        else {
            if (!ProductController::instance()->isActivated()
#if PURCHASING_BACKEND == VENDING_BACKEND
                && !ProductController::instance()->isFirstTry()
#endif
                ) {

                if (!video->saveAsIds().isEmpty()) {

                    bool alert_dialog_showed = false;

                    if (!alert_dialog_showed
                        && !downloadVideoEnabled(video->portal(), ProductController::instance()->isActivated()) //limit video
                        && !video->tracksInfo().video_tracks.isEmpty()
                        && video->saveAsIds()[0] == kPreset_OriginalVideo) {

                        AlertDialog::execDialog(EAlert::kAlert_LimitVideo, video->portal());
                        alert_dialog_showed = true;
                    }
                }
            }
        }
    });
}

void MainWindow::createUpdater() {
    QObject::connect(&tray_icon_, &QSystemTrayIcon::messageClicked, [=]() {
        if (tray_reason_ == kTrayActivation_Update)
            ProductController::instance()->installUpdate();
    });

    QObject::connect(ProductController::instance(), &ProductController::productDataNetError, [=]() {
    if (QDialog::Accepted != AlertDialog::execDialog(EAlert::kAlert_GetProductsData))
        QMetaObject::invokeMethod(this, "quitDeferred", Qt::QueuedConnection);
    });

    QObject::connect(ProductController::instance(), &ProductController::productDataError, [=]() {
        AlertDialog::execDialog(EAlert::kAlert_DownloadUpdate);
        QMetaObject::invokeMethod(this, "quitDeferred", Qt::QueuedConnection);
    });

    QObject::connect(ProductController::instance(), &ProductController::updateError, [=]() {
        AlertDialog::execDialog(EAlert::kAlert_DownloadUpdate);
        QMetaObject::invokeMethod(this, "quitDeferred", Qt::QueuedConnection);
    });
}

bool MainWindow::checkMandatoryUpdate() {
    if (ProductController::instance()->isUpdateDownloaded()) {
        bool result = (AlertDialog::execDialog(EAlert::kAlert_MandatoryUpdate) == QDialog::Accepted);

        //Call QApp::quit in the main loop;
        QMetaObject::invokeMethod(this, "quitDeferred", Qt::QueuedConnection);
        return result;
    }
    return false;
}

void MainWindow::quitDeferred() {
    QApplication::quit();
}

void MainWindow::createSystemTray() {

    tray_icon_.setIcon(QIcon(":/img/system-tray-logo.ico"));
    tray_icon_.show();
    connect(&tray_icon_, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason ) {

        qDebug() << "System tray activated";
     /*   this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        this->show();
        this->activateWindow();
        this->raise();*/

    });
    connect(&tray_icon_, &QSystemTrayIcon::messageClicked, [=]() {
        qDebug() << "System tray message clicked";
        if (QApplication::activeModalWidget() == 0) {
                                    //  pasteAndAnalyze();
            //controller_->pasteFromClipboard();
            this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            this->show();
            this->activateWindow();
            this->raise();
        }
    });

    QObject::connect(getflix_, &Getflix::error, [=]() {
        tray_reason_ = kTrayActivation_Error;
        qDebug() << "Tray activation: " << getflix_->status().message();

        if (getflix_->status().code() == EStatusCode::RegistrationFailed)
            getflix_->deleteCookies();

        QString subcode = QString::number(getflix_->status().subcode());
        QString status_desc = ProductController::instance()->statusDesc(getflix_->status().code(), subcode);
        QString status_message = getflix_->status().message();
        QString message = QString("%1\n\"%2\"").arg(status_desc).arg(status_message);
        if (status_message.isEmpty())
            message = QString("%1").arg(status_desc);

        tray_icon_.showMessage(
            QApplication::applicationName(),
            message,
            QSystemTrayIcon::Warning, 3000);
    });
}

bool MainWindow::checkDownloadThreadsTrack(const int data) {
    bool result = false;

    if (!ProductController::instance()->isActivated())
        if (data != 1)
            result = true;

    return result;
}

bool MainWindow::checkEncodingSpeedTrack(const int data) {
    bool result = false;

    if (checkDefaultSettingsEnabled()) {
        if (!ProductController::instance()->isActivated())
            if (data != kEncodingSelector_Ultrafast)
                result = true;
    }

    return result;
}

bool MainWindow::checkVideoStreamTrack(const int data) {
    bool result = false;
    
    if (checkDefaultSettingsEnabled()) {
        if (!ProductController::instance()->isActivated())
            if (data != kVideoSelector_MinimumSize)
                result = true;
    }

    return result;
}

bool MainWindow::checkAudioStreamTrack(const int data) {
    bool result = false;

    if (checkDefaultSettingsEnabled()) {
        if (!ProductController::instance()->isActivated())
            if (data != kAudioSelector_MinimumSize)
                result = true;
    }

    return result;
}

bool MainWindow::checkImageStreamTrack(const int data) {
    bool result = false;

    if (!ProductController::instance()->isActivated())
        if (data != kImageSelector_MinimumSize)
            result = true;

    return result;
}

bool MainWindow::checkSaveAsTrack(const int data) {
    bool result = false;

    if (checkDefaultSettingsEnabled()) {
        if (!ProductController::instance()->isActivated()) {
            if (forcedConversionToLowQualityEnabled()) {
                if (data != kPreset_MP3LowQuality)
                    result = true;
            }
            else {
                if (data != DEFAULTPRESET)
                    result = true;
            }
        }
    }

    return result;
}

bool MainWindow::checkAudioTypeTrack(const int data) {
    bool result = false;

    if (checkDefaultSettingsEnabled()) {
        if (!ProductController::instance()->isActivated())
            if (data != kAudioType_Original)
                result = true;
    }

    return result;
}

bool MainWindow::checkAudioLanguageTrack(const QAction* action, const QList<QAction*>& subactions) {
    bool result = false;

    if (checkDefaultSettingsEnabled()) {
        if (!ProductController::instance()->isActivated()) {
            for (auto it : subactions) {
                if (it->isChecked()
                    && it->text() != action->text()
                    && it->data().toString() != kLanguagesAndCodes[0][1]) {

                    result = true;
                    break;
                }
            }
        }
    }

    return result;
}

bool MainWindow::checkSubtitleLanguageTrack(const QString& data) {
    bool result = false;

    if (!ProductController::instance()->isActivated())
        if (data != settings::kSubtitleLanguageDefault)
            result = true;

    return result;
}

bool MainWindow::checkSaveSubtitlesAsTrack(const int data) {
    bool result = false;

    if (!ProductController::instance()->isActivated())
        if (data != kSaveSubtitlesAs_Original)
            result = true;

    return result;
}

QNetworkProxy::ProxyType MainWindow::getProxyTypeByIndex(int index) {
    QNetworkProxy::ProxyType res = QNetworkProxy::NoProxy;

    switch (index) {
    case 1:
        res = QNetworkProxy::HttpProxy;
        break;
    case 2:
        res = QNetworkProxy::Socks5Proxy;
        break;
    }

    return res;
}

void MainWindow::deleteAccounts() {
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    if (dir.exists())
        dir.removeRecursively(); //if web_page_ exists it doesnt work
}

void MainWindow::cleanupProgramData() {
    QString configDataDir = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).at(1);
    QDir dir(configDataDir);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
        dir.remove(dirFile);

    QDirIterator it(configDataDir, QDir::Dirs | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        QString path = it.next();
        if (!path.contains(TESSERACTFOLDER)) {
            QDir dir(path);
            dir.removeRecursively();
        }
    }
}

//void MainWindow::systemTrayIconActivated(QSystemTrayIcon::ActivationReason) {
//    if (!this->isHidden()) {
//        this->hide();
//    }
//    else {
//        this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
//        this->show();
//        this->activateWindow();
//        this->raise();
//    }
//}
//
//void MainWindow::systemTrayMessageClicked() {
//    if (QApplication::activeModalWidget() == 0) {
//        //  pasteAndAnalyze();
//        controller_->pasteFromClipboard();
//        this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
//        this->show();
//        this->activateWindow();
//        this->raise();
//    }
//}

