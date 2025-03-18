#ifndef __HARDWARE_ID_H
#define __HARDWARE_ID_H

#ifndef char_t
#ifdef _UNICODE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif
#endif

#ifndef string_t
#ifdef _UNICODE
typedef std::wstring string_t;
#else
typedef std::string string_t;
#endif
#endif

class HardwareId
{
public:
	static const char_t * GetCurrentHardwareId();
	static bool MatchCurrentHardwareId(const char_t * hwid);

private:
	static unsigned short HashString(const char * str);
	static unsigned short HashInt(int val);
	static string_t hardwareId;
};

#endif
