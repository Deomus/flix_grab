using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Web;

namespace Licensing.Web.Models
{
    public class Download
    {
        [Key, Required, DatabaseGenerated(DatabaseGeneratedOption.Identity)]
        public int Id { get; set; }

        public string Title { get; set; }

        public string Description { get; set; }

        [Required]
        public string LocalPath { get; set; }

        public string Url { get; set; }

        public string MimeType { get; set; }

        public int? ProductId { get; set; }

        public string NotificationEmail { get; set; }

        public bool IsPublic { get; set; }
    }
}
