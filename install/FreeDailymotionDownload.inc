#include "company\freegrabapp.inc"

#define FreeDailymotionDownload     "FreeDailymotionDownload"
#define AppName                     "FreeDailymotionDownload"
#define AppHumanName                "Free Dailymotion Download"

[Setup]

WizardImageFile         = installer_files\freedailymotiondownload.bmp

[Files]

Source: installer_files\dailymotion-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;