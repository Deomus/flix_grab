#ifndef _PORTABLE_SNPRINTF_H_
#define _PORTABLE_SNPRINTF_H_

//#define PORTABLE_SNPRINTF_VERSION_MAJOR 2
//#define PORTABLE_SNPRINTF_VERSION_MINOR 2

#if defined(WIN32) || defined(WIN64)
#define HAVE_STDARG_H   1
#define HAVE_VSNPRINTF  0
#define HAVE_SNPRINTF   0
#define HAVE_VASPRINTF  1
#define HAVE_ASPRINTF   1
#endif

#ifdef __cplusplus
extern "C" {
#endif


#if HAVE_STDARG_H
#include <stdarg.h>
#include <stdio.h>
#if !HAVE_VSNPRINTF
    extern int rpl_vsnprintf(char *, size_t, const char *, va_list);
#endif    /* !HAVE_VSNPRINTF */
#if !HAVE_SNPRINTF
    extern int rpl_snprintf(char *, size_t, const char *, ...);
#endif    /* !HAVE_SNPRINTF */
#if !HAVE_VASPRINTF
    int rpl_vasprintf(char **, const char *, va_list);
#endif    /* !HAVE_VASPRINTF */
#if !HAVE_ASPRINTF
    int rpl_asprintf(char **, const char *, ...);
#endif    /* !HAVE_ASPRINTF */
#endif    /* HAVE_STDARG_H */

//#ifdef HAVE_SNPRINTF
//#include <stdio.h>
//#else
//    extern int snprintf(char *, size_t, const char *, /*args*/ ...);
//    extern int vsnprintf(char *, size_t, const char *, va_list);
//#endif
//
//#if defined(HAVE_SNPRINTF) && defined(PREFER_PORTABLE_SNPRINTF)
//    extern int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
//    extern int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);
//   
//#define snprintf  portable_snprintf
//#define vsnprintf portable_vsnprintf
//#endif
//
//    extern int asprintf  (char **ptr, const char *fmt, /*args*/ ...);
//    extern int vasprintf (char **ptr, const char *fmt, va_list ap);
//    extern int asnprintf (char **ptr, size_t str_m, const char *fmt, /*args*/ ...);
//    extern int vasnprintf(char **ptr, size_t str_m, const char *fmt, va_list ap);
//
//#endif

#ifdef __cplusplus
};
#endif

#endif