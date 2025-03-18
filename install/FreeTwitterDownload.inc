#include "company\freegrabapp.inc"

#define FreeTwitterDownload     "FreeTwitterDownload"
#define AppName                 "FreeTwitterDownload"
#define AppHumanName            "Free Twitter Download"

[Setup]

WizardImageFile         = installer_files\freetwitterdownload.bmp

[Files]

Source: installer_files\twitter-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;