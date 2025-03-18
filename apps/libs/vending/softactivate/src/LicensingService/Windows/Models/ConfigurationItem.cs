using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace Licensing.Web.Models
{
    [Table("Configuration")]
    public class ConfigurationItem
    {
        public ConfigurationItem()
        {

        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        [StringLength(100)]
        public string Name { get; set; }

        public string Value { get; set; }
    }
}
