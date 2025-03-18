#pragma once

#include "hardware_logs.h"
#include "hwid.h"

const char_t * GetCurrentHardwareId(const MapProps& props);
bool MatchCurrentHardwareId(const char_t * hwid, const MapProps& props);
