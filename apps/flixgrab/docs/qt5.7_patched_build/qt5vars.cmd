
CALL "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
SET MY_ROOT=D:\qt_build\qt-everywhere-opensource-src-5.7.0
SET PATH=%MY_ROOT%\qtbase\bin;%MY_ROOT%\gnuwin32\bin;%PATH%
REM Uncomment the below line when using a git checkout of the source repository
REM SET PATH=%_ROOT%\qtrepotools\bin;%PATH%
SET QMAKESPEC=win32-msvc2013
REM When compiling with ICU, uncomment the lines below and change <icupath> appropriately:
SET INCLUDE=c:\icu_53_1_msvc_2013_32\include;%INCLUDE%
SET LIB=c:\icu_53_1_msvc_2013_32\lib;%LIB%
SET PATH=c:\icu_53_1_msvc_2013_32;%PATH%
REM SET PATH=<icupath>\lib;%PATH%

set PATH=%PATH%;%MY_ROOT%\..\openssl-1.0.2g\bin
set INCLUDE=%MY_ROOT%\..\openssl-1.0.2g\include;%INCLUDE%

REM Python 
SET PATH=C:\Python27\Lib;C:\Python27\DLLs;C:\Python27\Lib\lib-tk;%PATH%

REM Ruby
REM SET PATH=c:\Ruby23\bin;%PATH%

echo Path:
echo %PATH%