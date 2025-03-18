using Licensing.Web.DAL;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using PagedList;
using Licensing.Web.Models;
using LicensingService.Filters;

namespace Licensing.ControlPanel.Controllers
{
    [MvcAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.StandardLegacy | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    public class DownloadController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();

        //
        // GET: /ControlPanel/Downloads/

        public ActionResult Index(int? page)
        {
            int pageNumber = (page ?? 1);

            return View(db.Downloads.OrderBy(p => p.Title).ToPagedList(pageNumber, 20));
        }

        //
        // GET: /ControlPanel/Downloads/Details/5

        public ActionResult Details(int id)
        {
            Download download = db.Downloads.Find(id);

            if (download == null)
            {
                return HttpNotFound();
            }

            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name", download.ProductId.ToString());

            ViewBag.IsReadOnly = true;

            return View("DownloadForm", download);
        }

        //
        // GET: /ControlPanel/Downloads/Create

        [Authorize(Roles = "Administrators")]
        public ActionResult Create()
        {
            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name");

            return View("DownloadForm");
        }

        //
        // POST: /LicenseKey/Create

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Create([Bind(Exclude="Id, ProductId")]Download download, int SelectedProduct)
        {
            if (ModelState.IsValid)
            {
                download.ProductId = SelectedProduct;

                db.Downloads.Add(download);

                db.SaveChanges();

                return RedirectToAction("Index");
            }

            return View("DownloadForm", download);
        }

        //
        // GET: /ControlPanel/Downloads/Edit/5

        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(int id = 0)
        {
            Download download = db.Downloads.Find(id);

            if (download == null)
            {
                return HttpNotFound();
            }

            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name", download.ProductId.ToString());

            return View("DownloadForm", download);
        }

        //
        // POST: /LicenseKey/Edit/5

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(Download download, int SelectedProduct)
        {
            if (ModelState.IsValid)
            {
                download.ProductId = SelectedProduct;

                db.Entry(download).State = EntityState.Modified;
                db.SaveChanges();

                return RedirectToAction("Index");
            }

            return View("DownloadForm", download);
        }

        //
        // DELETE:

        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult DeleteConfirmed(int id)
        {
            Download download = db.Downloads.Find(id);

            db.Downloads.Remove(download);
            db.SaveChanges();

            return RedirectToAction("Index");
        }
    }
}
