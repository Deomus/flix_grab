#include "company\flixgot.inc"

#define NetflixDownloader       "NetflixDownloader"
#define AppName                 "NetflixDownloader"
#define AppHumanName            "Netflix Downloader"

[Setup]

WizardImageFile         = installer_files\netflixdownloader.bmp

[Files]

Source: installer_files\netflix-downloader-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;