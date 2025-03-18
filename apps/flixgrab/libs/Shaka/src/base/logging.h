// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <cassert>
#include <string>
#include <cstring>
#include <sstream>

#include <chrono>
#include <ctime> 

#include "macros.h"
#include "base_export.h"
#include <iostream>
// #include "basictypes.h"


//Logger
typedef int LogSeverity;
const LogSeverity LOG_DEBUG = -2;
const LogSeverity LOG_VERBOSE = -1;  // This is level 1 verbosity
// Note: the log severities are used to index into the array of names,
// see log_severity_names.
const LogSeverity LOG_INFO = 0;
const LogSeverity LOG_WARNING = 1;
const LogSeverity LOG_ERROR = 2;
const LogSeverity LOG_FATAL = 3;
const LogSeverity LOG_NUM_SEVERITIES = 4;

#if defined(_WIN32)
// wingdi.h defines ERROR to be 0. When we call LOG(ERROR), it gets
// substituted with 0
#define ERROR 0
// Needed for LOG_IS_ON(ERROR).
const LogSeverity LOG_0 = LOG_ERROR;
#endif


class Log
{
private:
    std::ostream& _out_stream;

    std::ostringstream      buffer_;

public:

    //Constructor: User passes a custom log header and output stream, or uses defaults.
    Log(const std::string& log_header, std::ostream& out_stream = std::cout) : _out_stream(out_stream)
    {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now); //Uhhg, C APIs...
        
        char str[64]; 
        tm now_tm;
        localtime_s(&now_tm, &now_time_t); //More uhhg, C style... 

        //_out_stream << std::dec << "(" << now_tm->tm_hour << ":" << now_tm->tm_min << ":" << now_tm->tm_sec << ") " << log_header << ": ";
        buffer_ << std::dec << "(" << now_tm.tm_hour << ":" << now_tm.tm_min << ":" << now_tm.tm_sec << ") " << log_header << ": ";

    }

    ~Log() {
        buffer_ << std::endl;
        _out_stream << buffer_.str();

        //_out_stream << std::endl; 
    }


    //Overload for anything else:
    template<typename T>
    Log& operator<< (const T& data)
    {
        //_out_stream << data;
        buffer_ << data;

        return *this;
    }
};


class LoggerSettings {
public:

    static LogSeverity GetMinLogLevel() { return  min_severity_; }
    static void SetMinLogLevel(LogSeverity severity) { min_severity_ = severity; }

private:
    static LogSeverity             min_severity_;
};

// As special cases, we can assume that LOG_IS_ON(FATAL) always holds. Also,
// LOG_IS_ON(DFATAL) always holds in debug mode. In particular, CHECK()s will
// always fire if they fail.
#define LOG_IS_ON(severity) \
  ((LOG_##severity) >= LoggerSettings::GetMinLogLevel())


// This class is used to explicitly ignore values in the conditional
// logging macros.  This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
public:
    LogMessageVoidify() { }
    // This has to be an operator with a precedence lower than << but
    // higher than ?:
    void operator&(std::ostream&) { }
    void operator&(Log&) { }
};

// Helper macro which avoids evaluating the arguments to a stream if
// the condition doesn't hold. Condition is evaluated once and only once.
#define LAZY_STREAM(stream, condition)                                  \
  !(condition) ? (void) 0 : LogMessageVoidify() & (stream)

//#define LOG_STREAM(severity)    std::cout << std::endl << #severity << ": "
#define LOG_STREAM(severity)     Log(#severity)


#define LOG(severity) LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity))


#define LOG_IF(severity, condition) \
  LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity) && (condition))

#define SYSLOG(severity) LOG(severity)
#define SYSLOG_IF(severity, condition) LOG_IF(severity, condition)

//#define LOG(severity) (std::cout << std::endl << #severity << ": ")
#define DLOG(s) LOG(s) 
#define VLOG(n) LOG(VERBOSE) << n << " "

#define NOTIMPLEMENTED()        LOG(ERROR)

//#define DCHECK(condition)                                                \
//  if (!(condition) ) DLOG(ERROR) << "Check failed: " << ". " << __FILE__ << " line:" << __LINE__;

#define DCHECK(condition)                                                \
  if (!(condition) ) std::cout << "Check failed: " #condition ". " << __FILE__ << " line:" << std::dec << __LINE__ << std::endl;

//LAZY_STREAM(LOG_STREAM(DCHECK), DCHECK_IS_ON() ? !(condition) : false)
//      << "Check failed: " #condition ". "

#define DVLOG(n)  LOG(VERBOSE) << n << " "

#define MEDIA_LOG(x,y) DLOG(INFO)
#define LIMITED_MEDIA_LOG(a,b,c,d) DLOG(a)
#define DCHECK_GT(x,y) DCHECK(x>y)
#define DCHECK_EQ(x,y) DCHECK(x==y)
#define DCHECK_GE(x,y) DCHECK(x>=y)

#define CHECK(r) assert(r);

#define NOTREACHED() \
    std::cout << "NOTREACHED: " << __FILE__ << __LINE__ << ". "

namespace media {
    namespace mp4 {
        class MediaLog {};

        class NullMediaLog : public MediaLog {};

    }
}


#endif  // BASE_LOGGING_H_
