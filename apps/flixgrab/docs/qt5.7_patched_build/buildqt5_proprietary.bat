call ./configure  -prefix "%MY_ROOT%\build" -shared -release -opensource -confirm-license -openssl -nomake examples -nomake tests
call cd ./qtwebengine
call qmake WEBENGINE_CONFIG+=use_proprietary_codecs
call cd ../
call nmake
call nmake install

