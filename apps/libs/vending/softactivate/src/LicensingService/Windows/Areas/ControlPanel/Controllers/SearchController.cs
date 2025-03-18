using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using Licensing.ControlPanel.Models;
using Licensing.Web.Models;
using Licensing.Web.DAL;
using PagedList;
using System.Linq.Expressions;
using LicensingService.Filters;

namespace Licensing.ControlPanel.Controllers
{
    [MvcAuthorizeSDKLicensing(LicensingSDKEdition.Demo | LicensingSDKEdition.Professional | LicensingSDKEdition.Enterprise | LicensingSDKEdition.Ultimate)]
    public class SearchController : Controller
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();
        private GenericRepository<Activation> activationsRepository;
        private GenericRepository<OrderItem> ordersRepository;
        private GenericRepository<LicenseKey> licenseKeysRepository;

        public SearchController()
        {
            activationsRepository = new GenericRepository<Activation>(db);
            ordersRepository = new GenericRepository<OrderItem>(db);
            licenseKeysRepository = new GenericRepository<LicenseKey>(db);
        }

        public ActionResult Index(string q, int? page = 1, string pagedctl = null)
        {
            int pageNumber = page ?? 1;

            SearchResultsModel searchResultsModel = new SearchResultsModel();

            if (string.IsNullOrWhiteSpace(q))
                return View("SearchResults", searchResultsModel);

            q = q.Trim().ToUpper();

            string[] terms = q.Split(new char[] { ' ' });

            string term0 = (terms.Length > 0) ? terms[0] : "";
            string term1 = (terms.Length > 1) ? terms[1] : "";

            if (page == null || page == 1 || (page > 1 && pagedctl == "orderlist"))
            {
                Expression<Func<OrderItem, bool>> filter = s => (String.IsNullOrEmpty(q)
                                 || (s.Order.FirstName.ToUpper().Contains(q))
                                 || s.Order.LastName.ToUpper().Contains(q)
                                 || (q.Contains("@") && s.Order.Email.ToUpper().Contains(q))
                                 || s.Order.Company.ToUpper().Contains(q)
                                 || s.Order.TransactionId.ToUpper().Contains(q));

                if (terms.Length > 1)
                {
                    filter = s => (String.IsNullOrEmpty(q)
                                 || (s.Order.FirstName.ToUpper().Contains(q))
                                 || s.Order.LastName.ToUpper().Contains(q)
                                 || s.Order.Company.ToUpper().Contains(q)
                                 || s.Order.TransactionId.ToUpper().Contains(q)
                                 || (s.Order.FirstName.ToUpper().Contains(term0) && s.Order.LastName.ToUpper().Contains(term1))
                                 || (s.Order.FirstName.ToUpper().Contains(term1) && s.Order.LastName.ToUpper().Contains(term0)));
                }

                var orderItems = ordersRepository.GetQuery(
                    filter: filter,
                    orderBy: qo => qo.OrderBy(s => s.Id),
                    includeProperties: "Product, Order");

                searchResultsModel.OrderList = orderItems.OrderByDescending(s => s.Order.Date).ToPagedList(pageNumber, 20);
            }
            else
                searchResultsModel.OrderList = null;

            if (page == null || page == 1 || (page > 1 && pagedctl == "licensekeylist"))
            {
                Expression<Func<LicenseKey, bool>> filter = s => (String.IsNullOrEmpty(q)
                                     || s.Key.ToUpper().Contains(q.ToUpper())
                                     || s.OrderItem.Order.FirstName.ToUpper().Contains(q.ToUpper())
                                     || s.OrderItem.Order.LastName.ToUpper().Contains(q.ToUpper())
                                     || (q.Contains("@") && s.OrderItem.Order.Email.ToUpper().Contains(q))
                                     || s.OrderItem.Order.Company.ToUpper().Contains(q.ToUpper()));

                if (terms.Length > 1)
                {
                    filter = s => (String.IsNullOrEmpty(q)
                                     || s.Key.ToUpper().Contains(q.ToUpper())
                                     || s.OrderItem.Order.FirstName.ToUpper().Contains(q.ToUpper())
                                     || s.OrderItem.Order.LastName.ToUpper().Contains(q.ToUpper())
                                     || s.OrderItem.Order.Company.ToUpper().Contains(q.ToUpper())
                                     || (s.OrderItem.Order.FirstName.ToUpper().Contains(term0) && s.OrderItem.Order.LastName.ToUpper().Contains(term1))
                                     || (s.OrderItem.Order.FirstName.ToUpper().Contains(term1) && s.OrderItem.Order.LastName.ToUpper().Contains(term0)));
                }

                var licenseKeys = licenseKeysRepository.GetQuery(
                    filter: filter,
                    orderBy: qo => qo.OrderBy(s => s.Id),
                    includeProperties: "OrderItem, Product");

                searchResultsModel.LicenseKeyList = licenseKeys.OrderByDescending(k => k.LastActivationTime).ToPagedList(pageNumber, 20);
            }
            else
                searchResultsModel.LicenseKeyList = null;

            if (page == null || page == 1 || (page > 1 && pagedctl == "activationlist"))
            {
                Expression<Func<Activation, bool>> filter = s => (String.IsNullOrEmpty(q)
                                     || s.LicenseKey.Key.ToUpper().Contains(q.ToUpper())
                                     || s.LicenseKey.OrderItem.Order.FirstName.ToUpper().Contains(q.ToUpper())
                                     || s.LicenseKey.OrderItem.Order.LastName.ToUpper().Contains(q.ToUpper())
                                     || (q.Contains("@") && s.LicenseKey.OrderItem.Order.Email.ToUpper().Contains(q.ToUpper()))
                                     || s.LicenseKey.OrderItem.Order.Company.ToUpper().Contains(q.ToUpper()));

                if (terms.Length > 1)
                {
                    filter = s => (String.IsNullOrEmpty(q)
                            || s.LicenseKey.Key.ToUpper().Contains(q.ToUpper())
                            || s.LicenseKey.OrderItem.Order.FirstName.ToUpper().Contains(q.ToUpper())
                            || s.LicenseKey.OrderItem.Order.LastName.ToUpper().Contains(q.ToUpper())
                            || s.LicenseKey.OrderItem.Order.Company.ToUpper().Contains(q.ToUpper())
                            || (s.LicenseKey.OrderItem.Order.FirstName.ToUpper().Contains(term0) && s.LicenseKey.OrderItem.Order.LastName.ToUpper().Contains(term1))
                            || (s.LicenseKey.OrderItem.Order.FirstName.ToUpper().Contains(term1) && s.LicenseKey.OrderItem.Order.LastName.ToUpper().Contains(term0)));
                }

                var activations = activationsRepository.GetQuery(
                    filter: filter,
                    orderBy: qo => qo.OrderByDescending(s => s.Id),
                    includeProperties: "LicenseKey");

                searchResultsModel.ActivationList = activations.OrderByDescending(a => a.ActivationDate).ToPagedList(pageNumber, 20);
            }
            else
                searchResultsModel.ActivationList = null;

            return View("SearchResults", searchResultsModel);
        }
    }
}
