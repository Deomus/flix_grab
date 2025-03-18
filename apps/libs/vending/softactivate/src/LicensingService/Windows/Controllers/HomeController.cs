using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace LicensingService.Controllers
{
    public class HomeController : System.Web.Mvc.Controller
    {
        public ActionResult Index()
        {
            return System.IO.File.Exists(Server.MapPath("~/default.aspx")) ? (ActionResult)Redirect("~/default.aspx") : (ActionResult)RedirectToAction("Index", "Home", new { area = "ControlPanel" });
        }
    }
}
