#include "company\freegrabapp.inc"

#define FreeYouTubeDownload     "FreeYouTubeDownload"
#define AppName                 "FreeYouTubeDownload"
#define AppHumanName            "Free YouTube Download"

[Setup]

WizardImageFile         = installer_files\freeyoutubedownload.bmp

[Files]

Source: installer_files\youtube-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;