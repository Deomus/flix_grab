#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "hardware_logs.h"

#include <precomp.h>
#include "wmihelper.h"
#include "uniconv.h"

#include <hwid.h>


using namespace std;

#pragma comment(lib, "winhttp.lib") 
#pragma comment(lib, "Ws2_32.lib") 


void GetHardwareProperties(MapProps& props) {
    USES_CONVERSION;
    WmiHelper wmi;

    const char_t* hid = HardwareId::GetCurrentHardwareId();
            
    props[HID].push_back(W2UTF8(hid));
    props[HID_MATCH].push_back(to_string(HardwareId::MatchCurrentHardwareId(hid)));
            
    wmi.GetPropertyList("SELECT Capacity FROM Win32_PhysicalMemory", "Capacity", &props[MEM_CAPACITY]);
    wmi.GetPropertyList("SELECT ProcessorId FROM Win32_Processor", "ProcessorId", &props[PROC_ID]);
    wmi.GetPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%PCI%\"", "MACAddress", &props[PCI_MAC]);
    wmi.GetPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%VMBUS%\"", "MACAddress", &props[VMBUS_MAC]);
    wmi.GetPropertyList("SELECT SerialNumber FROM Win32_PhysicalMedia WHERE SerialNumber IS NOT NULL", "SerialNumber", &props[SERIAL]);

}

void WriteHardwareProperties(const MapProps& props, const string& filename) {
    ofstream to(filename, ios::binary | ios::trunc);

    for (auto prop : props) {
        for (auto value : prop.second) {
            to << '{' << prop.first << '=' << value << '}';
        }
    }
}

void ReadHardwareProperties(const string& filename, MapProps& props) {
    ifstream from(filename, ios::binary);
    string buffer;
    buffer.resize(1024);

    while (!from.fail()) {
        from.getline(&buffer[0], buffer.size(), '{');
        //from.ignore(1);
        from.getline(&buffer[0], buffer.size(), '=');

        if (from.fail())
            break;
        string key = buffer;

        from.getline(&buffer[0], buffer.size(), '}');
        props[key.c_str()].push_back(buffer.c_str());

    }
}

bool IsEqual(const MapProps& props1, const MapProps& props2) {
    for (auto prop : props1) {

        if (!prop.second.empty()) {
            auto it = props2.find(prop.first);
            if (it != props2.end()) {
                for (auto value : prop.second) {
                    auto result = std::find(it->second.begin(), it->second.end(), value);
                    
                    if (result == it->second.end())
                        return false;
                }

            } else
                return false;
        }
        
    }
    return true;
}

