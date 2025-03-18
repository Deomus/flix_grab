using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace SoftActivate.Licensing
{
    [System.Runtime.InteropServices.ComVisibleAttribute(true)]
    public partial class LicensingForm : Form
    {   
        public LicensingForm(LicenseTemplate licenseTemplate)
        {
            InitializeComponent();

            this.licenseTemplate = licenseTemplate;

            this.MaximumSize = this.MinimumSize = this.Size;

            webBrowser.ObjectForScripting = this;
            webBrowser.AllowNavigation = false;
            webBrowser.IsWebBrowserContextMenuEnabled = false;
            webBrowser.AllowWebBrowserDrop = false;
            webBrowser.ScriptErrorsSuppressed = true;

            //ContextMenuStrip contextMenu = new ContextMenuStrip();
            //contextMenu.Items.Add("Paste", null, Paste_Click);
            //webBrowser.ContextMenuStrip = contextMenu;

            webBrowser.PreviewKeyDown += new PreviewKeyDownEventHandler(webBrowser_PreviewKeyDown);

            webBrowser.Navigate("about:blank");
        }

        public void StartProgress(string message = null)
        {
            if (InvokeRequired)
            {
                Invoke(new Action<string>(StartProgress), message);
            }
            else
            {
                webBrowser.Document.InvokeScript("startProgress", new object[] { });
            }
        }

        public void StopProgress()
        {
            if (InvokeRequired)
            {
                Invoke(new Action(StopProgress));
            }
            else
            {
                webBrowser.Document.InvokeScript("stopProgress", new object[] { });
            }
        }

        public void DisplayError(string message = null)
        {
            if (InvokeRequired)
            {
                Invoke(new Action<string>(DisplayError), message);
            }
            else
            {
                webBrowser.Document.InvokeScript("displayError", new object[] { message });
                SetViewMode(ViewMode.Error);
            }
        }

        new public void Close()
        {
            if (InvokeRequired)
            {
                Invoke(new Action(Close));
            }
            else
            {
                base.Close();
            }
        }

        private void webBrowser_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            switch (e.KeyData)
            {
                //case Keys.Control | Keys.A: // Select All
                case Keys.Control | Keys.C: // Copy
                case Keys.Control | Keys.V: // Paste
                case Keys.Delete: // Delete
                    ((WebBrowser)sender).WebBrowserShortcutsEnabled = true;
                    break;

                default:
                    ((WebBrowser)sender).WebBrowserShortcutsEnabled = false;
                    break;
            }
        }

        private void Copy_Click(object sender, EventArgs e)
        {
            webBrowser.Document.ExecCommand("Copy", false, null);
        }

        private void Paste_Click(object sender, EventArgs e)
        {
            webBrowser.Document.ExecCommand("Paste", false, null);
        }

        private void OnKeyDown(object sender, HtmlElementEventArgs e)
        {
            if (e.CtrlKeyPressed)
            {
                if (e.KeyPressedCode == 'C')
                {
                    webBrowser.Document.ExecCommand("Copy", false, null);
                }

                if (e.KeyPressedCode == 'V')
                {
                    webBrowser.Document.ExecCommand("Paste", false, null);
                }
            }
        }

        private string MatchEvaluator(Match match)
        {
            string original = match.Groups[0].Value;
            string key;
            bool isImage = false;
            bool isCssUrl = false;

            if (match.Groups.Count < 8)
                return original;

            if (match.Groups[1].Success)
                key = match.Groups[1].Value;
            else
                if (match.Groups[3].Success)
                    key = match.Groups[3].Value;
                else
                    if (match.Groups[5].Success)
                    {
                        key = match.Groups[5].Value;
                        isImage = true;
                    }
                    else
                        if (match.Groups[7].Success)
                        {
                            key = match.Groups[7].Value;
                            isCssUrl = true;
                        } else
                            return original;

            string replace = licenseTemplate.GetProperty("/UserInterface", key);
            if (String.IsNullOrEmpty(replace))
                replace = licenseTemplate.GetProperty("/UserInterface/" + key, null);

            if (!String.IsNullOrEmpty(replace))
            {
                if (isImage) replace = "<img src=\"" + replace + "\" />";
                else
                    if (isCssUrl)
                        replace = "url(\"" + replace + "\")";

                return replace;
            }

            return original;
        }

        private void CustomizeHtmlTemplate()
        {
            htmlTemplate = Regex.Replace(htmlTemplate,
                                     @"(?:/\*##([^#]+?)##\*/(?!url)(.*?)/\*##\*/)|(?:<!--##([^#]+?)##-->([^<]*?)<!--##-->)|(?:<!--##([^#]+?)##--><img\s.*?src\s*=\s*""([^""]*?)"".*?/><!--##-->)|(?:/\*##([^#]+?)##\*/url\((.*?)\)/\*##\*/)",
                                     MatchEvaluator,
                                     RegexOptions.CultureInvariant | RegexOptions.Singleline);
        }

        private bool documentCompleted = false;

        private void webBrowser_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            if (!documentCompleted)
            {
                Stream stream = Assembly.GetExecutingAssembly().GetManifestResourceStream("SoftActivate.Licensing.LicensingForm.htm");
                
                htmlTemplate = (new StreamReader(stream)).ReadToEnd();

                CustomizeHtmlTemplate();

                WebBrowserExtensions.LoadHtmlIntoBrowser(webBrowser, htmlTemplate, "http://localhost");
                documentCompleted = true;
                
                return;
            }

            HtmlDocument ui = webBrowser.Document;

            ui.GetElementById("btnRegister").AttachEventHandler("onclick", RegisterBtn_Click);
            ui.GetElementById("btnRegister2").AttachEventHandler("onclick", RegisterBtn_Click);
            ui.GetElementById("btnStartTrial").AttachEventHandler("onclick", StartTrialBtn_Click);
            ui.GetElementById("btnContinueTrial").AttachEventHandler("onclick", ContinueTrialBtn_Click);
            ui.GetElementById("btnBuyNow").AttachEventHandler("onclick", BuyNowBtn_Click);
            ui.GetElementById("btnCloseWindow").AttachEventHandler("onclick", CloseWindowBtn_Click);

            ui.GetElementById("btnExit").AttachEventHandler("onclick", ExitBtn_Click);
            ui.GetElementById("btnContinue").AttachEventHandler("onclick", ExitBtn_Click);

            ui.GetElementById("btnBack").AttachEventHandler("onclick", BackBtn_Click);
            ui.GetElementById("btnBack2").AttachEventHandler("onclick", BackBtn_Click);

            ui.GetElementById("btnRegisterOffline").AttachEventHandler("onclick", RegisterOfflineBtn_Click);
            ui.GetElementById("btnRegisterOffline2").AttachEventHandler("onclick", RegisterOfflineBtn_Click);
            ui.GetElementById("btnCloseAndWait").AttachEventHandler("onclick", CloseWaitActivationKey_Click);

            ui.MouseDown += new HtmlElementEventHandler(webBrowser_OnMouseDown);
            ui.MouseUp += new HtmlElementEventHandler(webBrowser_OnMouseUp);
            ui.MouseMove += new HtmlElementEventHandler(webBrowser_OnMouseMove);

            string val;

            if (!String.IsNullOrEmpty(val = licenseTemplate.GetProperty("/General/CompanyName", null)))
                ui.GetElementById("txtCompanyName").InnerText = val;
            else
                ui.GetElementById("txtCompanyName").InnerText = "";

            if (!String.IsNullOrEmpty(val = licenseTemplate.GetProperty("/General/ProductName", null)))
                ui.GetElementById("txtProductName").InnerText = val;
            else
                ui.GetElementById("txtProductName").InnerText = "";

            if (!String.IsNullOrEmpty(val = licenseTemplate.GetProperty("/General/ProductSupportUrl", null)))
            {
                ui.GetElementById("btnContactSupport").AttachEventHandler("onclick", ContactSupportBtn_Click);
                ui.InvokeScript("setControlVisibility", new object[] { "btnContactSupport", true });
            }
            else
                ui.InvokeScript("setControlVisibility", new object[] { "btnContactSupport", false });

            if (!String.IsNullOrEmpty(licenseKey))
                ui.GetElementById("txtLicenseKey").InnerText = licenseKey;

            if (!String.IsNullOrEmpty(registrationName))
                ui.GetElementById("txtRegistrationName").InnerText = registrationName;

            if (!String.IsNullOrEmpty(registrationEmail))
                ui.GetElementById("txtRegistrationEmail").InnerText = registrationEmail;

            if (!String.IsNullOrEmpty(hardwareId))
                ui.GetElementById("txtHardwareId").InnerText = hardwareId;

            if (!String.IsNullOrEmpty(activationKey))
                ui.GetElementById("txtActivationKey").InnerText = activationKey;

            SetViewMode(viewMode);
        }

        private bool dragging = false;
        private Point dragCursorPoint;
        private Point dragFormPoint;

        private void webBrowser_OnMouseDown(object sender, HtmlElementEventArgs args)
        {
            dragging = true;
            OnMouseDown(new MouseEventArgs(args.MouseButtonsPressed, 1, args.MousePosition.X, args.MousePosition.Y, 0));
        }

        private void webBrowser_OnMouseUp(object sender, HtmlElementEventArgs args)
        {
            OnMouseUp(new MouseEventArgs(args.MouseButtonsPressed, 1, args.MousePosition.X, args.MousePosition.Y, 0));
        }

        private void webBrowser_OnMouseMove(object sender, HtmlElementEventArgs args)
        {
            OnMouseMove(new MouseEventArgs(args.MouseButtonsPressed, 1, args.MousePosition.X, args.MousePosition.Y, 0));
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            dragging = true;
            dragCursorPoint = Cursor.Position;
            dragFormPoint = this.Location;

            base.OnMouseDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            dragging = false;
            
            base.OnMouseUp(e);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (dragging)
            {
                Point dif = Point.Subtract(Cursor.Position, new Size(dragCursorPoint));
                this.Location = Point.Add(dragFormPoint, new Size(dif));
            }

            base.OnMouseMove(e);
        }

        private void RegisterBtn_Click(object sender, EventArgs args)
        {
            Register_Click(sender, args, false);
        }

        private void RegisterOfflineBtn_Click(object sender, EventArgs args)
        {
            Register_Click(sender, args, true);
        }

        private void Register_Click(object sender, EventArgs args, bool offlineRegistration)
        {
            LicensingFormEventArgs newArgs = new LicensingFormEventArgs(licenseKey);

            newArgs.offlineRegistration = offlineRegistration;

            lock (this)
            {
                HtmlDocument ui = webBrowser.Document;
                newArgs.licenseKey = ui.GetElementById("txtLicenseKey").GetAttribute("value");
                newArgs.registrationName = ui.GetElementById("txtRegistrationName").GetAttribute("value");
                newArgs.registrationEmail = ui.GetElementById("txtRegistrationEmail").GetAttribute("value");
                newArgs.activationKey = ui.GetElementById("txtActivationKey").GetAttribute("value");
            }

            OnRegister(this, newArgs);
        }

        private void CloseWaitActivationKey_Click(object sender, EventArgs args)
        {
            Close();
        }

        private void StartTrialBtn_Click(object sender, EventArgs args)
        {
            OnStartTrial(sender, args);
        }

        private void ContinueTrialBtn_Click(object sender, EventArgs args)
        {
            OnContinueTrial(sender, args);
        }

        private void BuyNowBtn_Click(object sender, EventArgs args)
        {
            OnBuyNow(sender, args);
        }

        private void ContactSupportBtn_Click(object sender, EventArgs args)
        {
            OnContactSupport(sender, args);
        }

        private void ExitBtn_Click(object sender, EventArgs args)
        {
            Close();
        }

        private void BackBtn_Click(object sender, EventArgs args)
        {
            if (previousViewModes.Count > 0)
                SetViewMode(previousViewModes.Pop());
        }

        private void CloseWindowBtn_Click(object sender, EventArgs args)
        {
            Close();
        }

        private delegate void SetStringPropertyDelegate(object o, object val, object[] index);

        internal string LicenseKey
        {
            get
            {
                return licenseKey;
            }
            set
            {
                licenseKey = value;

                if (InvokeRequired)
                    Invoke(new SetStringPropertyDelegate(GetType().GetProperty("LicenseKey").SetValue), this, licenseKey, null);
                else
                {
                    if (documentCompleted)
                    {
                        webBrowser.Document.GetElementById("txtLicenseKey").InnerText = value;
                    }
                }
            }
        }

        private void UISetRegistrationName(string name)
        {
            if (InvokeRequired)
                Invoke(new Action<string>(UISetRegistrationName), name);
            else
                if (documentCompleted)
                    webBrowser.Document.GetElementById("txtRegistrationName").InnerText = name;
        }

        private void UISetRegistrationEmail(string email)
        {
            if (InvokeRequired)
                Invoke(new Action<string>(UISetRegistrationEmail), email);
            else
                if (documentCompleted)
                    webBrowser.Document.GetElementById("txtRegistrationEmail").InnerText = email;
        }

        private void UISetHardwareId(string hwid)
        {
            if (InvokeRequired)
                Invoke(new Action<string>(UISetHardwareId), hwid);
            else
                if (documentCompleted)
                    webBrowser.Document.GetElementById("txtHardwareId").InnerText = hwid;
        }

        internal string RegistrationName
        {
            get
            {
                return registrationName;
            }
            set
            {
                registrationName = value;
                UISetRegistrationName(value);
            }
        }

        internal string RegistrationEmail
        {
            get
            {
                return registrationEmail;
            }
            set
            {
                registrationEmail = value;
                UISetRegistrationEmail(value);
            }
        }

        internal string HardwareId
        {
            get
            {
                return hardwareId;
            }
            set
            {
                hardwareId = value;
                UISetHardwareId(value);
            }
        }

        internal string ActivationKey
        {
            get
            {
                return activationKey;
            }
            set
            {
                activationKey = value;

                if (InvokeRequired)
                    Invoke(new SetStringPropertyDelegate(GetType().GetProperty("ActivationKey").SetValue), this, activationKey, null);
                else
                {
                    if (documentCompleted)
                    {
                        webBrowser.Document.GetElementById("txtActivationKey").InnerText = value;
                    }
                }
            }
        }

        internal void SetTrialDaysLeft(int days)
        {
            trialDaysLeft = days;
        }

        private delegate void Action();

        private string licenseKey;
        private string registrationName;
        private string registrationEmail;
        private string activationKey;
        private string hardwareId;

        private LicenseTemplate licenseTemplate;
        private string htmlTemplate;
        private ViewMode viewMode = ViewMode.FirstRunWithTrial;
        private Stack<ViewMode> previousViewModes = new Stack<ViewMode>();
        private int trialDaysLeft = 0;
        private DialogResult exitResult = DialogResult.Cancel;

        public event StartTrialEventHandler OnStartTrial;
        public event ContinueTrialEventHandler OnContinueTrial;
        public event RegisterEventHandler OnRegister;
        public event BuyNowEventHandler OnBuyNow;
        public event ContactSupportEventHandler OnContactSupport;

        internal enum ViewMode
        {
            Error,
            Success,
            FirstRunNoTrial,
            FirstRunWithTrial,
            Trial,
            TrialExpired,
            Register,
            RegisterWithEmail,
            RegisterWithName,
            RegisterOffline,
            RegisterOfflineWithName,
            RegisterOfflineWithEmail
        }

        internal void SetViewMode(ViewMode mode)
        {
            if (InvokeRequired)
                Invoke(new Action<ViewMode>(SetViewMode), mode);
            else
            {
                if (documentCompleted)
                {
                    string page = "mainPage";

                    switch (mode)
                    {
                        case ViewMode.Error:
                            page = "errorPage";
                            break;

                        case ViewMode.FirstRunNoTrial:
                            page = "trialPage";
                            break;
                        
                        case ViewMode.FirstRunWithTrial:
                            webBrowser.Document.InvokeScript("setTrialDaysLeft", new object[] { null });
                            page = "trialPage";
                            break;

                        case ViewMode.Trial:
                            webBrowser.Document.InvokeScript("setTrialDaysLeft", new object[] { trialDaysLeft });
                            page = "trialPage";
                            break;

                        case ViewMode.TrialExpired:
                            webBrowser.Document.InvokeScript("setTrialDaysLeft", new object[] { 0 });
                            page = "trialPage";
                            break;

                        case ViewMode.Register:
                            webBrowser.Document.InvokeScript("setRegistrationFieldsVisibility", new object[] { false, false, false });
                            page = "mainPage";
                            break;

                        case ViewMode.RegisterWithEmail:
                            webBrowser.Document.InvokeScript("setRegistrationFieldsVisibility", new object[] { false, true, false });
                            page = "mainPage";
                            break;

                        case ViewMode.RegisterWithName:
                            webBrowser.Document.InvokeScript("setRegistrationFieldsVisibility", new object[] { true, false, false });
                            page = "mainPage";
                            break;

                        case ViewMode.RegisterOffline:
                            webBrowser.Document.InvokeScript("setRegistrationFieldsVisibility", new object[] { false, false, true });
                            break;

                        case ViewMode.RegisterOfflineWithName:
                            webBrowser.Document.InvokeScript("setRegistrationFieldsVisibility", new object[] { true, false, true });
                            break;

                        case ViewMode.RegisterOfflineWithEmail:
                            webBrowser.Document.InvokeScript("setRegistrationFieldsVisibility", new object[] { false, true, true });
                            break;

                        case ViewMode.Success:
                            page = "successPage";
                            break;
                    }
                    
                    webBrowser.Document.InvokeScript("setPage", new object[] { page });
                }

                this.viewMode = mode;
            }
        }

        internal ViewMode GetViewMode()
        {
            return viewMode;
        }

        private void SaveRegistrationData()
        {
            if (InvokeRequired)
            {
                Invoke(new Action(SaveRegistrationData));
            } else
                if (documentCompleted)
                {
                    HtmlDocument ui = webBrowser.Document;

                    licenseKey = ui.GetElementById("txtLicenseKey").GetAttribute("value");
                    registrationName = ui.GetElementById("txtRegistrationName").GetAttribute("value");
                    registrationEmail = ui.GetElementById("txtRegistrationEmail").GetAttribute("value");
                }
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            SaveRegistrationData();

            base.OnClosing(e);

            DialogResult = exitResult;
        }

        internal void SetDialogResult(DialogResult exitResult)
        {
            this.exitResult = exitResult;
        }

        internal void MemorizeCurrentViewMode()
        {
            previousViewModes.Push(this.viewMode);
        }
    }

    public class LicensingFormEventArgs : EventArgs
    {
        public LicensingFormEventArgs(string licenseKey)
        {
            this.licenseKey = licenseKey;
        }

        public string LicenseKey
        {
            get
            {
                return licenseKey;
            }
        }

        public string RegistrationName
        {
            get
            {
                return registrationName;
            }
        }

        public string RegistrationEmail
        {
            get
            {
                return registrationEmail;
            }
        }

        public bool OfflineRegistration
        {
            get
            {
                return offlineRegistration;
            }
        }

        public string ActivationKey
        {
            get
            {
                return activationKey;
            }
        }

        internal string licenseKey;
        internal string registrationName;
        internal string registrationEmail;
        internal bool offlineRegistration;
        internal string activationKey;
    }

    public delegate void StartTrialEventHandler(object sender, EventArgs e);
    public delegate void ContinueTrialEventHandler(object sender, EventArgs e);
    public delegate void RegisterEventHandler(object sender, EventArgs e);
    public delegate void BuyNowEventHandler(object sender, EventArgs e);
    public delegate void ContactSupportEventHandler(object sender, EventArgs e);
}
