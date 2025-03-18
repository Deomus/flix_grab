#include "company\freegrabapp.inc"

#define FreeAmazonMusicDownload     "FreeAmazonMusicDownload"
#define AppName                     "FreeAmazonMusicDownload"
#define AppHumanName                "Free Amazon Music Download"

[Setup]

WizardImageFile         = installer_files\freeamazonmusicdownload.bmp

[Files]

Source: installer_files\amazonmusic-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;