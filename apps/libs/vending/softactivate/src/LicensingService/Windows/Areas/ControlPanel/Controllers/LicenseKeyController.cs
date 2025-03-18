using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using Licensing.Web.Models;
using Licensing.Web.DAL;
using PagedList;
using LicensingService.Filters;

namespace Licensing.ControlPanel.Controllers
{
    public class LicenseKeyController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();
        private GenericRepository<Product> productsRepository;
        private GenericRepository<Order> ordersRepository;
        private GenericRepository<LicenseKey> licenseKeysRepository;

        public LicenseKeyController()
        {
            productsRepository = new GenericRepository<Product>(db);
            ordersRepository = new GenericRepository<Order>(db);
            licenseKeysRepository = new GenericRepository<LicenseKey>(db);
        }

        //
        // GET: /LicenseKey/

        public ActionResult Index(DateTime? startDate, DateTime? endDate, int? selectedProduct, int? page)
        {
            var products = productsRepository.Get(orderBy: q => q.OrderBy(d => d.Name));

            ViewBag.SelectedProduct = new SelectList(products, "Id", "Name", selectedProduct);
            ViewBag.StartDate = (startDate == null) ? DateTime.UtcNow.AddDays(-7) : startDate;
            ViewBag.EndDate = (endDate == null) ? DateTime.UtcNow : endDate;

            int productId = selectedProduct.GetValueOrDefault();

            if (endDate.HasValue)
                endDate = endDate.Value.AddDays(1);

            var licenseKeys = licenseKeysRepository.GetQuery(
                filter: s => (!selectedProduct.HasValue || s.ProductId == productId)
                             && (startDate == null || s.DateAdded >= startDate)
                             && (endDate == null || s.DateAdded < endDate),
                orderBy: q => q.OrderByDescending(s => s.DateAdded),
                includeProperties: "OrderItem, Product");

            int pageNumber = (Request.HttpMethod == "POST") ? 1 : (page ?? 1);

            return View(licenseKeys.ToPagedList(pageNumber, 20));
        }

        //
        // GET: /LicenseKey/Details/5

        public ActionResult Details(int id = 0)
        {
            LicenseKey licensekey = db.LicenseKeys.Find(id);
            if (licensekey == null)
            {
                return HttpNotFound();
            }

            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name", licensekey.ProductId.ToString());

            ViewBag.IsReadOnly = true;

            return View("LicenseKeyForm", licensekey);
        }

        //
        // GET: /LicenseKey/Create

        [Authorize(Roles = "Administrators")]
        public ActionResult Create()
        {
            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name");

            return View("LicenseKeyForm");
        }

        //
        // POST: /LicenseKey/Create

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Create([Bind(Exclude = "Id, ProductId, DateAdded")]LicenseKey licensekey, int SelectedProduct)
        {
            if (ModelState.IsValid)
            {
                licensekey.ProductId = SelectedProduct;
                licensekey.DateAdded = DateTime.UtcNow;
                db.LicenseKeys.Add(licensekey);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View("LicenseKeyForm", licensekey);
        }

        //
        // GET: /LicenseKey/Edit/5

        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(int id = 0)
        {
            LicenseKey licensekey = db.LicenseKeys.Find(id);
            if (licensekey == null)
            {
                return HttpNotFound();
            }

            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name", licensekey.ProductId.ToString());

            return View("LicenseKeyForm", licensekey);
        }

        //
        // POST: /LicenseKey/Edit/5

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(LicenseKey licensekey, int SelectedProduct)
        {
            if (ModelState.IsValid)
            {
                licensekey.OrderItem = null;
                licensekey.ProductId = SelectedProduct;

                db.Entry(licensekey).State = EntityState.Modified;
                db.SaveChanges();

                return RedirectToAction("Index");
            }

            return View("LicenseKeyForm", licensekey);
        }

        //
        // POST: /LicenseKey/Delete/5

        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult DeleteConfirmed(int id)
        {
            LicenseKey licensekey = db.LicenseKeys.Find(id);
            db.LicenseKeys.Remove(licensekey);
            db.SaveChanges();

            return RedirectToAction("Index");
        }

        //
        // POST: /LicenseKey/Block/5

        [HttpPost, ActionName("Block")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult BlockConfirmed(int id)
        {
            LicenseKey licensekey = db.LicenseKeys.Find(id);
            licensekey.Active = false;

            db.Entry(licensekey).State = EntityState.Modified;
            db.SaveChanges();

            return RedirectToAction("Index");
        }

        //
        // POST: /LicenseKey/Unblock/5

        [HttpPost, ActionName("Unblock")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult UnblockConfirmed(int id)
        {
            LicenseKey licensekey = db.LicenseKeys.Find(id);
            licensekey.Active = true;

            db.Entry(licensekey).State = EntityState.Modified;
            db.SaveChanges();

            return RedirectToAction("Index");
        }
    }
}
