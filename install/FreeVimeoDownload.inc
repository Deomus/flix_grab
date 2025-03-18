#include "company\freegrabapp.inc"

#define FreeVimeoDownload           "FreeVimeoDownload"
#define AppName                     "FreeVimeoDownload"
#define AppHumanName                "Free Vimeo Download"

[Setup]

WizardImageFile         = installer_files\freevimeodownload.bmp

[Files]

Source: installer_files\vimeo-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;