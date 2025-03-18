//#include "uninstallchecklistform.inc"

[Icons]
#ifdef Beta
    Name: "{commondesktop}\{#AppHumanName} Beta";   FileName: "{#destPath}\{#AppHumanName}\{#AppName}.exe";     WorkingDir: "{#destPath}\{#AppHumanName}";
    Name: "{group}\{#AppHumanName} Beta";           Filename: "{#destPath}\{#AppHumanName}\{#AppName}.exe";     WorkingDir: "{#destPath}\{#AppHumanName}";
#else
    Name: "{commondesktop}\{#AppHumanName}";        FileName: "{#destPath}\{#AppHumanName}\{#AppName}.exe";     WorkingDir: "{#destPath}\{#AppHumanName}";
    Name: "{group}\{#AppHumanName}";                Filename: "{#destPath}\{#AppHumanName}\{#AppName}.exe";     WorkingDir: "{#destPath}\{#AppHumanName}";
#endif

[Run]
#ifdef Beta
    Filename: "{#destPath}\{#AppHumanName}\{#AppName}.exe";     Description: {cm:LaunchProgram, {#AppHumanName} Beta};  WorkingDir: "{#destPath}\{#AppHumanName}";              Flags: postinstall nowait runascurrentuser;
#else
    Filename: "{#destPath}\{#AppHumanName}\{#AppName}.exe";     Description: {cm:LaunchProgram, {#AppHumanName}};       WorkingDir: "{#destPath}\{#AppHumanName}";              Flags: postinstall nowait runascurrentuser;
#endif

[Registry]

    Root: HKLM;  Subkey: "SOFTWARE\{#CompanyName}\AppPaths";        ValueType: string;  ValueName: "{#AppName}";       ValueData: "{#destPath}\{#AppHumanName}\{#AppName}.exe";
    Root: HKLM;  Subkey: "SOFTWARE\{#CompanyName}\UninstallPaths";  ValueType: string;  ValueName: "{#AppHumanName}";  ValueData: "{uninstallexe}";                             Flags: uninsdeletevalue;

[Files]

    Source: "products\{#AppName}\*";                                DestDir: {#destPath}\{#AppHumanName};               Flags: ignoreversion recursesubdirs;
    //Source: "installer_files\vcredist_x86.exe";  DestDir: {tmp};                        Flags: dontcopy;

[Code]

procedure WriteDefaultAppValues();
var
    reg_app_path: String;
    key: AnsiString;
begin
    if (RegQueryBinaryValue(HKEY_CURRENT_USER, ExpandConstant('Software\{#CompanyName}\Licenses'),ExpandConstant('{#AppName}'), key) = false) then begin
        reg_app_path := ExpandConstant('Software\{#CompanyName}\{#AppName}');

        //RegWriteBinaryValue(HKEY_CURRENT_USER, reg_app_path, 'AudioLanguage', GetFullCultureID(ActiveLanguage()));
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'AudioStream', 0);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'AudioType', 0);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'DownloadThreads', 3);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'EncodingSpeed', 1);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'ImageStream', 0);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'LeftPanelVisible', 0);
        //RegWriteBinaryValue(HKEY_CURRENT_USER, reg_app_path, 'PostDownloadAction', #1);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'SaveAs', 0);
        //RegWriteBinaryValue(HKEY_CURRENT_USER, reg_app_path, 'SaveSubtitlesAs', #1);
        //RegWriteBinaryValue(HKEY_CURRENT_USER, reg_app_path, 'SubtitleLanguage', #0);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'TutorialVisible', 1);
        RegWriteDwordValue(HKEY_CURRENT_USER, reg_app_path, 'VideoStream', 0);
    end;
end;

procedure BaseCurStepChanged(CurStep: TSetupStep);
var
    LogFilePathName, NewLogFilePath: string;
begin
    NewLogFilePath := ExpandConstant('{userappdata}\{#CompanyName}\logs\');
    if (CurStep = ssInstall) then begin
        //ShowAdvertising('inst', 0);
        wizardform.cancelButton.enabled := false;
        sAppName := ExpandConstant('{#AppName}');
        sAppHumanName := ExpandConstant('{#AppHumanName}');
        sAppVer := ExpandConstant('{#VerString}');
        sActLang := ActiveLanguage();
        DeleteFile(NewLogFilePath + sAppName + '.log');
        DeleteFile(NewLogFilePath + sAppName + '_uninstall.txt');
        WaitForFGAProgramsToClose();
        WriteDefaultAppValues();
        //ExtractTemporaryFile('vcredist_x86.exe');
        //ShellExec('open', ExpandConstant('{tmp}\vcredist_x86.exe'), '/Q','', SW_HIDE, ewWaitUntilTerminated, iResultCode);        
    end;

    if (CurStep = ssDone) then begin
        //ShowAdvertising('inst', 0);
        wizardform.cancelButton.enabled := false;
        LogFilePathName := expandconstant('{log}');
        CreateDir(NewLogFilePath);
        FileCopy(LogFilePathName, NewLogFilePath + sAppName + '_install.txt', false);
    end;
end;


procedure BaseDeinitializeSetup();
begin
    if(CheckIsSettingsFileExists(sAppName)) then begin
        Exec(ExpandConstant('{win}') + '\regedit.exe', '/s ' + ExpandConstant('{userappdata}\{#CompanyName}\') + sAppName + '\' + sAppName + '.reg', '', SW_HIDE, ewWaitUntilTerminated, iResultCode);
        DeleteFile(ExpandConstant('{userappdata}\{#CompanyName}\') + sAppHumanName + '.url');
    end;
    SetFirstInstallDate();
    SendAnalyticsRequest(1);
    RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\' + sAppHumanName + '_is1', 'UninstallString', ExpandConstant('{uninstallexe}') + ' /SILENT /SUPPRESSMSGBOXES');
    CallInstallStepRedirect(redirects.full);
end;


function InitializeUninstall(): Boolean;
begin
    CreateLogFolderIfItDoesntExist();
    sActLang := ActiveLanguage();
    sAppName := ExpandConstant('{#AppName}');
    sAppHumanName := ExpandConstant('{#AppHumanName}');
    sAppVer := '';
    GetVersionNumbersString(ExpandConstant('{app}\') + sAppHumanName + '\' + sAppName + '.exe', sAppVer);
    sAppHumanName := ExpandConstant('{#AppHumanName}');
    GetStepRedirects();
    if (sAppName <> 'NetflixDownloader') then begin
        //SetAnalyticsRequestParams(survey);
        //CreateUninstallChecklistForm();   //set bEnableUninstall
        //if (bEnableUninstall) then begin
            SendAnalyticsRequest(3);
        //end
        //else begin
        //    CallInstallStepRedirect(redirects.uninstallcancel);
        //    ExitProcess(0);
        //end;
    end;
    
    Result := true;
    KillFGAApps(sAppName);
    DeleteFile(ExpandConstant('{commondesktop}\') + sAppHumanName + '.lnk');
end;

procedure DeinitializeUninstall();
var
    sAppToDelete: String;
begin
    DelTree(ExpandConstant('{commonappdata}\{#CompanyName}\' + sAppName + '\'), true, true, true);
    DelTree(ExpandConstant('{app}\' + sAppHumanName + '\'), true, true, true);
    RegDeleteValue(HKEY_LOCAL_MACHINE, ExpandConstant('SOFTWARE\{#CompanyName}\AppPaths\'), sAppName);
    RegDeleteValue(HKEY_CURRENT_USER, ExpandConstant('SOFTWARE\{#CompanyName}\'), sAppName);
    RegDeleteValue(HKEY_CURRENT_USER, ExpandConstant('SOFTWARE\{#CompanyName}\'), 'Products');
    
    sAppToDelete := ExpandConstant('{#destPath}');
    if(IsEmptyDir(sAppToDelete)) then
        DelTree(sAppToDelete, true, true, true);
        
    sAppToDelete := ExpandConstant('{group}');
    if(IsEmptyDir(sAppToDelete)) then
        DelTree(sAppToDelete, true, true, true);

    sAppToDelete := ExpandConstant('{userappdata}\{#CompanyName}');
    if(IsEmptyDir(sAppToDelete)) then
        DelTree(sAppToDelete, true, true, true);
    sCancelPageID := '_' + IntToStr(GetNumberDaysOfLife());
    SetAnalyticsRequestParams(install);
    SendAnalyticsRequest(4);
    DelTree(ExpandConstant('{userappdata}\{#CompanyName}'), true, true, true);
    //DelTree(ExpandConstant('{localappdata}\{#CompanyName}'), true, true, true);
    CallInstallStepRedirect(redirects.uninstall);
end;