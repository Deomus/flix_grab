using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace Licensing.Web.Models
{
    public class OrderItem
    {
        public OrderItem()
        {

        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        public int OrderId { get; set; }

        [Required]
        public int ProductId { get; set; }

        [Required]
        [Range(0, 32767, ErrorMessage="The quantity must be between 0 and 32767")]
        public short Quantity { get; set; }

        [Required]
        public decimal TotalPrice { get; set; }

        public decimal? TotalPriceExclFees { get; set; }

        public decimal? OriginalProductPrice { get; set; }

        [StringLength(3, MinimumLength=1, ErrorMessage="If provided, currency must have between 1 and 3 characters (eg. \"USD\", \"EUR\", etc.)")]
        public string OriginalProductCurrency { get; set; }

        public virtual IList<LicenseKey> LicenseKeys { get; set; }

        public virtual Order Order { get; set; }

        public virtual Product Product { get; set; }
    }
}
