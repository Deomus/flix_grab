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
using System.Data.Entity.Infrastructure;
using System.Data.Entity.Validation;
using System.Xml;

namespace Licensing.ControlPanel.Controllers
{
    public class ProductController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();

        //
        // GET: /Product/
        public ActionResult Index(int? page)
        {
            int pageNumber = (page ?? 1);

            return View(db.Products.OrderBy(p => p.Name).ToPagedList(pageNumber, 20));
        }

        //
        // GET: /Product/Details/5

        public ActionResult Details(int id = 0)
        {
            Product product = db.Products.Find(id);
            
            if (product == null)
            {
                return HttpNotFound();
            }

            foreach (PaymentServiceMapping m in db.PaymentServiceMappings.Where(m => m.ProductId == id))
            {
                switch (m.PaymentServiceType)
                {
                    case 1:
                        product.PayPalProductId = m.PaymentServiceProductId;
                        product.PayPalAccountEmail = m.PaymentServiceAccountId;
                        break;
                    case 2: product.RegNowProductId = m.PaymentServiceProductId; break;
                    case 3: product.ShareItProductId = m.PaymentServiceProductId; break;
                    case 4: product.FastSpringProductId = m.PaymentServiceProductId; break;
                    case 5: product.BraintreeProductId = m.PaymentServiceProductId; break;
                    default: break;
                }
            }

            ViewBag.IsReadOnly = true;

            return View("ProductForm", product);
        }

        //
        // GET: /Product/Create

        [Authorize(Roles = "Administrators")]
        public ActionResult Create()
        {
            Product p = new Product();

            using (LicensingServiceDbContext db = new LicensingServiceDbContext())
            {
                p.OrderEmailTemplate = db.Configuration.Where(c => c.Name == "DefaultOrderEmailTemplate").SingleOrDefault().Value;
            }

            return View("ProductForm", p);
        }

        //
        // POST: /Product/Create

        [HttpPost]
        [ValidateAntiForgeryToken]
        [ValidateInput(false)]
        [Authorize(Roles = "Administrators")]
        public ActionResult Create(Product product)
        {
            if (ModelState.IsValid)
            {
                PaymentServiceMapping mapping;

                if (!String.IsNullOrWhiteSpace(product.PayPalProductId))
                {
                    mapping = new PaymentServiceMapping();
                    mapping.PaymentServiceType = 1;
                    mapping.PaymentServiceProductId = product.PayPalProductId;
                    mapping.PaymentServiceAccountId = product.PayPalAccountEmail;
                    mapping.ProductId = product.Id;
                    db.PaymentServiceMappings.Add(mapping);
                }

                if (!String.IsNullOrWhiteSpace(product.RegNowProductId))
                {
                    mapping = new PaymentServiceMapping();
                    mapping.PaymentServiceType = 2;
                    mapping.PaymentServiceProductId = product.RegNowProductId;
                    mapping.ProductId = product.Id;
                    db.PaymentServiceMappings.Add(mapping);
                }

                if (!String.IsNullOrWhiteSpace(product.ShareItProductId))
                {
                    mapping = new PaymentServiceMapping();
                    mapping.PaymentServiceType = 3;
                    mapping.PaymentServiceProductId = product.ShareItProductId;
                    mapping.ProductId = product.Id;
                    db.PaymentServiceMappings.Add(mapping);
                }

                if (!String.IsNullOrWhiteSpace(product.FastSpringProductId))
                {
                    mapping = new PaymentServiceMapping();
                    mapping.PaymentServiceType = 4;
                    mapping.PaymentServiceProductId = product.FastSpringProductId;
                    mapping.ProductId = product.Id;
                    db.PaymentServiceMappings.Add(mapping);
                }

                if (!String.IsNullOrWhiteSpace(product.BraintreeProductId))
                {
                    mapping = new PaymentServiceMapping();
                    mapping.PaymentServiceType = 5;
                    mapping.PaymentServiceProductId = product.BraintreeProductId;
                    mapping.ProductId = product.Id;
                    db.PaymentServiceMappings.Add(mapping);
                }

                db.Products.Add(product);

                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(product);
        }

        //
        // GET: /Product/Edit/5
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(int id)
        {
            Product product = db.Products.Find(id);
            if (product == null)
            {
                return HttpNotFound();
            }

            foreach (PaymentServiceMapping m in db.PaymentServiceMappings.Where(m => m.ProductId == id))
            {
                switch (m.PaymentServiceType)
                {
                    case 1:
                        product.PayPalProductId = m.PaymentServiceProductId;
                        product.PayPalAccountEmail = m.PaymentServiceAccountId;
                        break;
                    case 2: product.RegNowProductId = m.PaymentServiceProductId; break;
                    case 3: product.ShareItProductId = m.PaymentServiceProductId; break;
                    case 4: product.FastSpringProductId = m.PaymentServiceProductId; break;
                    case 5: product.BraintreeProductId = m.PaymentServiceProductId; break;
                    default: break;
                }
            }

            return View("ProductForm", product);
        }

        //
        // POST: /Product/Edit/5

        [HttpPost]
        [ValidateAntiForgeryToken]
        [ValidateInput(false)]
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(Product product)
        {
            if (ModelState.IsValid)
            {
                UpdatePaymentServiceMapping(1, product.Id, product.PayPalProductId, product.PayPalAccountEmail);

                UpdatePaymentServiceMapping(2, product.Id, product.RegNowProductId, null);

                UpdatePaymentServiceMapping(3, product.Id, product.ShareItProductId, null);

                UpdatePaymentServiceMapping(4, product.Id, product.FastSpringProductId, null);

                UpdatePaymentServiceMapping(5, product.Id, product.BraintreeProductId, null);

                db.Entry(product).State = EntityState.Modified;

                db.SaveChanges();

                return RedirectToAction("Index");
            }

            return View(product);
        }

        //
        // POST: /Product/Delete/5

        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult DeleteConfirmed(int id)
        {
            Product product = db.Products.Find(id);
            db.Products.Remove(product);
            db.SaveChanges();
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            db.Dispose();
            base.Dispose(disposing);
        }

        private void UpdatePaymentServiceMapping(byte paymentServiceType, int productId, string paymentServiceProductId, string paymentServiceAccountId)
        {
            PaymentServiceMapping mapping = db.PaymentServiceMappings.Where(m => m.PaymentServiceType == paymentServiceType && m.ProductId == productId).SingleOrDefault();

            if (mapping != null)
            {
                if (mapping.PaymentServiceAccountId != paymentServiceAccountId)
                {
                    if (!String.IsNullOrWhiteSpace(paymentServiceAccountId))
                    {
                        mapping.PaymentServiceAccountId = paymentServiceAccountId;
                        db.Entry(mapping).State = EntityState.Modified;
                    }
                }

                if (mapping.PaymentServiceProductId != paymentServiceProductId)
                {
                    if (!String.IsNullOrWhiteSpace(paymentServiceProductId))
                    {
                        mapping.PaymentServiceProductId = paymentServiceProductId;
                        db.Entry(mapping).State = EntityState.Modified;
                    }
                    else
                        db.Entry(mapping).State = EntityState.Deleted;
                }
            }
            else
            {
                if (!String.IsNullOrWhiteSpace(paymentServiceProductId))
                {
                    mapping = new PaymentServiceMapping();
                    mapping.PaymentServiceType = paymentServiceType;
                    mapping.ProductId = productId;
                    mapping.PaymentServiceProductId = paymentServiceProductId;
                    mapping.PaymentServiceAccountId = paymentServiceAccountId;

                    db.PaymentServiceMappings.Add(mapping);
                }
            }
        }
    }
}
