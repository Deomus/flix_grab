//version_util.h

#ifndef MAC_COMMON__VERSION_UTIL_H
#define MAC_COMMON__VERSION_UTIL_H

#include "str/tstring.h"
#include "fs/file_utils.h"

namespace sys
{
    inline std::string getVersionFromPlist(std::string sVerFilePath)
    {
        std::string sCmd = sVerFilePath;
        return "5.1.1.1";
    }

    inline int getFileVersion(tstring wsTargetPath, std::string & sVerValue)
    {
        sVerValue = "";
        LOG_DBG("sLocalPathToUpdateZip: " << wsTargetPath.c_str());
        tstring sAppName = fs::getFileNameWithoutExtension(fs::getFileNameWithoutExtension(fs::getFileName(wsTargetPath)));
        
        int nRes = 0;
        std::string sCmd = "";
        std::string sError = "";
        std::string sVersionFilepath = "";

        do  //extract version file
        {
            sCmd = "tar -xf \"";
            sCmd += wsTargetPath.c_str();
            std::vector<std::string> vParts;
            vParts.clear();
            str::tokenize <char>(wsTargetPath.c_str(), sAppName.c_str(), vParts);
            if (vParts.size() != 0)
                sVersionFilepath = vParts[0];

            sCmd += "\" -C " + sVersionFilepath;

            LOG_DBG("installNewVersion | openProcess cmd:" << sCmd.c_str());
            nRes = sys::openProcess(sCmd, sError);
            if (nRes != 0)
            {
                LOG_DBG("Failed to exec(): " << sCmd.c_str());
                LOG_ERR("exec error: " << sError.c_str());
                return nRes;
            }
        } while (false);

        //get version from file
        sVerValue = getVersionFromPlist(sVersionFilepath + digi::getHumanFriendlyAppName(digi::getAppId(sAppName.c_str())) + ".app/Contents/Info.plist");
        return nRes;
    }
}
#endif //MAC_COMMON__VERSION_UTIL_H