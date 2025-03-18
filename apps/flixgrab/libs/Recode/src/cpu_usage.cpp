#include "cpu_usage.h"


#include <windows.h>
#include <stdint.h>
#include <assert.h>

inline int64_t  GetTime(const FILETIME &a) {
	LARGE_INTEGER l;
	l.LowPart = a.dwLowDateTime;
	l.HighPart = a.dwHighDateTime;
	return l.QuadPart;
}

inline bool		GetUsage(int64_t& load_time, int64_t& full_time) {
	FILETIME sys_idle, sys_kernel, sys_user;
	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);

	if (GetSystemTimes(&sys_idle, &sys_kernel, &sys_user)) {

		full_time = (GetTime(sys_kernel) + GetTime(sys_user));
		load_time = full_time - GetTime(sys_idle);
		full_time /= sys_info.dwNumberOfProcessors;
		load_time /= sys_info.dwNumberOfProcessors;

		assert(load_time < full_time);

		return true;
	}
	return false;
}

float CpuUsage::load() const {
	int64_t		load_time, full_time;  //100-nanosec;

	if (GetUsage(load_time, full_time)) {
		return (float)(load_time - load_time_) / (float)(full_time - full_time_);
	}
	return 0;
}

int64_t CpuUsage::sleepForBalance(double load_percentage) const {
	int64_t		load_time, full_time;  //100-nanosec;

	if (GetUsage(load_time, full_time)) {
		load_time -= load_time_;
		full_time -= full_time_;

		return (int64_t)((load_time / load_percentage - full_time) / 10000);
	}
	return 0;
}

bool CpuUsage::reset() {
	return GetUsage(load_time_, full_time_);
}
