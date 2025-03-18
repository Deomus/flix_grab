#include "company\freegrabapp.inc"

#define FlixGrabMusic     "FlixGrabMusic"
#define AppName           "FlixGrabMusic"
#define AppHumanName      "FlixGrabMusic"

[Setup]

WizardImageFile         = installer_files\flixgrabmusic.bmp

[Files]

Source: installer_files\flixgrabmusic-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;