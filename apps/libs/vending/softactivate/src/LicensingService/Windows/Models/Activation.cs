using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Licensing.Web.Models
{
    public class Activation
    {
        public Activation()
        {

        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        [StringLength(256, MinimumLength=1, ErrorMessage="Activation key is either too small or too long. Must be less than 256 characters")]
        public string ActivationKey { get; set; }
        
        [Required]
        public int LicenseKeyId { get; set; }
        
        [Required]
        [StringLength(64, MinimumLength=1, ErrorMessage="Hardware id is either too small or too long. Must be less than 64 characters")]
        public string HardwareId { get; set; }

        [Required]
        public DateTime ActivationDate { get; set; }
        
        public DateTime? ExpirationDate { get; set; }
        
        [StringLength(45, ErrorMessage="IP Address must be smaller than 45 characters")]
        public string IPAddress { get; set; }

        public virtual LicenseKey LicenseKey { get; set; }
    }
}
