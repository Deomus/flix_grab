#include "company\freegrabapp.inc"

#define FreeHBODownload         "FreeHBODownload"
#define AppName                 "FreeHBODownload"
#define AppHumanName            "Free HBO Download"

[Setup]

WizardImageFile                 = installer_files\freehbodownload.bmp

[Files]

Source: installer_files\hbo-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;