#ifndef __UNICONV_H
#define __UNICONV_H

#ifdef _WIN32

#include <windows.h>
#include <assert.h>

inline LPWSTR WINAPI A2WHelper(LPWSTR lpw, LPCSTR lpa, int nChars, UINT acp) throw()
{
	assert(lpa != NULL);
	assert(lpw != NULL);

	if (lpw == NULL || lpa == NULL)
		return NULL;

	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	lpw[0] = '\0';
	int ret = MultiByteToWideChar(acp, 0, lpa, -1, lpw, nChars);
	if(ret == 0)
	{
		assert(0);
		return NULL;
	}		
	return lpw;
}

inline LPSTR WINAPI W2AHelper(LPSTR lpa, LPCWSTR lpw, int nChars, UINT acp) throw()
{
	assert(lpw != NULL);
	assert(lpa != NULL);

	if (lpa == NULL || lpw == NULL)
		return NULL;

	// verify that no illegal character present
	// since lpa was allocated based on the size of lpw
	// don't worry about the number of chars
	lpa[0] = '\0';
	int ret = WideCharToMultiByte(acp, 0, lpw, -1, lpa, nChars, NULL, NULL);
	if(ret == 0)
	{
		assert(0);
		return NULL;
	}
	return lpa;
}

#ifndef USES_CONVERSION

#ifdef NDEBUG
	#define USES_CONVERSION int _convert; UINT _acp = CP_ACP; LPCWSTR _lpw; LPCSTR _lpa;
	#define USES_CONVERSION_2 int _convert2; LPCWSTR _lpw2; LPCSTR _lpa2;
	#define USES_CONVERSION_3 int _convert3; LPCWSTR _lpw3; LPCSTR _lpa3;
	#define USES_CONVERSION_4 int _convert4; LPCWSTR _lpw4; LPCSTR _lpa4;
	#define USES_CONVERSION_5 int _convert5; LPCWSTR _lpw5; LPCSTR _lpa5;
#else
	#define USES_CONVERSION int _convert = 0; UINT _acp = CP_ACP; LPCWSTR _lpw = NULL; LPCSTR _lpa = NULL;
	#define USES_CONVERSION_2 int _convert2 = 0; LPCWSTR _lpw2; LPCSTR _lpa2;
	#define USES_CONVERSION_3 int _convert3 = 0; LPCWSTR _lpw3; LPCSTR _lpa3;
	#define USES_CONVERSION_4 int _convert4 = 0; LPCWSTR _lpw4; LPCSTR _lpa4;
	#define USES_CONVERSION_5 int _convert5 = 0; LPCWSTR _lpw5; LPCSTR _lpa5;
#endif

#endif

#define W2A(lpw) (\
	((_lpw = lpw) == NULL) ? NULL : (\
		_convert = (lstrlenW(_lpw)+1)*4,\
		W2AHelper((LPSTR) alloca(_convert), _lpw, _convert, _acp)))

#define W2A_2(lpw) (((lpw) == NULL) ? NULL : W2AHelper((LPSTR) alloca((lstrlenW(lpw)+1)*4), lpw, (lstrlenW(lpw)+1)*4, CP_ACP))

#define W2A_3 W2A_2
#define W2A_4 W2A_2
#define W2A_5 W2A_2

#define A2W(lpa) (\
	((_lpa = lpa) == NULL) ? NULL : (\
		_convert = (lstrlenA(_lpa)+1),\
		A2WHelper((LPWSTR) alloca(_convert*4), _lpa, _convert, _acp)))

#define A2W_2(lpa) (\
	((_lpa2 = lpa) == NULL) ? NULL : (\
		_convert2 = (lstrlenA(_lpa2)+1),\
		A2WHelper((LPWSTR) alloca(_convert2*4), _lpa2, _convert2, _acp)))

#define A2W_3(lpa) (\
	((_lpa3 = lpa) == NULL) ? NULL : (\
		_convert3 = (lstrlenA(_lpa3)+1),\
		A2WHelper((LPWSTR) alloca(_convert3*4), _lpa3, _convert3, _acp)))

#define A2W_4(lpa) (\
	((_lpa4 = lpa) == NULL) ? NULL : (\
		_convert4 = (lstrlenA(_lpa4)+1),\
		A2WHelper((LPWSTR) alloca(_convert4*4), _lpa4, _convert4, _acp)))

#define A2W_5(lpa) (\
	((_lpa5 = lpa) == NULL) ? NULL : (\
		_convert5 = (lstrlenA(_lpa5)+1),\
		A2WHelper((LPWSTR) alloca(_convert5*4), _lpa5, _convert5, _acp)))

#define UTF82W(lpa) (\
	((_lpa = lpa) == NULL) ? NULL : (\
		_convert = (lstrlenA(_lpa)+1),\
		A2WHelper((LPWSTR) alloca(_convert*4), _lpa, _convert, CP_UTF8)))

#define UTF82W_2(lpa) (\
	((_lpa2 = lpa) == NULL) ? NULL : (\
		_convert2 = (lstrlenA(_lpa2)+1),\
		A2WHelper((LPWSTR) alloca(_convert2*4), _lpa2, _convert2, CP_UTF8)))

#define W2UTF8(lpw) (\
	((_lpw = lpw) == NULL) ? NULL : (\
		_convert = (lstrlenW(_lpw)+1)*4,\
		W2AHelper((LPSTR) alloca(_convert), _lpw, _convert, CP_UTF8)))

#define W2UTF8_2(lpw) (\
	((_lpw2 = lpw) == NULL) ? NULL : (\
		_convert2 = (lstrlenW(_lpw2)+1)*4,\
		W2AHelper((LPSTR) alloca(_convert2), _lpw2, _convert2, CP_UTF8)))


#else // !_WIN32

#define USES_CONVERSION
#define USES_CONVERSION_2
#define USES_CONVERSION_3
#define USES_CONVERSION_4
#define USES_CONVERSION_5

#ifdef _UNICODE
#define TCHAR wchar_t
#define _stprintf wsprintf
#else
#define TCHAR char
#define _stprintf sprintf
#endif

#define A2W(s) ((wchar_t *)(s))
#define A2W_2(s) ((wchar_t *)(s))
#define A2W_3(s) ((wchar_t *)(s))
#define A2W_4(s) ((wchar_t *)(s))
#define A2W_5(s) ((wchar_t *)(s))

#define W2A(s) ((char *)(s))
#define W2A_2(s) ((char *)(s))
#define W2A_3(s) ((char *)(s))
#define W2A_4(s) ((char *)(s))
#define W2A_5(s) ((char *)(s))

#define UTF82W(s) ((wchar_t *)(s))
#define UTF82W_2(s) ((wchar_t *)(s))

#define W2UTF8(s) ((char *)(s))
#define W2UTF8_2(s) ((char *)(s))

#endif

#ifdef _UNICODE
	#define A2T A2W
	#define A2T_2 A2W_2
	#define A2T_3 A2W_3
	#define A2T_4 A2W_4
	#define A2T_5 A2W_5

	#define UTF82T   UTF82W
    #define UTF82T_2 UTF82W_2

	#define T2A W2A
	#define T2UTF8 W2UTF8
    #define T2UTF8_2 W2UTF8_2

	#define W2T(lpt) lpt
	#define W2T_2(lpt) lpt
	#define W2T_3(lpt) lpt
	#define W2T_4(lpt) lpt
	#define W2T_5(lpt) lpt

	#define T2W(lpt) lpt

#ifndef _WIN32
	#define _itot itow
#endif

#else
	#define A2T(lpt) lpt
	#define A2T_2(lpt) lpt
	#define A2T_3(lpt) lpt
	#define A2T_4(lpt) lpt	
	#define A2T_5(lpt) lpt

	#define UTF82T(lpt) lpt /* NOT OK */
	#define UTF82T_2(lpt) lpt

	#define T2A(lpt) lpt
	
	#define T2UTF8(lpt) lpt
    #define T2UTF8_2(lpt) lpt
	
	#define W2T W2A
	#define W2T_2 W2A_2
	#define W2T_3 W2A_3
	#define W2T_4 W2A_4
	#define W2T_5 W2A_5
    
	#define T2W A2W

#ifndef _WIN32
	#define _itot itoa
#endif

#endif

#endif
