#include "company\freegrabapp.inc"

#define FreeHuluDownload            "FreeHuluDownload"
#define AppName                     "FreeHuluDownload"
#define AppHumanName                "Free Hulu Download"

[Setup]

WizardImageFile         = installer_files\freehuludownload.bmp

[Files]

Source: installer_files\hulu-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;