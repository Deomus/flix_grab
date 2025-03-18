#pragma once
#include <string>
#include <iostream>
#include <list>
#include <map>



const char MEM_CAPACITY[] = "Capacity";
const char PROC_ID[] = "ProcId";
const char PCI_MAC[] = "PciMac";
const char VMBUS_MAC[] = "VmbusMac";
const char SERIAL[] = "Serial";
const char HID[] = "HwId";
const char HID_MATCH[] = "HwMatch";


typedef std::map<std::string, std::list<std::string>>    MapProps;


void GetHardwareProperties(MapProps& props);
void WriteHardwareProperties(const MapProps& props, const std::string& filename);
void ReadHardwareProperties(const std::string& filename, MapProps& props);
bool IsEqual(const MapProps& props1, const MapProps& props2);

