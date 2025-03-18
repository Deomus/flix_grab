#include "jsonfunc.inc"

[Languages]

Name: en;  MessagesFile: "installer_files/inno_languages/Default.isl";
Name: de;  MessagesFile: "installer_files/inno_languages/German.isl";
Name: fr;  MessagesFile: "installer_files/inno_languages/French.isl";
Name: pl;  MessagesFile: "installer_files/inno_languages/Polish.isl";
Name: pt;  MessagesFile: "installer_files/inno_languages/Portuguese.isl";
Name: it;  MessagesFile: "installer_files/inno_languages/Italian.isl";
Name: es;  MessagesFile: "installer_files/inno_languages/Spanish.isl";
Name: hi;  MessagesFile: "installer_files/inno_languages/Hindi.isl";
Name: jp;  MessagesFile: "installer_files/inno_languages/Japanese.isl";
Name: ko;  MessagesFile: "installer_files/inno_languages/Korean.isl";
Name: nl;  MessagesFile: "installer_files/inno_languages/Dutch.isl";
Name: ru;  MessagesFile: "installer_files/inno_languages/Russian.isl";
Name: cn;  MessagesFile: "installer_files/inno_languages/ChineseSimplified.isl";
Name: tw;  MessagesFile: "installer_files/inno_languages/ChineseTraditional.isl";
Name: cs;  MessagesFile: "installer_files/inno_languages/Czech.isl";
Name: tr;  MessagesFile: "installer_files/inno_languages/Turkish.isl";
Name: da;  MessagesFile: "installer_files/inno_languages/Danish.isl";
Name: sv;  MessagesFile: "installer_files/inno_languages/Swedish.isl";


[Code]

const
   LF = #10;
   CR = #13;
   CRLF = CR + LF;

type
    TGuidInst = record
    D1: LongWord;
    D2: Word;
    D3: Word;
    D4: array[0..7] of Byte;
end;

type TAnaliticsAccount = (
    install,
    survey);

type 
    TInstallRedirects = record
    full: String;
    cancelwelcome: String;
    canceloutput: String;
    cancelupdate: String;
    connectionerror: String;
    uninstallcancel: String;
    uninstall: String;
end;


var
    redirects: TInstallRedirects;
    iResultCode: Integer;
    sAppName, sAppHumanName, sAppVer, sActLang, sInstType, sAnalitReqURL, sCancelPageID, sUninstallChecklistAnswer, sUninstallUserMessage: String;
    lblBlank, lblEULA: TNewStaticText;
    no_connection_: Boolean;
    

procedure ExitProcess(exitCode: Integer);
    external 'ExitProcess@kernel32.dll stdcall';
    
function CoCreateGuid(var Guid: TGuidInst): Integer;
    external 'CoCreateGuid@ole32.dll stdcall';

procedure ConvertLineEndings(var line : String);
  var
     FilePath : String;
     FileContents : String;
begin
   StringChangeEx(line, LF, CRLF, False);
end;

function BoolToStr(value : Boolean) : String;
begin
    Result := 'False';
    if (value) then 
        Result := 'True';
end;

function GetFullCultureID( language: string ): String;
begin
    if      (language = 'de')  then Result := 'de-DE'
    else if (language = 'da')  then Result := 'da-DK'
    else if (language = 'jp')  then Result := 'ja-JP'
    else if (language = 'fr')  then Result := 'fr-FR'
    else if (language = 'es')  then Result := 'es-ES'
    else if (language = 'pt')  then Result := 'pt-PT'
    else if (language = 'br')  then Result := 'pt-BR'
    else if (language = 'ru')  then Result := 'ru-RU'
    else if (language = 'cn')  then Result := 'zh-CN'
    else if (language = 'tw')  then Result := 'zh-TW'
    else if (language = 'it')  then Result := 'it-IT'
    else if (language = 'nl')  then Result := 'nl-NL'
    else if (language = 'pl')  then Result := 'pl-PL'
    else if (language = 'el')  then Result := 'el-GR'
    else if (language = 'tr')  then Result := 'tr-TR'
    else if (language = 'hu')  then Result := 'hu-HU'
    else if (language = 'sv')  then Result := 'sv-SE'
    else if (language = 'fi')  then Result := 'fi-FI'
    else if (language = 'sl')  then Result := 'sl-SI'
    else if (language = 'sk')  then Result := 'sk-SK'
    else if (language = 'vn')  then Result := 'vi-VN'
    else if (language = 'hi')  then Result := 'hi-IN'
    else if (language = 'ko')  then Result := 'ko-KR'
    else if (language = 'cs')  then Result := 'cs-CZ'
    else                            Result := 'en-US'
end;

procedure DirectoryCopy(SourcePath, DestPath: string);
var
  FindRec: TFindRec;
  SourceFilePath: string;
  DestFilePath: string;
begin
    if FindFirst(SourcePath + '\*', FindRec) then begin
        try
            repeat
                if (FindRec.Name <> '.') and (FindRec.Name <> '..') then begin
                    SourceFilePath := SourcePath + '\' + FindRec.Name;
                    DestFilePath := DestPath + '\' + FindRec.Name;
                    if FindRec.Attributes and FILE_ATTRIBUTE_DIRECTORY = 0 then begin
                        if FileCopy(SourceFilePath, DestFilePath, False) then begin
                            Log(Format('Copied %s to %s', [SourceFilePath, DestFilePath]));
                        end
                        else begin
                            Log(Format('Failed to copy %s to %s', [SourceFilePath, DestFilePath]));
                        end;
                    end
                    else begin
                        if DirExists(DestFilePath) or CreateDir(DestFilePath) then begin
                            Log(Format('Created %s', [DestFilePath]));
                            DirectoryCopy(SourceFilePath, DestFilePath);
                        end
                        else begin
                            Log(Format('Failed to create %s', [DestFilePath]));
                        end;
                    end;
                end;
            until not FindNext(FindRec);
        finally
            FindClose(FindRec);
        end;
    end
    else begin
        Log(Format('Failed to list %s', [SourcePath]));
    end;
end;

function CompareVersions(arrayInstVersion: TArrayOfString; arrayCurrentVersion: TArrayOfString): Integer;
begin
    Result := 0;
    if (StrToInt(arrayInstVersion[0]) < StrToInt(arrayCurrentVersion[0])) then begin
        Result := 1;
    end
    else if (StrToInt(arrayInstVersion[0]) = StrToInt(arrayCurrentVersion[0])) then begin
        if (StrToInt(arrayInstVersion[1]) < StrToInt(arrayCurrentVersion[1])) then begin
            Result := 1;
            Exit;
        end
        else if (StrToInt(arrayInstVersion[1]) = StrToInt(arrayCurrentVersion[1])) then begin
            if (StrToInt(arrayInstVersion[2]) < StrToInt(arrayCurrentVersion[2])) then begin
                Result := 1;
                Exit;
            end;
        end;
    end;
end;


procedure CallInstallStepRedirect(url: String);
begin
    //default behavior
    if (url = '') then
        url := ExpandConstant('https://{#CompanyDomain}');
    ShellExec('open', url,  '', '', SW_SHOW, ewNoWait, iResultCode);
    ExitProcess(0);
end;


function SplitString(var input_str: String; delimeter: String): TArrayOfString;
var
    input_array, output_array: TArrayOfString;
    i: Integer;
begin
    i := 0;
    repeat
        SetArrayLength(input_array, i + 1);
        SetArrayLength(output_array, i + 1);
        if Pos(delimeter, input_str) > 0 then begin
            input_array[i] := Copy(input_str, 1, Pos(delimeter, input_str)-1);
            output_array[i] := input_array[i];
            input_str := Copy(input_str, Pos(delimeter, input_str) + Length(delimeter), Length(input_str));
            i := i + 1;
        end else begin
             input_array[i] := input_str;
             output_array[i] := input_array[i];
             input_str := '';
        end;
    until Length(input_str)=0;
    Result := output_array;
end;


function GetInstType(): String;
begin
    Result := 'clean';
end;


function GetHTTPRequest(sURL: String; blocking_check: Boolean): String;
var
    winHttpReq: Variant;
    show_connection_error: Boolean;
    i: Integer;
begin
    result := '';
    if (blocking_check) then
        no_connection_ := true;
    show_connection_error := false;
    i := 0;
    repeat
        try
            winHttpReq := CreateOleObject('WinHttp.WinHttpRequest.5.1');
            winHttpReq.SetTimeouts(10000,10000,10000,10000);
            winHttpReq.Open('GET', sURL, false);
            winHttpReq.SetRequestHeader('Content-Type', 'text/plain');
            winHttpReq.Send('');
            result := winHttpReq.ResponseText;

            if (winHttpReq.Status <> 200) then
                show_connection_error := true;
        except
            show_connection_error := true;
        end;

        if (show_connection_error and blocking_check and (i > 2)) then begin
            show_connection_error := false;
            result := '';
            if(MsgBox(ExpandConstant('{cm:checkInternetConnection}'), mbConfirmation, MB_OKCANCEL) = idOk) then begin
                CallInstallStepRedirect(redirects.connectionerror);
                ExitProcess(0);
            end;
        end;
        i := i + 1;
    until (result <> '') or (i > 2);
    no_connection_ := false;
end;


function PostHTTPRequest(sURL: String): String;
var
    winHttpReq: Variant;
    connection_error: Boolean;
    i: Integer;
begin
    Result := '';
    connection_error := false;
    i := 0;
    repeat
        try
            winHttpReq := CreateOleObject('WinHttp.WinHttpRequest.5.1');
            winHttpReq.SetTimeouts(5000,5000,5000,5000);
            winHttpReq.Open('POST', sURL, false);
            winHttpReq.SetRequestHeader('Content-Type', 'text/plain');
            winHttpReq.Send('');
            if winHttpReq.Status <> 200 then
                connection_error := true;
        except
            connection_error := true;
        end;
        i := i + 1;
    until (not connection_error or (i > 2));
end;

function IsAppRunning(const FileName : string): Boolean;
var
    FSWbemLocator: Variant;
    FWMIService   : Variant;
    FWbemObjectSet: Variant;
begin
    Result := false;
    FSWbemLocator := CreateOleObject('WBEMScripting.SWBEMLocator');
    FWMIService := FSWbemLocator.ConnectServer('', 'root\CIMV2', '', '');
    FWbemObjectSet := FWMIService.ExecQuery(Format('SELECT Name FROM Win32_Process Where Name="%s"', [FileName]));
    Result := (FWbemObjectSet.Count > 0);
    FWbemObjectSet := Unassigned;
    FWMIService := Unassigned;
    FSWbemLocator := Unassigned;
end;

procedure KillFGAApps(sAppName: String);
begin
    //if (IsAppRunning(sAppName + '.exe')) then begin
    ShellExec('open', 'taskkill.exe', '/im ' + sAppName + '.exe' + ' /f','',SW_HIDE,ewNoWait, iResultCode);
    if (sAppName = 'FlixGrab') then
        ShellExec('open', 'taskkill.exe', '/im FlixGrabMS.exe' + ' /f','',SW_HIDE,ewNoWait, iResultCode);
    if (sAppName = 'FlixGrabMusic') then
        ShellExec('open', 'taskkill.exe', '/im FlixGrabMusicMS.exe' + ' /f','',SW_HIDE,ewNoWait, iResultCode);
end;


function CheckForFGAMutexes(installStep: Integer): String;
begin
    if CheckForMutexes('Global\DW_BROWSER_PROJECT') then
        Result := 'Download It Browser|DownloadItBrowser1'; //need real mutex
end;


function WaitForFGAProgramsToClose(): Boolean;
var
  sMutexRes, sAppToKill: string;
  bEnableNextStep: boolean;
begin
    repeat
        begin
            sMutexRes := CheckForFGAMutexes(0);
            if (sMutexRes <> '') then begin
                bEnableNextStep := false;
                sAppToKill := Copy(sMutexRes, Pos('|', sMutexRes) + 1, Length(sMutexRes) - Pos('|', sMutexRes)) + '.exe';
                KillFGAApps(sAppToKill); //need check sAppToKill
                Sleep(200);
            end
            else begin
                bEnableNextStep := true;
            end;
        end;
    until sMutexRes = '';
    Result := bEnableNextStep;
end;


function CheckIsSettingsFileExists(sAppName: String): Boolean;
begin
    Result:= false;
    if(FileExists(ExpandConstant('{userappdata}\GrbApp\') + sAppName + '\' + sAppName + '.reg')) then
        Result:= true;
end;


function GetCurrentDate(): Longint;
var
    strCurdate: String;
begin
    strCurdate := GetDateTimeString( 'yyyymmdd', ',', ',' );
    Result := (StrToInt(Copy(strCurdate, 1, 4)))*365 + (StrToInt(Copy(strCurdate, 5, 2)))*31 + StrToInt(Copy(strCurdate, 7, 2));
end;


function GetNumberDaysOfLife(): Integer;
var
    iInstallDate: Cardinal;
    sAppConfPath, sFirstInstall: String;
begin
    Result := -1;
    sAppConfPath := ExpandConstant('{userappdata}\{#CompanyName}\' + sAppName + '\' + sAppName + '.ini');
    if(FileExists(sAppConfPath)) then begin
        sFirstInstall := GetIniString('Common', 'FirstInstall', '', sAppConfPath);
        if(sFirstInstall <> '') then
            Result := GetCurrentDate() - StrToInt(sFirstInstall);
    end;
end;


procedure CreateLogFolderIfItDoesntExist();
var
    instLogsFolder: String;
begin
    instLogsFolder := ExpandConstant('{userappdata}\{#CompanyName}\logs\');
    if (DirExists(instLogsFolder) = false) then begin
        CreateDir(instLogsFolder);
    end;
end;


procedure HideCustomWelcomePageControls();
begin
    lblBlank.hide;
    lblEULA.hide;
end;


procedure lblEULAOnClick(Sender: TObject);
var
  ErrorCode: Integer;
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/en/eula'), '', '', SW_SHOWNORMAL, ewNoWait, ErrorCode);
end;


procedure GetLatestVerAndURLToUpdate(var site_version, update_url: String);
var
    url_content, cur_app: String;
    JsonParser: TJsonParser;
    JsonRoot, InfoObject: TJsonObject;
    //UserNumber: TJsonNumber; { = Double }
    version, url: TJsonString; { = WideString = string }
begin
    url_content := GetHTTPRequest(ExpandConstant('{#version_url}'), false);
    if (url_content <> '') and (Pos('Error', url_content) = 0) then begin
        if ParseJsonAndLogErrors(JsonParser, url_content) then begin
            JsonRoot := GetJsonRoot(JsonParser.Output);
            if FindJsonObject(JsonParser.Output, JsonRoot, sAppName, InfoObject)
                //and FindJsonNumber(JsonParser.Output, InfoObject, 'User', UserNumber)
                and FindJsonString(JsonParser.Output, InfoObject, 'product_url', url)
                and FindJsonString(JsonParser.Output, InfoObject, 'version', version) then begin
                //Log(Format(sAppName + ':User:%d', [Round(UserNumber)]));
                //Log(Format(sAppName + ':String:%s', [UserString]));
                site_version := Format('%s', [version]);
                update_url := Format('%s', [url]);
            end;
        end;
        ClearJsonParser(JsonParser);
    end;
end;


procedure GetStepRedirects();
begin
    redirects.full := ExpandConstant('https://{#CompanyDomain}/install/');
    redirects.cancelwelcome := ExpandConstant('https://{#CompanyDomain}/cancelwelcome/');
    redirects.canceloutput := ExpandConstant('https://{#CompanyDomain}/canceloutput/');
    redirects.cancelupdate := ExpandConstant('https://{#CompanyDomain}/cancelupdate/');
    redirects.connectionerror := ExpandConstant('https://{#CompanyDomain}/connectionerror/');
    redirects.uninstallcancel := ExpandConstant('https://{#CompanyDomain}/uninstallcancel/');
    redirects.uninstall := ExpandConstant('https://{#CompanyDomain}/uninstall/');
end;


function IsEmptyDir(dirName: String): Boolean;
var
    FindRec: TFindRec;
    FileCount: Integer;
begin
    Result := False;
    if FindFirst(dirName+'\*', FindRec) then begin
        try
            repeat
                if (FindRec.Name <> '.') and (FindRec.Name <> '..') then begin
                    FileCount := 1;
                    break;
                end;
            until not FindNext(FindRec);
            finally
                FindClose(FindRec);
            if FileCount = 0 then 
                Result := True;
        end;
    end;
end;


function IntToHex(l: Longword; digits: Integer): String;
var 
    hexchars: String;
begin
    hexchars:='0123456789ABCDEF';
    setlength(Result,digits);
    while (digits>0) do begin
        Result[digits]:=hexchars[l mod 16+1];
        l:=l div 16;
        digits:=digits-1;
    end;
end;


function GetClientID(): String;
var 
    ClientID: TGuidInst;
    sCommonConfPath, sGuidFromConf: String;
begin
    Result:= '00000000-0000-0000-0000-000000000000';
    sCommonConfPath := ExpandConstant('{userappdata}\{#CompanyName}\common.ini');
    if(FileExists(sCommonConfPath)) then begin
        sGuidFromConf := GetIniString('Common', 'ClientID', '', sCommonConfPath);
        if(sGuidFromConf = '') then begin
            if (CoCreateGuid(ClientID) = 0) then begin
                Result:= IntToHex(ClientID.D1,8) + '-' + IntToHex(ClientID.D2,4) + '-' + IntToHex(ClientID.D3,4) + '-' + IntToHex(ClientID.D4[0],2) + IntToHex(ClientID.D4[1],2) + '-' + IntToHex(ClientID.D4[2],2) + IntToHex(ClientID.D4[3],2) + IntToHex(ClientID.D4[4],2) + IntToHex(ClientID.D4[5],2) + IntToHex(ClientID.D4[6],2) + IntToHex(ClientID.D4[7],2);
                SetIniString('Common', 'ClientID', Result, sCommonConfPath);
            end;
        end
        else begin
            Result:= sGuidFromConf;
        end;
    end
    else begin
        if (CoCreateGuid(ClientID) = 0) then begin
            if (DirExists(ExpandConstant('{userappdata}\{#CompanyName}')) = false) then
                CreateDir(ExpandConstant('{userappdata}\{#CompanyName}'));
            Result:= IntToHex(ClientID.D1,8) + '-' + IntToHex(ClientID.D2,4) + '-' + IntToHex(ClientID.D3,4) + '-' + IntToHex(ClientID.D4[0],2) + IntToHex(ClientID.D4[1],2) + '-' + IntToHex(ClientID.D4[2],2) + IntToHex(ClientID.D4[3],2) + IntToHex(ClientID.D4[4],2) + IntToHex(ClientID.D4[5],2) + IntToHex(ClientID.D4[6],2) + IntToHex(ClientID.D4[7],2);
            SetIniString('Common', 'ClientID', Result, sCommonConfPath);
        end;
    end;
end;


function GetWinBuildVer(): String;
var
    winVer: TWindowsVersion;
begin
    GetWindowsVersionEx(winVer);
    Result := IntToStr(winVer.Major) + '.' + IntToStr(winVer.Minor) + '.SP' + IntToStr(winVer.ServicePackMajor);
end;


procedure SetAnalyticsRequestParams(analiticsName: TAnaliticsAccount);
var
    sGoogleAnalitURL, sProtocolVer, sAnalyticsID, sClientID, sDataSource, sAppN, sAppID, sAppV, sUserAgent, sAppLanguage: String;
begin
    sGoogleAnalitURL :=   'https://www.google-analytics.com/collect?';
    sProtocolVer :=       'v=1';
    if(analiticsName = install) then
        sAnalyticsID := '&tid=UA-66073836-2';
    if(analiticsName = survey) then
        sAnalyticsID := '&tid=UA-66073836-6';
    sClientID :=          '&cid=' + GetClientID();
    sDataSource :=        '&ds=app';
    sAppN :=              '&an=' + sAppHumanName;
    sAppID :=             '&aid=' + sAppName;
    sAppV :=              '&av=' + sAppVer;
    sUserAgent :=         '&ua=' + sAppName + '/' + sAppVer + ' (Windows NT ' + GetWinBuildVer() + ' )';
    sAppLanguage :=       '&ul=' + sActLang;
    sAnalitReqURL := sGoogleAnalitURL + sProtocolVer + sAnalyticsID + sClientID + sDataSource + sAppN + sAppID + sAppV + sUserAgent + sAppLanguage;
end;


procedure SendAnalyticsRequest(step: Integer);
begin
    case step of
        0: begin
            PostHTTPRequest(sAnalitReqURL + '&sc=start&t=event&ec=installer&ea=install&el=start&sc=end');                                                               //start install &ea=NewStart&el=Success
        end;
        1: begin
            PostHTTPRequest(sAnalitReqURL + '&sc=start&t=event&ec=installer&ea=install&el=full&sc=end');                                                                //end install
        end;
        2: begin
            PostHTTPRequest(sAnalitReqURL + '&sc=start&t=event&ec=installer&ea=install&el=cancel_page&sc=end');                                                         //cancel install
        end;
        3: begin
            if (sUninstallChecklistAnswer <> '') then
                PostHTTPRequest(sAnalitReqURL + '&sc=start&t=event&ec=survey_uninstall&ea=' + sUninstallChecklistAnswer + '&el=' + sUninstallUserMessage + '&sc=end');  //start uninstall
        end;
        4: begin
            PostHTTPRequest(sAnalitReqURL + '&sc=start&t=event&ec=installer&ea=uninstall&el=full&sc=end');                                                              //end uninstall
        end;
    end;
end;


procedure SetFirstInstallDate();
var
    sAppConfPath, sFirstInstallDateIndays: String;
begin
    sAppConfPath := ExpandConstant('{userappdata}\{#CompanyName}\' + sAppName + '\' + sAppName + '.ini');
    if (DirExists(ExpandConstant('{userappdata}\{#CompanyName}\') + sAppName) = false) then
        CreateDir(ExpandConstant('{userappdata}\{#CompanyName}\') + sAppName);
    if(FileExists(sAppConfPath) = false) then begin
        sFirstInstallDateIndays := GetIniString('Common', 'FirstInstall', '', sAppConfPath);
        if(sFirstInstallDateIndays = '') then
            SetIniString('Common', 'FirstInstall', IntToStr(GetCurrentDate()), sAppConfPath);
    end;
end;