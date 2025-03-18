#include "company\freegrabapp.inc"

#define FreeInstagramDownload       "FreeInstagramDownload"
#define AppName                     "FreeInstagramDownload"
#define AppHumanName                "Free Instagram Download"

[Setup]

WizardImageFile         = installer_files\freeinstagramdownload.bmp

[Files]

Source: installer_files\instagram-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;