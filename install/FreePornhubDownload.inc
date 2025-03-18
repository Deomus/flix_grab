#include "company\freegrabapp.inc"

#define FreePornhubDownload     "FreePornhubDownload"
#define AppName                 "FreePornhubDownload"
#define AppHumanName            "Free Pornhub Download"

[Setup]

WizardImageFile         = installer_files\freepornhubdownload.bmp

[Files]

Source: installer_files\pornhub-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;