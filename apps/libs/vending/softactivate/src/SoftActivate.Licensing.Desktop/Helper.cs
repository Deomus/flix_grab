using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Management;
using System.Diagnostics;
using System.IO; 

namespace SoftActivate.Licensing
{
    public class KeyHelper
    {
        public static string GetCurrentHardwareId()
        {
            byte[] buf = new byte[16];
            ushort h0 = HashInt(0);

            BitStream2 stream = new BitStream2();
            stream.Attach(buf, 125);

            // write version 0
            stream.Write(buf, 3);

            UInt64 memSize = 0;

            string[] a = GetWmiPropertyList("SELECT Capacity FROM Win32_PhysicalMemory", "Capacity");

            foreach (string s in a)
            {
                memSize += UInt64.Parse(s);
            }

            memSize = memSize / (1024 * 1024);

            stream.WriteUInt16(HashInt((int)memSize));

            ushort[] propList = HashStringArray(GetWmiPropertyList("SELECT ProcessorId FROM Win32_Processor", "ProcessorId"));

            stream.WriteUInt16((propList.Length > 0) ? propList[0] : h0);
            stream.WriteUInt16((propList.Length > 1) ? propList[1] : h0);

            propList = HashStringArray(GetWmiPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%PCI%\"", "MACAddress"));

            if (propList.Length == 0)
                propList = HashStringArray(GetWmiPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%VMBUS%\"", "MACAddress"));

            stream.WriteUInt16((propList.Length > 0) ? propList[0] : h0);
            stream.WriteUInt16((propList.Length > 1) ? propList[1] : h0);

            propList = HashStringArray(GetWmiPropertyList("SELECT SerialNumber FROM Win32_PhysicalMedia WHERE SerialNumber IS NOT NULL", "SerialNumber"));

            stream.WriteUInt16((propList.Length > 0) ? propList[0] : h0);
            stream.WriteUInt16((propList.Length > 1) ? propList[1] : h0);

            string hwid = Base32.Encode(buf, 0, 16);
            hwid = hwid.Substring(0, hwid.Length - 1);

            int i;
            int insertPos;

            // separate character groups
            for (i = 0, insertPos = 5; i < 4; i++)
            {
                hwid = hwid.Insert(insertPos, "-");
                insertPos += 6;
            }

            return hwid;
        }

        public static bool MatchCurrentHardwareId(string hwid)
        {
            ushort h0 = HashInt(0);

            // ungroup
            for (int i = 0, erasePos = 0; i < 4; i++)
            {
                erasePos += 5;
                hwid = hwid.Remove(erasePos, 1);
            }

            byte[] buf = new byte[4];
            byte[] hardwareId = Base32.Decode(hwid, 0, 25);
            BitStream2 bits = new BitStream2();
            bits.Attach(hardwareId, 125);

            bits.Read(buf, 3);
            
            if ((buf[0] & 0xE0) != 0)
                throw new Exception("invalid hardware id version");

            ushort hash1, hash2;

            bits.ReadUInt16(out hash1);

            UInt64 memSize = 0;

            string[] a = GetWmiPropertyList("SELECT Capacity FROM Win32_PhysicalMemory", "Capacity");

            foreach (string s in a)
            {
                memSize += UInt64.Parse(s);
            }

            memSize = memSize / (1024 * 1024);

            if (hash1 != h0 && hash1 != HashInt((int)memSize))
                return false;

            bits.ReadUInt16(out hash1);
            bits.ReadUInt16(out hash2);

            ushort[] propList = HashStringArray(GetWmiPropertyList("SELECT ProcessorId FROM Win32_Processor", "ProcessorId"));

            if (propList.Length > 0)
                if (propList[0] != hash1 && propList[0] != hash2)
                    return false;

            if (propList.Length > 1)
                if (propList[1] != hash1 && propList[1] != hash2)
                    return false;

            bits.ReadUInt16(out hash1);
            bits.ReadUInt16(out hash2);

            propList = HashStringArray(GetWmiPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%PCI%\"", "MACAddress"));

            if (propList.Length == 0)
                propList = HashStringArray(GetWmiPropertyList("SELECT MACAddress FROM Win32_NetworkAdapter WHERE (AdapterType = \"Ethernet 802.3\") AND (MACAddress IS NOT NULL) AND PNPDeviceId LIKE \"%VMBUS%\"", "MACAddress"));

            if (propList.Length > 0)
                if ((hash1 != h0 && propList[0] == hash1) || (hash2 != h0 && propList[0] == hash2))
                    return true;

            if (propList.Length > 1)
                if ((hash1 != h0 && propList[1] == hash1) || (hash2 != h0 && propList[1] == hash2))
                    return true;

            bits.ReadUInt16(out hash1);
            bits.ReadUInt16(out hash2);

            propList = HashStringArray(GetWmiPropertyList("SELECT SerialNumber FROM Win32_PhysicalMedia WHERE SerialNumber IS NOT NULL", "SerialNumber"));

            if (propList.Length > 0)
                if ((hash1 != h0 && propList[0] == hash1) || (hash2 != h0 && propList[0] == hash2))
                    return true;

            if (propList.Length > 1)
                if ((hash1 != h0 && propList[1] == hash1) || (hash2 != h0 && propList[1] == hash2))
                    return true;

            return false;
        }

        private static string[] GetWmiPropertyList(string wqlQuery, string propertyName)
        {
            ArrayList propertyList = new ArrayList();
            ManagementObjectSearcher searcher = new ManagementObjectSearcher(wqlQuery);

            foreach (ManagementObject obj in searcher.Get())
            {
                try
                {
                    if (obj[propertyName] != null)
                    {
                        string propertyValue = obj[propertyName].ToString();
                        if (!string.IsNullOrEmpty(propertyValue))
                            if (!string.IsNullOrEmpty(propertyValue.Trim()))
                            {
                                propertyList.Add(propertyValue.Trim());
                            }
                    }
                }
                catch (Exception)
                {

                }
            }

            return (string[])propertyList.ToArray(typeof(string));
        }

        private static ushort[] HashStringArray(string[] stringArray)
        {
            ArrayList hashArray = new ArrayList();

            foreach (string s in stringArray)
            {
                Crc32 crc = new Crc32();
                crc.Initialize();
                crc.ComputeHash(Encoding.UTF8.GetBytes(s));
                hashArray.Add((ushort)(((ushort)crc.Hash[0] << 8) | (ushort)crc.Hash[1]));
            }

            return (ushort[])hashArray.ToArray(typeof(ushort));
        }

        private static ushort HashInt(int val)
        {
            byte[] split = new byte[4] { (byte)((val >> 24) & 0xFF), (byte)((val >> 16) & 0xFF), (byte)((val >> 8) & 0xFF), (byte)(val & 0xFF) };
            Crc32 crc = new Crc32();
            crc.Initialize();
            crc.ComputeHash(split);
            return (ushort)(((ushort)crc.Hash[0] << 8) | (ushort)crc.Hash[1]);
        }

        public static bool DetectClockManipulation(DateTime thresholdTime)
        {
            DateTime adjustedThresholdTime = new DateTime(thresholdTime.Year, thresholdTime.Month, thresholdTime.Day, 23, 59, 59);

            string[] events = GetWmiPropertyList("SELECT EventIdentifier FROM Win32_NTLogEvent WHERE LogFile='system' AND TimeGenerated > '" + ManagementDateTimeConverter.ToDmtfDateTime(adjustedThresholdTime) + "'", 
                                                 "EventIdentifier");

            if (events.Length > 0)
                return true;

            string[] tempFiles = Directory.GetFiles(Path.GetTempPath(), "*.*", SearchOption.TopDirectoryOnly);

            foreach (string file in tempFiles)
            {
                if (File.GetLastWriteTime(file) > adjustedThresholdTime)
                    return true;
            }

            return false;
        }
    }
}
