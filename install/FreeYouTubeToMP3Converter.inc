#include "company\freegrabapp.inc"

#define FreeYouTubeToMP3Converter       "FreeYouTubeToMP3Converter"
#define AppName                         "FreeYouTubeToMP3Converter"
#define AppHumanName                    "Free YouTube to MP3 Converter"

[Setup]

WizardImageFile         = installer_files\freeyoutubetomp3converter.bmp

[Files]

Source: installer_files\youtubetomp3-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;