namespace EmailOrder
{
    partial class ProjectInstaller
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.EmailOrderProcessInstaller = new System.ServiceProcess.ServiceProcessInstaller();
            this.EmailOrderInstaller = new System.ServiceProcess.ServiceInstaller();
            // 
            // EmailOrderProcessInstaller
            // 
            this.EmailOrderProcessInstaller.Account = System.ServiceProcess.ServiceAccount.LocalSystem;
            this.EmailOrderProcessInstaller.Password = null;
            this.EmailOrderProcessInstaller.Username = null;
            // 
            // EmailOrderInstaller
            // 
            this.EmailOrderInstaller.Description = "Complete License Key by order form email";
            this.EmailOrderInstaller.DisplayName = "Email Order Service";
            this.EmailOrderInstaller.ServiceName = "EmailOrderService";
            this.EmailOrderInstaller.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.EmailOrderProcessInstaller,
            this.EmailOrderInstaller});

        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller EmailOrderProcessInstaller;
        private System.ServiceProcess.ServiceInstaller EmailOrderInstaller;
    }
}