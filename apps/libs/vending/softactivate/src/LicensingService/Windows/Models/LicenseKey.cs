using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Licensing.Web.Models
{
    public class LicenseKey
    {
        public LicenseKey()
        {
            Active = true;
        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        [StringLength(500, MinimumLength=1, ErrorMessage="License key is either too small or too big. Must have between 1 and 500 characters")]
        public string Key { get; set; }

        [StringLength(1000, ErrorMessage="Validation data is too large. Must have a maximum of 1000 characters")]
        public string ValidationData { get; set; }

        [Required]
        public int ProductId { get; set; }

        public int? OrderItemId { get; set; }
        
        [Range(0, 32767, ErrorMessage="If provided, the device limit must be between 0 and 32767")]
        public short? DeviceLimit { get; set; }

        public int? DeviceCount { get; set; }

        [Range(0, 32767, ErrorMessage="If provided, the activation limit must be between 0 and 32767")]
        public short? ActivationLimit { get; set; }

        public int? ActivationCount { get; set; }

        public DateTime? LastActivationTime { get; set; }

        [Range(0, 32767, ErrorMessage="If provided, per-device activation limit must be between 0 and 32767")]
        public short? PerDeviceActivationLimit { get; set; }
        
        [Range(0, 32767, ErrorMessage="If provided, license duration must be less than 32767 days. Use 0 for infinite.")]
        public short? LicenseDuration { get; set; }
        
        [Range(0, 32767, ErrorMessage="If provided, the license history duration must be less than 32767 days. Use 0 for infinite.")]
        public short? LicenseHistoryDuration { get; set; }

        public DateTime DateAdded { get; set; }

        public bool Active { get; set; }

        public virtual Product Product { get; set; }

        public virtual OrderItem OrderItem { get; set; }
    }
}
