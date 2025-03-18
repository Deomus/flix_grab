#include "company\freegrabapp.inc"

#define AppName           "FreeMySpassDownload"
#define AppHumanName      "Free MySpass Download"

[Setup]

WizardImageFile         = installer_files\freemyspassdownload.bmp

[Registry]
    Root: HKCU; Subkey: "Software\{#CompanyName}\{#AppHumanName}";  ValueType: dword;   ValueName: "LastFormat";      ValueData: "1";
    Root: HKCU; Subkey: "Software\{#CompanyName}\{#AppHumanName}";  ValueType: string;  ValueName: "DefaultQuality";  ValueData: "MPEG4";