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
using LicensingService;
using System.Data.Entity.Validation;

namespace Licensing.ControlPanel.Controllers
{
    public class OrderController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();
        private GenericRepository<Product> productRepository;
        private GenericRepository<Order> ordersRepository;
        private GenericRepository<OrderItem> orderItemsRepository;

        public OrderController()
        {
            productRepository = new GenericRepository<Product>(db);
            ordersRepository = new GenericRepository<Order>(db);
            orderItemsRepository = new GenericRepository<OrderItem>(db);
        }

        //
        // GET: /Order/
        public ActionResult Index(DateTime? startDate, DateTime? endDate, int? selectedProduct, string searchString, string sortOrder, int? page)
        {
            if (Request.HttpMethod == "GET" && startDate == null && endDate == null)
            {
                startDate = DateTime.UtcNow.AddDays(-7);
                endDate = DateTime.UtcNow;
            }

            var products = productRepository.Get(orderBy: q => q.OrderBy(d => d.Name));

            ViewBag.SelectedProduct = new SelectList(products, "Id", "Name", selectedProduct);
            ViewBag.StartDate = (startDate == null) ? DateTime.UtcNow.AddDays(-365) : startDate;
            ViewBag.EndDate = (endDate == null) ? DateTime.UtcNow : endDate;

            ViewBag.DateSortParam = (sortOrder == "Date" ? "Date desc" : "Date");
            ViewBag.ProductNameSortParam = (sortOrder == "ProductName" ? "ProductName desc" : "ProductName");
            ViewBag.QuantitySortParam = (sortOrder == "Quantity" ? "Quantity desc" : "Quantity");
            ViewBag.PriceSortParam = (sortOrder == "Price" ? "Price desc" : "Price");
            ViewBag.CountrySortParam = (sortOrder == "Country" ? "Country desc" : "Country");
            ViewBag.CustomerNameSortParam = (sortOrder == "LastName" ? "LastName desc" : "LastName");
            ViewBag.CompanyNameSortParam = (sortOrder == "Company" ? "Company desc" : "Company"); 

            int productId = selectedProduct.GetValueOrDefault();

            var orderItems = orderItemsRepository.GetQuery(
                                filter: s => (!selectedProduct.HasValue || s.ProductId == productId)
                         && (String.IsNullOrEmpty(searchString)
                             || (s.Order.FirstName.ToUpper().Contains(searchString.ToUpper()))
                             || s.Order.LastName.ToUpper().Contains(searchString.ToUpper())
                             || s.Order.Company.ToUpper().Contains(searchString.ToUpper()))
                         && (startDate == null || s.Order.Date >= startDate)
                         && (endDate == null || s.Order.Date <= endDate),
                orderBy: q => q.OrderBy(s => s.Id),
                includeProperties: "Product, Order");

            /*
            var orders = ordersRepository.Get(
                filter: s => (!selectedProduct.HasValue || s.OrderItems[0].ProductId == productId) 
                         && (String.IsNullOrEmpty(searchString) 
                             || (s.FirstName.ToUpper().Contains(searchString.ToUpper()) ) 
                             || s.LastName.ToUpper().Contains(searchString.ToUpper()) 
                             || s.Company.ToUpper().Contains(searchString.ToUpper()))
                         && (startDate == null || s.Date >= startDate)
                         && (endDate == null || s.Date <= endDate),
                orderBy: q => q.OrderBy(s => s.Id),
                includeProperties: "Product, OrderItem");
            */

            switch (sortOrder)
            {
                case "Date":
                    orderItems = orderItems.OrderBy(s => s.Order.Date);
                    break;

                case "Date desc":
                    orderItems = orderItems.OrderByDescending(s => s.Order.Date);
                    break;

                case "ProductName":
                    orderItems = orderItems.OrderBy(s => s.Product.Name);
                    break;

                case "ProductName desc":
                    orderItems = orderItems.OrderByDescending(s => s.Product.Name);
                    break;

                case "Quantity":
                    orderItems = orderItems.OrderBy(s => s.Quantity);
                    break;
                
                case "Quantity desc":
                    orderItems = orderItems.OrderByDescending(s => s.Quantity);
                    break;

                case "Price":
                    orderItems = orderItems.OrderBy(s => s.Order.OrderTotal);
                    break;

                case "Price desc":
                    orderItems = orderItems.OrderByDescending(s => s.Order.OrderTotal);
                    break;

                case "Country":
                    orderItems = orderItems.OrderBy(s => s.Order.CountryCode);
                    break;
                
                case "Country desc":
                    orderItems = orderItems.OrderByDescending(s => s.Order.CountryCode);
                    break;

                case "CustomerName":
                    orderItems = orderItems.OrderBy(s => s.Order.LastName);
                    break;

                case "CustomerName desc":
                    orderItems = orderItems.OrderByDescending(s => s.Order.LastName);
                    break;

                case "Company":
                    orderItems = orderItems.OrderBy(s => s.Order.Company);
                    break;
                
                case "Company desc":
                    orderItems = orderItems.OrderByDescending(s => s.Order.Company);
                    break;

                default:
                    orderItems = orderItems.OrderByDescending(s => s.Order.Date);
                    break;
            }

            int pageNumber = (Request.HttpMethod == "POST") ? 1 : (page ?? 1);

            return View(orderItems.ToPagedList(pageNumber, 20));
        }

        //
        // GET: /Order/Details/5

        public ActionResult Details(int id = 0)
        {
            OrderItem order = db.OrderItems.Find(id);
            if (order == null)
            {
                return HttpNotFound();
            }

            db.Products.Load();
            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name", order.ProductId.ToString());

            ViewBag.IsReadOnly = true;

            return View("OrderForm", order);
        }

        //
        // GET: /Order/Create
        [Authorize(Roles = "Administrators")]
        public ActionResult Create()
        {
            db.Products.Load();

            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name");
            ViewBag.Quantity = 1;

            OrderItem orderItem = new OrderItem();

            orderItem.Quantity = 1;

            orderItem.Order = new Order();

            orderItem.Order.Date = DateTime.UtcNow;

            orderItem.Order.Currency = "USD";

            return View("OrderForm", orderItem);
        }

        //
        // POST: /Order/Create

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Create([Bind(Exclude="Id, LicenseKeys")]OrderItem orderItem, int SelectedProduct, int Quantity, string LicenseKeys)
        {
            if (ModelState.IsValid)
            {
                //Order order = new Order();
                //order.Date = DateTime.UtcNow;

                if (orderItem.Order.TransactionId == null)
                    orderItem.Order.TransactionId = Guid.NewGuid().ToString();

                db.Orders.Add(orderItem.Order);

                db.SaveChanges();

                orderItem.OrderId = orderItem.Order.Id;
                orderItem.Quantity = (short)Quantity;
                orderItem.ProductId = SelectedProduct;

                db.OrderItems.Add(orderItem);
                db.SaveChanges();

                string licenseKeyList = LicenseKeys;

                string[] licenseKeys = licenseKeyList.Split(new string[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries);

                foreach (string k in licenseKeys)
                {
                    string key = k.Trim();

                    if (key.Length == 0)
                        continue;

                    LicenseKey licenseKey = new LicenseKey();
                    licenseKey.OrderItemId = orderItem.Id;
                    licenseKey.Key = key;
                    licenseKey.ProductId = orderItem.ProductId;
                    licenseKey.DateAdded = DateTime.UtcNow;
                    db.LicenseKeys.Add(licenseKey);
                }

                db.SaveChanges();
            }
            else
            {
                string message = "";

                var errors = ModelState.Select(x => x.Value.Errors).Where(y => y.Count > 0).ToList();
                foreach (ModelErrorCollection errors2 in errors)
                {
                    foreach (ModelError err in errors2)
                    {
                        message += err.Exception.Message;
                    }
                }

                EventLog.Log("Form Validation Error", message, EventType.Error);
            }

            return RedirectToAction("Index");
        }

        //
        // GET: /Order/Edit/5

        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(int id = 0)
        {
            OrderItem order = db.OrderItems.Find(id);

            if (order == null)
            {
                return HttpNotFound();
            }

            db.Products.Load();

            ViewBag.SelectedProduct = new SelectList(db.Products, "Id", "Name", order.ProductId.ToString());

            return View("OrderForm", order);
        }

        //
        // POST: /Order/Edit/5

        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult Edit(/*[Bind(Exclude="Id")]*/OrderItem orderItem, int SelectedProduct)
        {
            //if (ModelState.IsValid)
            {
                //db.OrderItems.Attach(orderItem);
                //db.Orders.OrderItems[0] = orderItem;
                //orderItem.ProductId = SelectedProduct;

                db.Entry(orderItem.Order).State = EntityState.Modified;

                orderItem.Order = null;
                orderItem.LicenseKeys = null;

                db.Entry(orderItem).State = EntityState.Modified;

                db.SaveChanges();
                
                return RedirectToAction("Index");
            }

            return View("OrderForm", orderItem);
        }

        //
        // POST: /Order/Delete/5

        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "Administrators")]
        public ActionResult DeleteConfirmed(int id)
        {
            foreach (OrderItem item in db.OrderItems.Where(i => i.OrderId == id))
            {
                db.OrderItems.Remove(item);
            }

            Order order = db.Orders.Find(id);
            db.Orders.Remove(order);

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
