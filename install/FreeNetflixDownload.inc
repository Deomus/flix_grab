#include "company\freegrabapp.inc"

#define FreeNetflixDownload     "FreeNetflixDownload"
#define AppName                 "FreeNetflixDownload"
#define AppHumanName            "Free Netflix Download"

[Setup]

WizardImageFile         = installer_files\freenetflixdownload.bmp

[Files]

Source: installer_files\netflix-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;