#include "company\freegrabapp.inc"

#define FreeXVideosDownload     "FreeXVideosDownload"
#define AppName                 "FreeXVideosDownload"
#define AppHumanName            "Free XVideos Download"

[Setup]

WizardImageFile         = installer_files\freexvideosdownload.bmp

[Files]

Source: installer_files\xvideos-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;