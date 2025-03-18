#include "company\freegrabapp.inc"

#define FreeFacebookVideoDownload       "FreeFacebookVideoDownload"
#define AppName                         "FreeFacebookVideoDownload"
#define AppHumanName                    "Free Facebook Video Download"

[Setup]

WizardImageFile         = installer_files\freefacebookvideodownload.bmp

[Files]

Source: installer_files\facebook-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;