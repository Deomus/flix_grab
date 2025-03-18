[Code]

function GetCompareVerRes(thisVersion, siteVersion: String): Boolean;
var
    strCurdate, sThisTimeStampVer, t, y, sThisTimeStampVerDay, sThisTimeStampVerMonth: String;
    aThisVer, aSiteVer: TArrayOfString;
    curDateCountDays, compileDateCountDays :Integer;
begin
    Result := false;
    aThisVer := SplitString(thisVersion, '.');
    aSiteVer := SplitString(siteVersion, '.');
    if (CompareVersions(aThisVer, aSiteVer) = 0) then
        Result := true;
end;


function IsFlashInstalled: Boolean;
var
  flash_ver: String;
begin
    Result := false;
if ( RegValueExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Microsoft\Active Setup\Installed Components\{D27CDB6E-AE6D-11cf-96B8-444553540000}', 'Version')) or ( RegValueExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Macromedia\FlashPlayer', 'CurrentVersion')) then
    Result := true;
end;
