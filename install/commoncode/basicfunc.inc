#include "installfunc.inc"
#include "localfunc.inc"
#include "customsteps.inc"
#include "products.inc"

[Code]

    function CheckVersion(thisVersion, site_version, update_url: String): Boolean;
    var 
        bCompareVerRes: Boolean;
    begin
        Result := false;
        if((thisVersion <> '') and (site_version <> '')) then begin
            Result := GetCompareVerRes(thisVersion, site_version);
            if(not Result) then begin
                if(MsgBox(ExpandConstant('{cm:installOldVer}'), mbConfirmation, MB_YESNO) = idYes) then begin
                    ShellExec('open', update_url, '', '', SW_SHOW, ewNoWait, iResultCode);
                    ExitProcess(0);
                end
                else begin
                    CallInstallStepRedirect(redirects.cancelupdate);
                end;
            end;
        end;
    end;


    function ShouldSkipPage(PageID: Integer): Boolean;
    begin
        if(PageID <> wpWelcome) then
            HideCustomWelcomePageControls();
    end;

    procedure InitializeWizard();
    begin
        sAppName := ExpandConstant('{#AppName}');
        sAppHumanName := ExpandConstant('{#AppHumanName}');
        sAppVer := ExpandConstant('{#VerString}');
        sActLang := ActiveLanguage();
        //SetControlCursor(WizardForm, crHourGlass);
        //SetControlCursor(WizardForm, crDefault);
    end;

    function InitializeSetup(): Boolean;
    begin
        sAppName := ExpandConstant('{#AppName}');
        sAppHumanName := ExpandConstant('{#AppHumanName}');
        sAppVer := ExpandConstant('{#VerString}');
        sActLang := ActiveLanguage();
        sInstType := 'InstType=' + GetInstType();
        KillFGAApps(sAppName);
        GetStepRedirects();
        SetAnalyticsRequestParams(install);
        SendAnalyticsRequest(0);
        //RegDeleteKeyIncludingSubkeys(HKEY_CURRENT_USER, ExpandConstant('Software\{#CompanyName}\123'));
        Result := true;
    end;

    function NextButtonClick(CurPageID: Integer): Boolean;
    var
        site_version, update_url, cur_language, reg_app_path: String;
    begin
        Result := true;
        sAppName := ExpandConstant('{#AppName}');
        sAppHumanName := ExpandConstant('{#AppHumanName}');
        sAppVer := ExpandConstant('{#VerString}');
        sActLang := ActiveLanguage();

        if (CurPageID = wpWelcome) then begin
            GetLatestVerAndURLToUpdate(site_version, update_url);
            if (site_version <> '') then begin
                Result := CheckVersion(sAppVer, site_version, update_url);
            end;
            //else begin
            //    if(MsgBox(ExpandConstant('{cm:checkInternetConnection}'), mbConfirmation, MB_OKCANCEL) = idOk) then begin
            //        Wizardform.NextButton.OnClick(nil);
            //    end
            //    else begin
            //        CallInstallStepRedirect(redirects.connectionerror);
            //        ExitProcess(0);
            //    end;
            //end;
            reg_app_path := ExpandConstant('Software\{#CompanyName}\{#AppName}');
            
            if (WizardSilent) then begin
                if (RegQueryStringValue(HKEY_CURRENT_USER, reg_app_path, 'Language', cur_language)) then begin
                    if (cur_language = '') then
                        RegWriteStringValue(HKEY_CURRENT_USER, reg_app_path, 'Language', GetFullCultureID(sActLang));
                end
                else begin
                    RegWriteStringValue(HKEY_CURRENT_USER, reg_app_path, 'Language', GetFullCultureID(sActLang));
                end;
            end
            else begin
                RegWriteStringValue(HKEY_CURRENT_USER, reg_app_path, 'Language', GetFullCultureID(sActLang));
            end;
        end;
        if (CurPageID > wpWelcome) then
            Result := ((OnNextButtonClick(CurPageID)) and (WaitForFGAProgramsToClose()));
    end;
    
    procedure WelcomePageChange();
    var
        nextbutton_left: Integer;
    begin
        WizardForm.WelcomeLabel1.WordWrap := true;
#ifdef Beta
        WizardForm.WelcomeLabel1.Caption := ExpandConstant('{#AppHumanName}') + ' Beta' + ExpandConstant('{cm:welcomeLabel1Custom,' + ExpandConstant('{#VerString}') + '}');
#else
        WizardForm.WelcomeLabel1.Caption := ExpandConstant('{#AppHumanName}') + ExpandConstant('{cm:welcomeLabel1Custom,' + ExpandConstant('{#VerString}') + '}');
#endif
        nextbutton_left := 27;
        if (WizardForm.Font.PixelsPerInch > 96) then
            nextbutton_left := 50;
        
        WizardForm.NextButton.Left := WizardForm.Width - (WizardForm.NextButton.Width + nextbutton_left);
        WizardForm.BackButton.Left := WizardForm.NextButton.Left - (WizardForm.BackButton.Width + 10);
        WizardForm.CancelButton.Left := WizardForm.Width + 10;
        
        if (sActLang = 'it') then
            WizardForm.WelcomeLabel2.Top := WizardForm.WelcomeLabel1.Top + ScaleY(55);
            
        if ((sActLang = 'ko')
            or (sActLang = 'jp')) then
            WizardForm.WelcomeLabel2.Top := WizardForm.WelcomeLabel1.Top + ScaleY(58);
        
        lblBlank := TNewStaticText.Create(WizardForm);
        lblBlank.Left := WizardForm.WelcomeLabel1.Left;
        lblBlank.Color := $ffffff;
        lblBlank.Parent := WizardForm;
        lblBlank.Top := WizardForm.WelcomeLabel2.Top + ScaleY(38);
        lblBlank.Width := WizardForm.WelcomeLabel1.Width;
        lblBlank.Height := WizardForm.Height - ScaleY(220);

        lblEULA := TNewStaticText.Create(WizardForm);
        lblEULA.Left := WizardForm.WelcomeLabel1.Left;
        lblEULA.Color := $ffffff;
        lblEULA.Caption := CustomMessage('readEULA');
        lblEULA.Cursor := crHand;
        lblEULA.Parent := WizardForm;
        lblEULA.Top := WizardForm.WelcomeLabel2.Top + ScaleY(43);
        if ((sActLang = 'de')
            or (sActLang = 'cs')
            or (sActLang = 'nl')
            or (sActLang = 'es')
            or (sActLang = 'fr')
            or (sActLang = 'el')
            or (sActLang = 'it')
            or (sActLang = 'hu')
            or (sActLang = 'pl')
            or (sActLang = 'br')
            or (sActLang = 'pt')
            or (sActLang = 'sk')
            or (sActLang = 'ru')
            or (sActLang = 'vn')
            or (sActLang = 'tw')
            or (sActLang = 'tr')
            or (sActLang = 'da')) then
            lblEULA.Top := lblEULA.Top + ScaleY(13);
        lblEULA.Cursor := crHand;
        lblEULA.OnClick := @lblEULAOnClick;
        lblEULA.Font.Style := lblEULA.Font.Style + [fsUnderline];
        lblEULA.Font.Color := clBlue;
    end;


    procedure CurPageChanged(CurPageID: Integer);
    begin
        if (CurPageID = wpWelcome) then begin
            WelcomePageChange();
        end;
        
        if (CurPageID = wpInstalling) then begin
            //ShowAdvertising();
        end;
    end;
    
    
    procedure CurStepChanged(CurStep: TSetupStep);
    begin
        BaseCurStepChanged(CurStep);    
    end;
      

    procedure CancelButtonClick(CurPageID: Integer; var Cancel, Confirm: Boolean);
    begin
        if(Confirm) then begin
            sCancelPageID := '_' + IntToStr(CurPageID);
            SendAnalyticsRequest(2);
            
            if(CurPageID = wpWelcome) then begin
                CallInstallStepRedirect(redirects.cancelwelcome);
            end
            else begin
                CallInstallStepRedirect(redirects.canceloutput);
            end;
        end;
    end;


    procedure DeinitializeSetup();
    begin
        if (no_connection_) then begin
            MsgBox(ExpandConstant('{cm:checkInternetConnectionOk}'), mbConfirmation, MB_OK);
            ExitProcess(0);
        end;
        BaseDeinitializeSetup();
    end;