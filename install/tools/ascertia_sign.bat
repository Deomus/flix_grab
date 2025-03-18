set signCmdLine="c:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\signtool.exe" sign /v /f "..\installer_files\ascertia_freegrabapp.pfx" /p Qweszxc45 /du "https://www.freegrabapp.com/" /fd sha256 /tr http://sha256timestamp.ws.symantec.com/sha256/timestamp

%signCmdLine% ..\release\FreeYouTubeToMP3Converter.exe

pause
cls
