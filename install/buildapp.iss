#define destPath "{app}"

#include "temp.inc"
#include "commoncode\commonfunc.inc"

#define VerString GetFileVersion('products\' + AppName + '\' + AppName + '.exe')


[Setup]

AppID                   = {#AppHumanName}

#ifdef Beta
    AppName                 = {#AppHumanName} Beta
    AppVerName              = {#AppHumanName} Beta
#else
    AppName                 = {#AppHumanName}
    AppVerName              = {#AppHumanName}
#endif

AppVersion              = {#VerString}
VersionInfoVersion      = {#VerString}
AppMutex                = {#CompanyName},Global\{#CompanyName}
AppUpdatesURL           = {#UpdatesURL}/{#AppName}.exe
DefaultGroupName        = {#CompanyName}
DefaultDirName          = {pf}\{#CompanyName}
OutputBaseFilename      = {#AppName}
UninstallDisplayIcon    = {#destPath}\{#AppHumanName}\{#AppName}.exe

#ifdef FreeAppleMusicDownload
    WizardSmallImageFile    = installer_files\logo-install-applem.bmp
    SetupIconFile           = installer_files\applem.ico
#endif
#ifdef FreeAmazonPrimeDownload
    WizardSmallImageFile    = installer_files\logo-install-amazon.bmp
    SetupIconFile           = installer_files\amazon.ico
#endif
#ifdef FreeAmazonMusicDownload
    WizardSmallImageFile    = installer_files\logo-install-amazonmusic.bmp
    SetupIconFile           = installer_files\amazonmusic.ico
#endif 
#ifdef FreeHuluDownload
    WizardSmallImageFile    = installer_files\logo-install-hulu.bmp
    SetupIconFile           = installer_files\hulu.ico
#endif
#ifdef FlixGrab
    WizardSmallImageFile    = installer_files\logo-install-flixgrab.bmp
    SetupIconFile           = installer_files\flixgrab.ico
#endif
#ifdef FlixGrabMusic
    WizardSmallImageFile    = installer_files\logo-install-flixgrabmusic.bmp
    SetupIconFile           = installer_files\flixgrabmusic.ico
#endif
#ifdef FreeNetflixDownload
    WizardSmallImageFile    = installer_files\logo-install-netflix.bmp
    SetupIconFile           = installer_files\netflix.ico
#endif
#ifdef NetflixDownloader
    WizardSmallImageFile    = installer_files\logo-install-netflix-downloader.bmp
    SetupIconFile           = installer_files\netflix-downloader.ico
#endif
#ifdef FreeDisneyPlusDownload
    WizardSmallImageFile    = installer_files\logo-install-disney.bmp
    SetupIconFile           = installer_files\disney.ico
#endif
#ifdef FreeHBODownload
    WizardSmallImageFile    = installer_files\logo-install-hbo.bmp
    SetupIconFile           = installer_files\hbo.ico
#endif
#ifdef FreeYouTubeDownload
    WizardSmallImageFile    = installer_files\logo-install-youtube.bmp
    SetupIconFile           = installer_files\youtube.ico
#endif
#ifdef FreeYouTubeToMP3Converter
    WizardSmallImageFile    = installer_files\logo-install-youtubetomp3.bmp
    SetupIconFile           = installer_files\youtubetomp3.ico
#endif
#ifdef FreeSpotifyDownload
    WizardSmallImageFile    = installer_files\logo-install-spotify.bmp
    SetupIconFile           = installer_files\spotify.ico
#endif
#ifdef FreeXVideosDownload
    WizardSmallImageFile    = installer_files\logo-install-xvideos.bmp
    SetupIconFile           = installer_files\xvideos.ico
#endif
#ifdef FreeFacebookVideoDownload
    WizardSmallImageFile    = installer_files\logo-install-facebook.bmp
    SetupIconFile           = installer_files\facebook.ico
#endif
#ifdef FreeInstagramDownload
    WizardSmallImageFile    = installer_files\logo-install-instagram.bmp
    SetupIconFile           = installer_files\instagram.ico
#endif
#ifdef FreeVimeoDownload
    WizardSmallImageFile    = installer_files\logo-install-vimeo.bmp
    SetupIconFile           = installer_files\vimeo.ico
#endif
#ifdef FreeDailymotionDownload
    WizardSmallImageFile    = installer_files\logo-install-dailymotion.bmp
    SetupIconFile           = installer_files\dailymotion.ico
#endif
#ifdef FreePornhubDownload
    WizardSmallImageFile    = installer_files\logo-install-pornhub.bmp
    SetupIconFile           = installer_files\pornhub.ico
#endif
#ifdef FreeTwitchDownload
    WizardSmallImageFile    = installer_files\logo-install-twitch.bmp
    SetupIconFile           = installer_files\twitch.ico
#endif
#ifdef FreeTwitterDownload
    WizardSmallImageFile    = installer_files\logo-install-twitter.bmp
    SetupIconFile           = installer_files\twitter.ico
#endif

//WizardSmallImageFile    = installer_files\installsmall.bmp
//SetupIconFile           = installer_files\insticon.ico
//MinVersion              =  0,5.01sp3

SignTool                = signtool
MinVersion              = 6.1.7601
UsePreviousGroup        = no
UsePreviousAppDir       = no
AllowNoIcons            = yes
Compression             = lzma2/ultra64
SolidCompression        = yes
DisableProgramGroupPage = yes
PrivilegesRequired      = admin
SetupLogging            = yes
CompressionThreads      = 4
DirExistsWarning        = no
CloseApplications       = no
AlwaysRestart           = no
DisableWelcomePage      = no
DisableReadyPage        = yes
LanguageDetectionMethod = uilanguage
OutputDir               = release

#include "commoncode/basicfunc.inc"