using System.Web.Mvc;
using System.Web.Routing;

namespace LicensingControlPanel
{
    public class ControlPanelAreaRegistration : AreaRegistration
    {
        public override string AreaName
        {
            get
            {
                return "ControlPanel";
            }
        }

        public override void RegisterArea(AreaRegistrationContext context)
        {
            context.MapRoute(
                name: "ControlPanel_default",
                url: "ControlPanel/{controller}/{action}/{id}",
                defaults: new { controller = "Home", action = "Index", area = "ControlPanel", id = "" },
                namespaces: new[] { "Licensing.ControlPanel.Controllers" }
            ).DataTokens["UseNamespaceFallback"] = false;
        }
    }
}
