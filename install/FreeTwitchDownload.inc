#include "company\freegrabapp.inc"

#define FreeTwitchDownload      "FreeTwitchDownload"
#define AppName                 "FreeTwitchDownload"
#define AppHumanName            "Free Twitch Download"

[Setup]

WizardImageFile         = installer_files\freetwitchdownload.bmp

[Files]

Source: installer_files\twitch-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;