using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Licensing.ControlPanel.Models
{
    public class SearchResultsModel
    {
        public PagedList.IPagedList<Licensing.Web.Models.OrderItem> OrderList { get; set; }
        public PagedList.IPagedList<Licensing.Web.Models.LicenseKey> LicenseKeyList { get; set; }
        public PagedList.IPagedList<Licensing.Web.Models.Activation> ActivationList { get; set; }
    }
}
