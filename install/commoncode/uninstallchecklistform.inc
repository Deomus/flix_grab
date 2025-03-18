[Code]

var
    frmMessageForm:                                                                                 TSetupForm;
    rbtnChecklist0, rbtnChecklist1, rbtnChecklist2, rbtnChecklist3, rbtnChecklist4, rbtnChecklist5: TNewRadioButton;
    edtUnsupportedURL, edtOther:                                                                    TEdit;
    bEnableUninstall: Boolean;
    sUninstallChecklist: array of String;

type TRestoreStates = (
    url,
    other,
    both);


procedure InitUninstallChecklist(var sUninstallChecklist: array of String);
begin
    SetLength(sUninstallChecklist, 7);
    sUninstallChecklist[0] := 'Does not start';
    sUninstallChecklist[1] := 'Do not like the functionality';
    sUninstallChecklist[2] := 'There are errors in the work';
    sUninstallChecklist[3] := 'I have installed to download only a one video link';
    sUninstallChecklist[4] := 'I cannot download the video link';
    sUninstallChecklist[5] := 'Other';
    sUninstallChecklist[6] := 'Empty'; //Empty user message
end;


procedure RestoreUserMessagesFieldsToDefaultStates(restore: TRestoreStates);
begin
    if(restore = both) then begin
        if(edtUnsupportedURL.Text <> ExpandConstant(' {cm:edtUnsupportedURL}')) then begin
            edtUnsupportedURL.Text          := ExpandConstant(' {cm:edtUnsupportedURL}');
            edtUnsupportedURL.Font.Color    := $9B9C9C;
        end;
        if(edtOther.Text <> ExpandConstant(' {cm:edtOther}')) then begin
            edtOther.Text       := ExpandConstant(' {cm:edtOther}');
            edtOther.Font.Color := $9B9C9C;
        end;
    end;
    if(restore = url) then begin
        if(edtUnsupportedURL.Text <> ExpandConstant(' {cm:edtUnsupportedURL}')) then begin
            edtUnsupportedURL.Text          := ExpandConstant(' {cm:edtUnsupportedURL}');
            edtUnsupportedURL.Font.Color    := $9B9C9C;
        end;
    end;
    if(restore = other) then begin
        if(edtOther.Text <> ExpandConstant(' {cm:edtOther}')) then begin
            edtOther.Text       := ExpandConstant(' {cm:edtOther}');
            edtOther.Font.Color := $9B9C9C;
        end;
    end;
end;


procedure AcceptSubmitOnClick(Sender: TObject);
var
    closeForm: Boolean;
begin
    if (rbtnChecklist0.Checked) then begin
        closeForm := true;
        sUninstallChecklistAnswer := sUninstallChecklist[0];
    end
    else if (rbtnChecklist1.Checked) then begin
        closeForm := true;
        sUninstallChecklistAnswer := sUninstallChecklist[1];
    end
    else if (rbtnChecklist2.Checked) then begin
        closeForm := true;
        sUninstallChecklistAnswer := sUninstallChecklist[2];
    end
    else if (rbtnChecklist3.Checked) then begin
        closeForm := true;
        sUninstallChecklistAnswer := sUninstallChecklist[3];
    end
    else if (rbtnChecklist4.Checked) then begin
        closeForm := true;
        if((edtUnsupportedURL.Text = ExpandConstant(' {cm:edtUnsupportedURL}')) or (edtUnsupportedURL.Text = '')) then begin
            closeForm := false;
            MsgBox(ExpandConstant('{cm:addURL}'), mbInformation, MB_OK);
            RestoreUserMessagesFieldsToDefaultStates(url);
        end
        else begin
            sUninstallUserMessage := edtUnsupportedURL.Text;
        end;
        sUninstallChecklistAnswer := sUninstallChecklist[4];
    end
    else if (rbtnChecklist5.Checked) then begin
        if((edtOther.Text = ExpandConstant(' {cm:edtOther}')) or (edtOther.Text = '')) then begin
            sUninstallUserMessage := sUninstallChecklist[6];
        end
        else begin
            sUninstallUserMessage := edtOther.Text;
        end;
        closeForm := true;
        sUninstallChecklistAnswer := sUninstallChecklist[5];
    end;
    if(closeForm) then begin
        bEnableUninstall := true;
        frmMessageForm.Close;
    end;
end;


procedure OnEditUnsupportedURL(Sender: TObject);
begin
    edtUnsupportedURL.Font.Color    := clblack;
    rbtnChecklist4.Checked          := true;
    if(edtUnsupportedURL.Text = ExpandConstant(' {cm:edtUnsupportedURL}')) then begin
        edtUnsupportedURL.Text := '';
        RestoreUserMessagesFieldsToDefaultStates(other);
    end;
end;


procedure OnEditOther(Sender: TObject);
begin
    edtOther.Font.Color    := clblack;
    rbtnChecklist5.Checked := true;
    if(edtOther.Text = ExpandConstant(' {cm:edtOther}')) then begin
        edtOther.Text := '';
        RestoreUserMessagesFieldsToDefaultStates(url);
    end;
end;


procedure OnCheckWithoutMessage(Sender: TObject);
begin
    RestoreUserMessagesFieldsToDefaultStates(both);
end;


procedure OnCheckRbtnChecklist4(Sender: TObject);
begin
    RestoreUserMessagesFieldsToDefaultStates(other);
end;


procedure OnCheckRbtnChecklist5(Sender: TObject);
begin
    RestoreUserMessagesFieldsToDefaultStates(url);
end;


function CreateUninstallChecklistForm(): Boolean;
var
    lblChecklist0, lblChecklist1, lblChecklist2, lblChecklist3, lblChecklist4:  TLabel;
    btnSubmit:                                                                  TButton;
begin
    InitUninstallChecklist(sUninstallChecklist);

    frmMessageForm                  := CreateCustomForm();
    frmMessageForm.Width            := ScaleX(640);
    if(sActLang = 'de') then
        frmMessageForm.Width        := frmMessageForm.Width + ScaleX(179);
    frmMessageForm.Height           := ScaleY(541);
    frmMessageForm.Position         := poScreenCenter;
    frmMessageForm.FormStyle        := bsDialog;
    frmMessageForm.Color            := clWhite;
    
    lblChecklist0                   := TLabel.Create(frmMessageForm);
    lblChecklist0.Parent            := frmMessageForm;
    lblChecklist0.Left              := ScaleX(36);
    lblChecklist0.Top               := ScaleY(12); 
    lblChecklist0.Font.Height       := ScaleY(37);
    lblChecklist0.Font.Style        := [fsBold];
    lblChecklist0.Caption           := ExpandConstant('{cm:lblChecklist0}');
    
    lblChecklist1                   := TLabel.Create(frmMessageForm);
    lblChecklist1.Parent            := frmMessageForm;
    lblChecklist1.Left              := lblChecklist0.Left;
    lblChecklist1.Top               := lblChecklist0.Top + ScaleY(61); 
    lblChecklist1.Font.Height       := ScaleY(14);
    lblChecklist1.Caption           := ExpandConstant('{cm:lblChecklist1,' + sAppHumanName + '}');
    
    lblChecklist2                   := TLabel.Create(frmMessageForm);
    lblChecklist2.Parent            := frmMessageForm;
    lblChecklist2.Left              := lblChecklist0.Left;
    lblChecklist2.Top               := lblChecklist1.Top + ScaleY(37);
    lblChecklist2.Width             := frmMessageForm.Width; 
    lblChecklist2.Transparent       := True;
    lblChecklist2.Font.Height       := ScaleY(14);
    lblChecklist2.Font.Color        := $3744DB;
    lblChecklist2.Caption           := '*' + ExpandConstant('{cm:lblChecklist2}');
    
    lblChecklist3                   := TLabel.Create(frmMessageForm);
    lblChecklist3.Parent            := frmMessageForm;
    lblChecklist3.Left              := lblChecklist0.Left;
    lblChecklist3.Top               := lblChecklist2.Top + ScaleY(38);
    lblChecklist3.Font.Height       := ScaleY(23);
    lblChecklist3.Font.Style        := [fsBold];
    lblChecklist3.Caption           := ExpandConstant('{cm:lblChecklist3}');
    
    lblChecklist4                   := TLabel.Create(frmMessageForm);
    lblChecklist4.Parent            := frmMessageForm;
    lblChecklist4.Left              := lblChecklist3.Left + lblChecklist3.Width;
    lblChecklist4.Top               := lblChecklist3.Top;
    lblChecklist4.Transparent       := True;
    lblChecklist4.Font.Height       := ScaleY(23);
    lblChecklist4.Font.Style        := [fsBold];
    lblChecklist4.Font.Color        := $3744DB;
    lblChecklist4.Caption           := '*';
    
    rbtnChecklist0                  := TNewRadioButton.Create(frmMessageForm);
    rbtnChecklist0.Parent           := frmMessageForm;
    rbtnChecklist0.Checked          := false;
    rbtnChecklist0.Top              := lblChecklist4.Top + ScaleY(44);
    rbtnChecklist0.Left             := lblChecklist0.Left;
    rbtnChecklist0.Width            := frmMessageForm.Width;
    rbtnChecklist0.Height           := ScaleY(53);
    rbtnChecklist0.Font.Size        := ScaleY(12);
    rbtnChecklist0.Caption          := ExpandConstant('{cm:question0}');
    rbtnChecklist0.OnClick          := @OnCheckWithoutMessage;
    
    rbtnChecklist1                  := TNewRadioButton.Create(frmMessageForm);
    rbtnChecklist1.Parent           := frmMessageForm;
    rbtnChecklist1.Checked          := false;
    rbtnChecklist1.Top              := rbtnChecklist0.Top + ScaleY(40);
    rbtnChecklist1.Left             := lblChecklist0.Left;
    rbtnChecklist1.Width            := frmMessageForm.Width;
    rbtnChecklist1.Height           := ScaleY(53);
    rbtnChecklist1.Font.Size        := ScaleY(12);
    rbtnChecklist1.Caption          := ExpandConstant('{cm:question1}');
    rbtnChecklist1.OnClick          := @OnCheckWithoutMessage;
    
    rbtnChecklist2                  := TNewRadioButton.Create(frmMessageForm);
    rbtnChecklist2.Parent           := frmMessageForm;
    rbtnChecklist2.Checked          := false;
    rbtnChecklist2.Top              := rbtnChecklist1.Top + ScaleY(40);
    rbtnChecklist2.Left             := lblChecklist0.Left;
    rbtnChecklist2.Width            := frmMessageForm.Width;
    rbtnChecklist2.Height           := ScaleY(53);
    rbtnChecklist2.Font.Size        := ScaleY(12);
    rbtnChecklist2.Caption          := ExpandConstant('{cm:question2}');
    rbtnChecklist2.OnClick          := @OnCheckWithoutMessage;
    
    rbtnChecklist3                  := TNewRadioButton.Create(frmMessageForm);
    rbtnChecklist3.Parent           := frmMessageForm;
    rbtnChecklist3.Checked          := false;
    rbtnChecklist3.Top              := rbtnChecklist2.Top + ScaleY(40);
    rbtnChecklist3.Left             := lblChecklist0.Left;
    rbtnChecklist3.Width            := frmMessageForm.Width;
    rbtnChecklist3.Height           := ScaleY(53);
    rbtnChecklist3.Font.Size        := ScaleY(12);
    rbtnChecklist3.Caption          := ExpandConstant('{cm:question3}');
    rbtnChecklist3.OnClick          := @OnCheckWithoutMessage;
    
    rbtnChecklist4                  := TNewRadioButton.Create(frmMessageForm);
    rbtnChecklist4.Parent           := frmMessageForm;
    rbtnChecklist4.Checked          := false;
    rbtnChecklist4.Top              := rbtnChecklist3.Top + ScaleY(40);
    rbtnChecklist4.Left             := lblChecklist0.Left;
    rbtnChecklist4.Width            := ScaleX(249);
    if(sActLang = 'de') then
        rbtnChecklist4.Width        := rbtnChecklist4.Width + ScaleX(81);
    rbtnChecklist4.Height           := ScaleY(53);
    rbtnChecklist4.Font.Size        := ScaleY(12);
    rbtnChecklist4.Caption          := ExpandConstant('{cm:question4}');
    rbtnChecklist4.OnClick          := @OnCheckRbtnChecklist4;
    
    edtUnsupportedURL               := TEdit.Create(frmMessageForm);
    edtUnsupportedURL.Parent        := frmMessageForm;
    edtUnsupportedURL.Top           := rbtnChecklist4.Top + ScaleY(15);
    edtUnsupportedURL.Left          := rbtnChecklist4.Left + rbtnChecklist4.Width + ScaleX(20);
    edtUnsupportedURL.Width         := ScaleX(293);
    if(sActLang = 'de') then
        edtUnsupportedURL.Width     := edtUnsupportedURL.Width + ScaleX(190) - ScaleX(81);
    edtUnsupportedURL.Height        := rbtnChecklist4.Height;
    edtUnsupportedURL.Font.Size     := ScaleY(10);
    edtUnsupportedURL.Font.Color    := $9B9C9C;
    edtUnsupportedURL.MaxLength     := 255;
    edtUnsupportedURL.Text          := ExpandConstant(' {cm:edtUnsupportedURL}');
    edtUnsupportedURL.AutoSelect    := false;
    edtUnsupportedURL.OnClick       := @OnEditUnsupportedURL;
    edtUnsupportedURL.TabOrder      := 1;
    
    rbtnChecklist5                  := TNewRadioButton.Create(frmMessageForm);
    rbtnChecklist5.Parent           := frmMessageForm;
    rbtnChecklist5.Checked          := false;
    rbtnChecklist5.Top              := rbtnChecklist4.Top + ScaleY(40);
    rbtnChecklist5.Left             := lblChecklist0.Left;
    rbtnChecklist5.Width            := ScaleX(57);
    if(sActLang = 'de') then
        rbtnChecklist5.Width        := rbtnChecklist5.Width + ScaleX(10);
    rbtnChecklist5.Height           := ScaleY(53);
    rbtnChecklist5.Font.Size        := ScaleY(12);
    rbtnChecklist5.Caption          := ExpandConstant('{cm:question5}');
    rbtnChecklist5.OnClick          := @OnCheckRbtnChecklist5;
    
    edtOther                        := TEdit.Create(frmMessageForm);
    edtOther.Parent                 := frmMessageForm;
    edtOther.Top                    := rbtnChecklist5.Top + ScaleY(15);
    edtOther.Left                   := rbtnChecklist5.Left + rbtnChecklist5.Width + ScaleX(20);
    edtOther.Width                  := ScaleX(485);
    if(sActLang = 'de') then
        edtOther.Width              := edtOther.Width + ScaleX(190) - ScaleX(10);
    edtOther.Height                 := rbtnChecklist5.Height;
    edtOther.Font.Size              := ScaleY(10);
    edtOther.Font.Color             := $9B9C9C;
    edtOther.MaxLength              := 255;
    edtOther.Text                   := ExpandConstant(' {cm:edtOther}');
    edtOther.AutoSelect             := false;
    edtOther.OnClick                := @OnEditOther;
    edtOther.TabOrder               := 2;

    btnSubmit                       := TButton.Create(frmMessageForm);
    btnSubmit.Parent                := frmMessageForm;
    btnSubmit.Caption               := ExpandConstant('{cm:btnSubmit}');
    btnSubmit.Font.Height           := ScaleY(16);
    btnSubmit.Width                 := ScaleX(145);
    btnSubmit.Height                := ScaleY(27);
    btnSubmit.Left                  := frmMessageForm.Width / 2 - btnSubmit.Width / 2;
    btnSubmit.Top                   := rbtnChecklist5.Top + ScaleY(66);
    btnSubmit.OnClick               := @AcceptSubmitOnClick;
    btnSubmit.TabOrder              := 3;

    Result := true;
    frmMessageForm.ShowModal;
end;