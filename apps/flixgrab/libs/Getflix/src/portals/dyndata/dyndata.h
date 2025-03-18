#ifndef DYNDATA2_H
#define DYNDATA2_H

#include <string>
#include <map>
#include <iostream>
#include <bitset>
#include <vector>
//#include <fs/file_utils.h>
#include <common/str_parse.h>
#include <dyndata/lite_gamma.h>
#include <dyndata/databuf.h>


namespace yt_dyndata2
{
    //const tstring sDyndataFileName          = LL("dlibhash");
    //const std::string sDyndataSitePath      = "http://tools.dvdvideosoft.com/updates/dyndata2";
/*
1) 8 бит - (0-255) число timestamps
2) Повторяется столько раз сколько указано в 1) блоке
	2.1) 8 бит  - (0-255) длина сигнатуры
	2.2) 3 * 8 бит - (0-16777215) timestamp
	2.3) 8 бит - (0-255) число функций (ss 58|rs|ss 50|cs 1|rs|cs 1|rs|ss 11|cs 3)
	2.4) Повторяется столько раз сколько указано в 2.3) блоке
		2 бита означают: 
		00 - rs, следом ничего 
		01 - cs, следующие 8 бит - (0-255) число
		10 - ss, следующие 8 бит - (0-255) число
3) в конце остаток до целого заполняется нулями
		
минимум получается 8+8+3*8+8+2 = 50
*/


enum class SgFunctionType
{
    kRS = 0,
    kCS = 1,
    kSS = 2
};

struct SgFunction
{
    SgFunctionType m_type;
    char           m_number;
};

class SgInfo
{
public:
    unsigned short              m_nSgLength;
    unsigned int                m_nSgTimeStamp;
    std::vector<SgFunction>     m_vSgFunctions;

public:
    size_t getSize() // bits
    {
        size_t nRet = 0;
        for (size_t i = 0; i < m_vSgFunctions.size(); i++)
        {
            if (m_vSgFunctions[i].m_type != SgFunctionType::kRS)
                nRet += 8;
            nRet += 2;
        }
        nRet += 3 * 8; // m_nSgTimeStamp
        nRet += 8; // m_nSgLength
        return nRet;
    }
};

// #87-16777|ss 13|rs|cs 3|ss 2|rs|cs 3|ss 36#85-16806|cs 1|rs|ss 65|cs 3|rs#84-16812|rs|cs 3|ss 59
inline int tsEn(const std::string& sIn, sys::DataBuff& db)
{
    try
    {
        std::vector<std::string> vParts;
        str::tokenize<char>(sIn, "#", vParts);
        std::vector<SgInfo> vSgInfo;
        size_t totalBits = 8;
        for (size_t i = 0; i < vParts.size(); i++)
        {
            if (vParts[i].size() == 0)
                continue;

            SgInfo sgInfo;
            std::vector<std::string> vSgParts;
            str::tokenize<char>(vParts[i], "|", vSgParts);
            for (size_t j = 0; j < vSgParts.size(); j++)
            {
                if (j == 0)
                {
                    std::vector<std::string> vTmpSgParts;
                    str::tokenize<char>(vSgParts[0], "-", vTmpSgParts);

                    if (vTmpSgParts.size() != 2)
                        return -1;

                    std::string sSgLength = vTmpSgParts[0];
                    unsigned short nSgLength = str::stringToInt(sSgLength);
                    sgInfo.m_nSgLength = nSgLength;

                    std::string sSgTimeStamp = vTmpSgParts[1];
                    unsigned int nSgTimeStamp = str::stringToInt(sSgTimeStamp);
                    sgInfo.m_nSgTimeStamp = nSgTimeStamp;
                }
                else
                {
                    SgFunction sgFunction;
                    std::string sTmp = vSgParts[j].substr(0, 2);
                    sgFunction.m_type = SgFunctionType::kRS;
                    sgFunction.m_number = 0x0;
                    if (sTmp.compare("rs") == 0)
                        sgFunction.m_type = SgFunctionType::kRS;
                    else if (sTmp.compare("cs") == 0)
                        sgFunction.m_type = SgFunctionType::kCS;
                    else if (sTmp.compare("ss") == 0)
                        sgFunction.m_type = SgFunctionType::kSS;
                    else
                        return -2;

                    if (vSgParts[j].size() > 3)
                    {
                        std::string sNumberTmp = vSgParts[j].substr(3, vSgParts[j].size());
                        unsigned short nNumberTmp = str::stringToInt(sNumberTmp);
                        char* chTmp = (char*)&nNumberTmp;
                        sgFunction.m_number = *chTmp;
                    }

                    sgInfo.m_vSgFunctions.push_back(sgFunction);
                }
            }
            vSgInfo.push_back(sgInfo);
            totalBits += sgInfo.getSize();
        }

        size_t modTotalBits = totalBits % 8;
        size_t nTotalBites = totalBits / 8;
        if (modTotalBits != 0)
            nTotalBites += 1;

        std::bitset<8> sgCountTimeStampBits = vSgInfo.size();
        std::string sTotalBits = sgCountTimeStampBits.to_string();
        for (size_t i = 0; i < vSgInfo.size(); i++)
        {
            std::bitset<8> sgLengthBits;
            sgLengthBits = vSgInfo[i].m_nSgLength;
            sTotalBits += sgLengthBits.to_string();
            std::bitset<3 * 8> sgTimeStampBits;
            sgTimeStampBits = vSgInfo[i].m_nSgTimeStamp;
            sTotalBits += sgTimeStampBits.to_string();

            std::bitset<8> countSgFunctionBits = vSgInfo[i].m_vSgFunctions.size();
            sTotalBits += countSgFunctionBits.to_string();
            for (size_t j = 0; j < vSgInfo[i].m_vSgFunctions.size(); j++)
            {
                SgFunction sgFunction = vSgInfo[i].m_vSgFunctions[j];
                std::bitset<2> bs_type = (int)sgFunction.m_type;
                sTotalBits += bs_type.to_string();
                if (sgFunction.m_type != SgFunctionType::kRS)
                {
                    std::bitset<8> bs_number = sgFunction.m_number;
                    sTotalBits += bs_number.to_string();
                }
            }
        }
        if (modTotalBits > 0)
            for (size_t i = 0; i < 8 - modTotalBits; i++)
                sTotalBits += "0";

        for (size_t i = 0; i < sTotalBits.size(); i += 8)
        {
            std::string sTmp = sTotalBits.substr(i, 8);
            std::bitset<8> bs(sTmp);
            char ch = (int)bs.to_ulong();
            db.append(&ch, 1);
        }

        lite_gamma::ec(db.data(), db.size(), str::intToString(db.size()).c_str());
        //std::cout << "sTotalBits = " << sTotalBits.c_str() << std::endl;
        return 0;
    }
    catch (...)
    {
        return -1;
    }
}

inline std::string getNextSubstring(const std::string sIn, size_t& cur_index, const size_t length)
{
    std::string sRet = sIn.substr(cur_index, length);
    cur_index += length;
    return sRet;
}

inline std::string getStringFromSubstring(const std::string sIn, size_t& cur_index)
{
    std::string sTmp = getNextSubstring(sIn, cur_index, 8);
    std::bitset<8> sgTmpBits(sTmp);
    return sgTmpBits.to_string();
}

inline int tsDe(sys::DataBuff& db, std::string& sOut)
{
    std::string qwe = "";
    try
    {
        lite_gamma::dc(db.data(), db.size(), str::intToString(db.size()).c_str());

        std::string sTotalBits;
        char * pch = (char*)db.data();
        for (size_t i = 0; i < db.size(); i++)
        {
            std::bitset<8> bs(pch[i]);
            sTotalBits += bs.to_string();
        }

        size_t cur_index = 0;
        std::string sCountTimeStampBits = getNextSubstring(sTotalBits, cur_index, 8);
        std::bitset<8> sgCountTimeStampBits(sCountTimeStampBits);
        size_t nCountTimeStamp = sgCountTimeStampBits.to_ulong();
        for (size_t i = 0; i < nCountTimeStamp; i++)
        {
            sOut += "#";

            std::string sLength = getNextSubstring(sTotalBits, cur_index, 8);
            std::bitset<8> sgLengthBits(sLength);
            size_t nSgLength = sgLengthBits.to_ulong();
            sOut += str::intToString(nSgLength);
            sOut += "-";

            std::string sTimeStamp = getNextSubstring(sTotalBits, cur_index, 3 * 8);
            std::bitset<3 * 8> sgTimeStampBits(sTimeStamp);
            size_t nSgTimeStamp = sgTimeStampBits.to_ulong();
            sOut += str::intToString(nSgTimeStamp);


            std::string sSgFunctionsCount = getNextSubstring(sTotalBits, cur_index, 8);
            std::bitset<8> sgFunctionsCountBits(sSgFunctionsCount);
            size_t nSgFunctionsCount = sgFunctionsCountBits.to_ulong();
            for (size_t j = 0; j < nSgFunctionsCount; j++)
            {
                sOut += "|";
                std::string sType = getNextSubstring(sTotalBits, cur_index, 2);
                //std::bitset<2> bs_type(sType);
                //int nType = bs_type.to_ulong();
                //if (nType == (int)SgFunctionType::kRS)
                if (sType.compare("00") == 0)
                {
                    sOut += "rs";
                }
                else
                {
                    //if (nType == (int)SgFunctionType::kCS)
                    if (sType.compare("01") == 0)
                        sOut += "cs ";
                    else
                        sOut += "ss ";

                    std::string sNumber = getNextSubstring(sTotalBits, cur_index, 8);
                    std::bitset<8> bs_number(sNumber);
                    size_t nNumber = bs_number.to_ulong();
                    sOut += str::intToString(nNumber);
                }
            }
        }


        return 0;
    }
    catch (...)
    {
        return -1;
    }
}


}

#endif // DYNDATA2_H
