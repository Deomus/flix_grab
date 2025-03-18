using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Licensing.Web.Models
{
    public class PaymentServiceMapping
    {
        public PaymentServiceMapping()
        {

        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        public int ProductId { get; set; }

        [Required]
        public byte PaymentServiceType { get; set; }

        [Required]
        [StringLength(50)]
        public string PaymentServiceProductId { get; set; }

        [StringLength(250)]
        public string PaymentServiceAccountId { get; set; }
    }
}
