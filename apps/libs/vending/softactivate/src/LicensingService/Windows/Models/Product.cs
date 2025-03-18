using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;
using System.Web.Mvc;
using System.ComponentModel.DataAnnotations.Schema;

namespace Licensing.Web.Models
{
    public class Product
    {
        public Product()
        {
            DefaultMaxActivationsPerLicenseKey = 10;
        }

        [Display(Name = "Product Id", Description="A unique number identifying the product")]
        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.None)]
        public int Id { get; set; }
       
        [StringLength(100, MinimumLength=1, ErrorMessage="Product name must have between 1 and 100 characters")]
        [Required]
        [Display(Name="Product Name")]
        public string Name { get; set; }
        
        public Decimal? Price { get; set; }
        
        [Display(Description = "Currency code eg. \"USD\", \"EUR\", \"GBP\"")]
        [StringLength(3)]
        public string Currency { get; set; }
        
        [Display(Name="License Duration", Description = "Number of days since activation after which the license expires (leave blank for no expiration)")]
        [Range(0, 32767, ErrorMessage="Default lincense duration must be between 0 and 32767. Use 0 for infinite.")]
        public short? DefaultLicenseDuration { get; set; }
        
        [Display(Name="Max Devices Per License Key", Description = "Number of distinct devices that permit activation for a single license key")]
        [Range(0, 32767, ErrorMessage="Must be between 0 and 32767. Use 0 for infinite.")]
        public short? DefaultMaxUniqueHardwareIdsPerLicenseKey { get; set; }
        
        [Display(Name="Max Activations Per Device", Description = "Number of activations allowed on the same computer for a single license key")]
        [Range(0, 32767, ErrorMessage="Must be between 0 and 32767. Use 0 for infinite.")]
        public short? DefaultMaxActivationsPerHardwareId { get; set; }

        [Display(Name = "Total Max Activations Per License Key (use 0 for unlimited)", Description = "Maximum number of activations allowed for a single license key")]
        [Range(0, 32767, ErrorMessage="Must be between 0 and 32767. Use 0 for infinite.")]
        public short? DefaultMaxActivationsPerLicenseKey { get; set; }        

        [Display(Name="XML License Key Template", Description="The XML representation of the license key template used to generate keys. Create templates with the Licensing Tool.")]
        [DataType(DataType.MultilineText | DataType.Html)]
        [StringLength(4000, ErrorMessage="Maximum XML length (4000) exceeded. Tip: remove the /Properties/UserInterface section from XML, it is not needed on the server.")]
        [AllowHtml]
        public string LicenseKeyTemplate { get; set; }

        [Display(Name = "Order Email Template", Description = "The HTML template for the order emails")]
        [DataType(DataType.MultilineText | DataType.Html)]
        [AllowHtml]
        public string OrderEmailTemplate { get; set; }

        [Display(Name="Support Email")]
        [AllowHtml]
        [StringLength(100, ErrorMessage="Must have less than 100 characters")]
        public string SupportEmail { get; set; }

        [NotMapped]
        [Display(Name = "PayPal Product Id")]
        [StringLength(50)]
        public string PayPalProductId { get; set; }

        [NotMapped]
        [Display(Name= "PayPal Account Email")]
        [StringLength(250)]
        public string PayPalAccountEmail { get; set; }

        [NotMapped]
        [Display(Name = "RegNow Product Id")] 
        [StringLength(50)]
        public string RegNowProductId { get; set; }

        [NotMapped]
        [Display(Name = "ShareId Product Id")]
        [StringLength(50)]
        public string ShareItProductId { get; set; }

        [NotMapped]
        [Display(Name = "FastSpring Product Id")]
        [StringLength(50)]
        public string FastSpringProductId { get; set; }

        [NotMapped]
        [Display(Name = "Braintree Product Id")]
        [StringLength(50)]
        public string BraintreeProductId { get; set; }

        public virtual IList<LicenseKey> LicenseKeys { get; set; }
        public virtual IList<PaymentServiceMapping> PaymentServiceMappings { get; set; }
    }
}
