//sys_util.h - mac

#ifndef MAC__SYS_UTIL_H
#define MAC__SYS_UTIL_H

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "../../str/tstring.h"
#include "../../str/str_manip.h"

#include "platform_helpers.h"

namespace sys
{

inline int openProcess(const std::string& sCmd, std::string& sError)
{
    sError = "";
    std::string str = sCmd;
    str += " 2>&1";
    FILE* pipe = popen(str.c_str(), "r");
    if (!pipe)
    {
        sError = "ERROR! Create pipe failed!";
        return -1;
    }

    char buffer[128];
    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            sError += buffer;
    }

    int nRes = pclose(pipe);
    if (nRes != 0)
    {
        sError += "\n exit code = ";
        sError += str::intToString(nRes);
        return nRes;
    }
    return 0;
}

inline int exec(const tstring& cmd, const std::vector<tstring> & argv, bool bShow = true)
{
    tstring str = cmd;
    for(int i = 0; i < argv.size(); i++)
    {
        if (str.size() > 0)
            str += " ";
        str += argv[i];
    }
    //str += " &";
    //int nRes = std::system(str.c_str());
    FILE* pFile = popen(str.c_str(), "r");
    int nRes = 0;
    if (pFile == NULL)
        nRes = errno;
    else
        nRes = 0;
    return nRes;
}

inline int getPlatformSerialNumber(char *buf, int bufSize, std::string& sBufErr)
{
    try
    {
        std::string sCmd = "ioreg -l | grep IOPlatformSerialNumber | cut -c 37-46";
        std::string sBuf;
        if (sys::openProcess(sCmd, sBuf) == 0)
        {
            sBuf.erase(std::remove(sBuf.begin(), sBuf.end(), '\n'), sBuf.end());
            size_t nSize = sBuf.size();
            if (nSize > bufSize)
                nSize = bufSize;
            std::memcpy(buf, sBuf.c_str(), nSize);
        }
        else
        {
            sBufErr = sBuf;
            return -2;
        }
    }
    catch (...)
    {
        return -1;
    }
    return 0;
}


inline int registerApplicationForAutorun(const std::string & sAppName, bool bAutorun, bool bMinimized)
{
    return 0;
}

}

#endif //MAC__SYS_UTIL_H
