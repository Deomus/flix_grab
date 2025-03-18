::compile settings
set "build_binaries=true"
set "copy_binaries=true"
set "sign_binaries=true"
set "compile_intaller=true"
set "beta_intaller=false"


::build settings
set "apps=FreeYouTubeDownload"
::build_type:rebuild,build
::build_mode:release,debug
::build_platform:Win32,x64
set "build_type=rebuild"
set "build_mode=Release"
set "build_platform=Win32"
set "cert_subject=TECHNOLOGY LLC"


::environment settings
set "SoftPath=%~dp0"
set "BinDir=%SoftPath%\..\..\builds"
set "LogDir=%BinDir%\logs"
set "InstDir=%SoftPath%\..\..\install"
if not defined InnoDir set "InnoDir=C:\Program Files (x86)\Inno Setup 6"
if not defined VSToolsDir set "VSToolsDir=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools"


::code
call "%VSToolsDir%\vsdevcmd.bat"
call :parse "%apps%"
goto :end


:parse
setlocal
set list=%1

for /f "delims=, tokens=1*" %%f in (%list%) do (
    rem if the item exist
    if not "%%f" == "" call :getLineNumber %%f
    rem if next item exist
    if not "%%g" == "" call :parse "%%g"
)
endlocal
goto :end


:getLineNumber
setlocal
::delete old build log file start
set "LogFile=%LogDir%\%1_build.log"
if exist "%LogFile%" del /F /Q "%LogFile%"
::delete old build log file end

:buildbinaries
if %build_binaries% == false (
    goto :copybinaries 
)

::build visual studio solution start
if not exist %LogDir% mkdir %LogDir%
devenv.exe "%SoftPath%\msvc\%1.sln" /%build_type% "%build_mode%|%build_platform%" /out "%LogFile%"
if errorlevel 1 (
   color c
   echo.
   echo end %date% - %time%
   echo Failed! View log file %LogFile% if you prefer.
   color
)
::build visual studio solution end

:copybinaries
set "LogFileInst=%LogDir%\%1_inst.log"
if %copy_binaries% == false (
    goto :signbinaries 
)

::delete old inst log file start
if exist "%LogFileInst%" del /F /Q "%LogFileInst%"
::delete old inst log file end

rmdir /s /q "%InstDir%\products\%1"
xcopy /Y /E "%BinDir%\%build_mode%\%1" "%InstDir%\products\%1\"


:signbinaries
if %sign_binaries% == false (
    goto :compileinst 
)
if exist "%InstDir%\products\%1" (
    @echo off
    for %%f in (%InstDir%\products\%1\*.exe) do (
        signtool.exe verify /pa /q "%%f" && (
            echo File %%f is already signed, skipping
        ) || (
            signtool.exe sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /td sha256 /fd sha256 /n "%cert_subject%" "%%f"
        )
    )
    for %%f in (%InstDir%\products\%1\*.dll) do (
        signtool.exe verify /pa /q "%%f" && (
            echo File %%f is already signed, skipping
        ) || (
            signtool.exe sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /td sha256 /fd sha256 /n "%cert_subject%" "%%f"
        )
    )
)


:compileinst
if %compile_intaller% == false (
    goto :noinst 
)
set "inst_temp_file=%InstDir%\temp.inc"
if not exist %LogDir% mkdir %LogDir%
if exist "%inst_temp_file%" del /F /Q "%inst_temp_file%"
(echo #include "%1.inc") >> "%inst_temp_file%"
if %beta_intaller% == true (
(echo #define Beta) >> "%inst_temp_file%"
)
"%InnoDir%\ISCC.exe" /Q /S"signtool=signtool.exe sign /tr http://timestamp.globalsign.com/tsa/r6advanced1 /td sha256 /fd sha256 /n $q%cert_subject%$q $f" "%InstDir%\buildapp.iss" 2>> "%LogFileInst%"


:noinst

endlocal 
:end
