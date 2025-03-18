call  %QTDIR%\bin\lrelease.exe language_da.ts
call  %QTDIR%\bin\lrelease.exe language_de.ts
call  %QTDIR%\bin\lrelease.exe language_fr.ts
call  %QTDIR%\bin\lrelease.exe language_pt.ts
call  %QTDIR%\bin\lrelease.exe language_it.ts
call  %QTDIR%\bin\lrelease.exe language_es.ts
call  %QTDIR%\bin\lrelease.exe language_hi.ts
call  %QTDIR%\bin\lrelease.exe language_ja.ts
call  %QTDIR%\bin\lrelease.exe language_ko.ts
call  %QTDIR%\bin\lrelease.exe language_pl.ts
call  %QTDIR%\bin\lrelease.exe language_nl.ts
call  %QTDIR%\bin\lrelease.exe language_ru.ts
call  %QTDIR%\bin\lrelease.exe language_cn.ts
call  %QTDIR%\bin\lrelease.exe language_tw.ts
call  %QTDIR%\bin\lrelease.exe language_cs.ts
call  %QTDIR%\bin\lrelease.exe language_tr.ts
call  %QTDIR%\bin\lrelease.exe language_sv.ts

call  %QTDIR%\bin\rcc.exe -name resources ..\resources\resources.qrc -o ..\win32\GeneratedFiles\qrc_resources.cpp
pause
cls