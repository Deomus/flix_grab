#include <VMProtectSDK.h>

#include <iostream>
#include <time.h>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <QTextCodec>
#include <QtCore>

#include <company_info.h>

#include "main_window.h"
#include "single_instance.h"
#include "product_controller.h"


std::ofstream file_logger;
std::wstringstream wbuffer; // for std::wcout
std::mutex      file_lock_;

//#include <TlHelp32.h>
//
//static DWORD killChildProcs(bool withSelf, bool logging = true) {
//    DWORD dwBytesWritten;
//    HANDLE hFile = nullptr;
//    if (logging) {
//        hFile = CreateFile(TEXT("d:\\tmp\\crashfg.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//        WriteFile(hFile, "KillChilds...\r\n", 14, &dwBytesWritten, NULL);
//    }
//
//    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//    PROCESSENTRY32 pe = { 0 };
//    pe.dwSize = sizeof(PROCESSENTRY32);
//    const int maxParents = 64;
//    int parents[maxParents] = { 0 };
//    int iParent = 0;
//    parents[0] = GetCurrentProcessId(); // kill all childs of this victim
//    const int maxKilled = 64;
//    int killed[maxKilled] = { 0 };
//    int iKilled = 0;
//    char buf[16] = { 0 }; // pid string
//
//    for (int t = 0; t < 3; t++) {
//        if (Process32First(h, &pe)) {
//            do {
//                auto pid = pe.th32ProcessID;
//                // check if parent of pe is victim
//                bool parentIsVictim = false;
//                for (auto i = 0; i < maxParents; i++) {
//                    if (parents[i] == pe.th32ParentProcessID && parents[i] != 0)
//                        parentIsVictim = true;
//                }
//
//                bool alreadyKilled = false;
//                if (parentIsVictim) {
//                    for (auto i = 0; i < maxKilled; i++) {
//                        if (killed[i] == pid)
//                            alreadyKilled = true;
//                    }
//                }
//
//                if (parentIsVictim && !alreadyKilled) {
//                    if (++iParent < maxParents)
//                        parents[iParent] = pid; // all childs will be victims in next iteration
//
//                    if (logging) {
//                        WriteFile(hFile, "PID:", 4, &dwBytesWritten, NULL);
//                        memset(buf, 0, 16);
//                        itoa(pid, buf, 10);
//                        WriteFile(hFile, buf, 10, &dwBytesWritten, NULL);
//                        WriteFile(hFile, "\r\n", 1, &dwBytesWritten, NULL);
//                    }
//                    try {
//                        const auto explorer = OpenProcess(PROCESS_TERMINATE, false, pid);
//                        try {
//                            TerminateProcess(explorer, 1);
//                        }
//                        catch (...) {}
//                        CloseHandle(explorer);
//                        if (++iKilled < maxKilled)
//                            killed[iKilled] = pid;
//                    }
//                    catch (...) {}
//                }
//            } while (Process32Next(h, &pe));
//        }
//    }
//
//    CloseHandle(h);
//    if (logging)
//        CloseHandle(hFile);
//
//    if (withSelf) {
//        const auto explorer = OpenProcess(PROCESS_TERMINATE, false, parents[0]);
//        TerminateProcess(explorer, 1);
//        CloseHandle(explorer);
//    }
//
//    return 0;
//}
//
//#include <csignal>
//
//void sigabrt_handler(int) {
//    signal(SIGABRT, SIG_DFL);
//
//    killChildProcs(true);
//
//    // Caught SIGABRT C++ signal
//    // Terminate program
//    exit(3);
//}

void trace(QByteArray msg, QtMsgType type = QtMsgType::QtInfoMsg) {
    SYSTEMTIME time = { 0 };
    ::GetLocalTime(&time);
    //long id = ::GetCurrentThreadId();

    file_logger << std::setfill('0') << std::setw(2) << time.wHour << ":"
        << std::setfill('0') << std::setw(2) << time.wMinute << ":"
        << std::setfill('0') << std::setw(2) << time.wSecond << "."
        << std::setfill('0') << std::setw(3) << time.wMilliseconds
        << "; " << msg.constData() << std::endl;
}

void messageOutputHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    auto localMsg = msg.toLocal8Bit();
    //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
    //fprintf(stdout, "%.2d:%.2d:%.2d.%.3d; %s\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, 
    //    localMsg.constData());
    try {
        // to avoid race condition:
        std::unique_lock<std::mutex> lock(file_lock_);

        //here we dump output from 'std::wcout'
        auto buf = wbuffer.str(); // make a copy
        if (buf.length() > 0) {
            //todo: do separate thread for this:
            wbuffer.str({}); // do empty
            wbuffer.clear();
            auto ss = QString::fromStdWString(buf).toLocal8Bit();
            trace(ss);
        }
        //now dump the qt message:
        trace(localMsg);
        file_logger.flush();
    }
    catch (...) {}
}

void InitLogger() {
    SYSTEMTIME now = { 0 };
    ::GetLocalTime(&now);

    // create folder for logfiles
    QString  update_dir = QString("%1/%2/logs/%3").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(COMPANY_NAME).arg(PRODUCT_NAME);
    
    QDir dir(update_dir);
    if (!dir.exists())
        dir.mkpath(update_dir);

    // openlogfile
    auto log_filepath = dir.filePath(QString("%1-%2-%3-%4.log").arg(PRODUCT_NAME).arg(now.wYear).arg(now.wMonth, 2, 10, QChar('0')).arg(now.wDay, 2, 10, QChar('0')));
    std::cout << "logfile: " << log_filepath.toLocal8Bit().constData() << std::endl;
    QFile file(log_filepath);
    file.remove();
    file_logger.open(log_filepath.toLocal8Bit(), std::ofstream::out | std::ofstream::app);
    file_logger << std::endl;

    // redirect qt tracer to logfile
    qInstallMessageHandler(messageOutputHandler);

    // redirect stdout to logfile
    std::streambuf* old1 = std::cout.rdbuf(file_logger.rdbuf());
    std::streambuf* old2 = std::clog.rdbuf(file_logger.rdbuf());
    std::streambuf* old3 = std::cerr.rdbuf(file_logger.rdbuf());
    std::wstreambuf* wold1 = std::wcout.rdbuf(wbuffer.rdbuf());
    std::wstreambuf* wold2 = std::wclog.rdbuf(wbuffer.rdbuf());
    std::wstreambuf* wold3 = std::wcerr.rdbuf(wbuffer.rdbuf());
}

void    addBuildTimestamp() {
    tm time = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    std::istringstream input(__DATE__);
    input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
    input >> std::get_time(&time, "%b %d %Y");
    QString version = QApplication::applicationVersion();

    version = QString("%1.%2%3").arg(version)
        .arg(time.tm_mon + 1).arg(time.tm_mday, 2, 10, QChar('0'));

    QApplication::setApplicationVersion(version);
}

void startWatchDog(const QString applicationDirPath) {
    QProcess* process = new QProcess(nullptr);
    QString file = '"' + applicationDirPath + QString("/watchdog.exe\" ") + QString::number(GetCurrentProcessId());
    const auto s = file.toStdString();
    process->start(file);
}

int runApp(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    //QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QApplication app(argc, argv);
    //startWatchDog(app.applicationDirPath());

    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setOrganizationDomain(COMPANY_DOMAIN);
    QCoreApplication::setApplicationName(PRODUCT_NAME);
    QCoreApplication::setApplicationVersion(PRODUCT_VERSION);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    if (LOG_ENABLED)
        InitLogger();

    SingleInstance  instance;
    QWidget* main_widget = nullptr;
    QObject::connect(&instance, &SingleInstance::newInstance, [&]() {
        if (main_widget) {
            QApplication::alert(main_widget);
            main_widget->raise();
            main_widget->showNormal();
        }
    });

    if (instance.tryNew(QCoreApplication::applicationName(), PRODUCT_NAME + QString("Instance"))) {

        QSplashScreen splash(QPixmap(":/img/flix_splash.png"), Qt::FramelessWindowHint);
        splash.show();
        //Load Net Dlls
        if (!QSslSocket::supportsSsl())
            qDebug() << "SSL libs not found!";

        ProductController::instance()->initializeUi(&splash);
        
        int ret;
        {
            MainWindow window;
            main_widget = &window;
            splash.finish(main_widget);
#if !DEBUG
            //signal(SIGABRT, sigabrt_handler);
#endif

            if (!window.checkMandatoryUpdate())
                window.show();

                ret = app.exec();
        }

#ifdef _DEBUG
        //qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "23654");
        //uncomment above, run app and open http ://localhost:23654 in Chrome and then you will get full featured JS debugger, profiler, console
#endif

        if (LOG_ENABLED) {
            // dump the all queued messages and close the log:
            qDebug() << "app exit.";
            file_logger.close();
        }

        return ret;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    runApp(argc, argv);

   // addBuildTimestamp();

#if !DEBUG
    //killChildProcs(true);
#endif

    return 0;
}
