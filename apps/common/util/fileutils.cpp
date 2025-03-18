#include "fileutils.h"
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QProcess>



#if defined(Q_OS_WIN)
    #include <Windows.h>
    #include <ShellAPI.h>
    #include "win_utils.h"
	//#include <Shlobj.h>
#endif

//#define UI_MAX_PATH 1024
QString  FileUtilApi::GetMyVideoFolderPath(const QString & sInPathDef)
{
    QStringList qslRet = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
    if (qslRet.empty())
    {
        return sInPathDef;
    }
    return qslRet.at(0);
}
QString FileUtilApi::GetMyMusicFolderPath(const QString & sInPathDef)
{
    QStringList qslRet = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    if (qslRet.empty())
    {
        return sInPathDef;
    }
    return qslRet.at(0);
}
QString FileUtilApi::GetMyPictureFolderPath(const QString & sInPathDef)
{
    QStringList qslRet = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    if (qslRet.empty())
    {
        return sInPathDef;
    }
    return qslRet.at(0);
}
void     FileUtilApi::ShowFileInExplorer(const QString & sInPath)
{
	//QString sNativeSeparatorPath   = QDir::toNativeSeparators(sInPath);
	//ITEMIDLIST * pidl = ILCreateFromPathW(sNativeSeparatorPath.utf16());
 //   if(pidl) 
	//{
 //       SHOpenFolderAndSelectItems(pidl,0,0,0);
 //       ILFree(pidl);
 //   }    


    //if (sInPath.isEmpty())
    //	return;
    //QString param;
    //   if (!QFileInfo(sInPath).isDir())
    //       param = "/select,";
    //param +=  QDir::toNativeSeparators(sInPath);
    //std::wstring sPar = AString2WString(param.toStdString().c_str());
    //HINSTANCE hInstance = ::ShellExecuteW(NULL, L"open", L"explorer", sPar.c_str(), NULL, SW_SHOWNORMAL);

	// Mac, Windows support folder or file.
#if defined(Q_OS_WIN)
	
	//const QString explorer("explorer.exe ");
 //   QString param;
 //   if (!QFileInfo(sInPath).isDir())
 //      param = "/select, ";
 //   param = explorer + param + "\"" + sInPath + "\"";
	//QProcess::startDetached(param);

    if (sInPath.isEmpty())
    	return;
    QString param;
    if (!QFileInfo(sInPath).isDir())
           param = "/select, ";
    param += "\""+ QDir::toNativeSeparators(sInPath) + "\"";
    std::wstring sPar = AString2WString(param.toStdString().c_str());
    HINSTANCE hInstance = ::ShellExecuteW(NULL, L"open", L"explorer", sPar.c_str(), NULL, SW_SHOWNORMAL);

#elif defined(Q_OS_MAC)
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                                     .arg(sInPath);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute("/usr/bin/osascript", scriptArgs);
#else
    Q_UNUSED(sInPath)
#endif
}

bool checkAccessRigth(const std::wstring &sPathName)
{
    bool bRet = true;
    HANDLE hFile;
    hFile = CreateFileW(sPathName.c_str(),       // name of the write
                    GENERIC_WRITE,               // open for writing
                    0,                           // do not share
                    NULL,                        // default security
                    OPEN_EXISTING,               // create new file only
                    FILE_FLAG_BACKUP_SEMANTICS,  // normal file
                    NULL);                       // no attr. template
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DWORD nLastErr = GetLastError(); 
        if (nLastErr == ERROR_ACCESS_DENIED )
            bRet = false;
    }
    CloseHandle(hFile);
    return bRet;
}

