# Зависимости
- [Visual Studio 2022 Community](https://visualstudio.microsoft.com/ru/vs/community/)
	- Разработка классических приложений на C++
	- MSVC версии 142 - средства сборки C++ для VS 2019 для x64 или x86
	- MSVC версии 143 - средства сборки C++ для VS 2022 для x64 или x86
	- Пакет SDK для Windows 10 (10.0.19041.0)
	- Пакет SDK для .NET Framework 4.8
	- ATL-библиотека C++ версии 14.29 (16.11) для средств сборки версии 142 (x86 и x64)
- [Qt Visual Studio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2022)
- [Qt 5.15.2](https://www.qt.io/download-qt-installer-oss)
	- MSVC 2019 32-bit
	- Qt WebEngine
- [Inno Setup 6](https://jrsoftware.org/isdl.php)

# Настройки
В Visual Studio указать путь к установленному Qt. Для этого выбрать меню *Расширения*→*Qt VS Tools*→*Qt Vesions*. В открывшемся окне нажать *Browse*.

В файле **apps\scripts\BuildAllSlns.cmd** указать:
- Желаемые этапы сборки (в разделе *compile settings*)
- Желаемые приложения (переменная *apps*)
- Субъект сертификата цифровой подписи (переменная *cert_subject*). Сертификат должен быть установлен в хранилище пользователя.
- Тип, режим сборки и платформу (переменные *build_type*, *build_mode*, *build_platform*)
- Пути к установленным Inno Setup и Visual Studio  (переменные *InnoDir* и *VSToolsDir*). Эти переменные могут быть установлены в настройках переменных среды пользователя.

# Версии приложений
Установить желаемые версии приложений можно в файлах **version.h** в **apps\config\[AppName]**.

# Сборка
Для сборки запустить **BuildAllSlns.cmd**. Собранные приложения появятся в папке **builds**, а установщики - в папке **install\release**. При наличии ошибок проверьте журналы сборки в **builds\logs**.
