// widen.h

#ifndef COMMON_WIDEN_H
#define COMMON_WIDEN_H

#ifndef WIDEN
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#endif

#ifndef ADD_LL
#define ADD_LL2(x) LL( ## x ## )
#define ADD_LL(x) ADD_LL2(x)
#endif

#endif // COMMON_WIDEN_H
