using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace Licensing.Web.Models
{
    [Table("EventLog")]
    public class Event
    {
        public Event()
        {

        }

        [Required, Key, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        [Required]
        [Range(0, 127)]
        public byte Type { get; set; }

        [Required]
        [StringLength(200)]
        public string Subject { get; set; }

        public string Message { get; set; }

        public string Url { get; set; }

        public string UrlParams { get; set; }

        [StringLength(45)]
        public string IPAddress { get; set; }

        [StringLength(100)]
        public string Location { get; set; }

        [Required]
        public DateTime Date { get; set; }
    }
}
