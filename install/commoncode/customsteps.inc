[Code]

function OnNextButtonClick(CurPageID: Integer): Boolean;
var
    in_dir, tessdata_dir, out_dir: String;
begin
    Result := true; 
    if (CurPageID = wpSelectDir) then begin
        DelTree(ExpandConstant('{#destPath}\{#AppHumanName}'), true, true, true);
        //DelTree(ExpandConstant('{localappdata}\{#CompanyName}\' + sAppName), true, true, true);
        RegDeleteKeyIncludingSubkeys(HKEY_CURRENT_USER, ExpandConstant('Software\{#CompanyName}\Vending'));
        
        if ((sAppName = 'FlixGrab') or (sAppName = 'FreeNetflixDownload') or (sAppName = 'NetflixDownloader')) then begin
            in_dir := ExpandConstant('{commonappdata}\{#CompanyName}\' + sAppName);
            tessdata_dir := in_dir + '\tessdata'; 
            out_dir := ExpandConstant('{commonappdata}\{#CompanyName}\tessdata');

            if (DirExists(tessdata_dir)) then begin
                if (CreateDir(out_dir)) then begin
                    DirectoryCopy(tessdata_dir, out_dir);
                    DelTree(in_dir + '\*', false, True, True);
                end;
            end;
        end;
    end;
    if ((CurPageID = wpFinished) and ((sAppName = 'FlixGrab') or (sAppName = 'FreeNetflixDownload') or (sAppName = 'NetflixDownloader'))) then begin
        in_dir := ExpandConstant('{commonappdata}\{#CompanyName}\' + sAppName);
        tessdata_dir := in_dir + '\tessdata'; 
        out_dir := ExpandConstant('{commonappdata}\{#CompanyName}\tessdata');
    
        if (DirExists(out_dir)) then begin
            if (CreateDir(tessdata_dir)) then
                DirectoryCopy(out_dir, tessdata_dir);
            DelTree(out_dir, True, True, True);
        end;
    end;
end;