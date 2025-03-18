using Licensing.Web.DAL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Licensing.ControlPanel.Controllers
{
    public class HomeController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();

        public ActionResult Index()
        {
            ViewBag.Message = "Dashboard";

            DateTime now = DateTime.UtcNow;
            
            DateTime startDate = new DateTime(now.Year, now.Month, now.Day);
            DateTime endDate = startDate.AddDays(1);

            ViewBag.DailyOrderCount = db.Orders.Where(o => o.Date >= startDate && o.Date < endDate).Count();
            ViewBag.DailyActivationCount = db.Activations.Where(a => a.ActivationDate >= startDate && a.ActivationDate < endDate).Count();
            ViewBag.DailyUniqueActivationCount = db.Activations.Where(a => a.ActivationDate >= startDate && a.ActivationDate < endDate).Select(a => a.HardwareId).Distinct().Count();

            startDate = startDate.AddDays(-1);
            endDate = endDate.AddDays(-1);

            ViewBag.YesterdayOrderCount = db.Orders.Where(o => o.Date >= startDate && o.Date < endDate).Count(); ;

            ViewBag.TotalActivationCount = db.Activations.Count();
            ViewBag.TotalOrderCount = db.OrderItems.Count();
            ViewBag.TotalUniqueActivationCount = db.Activations.Select(a => a.HardwareId).Distinct().Count();

            /* 
            
            SqlCommand cmd = new SqlCommand("SELECT Count(1) FROM Activations", conn);
            TotalActivationsText.Text = cmd.ExecuteScalar().ToString();

            cmd = new SqlCommand("SELECT Count(DISTINCT HardwareId) FROM Activations", conn);
            TotalUniqueActivationsText.Text = cmd.ExecuteScalar().ToString();

            cmd = new SqlCommand("SELECT Count(1) FROM Activations WHERE DATEDIFF(day, ActivationDate, @ActivationDate) = 0", conn);
            cmd.Parameters.AddWithValue("@ActivationDate", DateTime.UtcNow);
            DailyActivationsText.Text = cmd.ExecuteScalar().ToString();

            cmd = new SqlCommand("SELECT Count(DISTINCT HardwareId) FROM Activations WHERE DATEDIFF(day, ActivationDate, @ActivationDate) = 0", conn);
            cmd.Parameters.AddWithValue("@ActivationDate", DateTime.UtcNow);
            DailyUniqueActivationsText.Text = cmd.ExecuteScalar().ToString();

            cmd = new SqlCommand("SELECT Count(1) FROM Purchases WHERE DATEDIFF(day, PurchaseDate, @PurchaseDate) = 0", conn);
            cmd.Parameters.AddWithValue("@PurchaseDate", DateTime.UtcNow);
            DailySalesText.Text = cmd.ExecuteScalar().ToString();

            cmd = new SqlCommand("SELECT Count(1) FROM Purchases WHERE DATEDIFF(day, PurchaseDate, @PurchaseDate) = 1", conn);
            cmd.Parameters.AddWithValue("@PurchaseDate", DateTime.UtcNow);
            YesterdaySalesText.Text = cmd.ExecuteScalar().ToString();

            cmd = new SqlCommand("SELECT Count(1) FROM Purchases", conn);
            TotalSalesText.Text = cmd.ExecuteScalar().ToString();
            
            */

            return View();
        }

        public ActionResult About()
        {
            ViewBag.Message = "Your app description page.";

            return View();
        }

        public ActionResult Contact()
        {
            ViewBag.Message = "Your contact page.";

            return View();
        }
    }
}
