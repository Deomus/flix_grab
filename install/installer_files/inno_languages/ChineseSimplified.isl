; *** Inno Setup version 6.1.0+ Chinese Simplified messages ***
;
; To download user-contributed translations of this file, go to:
;   https://jrsoftware.org/files/istrans/
;
; Note: When translating this text, do not add periods (.) to the end of
; messages that didn't have them already, because on those messages Inno
; Setup adds the periods automatically (appending a period would result in
; two periods being displayed).
;
; Maintained by Zhenghan Yang
; Email: 847320916@QQ.com
; Translation based on network resource
; The latest Translation is on https://github.com/kira-96/Inno-Setup-Chinese-Simplified-Translation
;

[LangOptions]
; The following three entries are very important. Be sure to read and 
; understand the '[LangOptions] section' topic in the help file.
LanguageName=���ļ���
; If Language Name display incorrect, uncomment next line
; LanguageName=<7B80><4F53><4E2D><6587>
; About LanguageID, to reference link:
; https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-lcid/a9eac961-e77d-41a6-90a5-ce1a8b0cdb9c
LanguageID=$0804
; About CodePage, to reference link:
; https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
LanguageCodePage=936
; If the language you are translating to requires special font faces or
; sizes, uncomment any of the following entries and change them accordingly.
;DialogFontName=
;DialogFontSize=8
;WelcomeFontName=Verdana
;WelcomeFontSize=12
;TitleFontName=Arial
;TitleFontSize=29
;CopyrightFontName=Arial
;CopyrightFontSize=8

[Messages]

; *** 应用程序标题
SetupAppTitle=安装
SetupWindowTitle=安装 - %1
UninstallAppTitle=卸载
UninstallAppFullTitle=%1 卸载

; *** Misc. common
InformationTitle=信息
ConfirmTitle=确认
ErrorTitle=错误

; *** SetupLdr messages
SetupLdrStartupMessage=现在将安�?%1。您想要继续吗？
LdrCannotCreateTemp=无法创建临时文件。安装程序已中止
LdrCannotExecTemp=无法执行临时目录中的文件。安装程序已中止
HelpTextNote=

; *** 启动错误消息
LastErrorMessage=%1�?n%n错误 %2: %3
SetupFileMissing=安装目录中缺少文�?%1。请修正这个问题或者获取程序的新副本�?
SetupFileCorrupt=安装文件已损坏。请获取程序的新副本�?
SetupFileCorruptOrWrongVer=安装文件已损坏，或是与这个安装程序的版本不兼容。请修正这个问题或获取新的程序副本�?
InvalidParameter=无效的命令行参数�?n%n%1
SetupAlreadyRunning=安装程序正在运行�?
WindowsVersionNotSupported=这个程序不支持当前计算机运行的Windows版本�?
WindowsServicePackRequired=这个程序需�?%1 服务�?%2 或更高�?
NotOnThisPlatform=这个程序将不能运行于 %1�?
OnlyOnThisPlatform=这个程序必须运行�?%1�?
OnlyOnTheseArchitectures=这个程序只能在为下列处理器结构设计的Windows版本中进行安装：%n%n%1
WinVersionTooLowError=这个程序需�?%1 版本 %2 或更高�?
WinVersionTooHighError=这个程序不能安装�?%1 版本 %2 或更高�?
AdminPrivilegesRequired=在安装这个程序时您必须以管理员身份登录�?
PowerUserPrivilegesRequired=在安装这个程序时您必须以管理员身份或有权限的用户组身份登录�?
SetupAppRunningError=安装程序发现 %1 当前正在运行�?n%n请先关闭正在运行的程序，然后点击“确定”继续，或点击“取消”退出�?
UninstallAppRunningError=卸载程序发现 %1 当前正在运行�?n%n请先关闭正在运行的程序，然后点击“确定”继续，或点击“取消”退出�?

; *** 启动问题
PrivilegesRequiredOverrideTitle=选择安装程序模式
PrivilegesRequiredOverrideInstruction=选择安装模式
PrivilegesRequiredOverrideText1=%1 可以为所有用户安�?需要管理员权限)，或仅为您安装�?
PrivilegesRequiredOverrideText2=%1 只能为您安装，或为所有用户安�?需要管理员权限)�?
PrivilegesRequiredOverrideAllUsers=为所有用户安�?&A)
PrivilegesRequiredOverrideAllUsersRecommended=为所有用户安�?&A) (建议选项)
PrivilegesRequiredOverrideCurrentUser=只为我安�?&M)
PrivilegesRequiredOverrideCurrentUserRecommended=只为我安�?&M) (建议选项)

; *** 其它错误
ErrorCreatingDir=安装程序不能创建目录�?1”�?
ErrorTooManyFilesInDir=无法在目录�?1”中创建文件，因为里面包含太多文�?

; *** 安装程序公共消息
ExitSetupTitle=退出安装程�?
ExitSetupMessage=安装程序还未完成安装。如果您现在退出，程序将不能安装�?n%n您可以以后再运行安装程序完成安装�?n%n现在退出安装程序吗�?
AboutSetupMenuItem=关于安装程序(&A)...
AboutSetupTitle=关于安装程序
AboutSetupMessage=%1 版本 %2%n%3%n%n%1 主页�?n%4
AboutSetupNote=
TranslatorNote=

; *** 按钮
ButtonBack=< 上一�?&B)
ButtonNext=下一�?&N) >
ButtonInstall=安装(&I)
ButtonOK=确定
ButtonCancel=取消
ButtonYes=�?&Y)
ButtonYesToAll=全是(&A)
ButtonNo=�?&N)
ButtonNoToAll=全否(&O)
ButtonFinish=完成(&F)
ButtonBrowse=浏览(&B)...
ButtonWizardBrowse=浏览(&R)...
ButtonNewFolder=新建文件�?&M)

; *** “选择语言”对话框消息
SelectLanguageTitle=选择安装语言
SelectLanguageLabel=选择安装时使用的语言�?

; *** 公共向导文字
ClickNext=点击“下一步”继续，或点击“取消”退出安装程序�?
BeveledLabel=
BrowseDialogTitle=浏览文件�?
BrowseDialogLabel=在下面的列表中选择一个文件夹，然后点击“确定”�?
NewFolderName=新建文件�?

; *** “欢迎”向导页
WelcomeLabel1=欢迎使用 [name] 安装向导
WelcomeLabel2=ͨ����� "��һ��"����ͬ�������û�����Э�����˽���ߵ������������

; *** “密码”向导页
WizardPassword=密码
PasswordLabel1=这个安装程序有密码保护�?
PasswordLabel3=请输入密码，然后点击“下一步”继续。密码区分大小写�?
PasswordEditLabel=密码(&P)�?
IncorrectPassword=您所输入的密码不正确，请重新输入�?

; *** “许可协议”向导页
WizardLicense=许可协议
LicenseLabel=请在继续安装前阅读以下重要信息�?
LicenseLabel3=请仔细阅读下列许可协议。在继续安装前您必须同意这些协议条款�?
LicenseAccepted=我同意此协议(&A)
LicenseNotAccepted=我不同意此协�?&D)

; *** “信息”向导页
WizardInfoBefore=信息
InfoBeforeLabel=请在继续安装前阅读以下重要信息�?
InfoBeforeClickLabel=准备好继续安装后，点击“下一步”�?
WizardInfoAfter=信息
InfoAfterLabel=请在继续安装前阅读以下重要信息�?
InfoAfterClickLabel=准备好继续安装后，点击“下一步”�?

; *** “用户信息”向导页
WizardUserInfo=用户信息
UserInfoDesc=请输入您的信息�?
UserInfoName=用户�?&U)�?
UserInfoOrg=组织(&O)�?
UserInfoSerial=序列�?&S)�?
UserInfoNameRequired=您必须输入用户名�?

; *** “选择目标目录”向导页
WizardSelectDir=选择目标位置
SelectDirDesc=您想�?[name] 安装在哪里？
SelectDirLabel3=安装程序将安�?[name] 到下面的文件夹中�?
SelectDirBrowseLabel=点击“下一步”继续。如果您想选择其它文件夹，点击“浏览”�?
DiskSpaceGBLabel=至少需要有 [gb] GB 的可用磁盘空间�?
DiskSpaceMBLabel=至少需要有 [mb] MB 的可用磁盘空间�?
CannotInstallToNetworkDrive=安装程序无法安装到一个网络驱动器�?
CannotInstallToUNCPath=安装程序无法安装到一个UNC路径�?
InvalidPath=您必须输入一个带驱动器卷标的完整路径，例如：%n%nC:\APP%n%n或UNC路径�?n%n\\server\share
InvalidDrive=您选定的驱动器�?UNC 共享不存在或不能访问。请选择其它位置�?
DiskSpaceWarningTitle=磁盘空间不足
DiskSpaceWarning=安装程序至少需�?%1 KB 的可用空间才能安装，但选定驱动器只�?%2 KB 的可用空间�?n%n您一定要继续吗？
DirNameTooLong=文件夹名称或路径太长�?
InvalidDirName=文件夹名称无效�?
BadDirName32=文件夹名称不能包含下列任何字符：%n%n%1
DirExistsTitle=文件夹已存在
DirExists=文件夹：%n%n%1%n%n已经存在。您一定要安装到这个文件夹中吗�?
DirDoesntExistTitle=文件夹不存在
DirDoesntExist=文件夹：%n%n%1%n%n不存在。您想要创建此文件夹吗？

; *** “选择组件”向导页
WizardSelectComponents=选择组件
SelectComponentsDesc=您想安装哪些程序的组件？
SelectComponentsLabel2=选择您想要安装的组件；清除您不想安装的组件。然后点击“下一步”继续�?
FullInstallation=完全安装
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=简洁安�?
CustomInstallation=自定义安�?
NoUninstallWarningTitle=组件已存�?
NoUninstallWarning=安装程序检测到下列组件已在您的电脑中安装：%n%n%1%n%n取消选定这些组件将不能卸载它们�?n%n您一定要继续吗？
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceGBLabel=当前选择的组件至少需�?[gb] GB 的磁盘空间�?
ComponentsDiskSpaceMBLabel=当前选择的组件至少需�?[mb] MB 的磁盘空间�?

; *** “选择附加任务”向导页
WizardSelectTasks=选择附加任务
SelectTasksDesc=您想要安装程序执行哪些附加任务？
SelectTasksLabel2=选择您想要安装程序在安装 [name] 时执行的附加任务，然后点击“下一步”�?

; *** “选择开始菜单文件夹”向导页
WizardSelectProgramGroup=选择开始菜单文件夹
SelectStartMenuFolderDesc=安装程序应该在哪里放置程序的快捷方式�?
SelectStartMenuFolderLabel3=安装程序将在下列开始菜单文件夹中创建程序的快捷方式�?
SelectStartMenuFolderBrowseLabel=点击“下一步”继续。如果您想选择其它文件夹，点击“浏览”�?
MustEnterGroupName=您必须输入一个文件夹名�?
GroupNameTooLong=文件夹名或路径太长�?
InvalidGroupName=无效的文件夹名字�?
BadGroupName=文件夹名不能包含下列任何字符�?n%n%1
NoProgramGroupCheck2=不创建开始菜单文件夹(&D)

; *** “准备安装”向导页
WizardReady=准备安装
ReadyLabel1=安装程序准备好了，现在可以开始安�?[name] 到您的电脑中�?
ReadyLabel2a=点击“安装”继续此安装程序。如果您想要重新考虑或修改任何设置，请点击“上一步”�?
ReadyLabel2b=点击“安装”继续此安装程序�?
ReadyMemoUserInfo=用户信息�?
ReadyMemoDir=目标位置�?
ReadyMemoType=安装类型�?
ReadyMemoComponents=选定组件�?
ReadyMemoGroup=开始菜单文件夹�?
ReadyMemoTasks=附加任务�?

; *** TDownloadWizardPage wizard page and DownloadTemporaryFile
DownloadingLabel=正在下载附加文件...
ButtonStopDownload=停止下载(&S)
StopDownload=您确定要停止下载吗？
ErrorDownloadAborted=下载已中�?
ErrorDownloadFailed=下载失败�?1 %2
ErrorDownloadSizeFailed=获取下载大小失败�?1 %2
ErrorFileHash1=校验文件哈希失败�?1
ErrorFileHash2=无效的文件哈希：预期 %1，实�?%2
ErrorProgress=无效的进度：%1，总共%2
ErrorFileSize=文件大小错误：预�?%1，实�?%2

; *** “正在准备安装”向导页
WizardPreparing=正在准备安装
PreparingDesc=安装程序正在准备安装 [name] 到您的电脑中�?
PreviousInstallNotCompleted=先前程序的安�?卸载未完成。您需要重新启动您的电脑才能完成安装�?n%n在重新启动电脑后，再运行安装完成 [name] 的安装�?
CannotContinue=安装程序不能继续。请点击“取消”退出�?
ApplicationsFound=以下应用程序正在使用需要由安装程序更新的文件。建议您允许安装程序自动关闭这些应用程序�?
ApplicationsFound2=以下应用程序正在使用需要由安装程序更新的文件。建议您允许安装程序自动关闭这些应用程序。安装完成后，安装程序将尝试重新启动这些应用程序�?
CloseApplications=自动关闭该应用程�?&A)
DontCloseApplications=不要关闭该应用程�?&D)
ErrorCloseApplications=安装程序无法自动关闭所有应用程序。建议您在继续之前关闭所有使用需要由安装程序更新的文件的应用程序�?
PrepareToInstallNeedsRestart=安装程序必须重新启动计算机。重新启动计算机后，请再次运行安装程序以完成 [name] 的安装�?n%n是否立即重新启动�?

; *** “正在安装”向导页
WizardInstalling=正在安装
InstallingLabel=安装程序正在安装 [name] 到您的电脑中，请稍等�?

; *** “安装完成”向导页
FinishedHeadingLabel=[name] 安装完成
FinishedLabelNoIcons=安装程序已在您的电脑中安装了 [name]�?
FinishedLabel=安装程序已在您的电脑中安装了 [name]。此应用程序可以通过选择安装的快捷方式运行�?
ClickFinish=点击“完成”退出安装程序�?
FinishedRestartLabel=要完�?[name] 的安装，安装程序必须重新启动您的电脑。您想要立即重新启动吗？
FinishedRestartMessage=要完�?[name] 的安装，安装程序必须重新启动您的电脑�?n%n您想要立即重新启动吗�?
ShowReadmeCheck=是，我想查阅自述文件
YesRadio=是，立即重新启动电脑(&Y)
NoRadio=否，稍后重新启动电脑(&N)
; used for example as 'Run MyProg.exe'
RunEntryExec=运行 %1
; used for example as 'View Readme.txt'
RunEntryShellExec=查阅 %1

; *** “安装程序需要下一张磁盘”提�?
ChangeDiskTitle=安装程序需要下一张磁�?
SelectDiskLabel2=请插入磁�?%1 并点击“确定”�?n%n如果这个磁盘中的文件可以在下列文件夹之外的文件夹中找到，请输入正确的路径或点击“浏览”�?
PathLabel=路径(&P)�?
FileNotInDir2=文件�?1”不能在�?2”定位。请插入正确的磁盘或选择其它文件夹�?
SelectDirectoryLabel=请指定下一张磁盘的位置�?

; *** 安装状态消�?
SetupAborted=安装程序未完成安装�?n%n请修正这个问题并重新运行安装程序�?
AbortRetryIgnoreSelectAction=选择操作
AbortRetryIgnoreRetry=重试(&T)
AbortRetryIgnoreIgnore=忽略错误并继�?&I)
AbortRetryIgnoreCancel=关闭安装程序

; *** 安装状态消�?
StatusClosingApplications=正在关闭应用程序...
StatusCreateDirs=正在创建目录...
StatusExtractFiles=正在解压缩文�?..
StatusCreateIcons=正在创建快捷方式...
StatusCreateIniEntries=正在创建 INI 条目...
StatusCreateRegistryEntries=正在创建注册表条�?..
StatusRegisterFiles=正在注册文件...
StatusSavingUninstall=正在保存卸载信息...
StatusRunProgram=正在完成安装...
StatusRestartingApplications=正在重启应用程序...
StatusRollback=正在撤销更改...

; *** 其它错误
ErrorInternal2=内部错误�?1
ErrorFunctionFailedNoCode=%1 失败
ErrorFunctionFailed=%1 失败；错误代�?%2
ErrorFunctionFailedWithMessage=%1 失败；错误代�?%2.%n%3
ErrorExecutingProgram=不能执行文件�?n%1

; *** 注册表错�?
ErrorRegOpenKey=打开注册表项时出错：%n%1\%2
ErrorRegCreateKey=创建注册表项时出错：%n%1\%2
ErrorRegWriteKey=写入注册表项时出错：%n%1\%2

; *** INI 错误
ErrorIniEntry=在文件�?1”中创建INI条目时出错�?

; *** 文件复制错误
FileAbortRetryIgnoreSkipNotRecommended=跳过这个文件(&S) (不推�?
FileAbortRetryIgnoreIgnoreNotRecommended=忽略错误并继�?&I) (不推�?
SourceIsCorrupted=源文件已损坏
SourceDoesntExist=源文件�?1”不存在
ExistingFileReadOnly2=无法替换现有文件，因为它是只读的�?
ExistingFileReadOnlyRetry=移除只读属性并重试(&R)
ExistingFileReadOnlyKeepExisting=保留现有文件(&K)
ErrorReadingExistingDest=尝试读取现有文件时出错：
FileExistsSelectAction=选择操作
FileExists2=文件已经存在�?
FileExistsOverwriteExisting=覆盖已经存在的文�?&O)
FileExistsKeepExisting=保留现有的文�?&K)
FileExistsOverwriteOrKeepAll=为所有的冲突文件执行此操�?&D)
ExistingFileNewerSelectAction=选择操作
ExistingFileNewer2=现有的文件比安装程序将要安装的文件更新�?
ExistingFileNewerOverwriteExisting=覆盖已经存在的文�?&O)
ExistingFileNewerKeepExisting=保留现有的文�?&K) (推荐)
ExistingFileNewerOverwriteOrKeepAll=为所有的冲突文件执行此操�?&D)
ErrorChangingAttr=尝试改变下列现有的文件的属性时出错�?
ErrorCreatingTemp=尝试在目标目录创建文件时出错�?
ErrorReadingSource=尝试读取下列源文件时出错�?
ErrorCopying=尝试复制下列文件时出错：
ErrorReplacingExistingFile=尝试替换现有的文件时出错�?
ErrorRestartReplace=重新启动替换失败�?
ErrorRenamingTemp=尝试重新命名以下目标目录中的一个文件时出错�?
ErrorRegisterServer=无法注册 DLL/OCX�?1
ErrorRegSvr32Failed=RegSvr32 失败；退出代�?%1
ErrorRegisterTypeLib=无法注册类型库：%1

; *** 卸载显示名字标记
; used for example as 'My Program (32-bit)'
UninstallDisplayNameMark=%1 (%2)
; used for example as 'My Program (32-bit, All users)'
UninstallDisplayNameMarks=%1 (%2, %3)
UninstallDisplayNameMark32Bit=32�?
UninstallDisplayNameMark64Bit=64�?
UninstallDisplayNameMarkAllUsers=所有用�?
UninstallDisplayNameMarkCurrentUser=当前用户

; *** 安装后错�?
ErrorOpeningReadme=尝试打开自述文件时出错�?
ErrorRestartingComputer=安装程序不能重新启动电脑，请手动重启�?

; *** 卸载消息
UninstallNotFound=文件�?1”不存在。无法卸载�?
UninstallOpenError=文件�?1”不能打开。无法卸载�?
UninstallUnsupportedVer=此版本的卸载程序无法识别卸载日志文件�?1”的格式。无法卸�?
UninstallUnknownEntry=在卸载日志中遇到一个未知的条目 (%1)
ConfirmUninstall=您确认想要完全删�?%1 及其所有组件吗�?
UninstallOnlyOnWin64=这个安装程序只能�?4位Windows中进行卸载�?
OnlyAdminCanUninstall=这个安装的程序需要有管理员权限的用户才能卸载�?
UninstallStatusLabel=正在从您的电脑中删除 %1，请稍等�?
UninstalledAll=%1 已顺利地从您的电脑中删除�?
UninstalledMost=%1 卸载完成�?n%n有一些内容无法被删除。您可以手动删除它们�?
UninstalledAndNeedsRestart=要完�?%1 的卸载，您的电脑必须重新启动�?n%n您想立即重新启动电脑吗？
UninstallDataCorrupted=文件�?1”已损坏，无法卸�?

; *** 卸载状态消�?
ConfirmDeleteSharedFileTitle=删除共享文件吗？
ConfirmDeleteSharedFile2=系统中包含的下列共享文件已经不再被其它程序使用。您想要卸载程序删除这些共享文件吗？%n%n如果这些文件被删除，但还有程序正在使用这些文件，这些程序可能不能正确执行。如果您不能确定，选择“否”。把这些文件保留在系统中以免引起问题�?
SharedFileNameLabel=文件名：
SharedFileLocationLabel=位置�?
WizardUninstalling=卸载状�?
StatusUninstalling=正在卸载 %1...

; *** Shutdown block reasons
ShutdownBlockReasonInstallingApp=正在安装 %1�?
ShutdownBlockReasonUninstallingApp=正在卸载 %1�?

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]

NameAndVersion=%1 版本 %2
AdditionalIcons=附加快捷方式�?
CreateDesktopIcon=创建桌面快捷方式(&D)
CreateQuickLaunchIcon=创建快速运行栏快捷方式(&Q)
ProgramOnTheWeb=%1 网站
UninstallProgram=卸载 %1
LaunchProgram=运行 %1
AssocFileExtension=�?%2 文件扩展名与 %1 建立关联(&A)
AssocingFileExtension=正在�?%2 文件扩展名与 %1 建立关联...
AutoStartProgramGroupDescription=启动组：
AutoStartProgram=自动启动 %1
AddonHostProgramNotFound=%1无法找到您所选择的文件夹�?n%n您想要继续吗�?

; *** FreeGrabAppMessages
welcomeLabel1Custom=%n�汾%1
readEULA=�Ķ�FreeGrabApp��EULA����˽����
installOldVer=������ͼ��װһ���ɰ汾�ĳ������������������µ���
checkInternetConnection=��װ�����޷���֤�������%n�������Ļ��������ӣ����� "OK "���� "ȡ�� "����ֹ��װ��
checkInternetConnectionOk=��װ�����޷���֤�������%n�������Ļ��������ӡ�
productslbl=���