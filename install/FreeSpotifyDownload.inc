#include "company\freegrabapp.inc"

#define FreeSpotifyDownload     "FreeSpotifyDownload"
#define AppName                 "FreeSpotifyDownload"
#define AppHumanName            "Free Spotify Download"

[Setup]

WizardImageFile         = installer_files\freespotifydownload.bmp

[Files]

Source: installer_files\spotify-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;