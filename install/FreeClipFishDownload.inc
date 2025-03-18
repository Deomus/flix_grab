#include "company\freegrabapp.inc"

#define AppName           "FreeClipFishDownload"
#define AppHumanName      "Free ClipFish Download"

[Setup]

WizardImageFile         = installer_files\freeclipfishdownload.bmp

[Registry]
    Root: HKCU; Subkey: "Software\{#CompanyName}\{#AppHumanName}";  ValueType: dword;   ValueName: "LastFormat";      ValueData: "1";
    Root: HKCU; Subkey: "Software\{#CompanyName}\{#AppHumanName}";  ValueType: string;  ValueName: "DefaultQuality";  ValueData: "MPEG4";