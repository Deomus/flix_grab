#include "company\freegrabapp.inc"

#define FreeAppleMusicDownload  "FreeAppleMusicDownload"
#define AppName                 "FreeAppleMusicDownload"
#define AppHumanName            "Free Apple Music Download"

[Setup]

WizardImageFile         = installer_files\freeapplemusicdownload.bmp

[Files]

Source: installer_files\applem-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;