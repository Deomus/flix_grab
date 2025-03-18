// shell_dir.h

#ifndef __COMMON_SHELL_DIR_H__
#define __COMMON_SHELL_DIR_H__

#if defined(Q_OS_WIN)
    #include <Windows.h>
    #include <ShellAPI.h>
    #include "util/win_utils.h"
#endif

namespace shell {


inline void showFileInExplorer(const QString & sInPath)
{
    if (sInPath.isEmpty())
    	return;
    QString param;
    if (!QFileInfo(sInPath).isDir())
           param = "/select, ";
    param += "\""+ QDir::toNativeSeparators(sInPath) + "\"";
    std::wstring sPar = AString2WString(param.toStdString().c_str());
    HINSTANCE hInstance = ::ShellExecuteW(NULL, L"open", L"explorer", sPar.c_str(), NULL, SW_SHOWNORMAL);
}


}

#endif // __COMMON_SHELL_DIR_H__
