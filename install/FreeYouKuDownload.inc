#include "company\freegrabapp.inc"

#define AppName           "FreeYouKuDownload"
#define AppHumanName      "Free YouKu Download"

[Setup]

WizardImageFile         = installer_files\freeyoutubedownload.bmp

[Registry]
    Root: HKCU; Subkey: "Software\{#CompanyName}\{#AppHumanName}";  ValueType: dword;   ValueName: "LastFormat";      ValueData: "1";
    Root: HKCU; Subkey: "Software\{#CompanyName}\{#AppHumanName}";  ValueType: string;  ValueName: "DefaultQuality";  ValueData: "MPEG4";