// platform_fork.h

#ifndef PLATFORM_FORK_H
#define PLATFORM_FORK_H

//itoa
#if defined(WIN32) || defined(WIN64)
#else
inline char* itoa(int value, char* result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
#endif

////////
#if defined(WIN32) || defined(WIN64)
    #define strcpy_safe(_Dst, _SizeInBytes, _Src)               strcpy_s((_Dst), (_SizeInBytes), (_Src))
    #define strncpy_safe(_Dst, _SizeInBytes, _Src, _MaxCount)   strncpy_s((_Dst), (_SizeInBytes), (_Src), (_MaxCount))
    #define atoi64(value)                                       _atoi64((value))
    #define atoi64u(value)                                      _atoi64((value))
    #define itoa(val, buf, radix)                               _itoa((val), (buf), (radix))
    #define itoa_safe(val, buf, bufsize, radix)                 _itoa_s((val), (buf), (bufsize), (radix))
    #define ttoi                                                _wtoi
    #define STRCASECMP _stricmp
#else
    #define strcpy_safe(_Dst, _SizeInBytes, _Src)               strcpy((_Dst), (_Src))
    #define strncpy_safe(_Dst, _SizeInBytes, _Src, _MaxCount)   strncpy((_Dst), (_Src), (_MaxCount))
    #define atoi64(value)                                       strtoll(value, 0, 10)
    #define atoi64u(value)                                      strtoull(value, 0, 10)
    #define itoa(val, buf, radix)                               itoa((val), (buf), (radix))
    #define itoa_safe(val, buf, bufsize, radix)                 itoa((val), (buf), (radix))
    #define ttoi                                                atoi
    #define STRCASECMP strcasecmp
#endif

#endif //PLATFORM_FORK_H
