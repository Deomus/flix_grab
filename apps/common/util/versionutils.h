#include <util/str/tstring.h>
#include <util/str/str_manip.h>
#include <util/str/str_parse.h>
#include <version.h>


inline std::string version(bool bHumanFriendly = false)
{
    static std::string s_ver;
    static std::string s_ShortVer;

    if (s_ver.empty())
    {
        static int n_ver[4] = { INTVER };
        s_ShortVer = QString("%1.%2.%3").arg(n_ver[0]).arg(n_ver[1]).arg(n_ver[2]).toStdString();
        s_ver = s_ShortVer + "." + std::to_string(n_ver[3]);
    }

    return bHumanFriendly ? s_ShortVer : s_ver;
}


inline std::string getCurrentVersion()
{
    std::string sCurVer = STRVER;
    str::replace(sCurVer, ",", ".");
    
    return sCurVer;
}


void Parse(int result[4], const std::string& input)
{
    std::istringstream parser(input);
    parser >> result[0];
    for (int idx = 1; idx < 4; idx++)
    {
        parser.get(); //Skip period
        parser >> result[idx];
    }
}


inline bool compareVersions(const std::string& sThisVer, const std::string& sSiteVer)
{
    //int parsedThisVer[4], parsedSiteVer[4];
    //Parse(parsedThisVer, sThisVer);
    //Parse(parsedSiteVer, sSiteVer);
    bool bRes = true;
    if (strcmp(sThisVer.c_str(), sSiteVer.c_str()) == 0)
        bRes = false;
    return bRes;

    //return std::lexicographical_compare(parsedThisVer, parsedThisVer + 4, parsedSiteVer, parsedSiteVer + 4);
}
