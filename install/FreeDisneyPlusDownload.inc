#include "company\freegrabapp.inc"

#define FreeDisneyPlusDownload  "FreeDisneyPlusDownload"
#define AppName                 "FreeDisneyPlusDownload"
#define AppHumanName            "Free Disney Plus Download"

[Setup]

WizardImageFile         = installer_files\freedisneyplusdownload.bmp

[Files]

Source: installer_files\disney-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;