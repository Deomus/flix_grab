[Files]

Source: "installer_files\products\amazon.bmp";              DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\amazonmusic.bmp";         DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\apple.bmp";               DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\dailymotion.bmp";         DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\disney.bmp";              DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\facebook.bmp";            DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\flixgrab.bmp";            DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\flixgrabmusic.bmp";       DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\hbo.bmp";                 DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\hulu.bmp";                DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\instagram.bmp";           DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\netflix.bmp";             DestDir: "{tmp}";  Flags: dontcopy
//Source: "installer_files\products\pornhub.bmp";             DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\spotify.bmp";             DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\twitch.bmp";              DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\twitter.bmp";             DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\vimeo.bmp";               DestDir: "{tmp}";  Flags: dontcopy
//Source: "installer_files\products\xvideos.bmp";             DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\youtube.bmp";             DestDir: "{tmp}";  Flags: dontcopy
Source: "installer_files\products\youtubetomp3.bmp";        DestDir: "{tmp}";  Flags: dontcopy


[Code]

procedure lblFlixGrabURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/flixgrab/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblFlixGrabMusicURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/flixgrab-music/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblNetflixURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-netflix-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblAmazonURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-amazon-prime-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblDisneyURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-disney-plus-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblHBOURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-hbo-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblHuluURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-hulu-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblAppleURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-apple-music-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblSpotifyURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-spotify-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblAmazonMusicURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-amazon-music-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblYoutubeToMP3URLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-youtube-to-mp3-converter/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblYoutubeURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-youtube-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblDailymotionURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-dailymotion-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblVimeoURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-vimeo-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblTwitchURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-twitch-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblFacebookURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-facebook-video-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblInstagramURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-instagram-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;

procedure lblTwitterURLOnClick(Sender: TObject);
begin
    ShellExec('open', ExpandConstant('https://{#CompanyDomain}/') + sActLang + '/product/free-twitter-download/', '', '', SW_SHOWNORMAL, ewNoWait, iResultCode);
end;


procedure ShowAdvertising();
var
    btmAmazon, btmAmazonmusic, btmApple, btmDailymotion, btmDisney, btmFacebook, btmFlixgrab, btmFlixgrabmusic, btmHBO, btmHulu, btmInstagram, btmNetflix, btmSpotify, btmTwitch, btmTwitter, btmVimeo, btmYoutube, btmYoutubetomp3: TBitmapImage;
    lblAmazon, lblAmazonmusic, lblApple, lblDailymotion, lblDisney, lblFacebook, lblFlixgrab, lblFlixgrabmusic, lblHBO, lblHulu, lblInstagram, lblNetflix, lblSpotify, lblTwitch, lblTwitter, lblVimeo, lblYoutube, lblYoutubetomp3: TLabel;
    i, j, k, m, apps_count, itemWidth, itemHeight: Integer;
    exists: Boolean;
    items: array of Integer;
begin
    apps_count := 18;
    k := 0;
    SetLength(items, 4);
    
    while (k < 4) do begin
        exists := false;
        m := Random(apps_count - 1);
        for j := 0 to GetArrayLength(items) - 1 do begin
            if (items[j] = m) then
                exists := true;
        end;
        if (exists = false) then begin
            items[k] := m;
            //MsgBox('key' + IntToStr(k) +'=' + IntToStr(m), mbConfirmation, MB_OK);
            k := k + 1;
        end;
    end;
    
    i := 0;

    itemWidth := 70;
    itemHeight := 70;
    
    for j := 0 to GetArrayLength(items) - 1 do begin
        if (items[j] = 0) then begin
            btmAmazon := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmAmazon do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('amazon.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\amazon.bmp' );
                OnClick    := @lblAmazonURLOnClick;
                
            end;
            lblAmazon := TLabel.Create(WizardForm.InstallingPage);
            with lblAmazon do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmAmazon.Top + btmAmazon.Height + btmAmazon.Height/8;
                Left       := btmAmazon.Left + (btmAmazon.Width - lblAmazon.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblAmazonURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 1) then begin
            btmAmazonmusic := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmAmazonmusic do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('amazonmusic.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\amazonmusic.bmp' );
                OnClick    := @lblAmazonmusicURLOnClick;
                
            end;
            lblAmazonmusic := TLabel.Create(WizardForm.InstallingPage);
            with lblAmazonmusic do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmAmazonmusic.Top + btmAmazonmusic.Height + btmAmazonmusic.Height/8;
                Left       := btmAmazonmusic.Left + (btmAmazonmusic.Width - lblAmazonmusic.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblAmazonmusicURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 2) then begin
            btmApple := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmApple do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('apple.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\apple.bmp' );
                OnClick    := @lblAppleURLOnClick;
                
            end;
            lblApple := TLabel.Create(WizardForm.InstallingPage);
            with lblApple do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmApple.Top + btmApple.Height + btmApple.Height/8;
                Left       := btmApple.Left + (btmApple.Width - lblApple.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblAppleURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 3) then begin
            btmDailymotion := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmDailymotion do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('dailymotion.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\dailymotion.bmp' );
                OnClick    := @lblDailymotionURLOnClick;
                
            end;
            lblDailymotion := TLabel.Create(WizardForm.InstallingPage);
            with lblDailymotion do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmDailymotion.Top + btmDailymotion.Height + btmDailymotion.Height/8;
                Left       := btmDailymotion.Left + (btmDailymotion.Width - lblDailymotion.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblDailymotionURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 4) then begin
            btmDisney := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmDisney do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('disney.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\disney.bmp' );
                OnClick    := @lblDisneyURLOnClick;
                
            end;
            lblDisney := TLabel.Create(WizardForm.InstallingPage);
            with lblDisney do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmDisney.Top + btmDisney.Height + btmDisney.Height/8;
                Left       := btmDisney.Left + (btmDisney.Width - lblDisney.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblDisneyURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 5) then begin
            btmFacebook := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmFacebook do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('facebook.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\facebook.bmp' );
                OnClick    := @lblFacebookURLOnClick;
                
            end;
            lblFacebook := TLabel.Create(WizardForm.InstallingPage);
            with lblFacebook do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmFacebook.Top + btmFacebook.Height + btmFacebook.Height/8;
                Left       := btmFacebook.Left + (btmFacebook.Width - lblFacebook.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblFacebookURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 6) then begin
            btmFlixgrab := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmFlixgrab do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('flixgrab.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\flixgrab.bmp' );
                OnClick    := @lblFlixgrabURLOnClick;
                
            end;
            lblFlixgrab := TLabel.Create(WizardForm.InstallingPage);
            with lblFlixgrab do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmFlixgrab.Top + btmFlixgrab.Height + btmFlixgrab.Height/8;
                Left       := btmFlixgrab.Left + (btmFlixgrab.Width - lblFlixgrab.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblFlixgrabURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 7) then begin
            btmFlixgrabmusic := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmFlixgrabmusic do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('flixgrabmusic.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\flixgrabmusic.bmp' );
                OnClick    := @lblFlixgrabmusicURLOnClick;
                
            end;
            lblFlixgrabmusic := TLabel.Create(WizardForm.InstallingPage);
            with lblFlixgrabmusic do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmFlixgrabmusic.Top + btmFlixgrabmusic.Height + btmFlixgrabmusic.Height/8;
                Left       := btmFlixgrabmusic.Left + (btmFlixgrabmusic.Width - lblFlixgrabmusic.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblFlixgrabmusicURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 8) then begin
            btmHBO := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmHBO do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('hbo.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\hbo.bmp' );
                OnClick    := @lblHBOURLOnClick;
                
            end;
            lblHBO := TLabel.Create(WizardForm.InstallingPage);
            with lblHBO do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmHBO.Top + btmHBO.Height + btmHBO.Height/8;
                Left       := btmHBO.Left + (btmHBO.Width - lblHBO.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblHBOURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 9) then begin
            btmHulu := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmHulu do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('hulu.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\hulu.bmp' );
                OnClick    := @lblHuluURLOnClick;
                
            end;
            lblHulu := TLabel.Create(WizardForm.InstallingPage);
            with lblHulu do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmHulu.Top + btmHulu.Height + btmHulu.Height/8;
                Left       := btmHulu.Left + (btmHulu.Width - lblHulu.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblHuluURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 10) then begin
            btmInstagram := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmInstagram do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('instagram.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\instagram.bmp' );
                OnClick    := @lblInstagramURLOnClick;
                
            end;
            lblInstagram := TLabel.Create(WizardForm.InstallingPage);
            with lblInstagram do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmInstagram.Top + btmInstagram.Height + btmInstagram.Height/8;
                Left       := btmInstagram.Left + (btmInstagram.Width - lblInstagram.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblInstagramURLOnClick;
            end;
            i := i + 1;
        end;
                
        if (items[j] = 11) then begin
            btmNetflix := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmNetflix do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('netflix.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\netflix.bmp' );
                OnClick    := @lblNetflixURLOnClick;
                
            end;
            lblNetflix := TLabel.Create(WizardForm.InstallingPage);
            with lblNetflix do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmNetflix.Top + btmNetflix.Height + btmNetflix.Height/8;
                Left       := btmNetflix.Left + (btmNetflix.Width - lblNetflix.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblNetflixURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 12) then begin
            btmSpotify := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmSpotify do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('spotify.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\spotify.bmp' );
                OnClick    := @lblSpotifyURLOnClick;
                
            end;
            lblSpotify := TLabel.Create(WizardForm.InstallingPage);
            with lblSpotify do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmSpotify.Top + btmSpotify.Height + btmSpotify.Height/8;
                Left       := btmSpotify.Left + (btmSpotify.Width - lblSpotify.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblSpotifyURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 13) then begin
            btmTwitch := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmTwitch do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('twitch.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\twitch.bmp' );
                OnClick    := @lblTwitchURLOnClick;
                
            end;
            lblTwitch := TLabel.Create(WizardForm.InstallingPage);
            with lblTwitch do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmTwitch.Top + btmTwitch.Height + btmTwitch.Height/8;
                Left       := btmTwitch.Left + (btmTwitch.Width - lblTwitch.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblTwitchURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 14) then begin
            btmTwitter := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmTwitter do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('twitter.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\twitter.bmp' );
                OnClick    := @lblTwitterURLOnClick;
                
            end;
            lblTwitter := TLabel.Create(WizardForm.InstallingPage);
            with lblTwitter do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmTwitter.Top + btmTwitter.Height + btmTwitter.Height/8;
                Left       := btmTwitter.Left + (btmTwitter.Width - lblTwitter.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblTwitterURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 15) then begin
            btmVimeo := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmVimeo do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('vimeo.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\vimeo.bmp' );
                OnClick    := @lblVimeoURLOnClick;
                
            end;
            lblVimeo := TLabel.Create(WizardForm.InstallingPage);
            with lblVimeo do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmVimeo.Top + btmVimeo.Height + btmVimeo.Height/8;
                Left       := btmVimeo.Left + (btmVimeo.Width - lblVimeo.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblVimeoURLOnClick;
            end;
            i := i + 1;
        end;
        
        if (items[j] = 16) then begin
            btmYoutube := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmYoutube do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('youtube.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\youtube.bmp' );
                OnClick    := @lblYoutubeURLOnClick;
                
            end;
            lblYoutube := TLabel.Create(WizardForm.InstallingPage);
            with lblYoutube do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmYoutube.Top + btmYoutube.Height + btmYoutube.Height/8;
                Left       := btmYoutube.Left + (btmYoutube.Width - lblYoutube.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblYoutubeURLOnClick;
            end;
            i := i + 1;
        end;
                
        if (items[j] = 17) then begin
            btmYoutubetomp3 := TBitmapImage.Create(WizardForm.InstallingPage);
            with btmYoutubetomp3 do begin
                Parent     := WizardForm.InstallingPage;
                Width      := itemWidth;
                Height     := itemHeight;
                Top        := WizardForm.ProgressGauge.Top + Height;
                Left       := (1 + i) * ((WizardForm.ProgressGauge.Width - Width * 4)/5) + i * Width;
                Cursor     := crHand;
                AutoSize   := True;
                ExtractTemporaryFile('youtubetomp3.bmp');
                Bitmap.LoadFromFile( ExpandConstant('{tmp}')+'\youtubetomp3.bmp' );
                OnClick    := @lblYoutubetomp3URLOnClick;
                
            end;
            lblYoutubetomp3 := TLabel.Create(WizardForm.InstallingPage);
            with lblYoutubetomp3 do begin
                Parent     := WizardForm.InstallingPage;
                Caption    := ExpandConstant('{cm:productslbl}');
                AutoSize   := True;
                Top        := btmYoutubetomp3.Top + btmYoutubetomp3.Height + btmYoutubetomp3.Height/8;
                Left       := btmYoutubetomp3.Left + (btmYoutubetomp3.Width - lblYoutubetomp3.Width)/2;
                Cursor     := crHand;
                Font.Style := [fsUnderline];
                Font.Color := $ce5811;
                Transparent:= True;
                Alignment  := taCenter;
                OnClick    := @lblYoutubetomp3URLOnClick;
            end;
            i := i + 1;
        end;
    end;
end;