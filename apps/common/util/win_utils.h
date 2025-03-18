#pragma once

#include <windows.h>
#include <string>

namespace win32
{
	static std::string WString2AString(const std::wstring& src, UINT nCodePage)
	{
		const int nLen = (int)src.length();

		if (nLen == 0)
			return "";

		const int BUFFSIZE = 1024;
		std::string strRes;

		const int nLenA = ::WideCharToMultiByte(nCodePage, 0, src.c_str(), nLen, NULL, 0, NULL, NULL);
		if (nLenA < BUFFSIZE)
		{
			char szBuff[BUFFSIZE] = { 0 };
			if (0 != ::WideCharToMultiByte(nCodePage, 0, src.c_str(), nLen, szBuff, nLenA, NULL, NULL))
				strRes = szBuff;
		}
		else
		{
			char* pBuff = new char[nLenA + 1];
			::ZeroMemory(pBuff, nLenA + 1);
			if (0 != ::WideCharToMultiByte(nCodePage, 0, src.c_str(), nLen, pBuff, nLenA, NULL, NULL))
				strRes = pBuff;
			delete[] pBuff;
		}

		return strRes;
	}

    static std::wstring AString2WString(const char* szSrc, UINT nCodePage)
    {
        if (!szSrc)
            return L"";
        const int nLen = (int)strlen(szSrc);

        if (nLen == 0)
            return L"";

        const int BUFFSIZE = 1024;
        std::wstring strRes;

        const int nLenW = ::MultiByteToWideChar(nCodePage, 0, szSrc, nLen + 1, NULL, 0);
        if (nLenW < BUFFSIZE)
        {
            wchar_t szBuff[BUFFSIZE] = { 0 };
            if (0 != ::MultiByteToWideChar(nCodePage, 0, szSrc, nLen + 1, szBuff, BUFFSIZE))
                strRes = szBuff;
        }
        else
        {
            wchar_t* pBuff = new wchar_t[nLenW + 1];
            ::ZeroMemory(pBuff, nLenW + 1);
            if (0 != ::MultiByteToWideChar(nCodePage, 0, szSrc, nLen + 1, pBuff, nLenW))
                strRes = pBuff;
            delete[] pBuff;
        }

        return strRes;
    }
}

static std::string WString2AString(const std::wstring& src)
{
	return win32::WString2AString(src, CP_UTF8);
}

static std::wstring AString2WString(const char* szSrc)
{
	return win32::AString2WString(szSrc, CP_UTF8);
}
