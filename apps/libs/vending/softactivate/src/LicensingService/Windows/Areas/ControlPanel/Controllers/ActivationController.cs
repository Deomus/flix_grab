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
    public class ActivationController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();
        private GenericRepository<Activation> activationsRepository;
        private GenericRepository<Product> productsRepository;

        public ActivationController()
        {
            activationsRepository = new GenericRepository<Activation>(db);
            productsRepository = new GenericRepository<Product>(db);
        }

        //
        // GET: /Activation/

        public ActionResult Index(DateTime? startDate, DateTime? endDate, int? selectedProduct, int? page)
        {
            var products = productsRepository.Get(orderBy: q => q.OrderBy(d => d.Name));

            ViewBag.SelectedProduct = new SelectList(products, "Id", "Name", selectedProduct);
            ViewBag.StartDate = (startDate == null) ? DateTime.UtcNow.AddDays(-7) : startDate;
            ViewBag.EndDate = (endDate == null) ? DateTime.UtcNow : endDate;

            int productId = selectedProduct.GetValueOrDefault();

            if (endDate.HasValue)
                endDate = endDate.Value.AddDays(1);

            var activations = activationsRepository.GetQuery(
                filter: s => (!selectedProduct.HasValue || s.LicenseKey.ProductId == productId)
                             && (startDate == null || s.ActivationDate >= startDate)
                             && (endDate == null || s.ActivationDate < endDate),
                orderBy: q => q.OrderByDescending(s => s.ActivationDate),
                includeProperties: "LicenseKey");

            int pageNumber = (Request.HttpMethod == "POST") ? 1 : (page ?? 1);

            return View(activations.ToPagedList(pageNumber, 20));
        }

        //
        // GET: /Activation/Details/5

        public ActionResult Details(int id = 0)
        {
            Activation activation = db.Activations.Find(id);
            if (activation == null)
            {
                return HttpNotFound();
            }

            ViewBag.IsReadOnly = true;

            return View("ActivationForm", activation);
        }

        //
        // GET: /Activation/Create

        [Authorize(Roles = "Administrators")]
        public ActionResult Create()
        {
            return View("ActivationForm");
        }

        //
        // POST: /Activation/Create

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Create([Bind(Exclude="Id")]Activation activation)
        {
            if (ModelState.IsValid)
            {
                db.Activations.Add(activation);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(activation);
        }

        //
        // GET: /Activation/Edit/5
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(int id = 0)
        {
            Activation activation = db.Activations.Find(id);
            if (activation == null)
            {
                return HttpNotFound();
            }

            return View("ActivationForm", activation);
        }

        //
        // POST: /Activation/Edit/5

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(Activation activation)
        {
            if (ModelState.IsValid)
            {
                activation.LicenseKey = null;

                db.Entry(activation).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(activation);
        }

        //
        // POST: /Activation/Delete/5

        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult DeleteConfirmed(int id)
        {
            Activation activation = db.Activations.Find(id);
            db.Activations.Remove(activation);
            db.SaveChanges();
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            db.Dispose();
            base.Dispose(disposing);
        }
    }
}