#pragma once
#include <stdint.h>

class CpuUsage {

public:
	CpuUsage() {
		reset();
	}

public:
	float		load() const;

	//How many time in ms process must sleep to keep load equal load_percentage;
	int64_t		sleepForBalance(double load_percentage) const;
	bool		reset();

private:
	int64_t		load_time_;  //100-nanosec;
	int64_t		full_time_;


};
