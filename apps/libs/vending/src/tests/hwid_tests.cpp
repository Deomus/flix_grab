
#include "hwid_tests.h"
#include "precomp.h"

#ifdef _WIN32

#include "wmihelper.h"

#else // !_WIN32

#endif

#include <list>
#include <string>
#include <stdio.h>

#include "bitstream2.h"
#include "base32.h"
#include "crc32.h"

#include "hwid.h"
#include "except.h"
#include "uniconv.h"

using namespace std;

static string_t hardwareId;


static unsigned short HashString(const char * str) {
    unsigned int crc = Crc32::Compute((const unsigned char *)str, strlen(str));

    return (unsigned short)(crc >> 16);
}

static unsigned short HashInt(int val) {
    unsigned char buf[4] = { (unsigned char)(val >> 24), (unsigned char)(val >> 16), (unsigned char)(val >> 8), (unsigned char)(val & 0xFF) };
    unsigned int crc = Crc32::Compute(buf, 4);

    return (unsigned short)(crc >> 16);
}

static list<string> GetProps(const MapProps& props, const string& key, int count = INT_MAX) {
    list<string> result;
    auto it = props.find(key);
    if (it != props.end()) {
        result = it->second;
        result.resize(min(count, result.size()));
    }
    return result;
}

static void FixSerialStrings(list<string>& props) {
    for (list<string>::iterator it = props.begin(); it != props.end(); ++it) {
        it->erase(0, it->find_first_not_of(" \t\n"));
        it->erase(it->find_last_not_of(" \t\n") + 1);
    }
}

const char_t * GetCurrentHardwareId(const MapProps& props) {
#ifdef _WIN32
    USES_CONVERSION;
    WmiHelper wmi;
#endif

    BitStream2 bits(125);
    list<string> propList;
    unsigned char zero = 0;
    unsigned short hash1, hash2;
    unsigned short h0 = HashInt(0);

    bits.Write(&zero, 3);

    propList.clear();

    propList = GetProps(props,MEM_CAPACITY);

//#ifdef _WIN32
//    wmi.GetPropertyList("SELECT Capacity FROM Win32_PhysicalMemory", "Capacity", &propList);
//#else
//    propList.push_back("4294967296");
//#endif

    unsigned long long memSize = 0;

    for (list<string>::iterator iter = propList.begin(); iter != propList.end(); iter++) {
        memSize += _atoi64(iter->c_str());
    }

    memSize = (memSize + 1023) / (1024 * 1024);

    hash1 = ((int)propList.size() > 0) ? HashInt(memSize) : h0;

    bits.WriteUInt16(hash1);

    propList.clear();

    propList = GetProps(props, PROC_ID, 2);
    
//#ifdef _WIN32
//    wmi.GetPropertyList("SELECT ProcessorId FROM Win32_Processor", "ProcessorId", &propList, 2);
//#else
//    GetCPUIDPropertyList(&propList);
//#endif

    hash1 = (propList.size() > 0) ? HashString(propList.front().c_str()) : HashInt(0);
    hash2 = (propList.size() > 1) ? HashString(propList.back().c_str()) : HashInt(0);

    bits.WriteUInt16(hash1);
    bits.WriteUInt16(hash2);

    propList.clear();

#ifdef _WIN32
    //wmi.GetPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%PCI%\"", "MACAddress", &propList, 2);
    
   
    propList = GetProps(props, PCI_MAC, 2);
   
    if (propList.size() == 0) {
        //wmi.GetPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%VMBUS%\"", "MACAddress", &propList, 2);
        propList = GetProps(props, VMBUS_MAC, 2);
    }
#else
    GetMACPropertyList(&propList, 2);
#endif

    hash1 = (propList.size() > 0) ? HashString(propList.front().c_str()) : HashInt(0);
    hash2 = (propList.size() > 1) ? HashString(propList.back().c_str()) : HashInt(0);

    bits.WriteUInt16(hash1);
    bits.WriteUInt16(hash2);

    propList.clear();

    propList = GetProps(props, SERIAL, 2);
/*
#ifdef _WIN32
    wmi.GetPropertyList("SELECT SerialNumber FROM Win32_PhysicalMedia WHERE SerialNumber IS NOT NULL", "SerialNumber", &propList, 2);
#else
    GetHDDPropertyList(&propList, 2);
#endif*/

    //Fix Serial number;
    FixSerialStrings(propList);
    hash1 = propList.size() > 0 ? HashString(propList.front().c_str()) : HashInt(0);
    hash2 = propList.size() > 1 ? HashString(propList.back().c_str()) : HashInt(0);
    
    /*if (propList.size() > 0) {
        string s = propList.front();
        s.erase(0, s.find_first_not_of(" \t\n"));
        s.erase(s.find_last_not_of(" \t\n") + 1);

        hash1 = HashString(s.c_str());
    }
    else
        hash1 = HashInt(0);

    if (propList.size() > 1) {
        string s = propList.back();
        s.erase(0, s.find_first_not_of(" \t\n"));
        s.erase(s.find_last_not_of(" \t\n") + 1);

        hash2 = HashString(s.c_str());
    }
    else
        hash2 = HashInt(0);*/

    bits.WriteUInt16(hash1);
    bits.WriteUInt16(hash2);

    unsigned char * rawHwid = bits.GetBuffer();

    BASE32 base32;

    char * encHwid = base32.encode(rawHwid, 16);

    hardwareId = A2T(encHwid);

    free(encHwid);

    hardwareId.erase(hardwareId.length() - 1);

    unsigned i;
    unsigned insertPos;
    // separate character groups
    for (i = 0, insertPos = 5; i < 4; i++) {
        hardwareId.insert(insertPos, _T("-"));
        insertPos += 6;
    }

    return hardwareId.c_str();
}

bool MatchCurrentHardwareId(const char_t * hwid, const MapProps& props) {
#ifdef _WIN32
    WmiHelper wmi;
    USES_CONVERSION;
#endif

    unsigned short h0 = HashInt(0);
    string hardwareId(T2A((char_t *)hwid));

    for (int i = 0, erasePos = 0; i < 4; i++) {
        erasePos += 5;
        hardwareId.erase(erasePos, 1);
    }

    BASE32 base32;
    int bufLen;

    int padLen;
    int len = base32.encode_pad_length(((int)hardwareId.length() * 5 + 7) >> 3, &padLen);

    if (len > (int)hardwareId.length())
        hardwareId.append(len - hardwareId.length(), 'A');

    if (padLen)
        hardwareId.append(padLen, '=');

    unsigned char * buf = base32.decode(hardwareId.c_str(), hardwareId.length(), &bufLen);

    BitStream2 bits;
    bits.Attach(buf);
    unsigned char version;

    bits.Read(&version, 3);

    if ((version & 0xE0) != 0)
        throw new KeyExceptionImpl(STATUS_GENERIC_ERROR, _T("invalid hardware id version"));

    list<string> propList;
    unsigned short hash1, hash2;

    bits.ReadUInt16(&hash1);

    if (hash1 != h0) {
        unsigned long long memSize = 0;

        propList = GetProps(props, MEM_CAPACITY);
//#ifdef _WIN32
//        wmi.GetPropertyList("SELECT Capacity FROM Win32_PhysicalMemory", "Capacity", &propList);
//#else
//        propList.push_back("4294967296");
//#endif

        if (propList.size() > 0) {
            for (list<string>::iterator iter = propList.begin(); iter != propList.end(); iter++) {
                memSize += _atoi64(iter->c_str());
            }

            memSize = memSize / (1024 * 1024);

            if (hash1 != HashInt((int)memSize))
                return false;
        }
    }

    bits.ReadUInt16(&hash1);
    bits.ReadUInt16(&hash2);

    propList.clear();

    propList = GetProps(props, PROC_ID, 2);

//
//#ifdef _WIN32
//    wmi.GetPropertyList("SELECT ProcessorId FROM Win32_Processor", "ProcessorId", &propList, 2);
//#else
//    GetCPUIDPropertyList(&propList);
//#endif

    if (propList.size() > 0)
        if (HashString(propList.front().c_str()) != hash1 && (HashString(propList.front().c_str()) != hash2))
            return false;

    if (propList.size() > 1)
        if (HashString(propList.back().c_str()) != hash1 && (HashString(propList.back().c_str()) != hash2))
            return false;

    bits.ReadUInt16(&hash1);
    bits.ReadUInt16(&hash2);

    propList.clear();

#ifdef _WIN32
    /*wmi.GetPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%PCI%\"", "MACAddress", &propList, 2);

    if (propList.size() == 0) {
        wmi.GetPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%VMBUS%\"", "MACAddress", &propList, 2);
    }*/
    propList = GetProps(props, PCI_MAC, 2);
    
    if (propList.size() == 0) {
        propList = GetProps(props, VMBUS_MAC, 2);
    }
#else
    GetMACPropertyList(&propList, 2);
#endif
   
    
    if (propList.size() > 0)
        if (((hash1 != h0) && (HashString(propList.front().c_str()) == hash1)) || ((hash2 != h0) && (HashString(propList.front().c_str()) == hash2)))
            return true;

    if (propList.size() > 1)
        if (((hash1 != h0) && (HashString(propList.back().c_str()) == hash1)) || ((hash2 != h0) && (HashString(propList.back().c_str()) == hash2)))
            return true;

    bits.ReadUInt16(&hash1);
    bits.ReadUInt16(&hash2);

    propList.clear();
    propList = GetProps(props, SERIAL, 2);
/*
#ifdef _WIN32
    wmi.GetPropertyList("SELECT SerialNumber FROM Win32_PhysicalMedia WHERE SerialNumber IS NOT NULL", "SerialNumber", &propList, 2);
#else
    GetHDDPropertyList(&propList, 2);
#endif*/

    FixSerialStrings(propList);

    if (propList.size() > 0)
        if (((hash1 != h0) && (HashString(propList.front().c_str()) == hash1)) || ((hash2 != h0) && (HashString(propList.front().c_str()) == hash2)))
            return true;

    if (propList.size() > 1)
        if (((hash1 != h0) && (HashString(propList.back().c_str()) == hash1)) || ((hash2 != h0) && (HashString(propList.back().c_str()) == hash2)))
            return true;

    return false;
}

