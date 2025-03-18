#pragma once

#include <stdint.h>

typedef int32_t             TResult;

enum EResult
{
    kResult_Success = 0,
    kResult_Fail,               //Procedure failed for unknown reason;
    kResult_WrongParam,         //Wrong input param detected;
    kResult_OutOfRange,         //Using for enumerate;
    kResult_NoMemory,
    kResult_IoError,
    kResult_Unsupported,
    kResult_Timeout,
    kResult_NotImplemented,
    kResult_Skipped,
    kResult_Unconfigured,
    kResult_WrongAddress,
    kResult_WrongAccess,
    kResult_WrongState,
    kResult_BufferOverflow,
    kResult_Interrupted,

    kResult_ForceDWORD                    = 0x7FFFFFFF,

};