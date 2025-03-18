#include "company\freegrabapp.inc"

#define FreeAmazonPrimeDownload     "FreeAmazonPrimeDownload"
#define AppName                     "FreeAmazonPrimeDownload"
#define AppHumanName                "Free Amazon Prime Download"

[Setup]

WizardImageFile         = installer_files\freeamazonprimedownload.bmp

[Files]

Source: installer_files\amazon-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;