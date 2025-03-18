#include "company\freegrabapp.inc"

#define FlixGrab          "FlixGrab"
#define AppName           "FlixGrab"
#define AppHumanName      "FlixGrab"

[Setup]

WizardImageFile         = installer_files\flixgrab.bmp

[Files]

Source: installer_files\flixgrab-win10.ico;  DestDir: {#destPath}\{#AppHumanName};  Flags: ignoreversion;