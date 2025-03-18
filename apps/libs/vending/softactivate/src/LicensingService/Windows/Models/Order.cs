using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Licensing.Web.Models
{
    public class Order
    {
        public Order()
        {

        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set;  }

        [StringLength(50)]
        public string TransactionId { get; set; }

        public Decimal? OrderTotal { get; set; }

        public Decimal? OrderTotalExclFees { get; set; }

        [StringLength(3, MinimumLength=1, ErrorMessage="The currency must have maximum 3 characters (eg. \"USD\", \"EUR\", etc.)")]
        public string Currency { get; set; }

        public DateTime Date { get; set; }

        [StringLength(100)]
        public string FirstName { get; set; }

        [StringLength(100)]
        public string LastName { get; set; }
        
        [StringLength(100)]
        public string Company { get; set; }

        [StringLength(50)]
        public string PhoneNumber { get; set; }

        [StringLength(100)]
        public string Email { get; set; }

        [StringLength(200)]
        public string Address { get; set; }

        [StringLength(50)]
        public string ZipCode { get; set; }

        [StringLength(50)]
        public string City { get; set; }

        [StringLength(50)]
        public string State { get; set; }
        
        [StringLength(50)]
        public string CountryCode { get; set; }

        public virtual IList<OrderItem> OrderItems { get; set; }
    }
}
