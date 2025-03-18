// Decompiled with JetBrains decompiler
// Type: PaddleSDK.PaddleProductWindow
// Assembly: PaddleSDK, Version=1.0.0.0, Culture=neutral, PublicKeyToken=b2cf3dc44105319d
// MVID: C7382385-AD81-415C-84F8-E0CBBE666E17
// Assembly location: D:\Projects\fga_source\paddle\Windows-Framework-master\PaddleSDK.dll

using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Net;
using System.Windows.Forms;

namespace PaddleSDK
{
  internal class PaddleProductWindow : Form
  {
    /// <summary>The Product we use to interact with.</summary>
    private PaddleProduct Product;
    /// <summary>The Id of the product being licensed.</summary>
    private string ProductId;
    /// <summary>Required designer variable.</summary>
    private IContainer components;
    private Label txtTrialText;
    private Label darkStripe;
    private ProgressBar trialProgress;
    private Label txtDaysRemaining;
    private Label label2;
    private Button btnContinue;
    private Button btnLicence;
    private Button btnBuy;
    private PictureBox imgIcon;
    private Label txtPricingVendorName;
    private Label txtPricingProductName;
    private Label txtPrice;
    private Label txtProductName;
    private Label lblThanksForTrying;
    private Button btnQuit;

    public PaddleProductWindow(PaddleProduct product, string productId)
    {
      this.Product = product;
      this.ProductId = productId;
      this.InitializeComponent();
      this.PopulateFromProduct();
    }

    /// <summary>
    /// Populates our window components from stored product data.
    /// </summary>
    private void PopulateFromProduct()
    {
      this.txtProductName.Text = this.Text = this.Product.ProductData.ProductName;
      this.lblThanksForTrying.Text = string.Format(this.lblThanksForTrying.Text, (object) this.Product.ProductData.ProductName);
      this.txtTrialText.Text = this.Product.ProductData.TrialText;
      int val2 = this.Product.DaysRemainingOnTrial > 0 ? this.Product.DaysRemainingOnTrial : 0;
      this.trialProgress.Minimum = 0;
      this.trialProgress.Maximum = Math.Max((int) this.Product.ProductData.TrialLength, val2);
      this.trialProgress.Value = val2;
      string str1 = new ComponentResourceManager(typeof (PaddleProductWindow)).GetString("txtDaysRemaining.One.Text");
      string text = this.txtDaysRemaining.Text;
      this.txtDaysRemaining.Text = string.Format(val2 != 1 || str1 == null ? text : str1, (object) val2);
      this.txtPricingProductName.Text = this.Product.ProductData.ProductName;
      this.txtPricingVendorName.Text = string.Format(this.txtPricingVendorName.Text, (object) this.Product.ProductData.DeveloperName);
      string str2 = this.Product.ProductData.Currency;
      switch (str2)
      {
        case "GBP":
          str2 = "£";
          break;
        case "EUR":
          str2 = "€";
          break;
        case "USD":
          str2 = "USD $";
          break;
      }
      this.txtPrice.Text = string.Format("{0} {1:F2}", (object) str2, (object) this.Product.ProductData.Price);
      bool flag = false;
      if (!string.IsNullOrEmpty(this.Product.ProductData.ImageUrl))
      {
        try
        {
          this.imgIcon.ImageLocation = this.Product.ProductData.ImageUrl;
          this.imgIcon.Load();
          flag = true;
        }
        catch (WebException ex)
        {
        }
      }
      if (!flag)
        this.imgIcon.Visible = false;
      this.btnQuit.Visible = !(this.btnContinue.Visible = this.Product.DaysRemainingOnTrial > 0);
    }

    /// <summary>Handles when the buy button is clicked.</summary>
    private void btnBuy_Click(object sender, EventArgs e)
    {
      string fileName = string.Format("https://checkout.paddle.com/checkout/product/{0}", (object) this.ProductId);
      try
      {
        Process.Start(fileName);
      }
      catch (Win32Exception ex)
      {
        if (ex.ErrorCode != -2147467259)
          return;
        int num = (int) MessageBox.Show("No browser was found to start payment", (string) null, MessageBoxButtons.OK, MessageBoxIcon.Hand);
      }
      catch (Exception ex)
      {
        int num = (int) MessageBox.Show(ex.Message, (string) null, MessageBoxButtons.OK, MessageBoxIcon.Hand);
      }
    }

    /// <summary>
    /// Handles when the users wants to show the licence key dialog.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void btnLicence_Click(object sender, EventArgs e)
    {
      switch (new PaddleLicenceWindow(this.Product).ShowDialog())
      {
        case DialogResult.OK:
          this.DialogResult = DialogResult.OK;
          this.Close();
          break;
        case DialogResult.Retry:
          this.btnBuy_Click(sender, e);
          break;
      }
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
      ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof (PaddleProductWindow));
      this.lblThanksForTrying = new Label();
      this.txtPrice = new Label();
      this.txtPricingProductName = new Label();
      this.txtTrialText = new Label();
      this.darkStripe = new Label();
      this.trialProgress = new ProgressBar();
      this.txtDaysRemaining = new Label();
      this.label2 = new Label();
      this.btnContinue = new Button();
      this.btnLicence = new Button();
      this.btnBuy = new Button();
      this.imgIcon = new PictureBox();
      this.txtPricingVendorName = new Label();
      this.txtProductName = new Label();
      this.btnQuit = new Button();
      Label label = new Label();
      ((ISupportInitialize) this.imgIcon).BeginInit();
      this.SuspendLayout();
      componentResourceManager.ApplyResources((object) label, "lblBuy");
      label.BackColor = Color.White;
      label.ForeColor = Color.FromArgb(83, 83, 83);
      label.Name = "lblBuy";
      componentResourceManager.ApplyResources((object) this.lblThanksForTrying, "lblThanksForTrying");
      this.lblThanksForTrying.AutoEllipsis = true;
      this.lblThanksForTrying.BackColor = Color.Transparent;
      this.lblThanksForTrying.ForeColor = Color.FromArgb(83, 83, 83);
      this.lblThanksForTrying.Name = "lblThanksForTrying";
      componentResourceManager.ApplyResources((object) this.txtPrice, "txtPrice");
      this.txtPrice.BackColor = Color.White;
      this.txtPrice.ForeColor = Color.FromArgb(83, 83, 83);
      this.txtPrice.Name = "txtPrice";
      componentResourceManager.ApplyResources((object) this.txtPricingProductName, "txtPricingProductName");
      this.txtPricingProductName.AutoEllipsis = true;
      this.txtPricingProductName.BackColor = Color.White;
      this.txtPricingProductName.ForeColor = Color.FromArgb(83, 83, 83);
      this.txtPricingProductName.Name = "txtPricingProductName";
      componentResourceManager.ApplyResources((object) this.txtTrialText, "txtTrialText");
      this.txtTrialText.AutoEllipsis = true;
      this.txtTrialText.BackColor = Color.Transparent;
      this.txtTrialText.ForeColor = Color.FromArgb(110, 110, 110);
      this.txtTrialText.Name = "txtTrialText";
      componentResourceManager.ApplyResources((object) this.darkStripe, "darkStripe");
      this.darkStripe.BackColor = Color.FromArgb(216, 216, 216);
      this.darkStripe.Name = "darkStripe";
      componentResourceManager.ApplyResources((object) this.trialProgress, "trialProgress");
      this.trialProgress.Name = "trialProgress";
      this.trialProgress.Value = 50;
      componentResourceManager.ApplyResources((object) this.txtDaysRemaining, "txtDaysRemaining");
      this.txtDaysRemaining.AutoEllipsis = true;
      this.txtDaysRemaining.BackColor = Color.FromArgb(216, 216, 216);
      this.txtDaysRemaining.ForeColor = Color.FromArgb(83, 83, 83);
      this.txtDaysRemaining.Name = "txtDaysRemaining";
      componentResourceManager.ApplyResources((object) this.label2, "label2");
      this.label2.BackColor = Color.White;
      this.label2.Name = "label2";
      componentResourceManager.ApplyResources((object) this.btnContinue, "btnContinue");
      this.btnContinue.DialogResult = DialogResult.OK;
      this.btnContinue.Name = "btnContinue";
      this.btnContinue.UseVisualStyleBackColor = true;
      componentResourceManager.ApplyResources((object) this.btnLicence, "btnLicence");
      this.btnLicence.Name = "btnLicence";
      this.btnLicence.UseVisualStyleBackColor = true;
      this.btnLicence.Click += new EventHandler(this.btnLicence_Click);
      componentResourceManager.ApplyResources((object) this.btnBuy, "btnBuy");
      this.btnBuy.Name = "btnBuy";
      this.btnBuy.UseVisualStyleBackColor = true;
      this.btnBuy.Click += new EventHandler(this.btnBuy_Click);
      componentResourceManager.ApplyResources((object) this.imgIcon, "imgIcon");
      this.imgIcon.BackColor = Color.White;
      this.imgIcon.Name = "imgIcon";
      this.imgIcon.TabStop = false;
      componentResourceManager.ApplyResources((object) this.txtPricingVendorName, "txtPricingVendorName");
      this.txtPricingVendorName.AutoEllipsis = true;
      this.txtPricingVendorName.BackColor = Color.White;
      this.txtPricingVendorName.ForeColor = Color.FromArgb(110, 110, 110);
      this.txtPricingVendorName.Name = "txtPricingVendorName";
      componentResourceManager.ApplyResources((object) this.txtProductName, "txtProductName");
      this.txtProductName.AutoEllipsis = true;
      this.txtProductName.BackColor = Color.Transparent;
      this.txtProductName.ForeColor = Color.FromArgb(83, 83, 83);
      this.txtProductName.Name = "txtProductName";
      componentResourceManager.ApplyResources((object) this.btnQuit, "btnQuit");
      this.btnQuit.DialogResult = DialogResult.OK;
      this.btnQuit.Name = "btnQuit";
      this.btnQuit.UseVisualStyleBackColor = true;
      componentResourceManager.ApplyResources((object) this, "$this");
      this.AutoScaleMode = AutoScaleMode.Font;
      this.BackColor = Color.FromArgb(237, 237, 237);
      this.ControlBox = false;
      this.Controls.Add((Control) label);
      this.Controls.Add((Control) this.txtPrice);
      this.Controls.Add((Control) this.txtPricingVendorName);
      this.Controls.Add((Control) this.txtPricingProductName);
      this.Controls.Add((Control) this.imgIcon);
      this.Controls.Add((Control) this.btnBuy);
      this.Controls.Add((Control) this.btnLicence);
      this.Controls.Add((Control) this.label2);
      this.Controls.Add((Control) this.txtDaysRemaining);
      this.Controls.Add((Control) this.trialProgress);
      this.Controls.Add((Control) this.darkStripe);
      this.Controls.Add((Control) this.txtTrialText);
      this.Controls.Add((Control) this.btnContinue);
      this.Controls.Add((Control) this.btnQuit);
      this.Controls.Add((Control) this.lblThanksForTrying);
      this.Controls.Add((Control) this.txtProductName);
      this.FormBorderStyle = FormBorderStyle.FixedDialog;
      this.MaximizeBox = false;
      this.MinimizeBox = false;
      this.Name = "PaddleProductWindow";
      this.ShowIcon = false;
      ((ISupportInitialize) this.imgIcon).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();
    }
  }
}
