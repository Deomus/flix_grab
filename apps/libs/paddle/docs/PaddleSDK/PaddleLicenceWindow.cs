// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleLicenceWindow
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace PaddleSDK
{
  internal class PaddleLicenceWindow : Form
  {
    /// <summary>The product we are activating.</summary>
    private PaddleProduct Product;
    /// <summary>Required designer variable.</summary>
    private IContainer components;
    private Label txtActivateText;
    private Label lblSubheading;
    private TextBox textboxEmail;
    private Label lblEmail;
    private Label lblKey;
    private TextBox textboxLicence;
    private Label darkStripe;
    private Button btnCancel;
    private Button btnBuyNow;
    private Button btnClose;
    private Button btnKey;
    private Label lblHeadingActivated;
    private Label lblSubheadingActivated;

    public PaddleLicenceWindow(PaddleProduct product)
    {
      this.Product = product;
      this.InitializeComponent();
      this.btnKey.Enabled = false;
      this.btnClose.Visible = false;
      this.lblHeadingActivated.Visible = this.lblSubheadingActivated.Visible = false;
      this.PopulateFromProduct();
    }

    /// <summary>
    /// Shows the window with existing licence information that the user can not edit.
    /// </summary>
    public void SetExistingLicence(string email, string key)
    {
      this.textboxEmail.Text = email;
      this.textboxLicence.Text = key;
      this.textboxEmail.ReadOnly = this.textboxLicence.ReadOnly = true;
      this.btnBuyNow.Visible = this.btnCancel.Visible = this.btnKey.Visible = false;
      this.btnClose.Visible = true;
      this.txtActivateText.Visible = this.lblSubheading.Visible = false;
      this.lblHeadingActivated.Visible = this.lblSubheadingActivated.Visible = true;
    }

    /// <summary>
    /// Populates our window components from stored product data.
    /// </summary>
    private void PopulateFromProduct()
    {
      this.txtActivateText.Text = string.Format(this.txtActivateText.Text, (object) this.Product.ProductData.ProductName);
      this.lblSubheading.Text = string.Format(this.lblSubheading.Text, (object) this.Product.ProductData.ProductName);
      this.lblHeadingActivated.Text = string.Format(this.lblHeadingActivated.Text, (object) this.Product.ProductData.ProductName);
      this.lblSubheadingActivated.Text = string.Format(this.lblSubheadingActivated.Text, (object) this.Product.ProductData.ProductName);
    }

    /// <summary>
    /// Handles when the user clicks the Activate button to activate a licence.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void btnKey_Click(object sender, EventArgs e)
    {
      this.btnBuyNow.Enabled = this.btnKey.Enabled = this.btnCancel.Enabled = false;
      if (!this.Product.ActivateLicence(this.textboxEmail.Text, this.textboxLicence.Text))
      {
        int num1 = (int) MessageBox.Show(string.Format(Resources.VerifyFailDialogBody, (object) this.Product.LastErrorMessage), Resources.VerifyFailDialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
      }
      else
      {
        int num2 = (int) MessageBox.Show(string.Format(Resources.VerifySucceedDialogBody, (object) this.Product.ProductData.ProductName), Resources.VerifySucceedDialogTitle, MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
        this.DialogResult = DialogResult.OK;
        this.Close();
      }
      this.btnBuyNow.Enabled = this.btnKey.Enabled = this.btnCancel.Enabled = true;
    }

    /// <summary>Handles when the email text box is changed.</summary>
    private void textboxEmail_TextChanged(object sender, EventArgs e)
    {
      this.btnKey.Enabled = !string.IsNullOrEmpty(this.textboxEmail.Text) && !string.IsNullOrEmpty(this.textboxLicence.Text);
    }

    /// <summary>Handles when the licence text box is changed.</summary>
    private void textboxLicence_TextChanged(object sender, EventArgs e)
    {
      this.textboxEmail_TextChanged(sender, e);
    }

    /// <summary>Clean up any resources being used.</summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && this.components != null)
        this.components.Dispose();
      base.Dispose(disposing);
    }

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof (PaddleLicenceWindow));
      this.txtActivateText = new Label();
      this.lblSubheading = new Label();
      this.textboxEmail = new TextBox();
      this.lblEmail = new Label();
      this.lblKey = new Label();
      this.textboxLicence = new TextBox();
      this.darkStripe = new Label();
      this.btnCancel = new Button();
      this.btnBuyNow = new Button();
      this.btnClose = new Button();
      this.btnKey = new Button();
      this.lblHeadingActivated = new Label();
      this.lblSubheadingActivated = new Label();
      this.SuspendLayout();
      componentResourceManager.ApplyResources((object) this.txtActivateText, "txtActivateText");
      this.txtActivateText.BackColor = Color.White;
      this.txtActivateText.Name = "txtActivateText";
      componentResourceManager.ApplyResources((object) this.lblSubheading, "lblSubheading");
      this.lblSubheading.AutoEllipsis = true;
      this.lblSubheading.BackColor = Color.White;
      this.lblSubheading.Name = "lblSubheading";
      componentResourceManager.ApplyResources((object) this.textboxEmail, "textboxEmail");
      this.textboxEmail.Name = "textboxEmail";
      this.textboxEmail.TextChanged += new EventHandler(this.textboxEmail_TextChanged);
      componentResourceManager.ApplyResources((object) this.lblEmail, "lblEmail");
      this.lblEmail.Name = "lblEmail";
      componentResourceManager.ApplyResources((object) this.lblKey, "lblKey");
      this.lblKey.Name = "lblKey";
      componentResourceManager.ApplyResources((object) this.textboxLicence, "textboxLicence");
      this.textboxLicence.Name = "textboxLicence";
      this.textboxLicence.TextChanged += new EventHandler(this.textboxLicence_TextChanged);
      componentResourceManager.ApplyResources((object) this.darkStripe, "darkStripe");
      this.darkStripe.BackColor = Color.White;
      this.darkStripe.Name = "darkStripe";
      componentResourceManager.ApplyResources((object) this.btnCancel, "btnCancel");
      this.btnCancel.DialogResult = DialogResult.Cancel;
      this.btnCancel.Name = "btnCancel";
      this.btnCancel.UseVisualStyleBackColor = true;
      componentResourceManager.ApplyResources((object) this.btnBuyNow, "btnBuyNow");
      this.btnBuyNow.DialogResult = DialogResult.Retry;
      this.btnBuyNow.Name = "btnBuyNow";
      this.btnBuyNow.UseVisualStyleBackColor = true;
      componentResourceManager.ApplyResources((object) this.btnClose, "btnClose");
      this.btnClose.DialogResult = DialogResult.Cancel;
      this.btnClose.Name = "btnClose";
      this.btnClose.UseVisualStyleBackColor = true;
      componentResourceManager.ApplyResources((object) this.btnKey, "btnKey");
      this.btnKey.Name = "btnKey";
      this.btnKey.UseVisualStyleBackColor = true;
      this.btnKey.Click += new EventHandler(this.btnKey_Click);
      componentResourceManager.ApplyResources((object) this.lblHeadingActivated, "lblHeadingActivated");
      this.lblHeadingActivated.BackColor = Color.White;
      this.lblHeadingActivated.Name = "lblHeadingActivated";
      componentResourceManager.ApplyResources((object) this.lblSubheadingActivated, "lblSubheadingActivated");
      this.lblSubheadingActivated.AutoEllipsis = true;
      this.lblSubheadingActivated.BackColor = Color.White;
      this.lblSubheadingActivated.Name = "lblSubheadingActivated";
      componentResourceManager.ApplyResources((object) this, "$this");
      this.AutoScaleMode = AutoScaleMode.Font;
      this.Controls.Add((Control) this.lblSubheadingActivated);
      this.Controls.Add((Control) this.lblHeadingActivated);
      this.Controls.Add((Control) this.btnBuyNow);
      this.Controls.Add((Control) this.btnCancel);
      this.Controls.Add((Control) this.lblKey);
      this.Controls.Add((Control) this.textboxLicence);
      this.Controls.Add((Control) this.lblEmail);
      this.Controls.Add((Control) this.textboxEmail);
      this.Controls.Add((Control) this.lblSubheading);
      this.Controls.Add((Control) this.txtActivateText);
      this.Controls.Add((Control) this.darkStripe);
      this.Controls.Add((Control) this.btnKey);
      this.Controls.Add((Control) this.btnClose);
      this.FormBorderStyle = FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "PaddleLicenceWindow";
      this.ShowInTaskbar = false;
      this.ResumeLayout(false);
      this.PerformLayout();
    }
  }
}
