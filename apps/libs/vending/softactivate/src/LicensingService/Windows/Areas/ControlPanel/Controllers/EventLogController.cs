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

namespace Licensing.ControlPanel.Controllers
{
    public class EventLogController : ControlPanelController
    {
        private LicensingServiceDbContext db = new LicensingServiceDbContext();

        //
        // GET: /ControlPanel/EventLog/

        public ActionResult Index(int? page)
        {
            int pageNumber = (page ?? 1);
            return View(db.EventLog.OrderByDescending(item => item.Date).ToPagedList(pageNumber, 20));
        }

        //
        // GET: /ControlPanel/EventLog/Details/5

        public ActionResult Details(int id = 0)
        {
            Event e = db.EventLog.Find(id);
            if (e == null)
            {
                return HttpNotFound();
            }
            return View(e);
        }

        //
        // GET: /ControlPanel/EventLog/Create

        public ActionResult Create()
        {
            return View();
        }

        //
        // POST: /ControlPanel/EventLog/Create

        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create(Event e)
        {
            if (ModelState.IsValid)
            {
                db.EventLog.Add(e);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(e);
        }

        //
        // GET: /ControlPanel/EventLog/Edit/5

        public ActionResult Edit(int id = 0)
        {
            Event e = db.EventLog.Find(id);
            if (e == null)
            {
                return HttpNotFound();
            }
            return View(e);
        }

        //
        // POST: /ControlPanel/EventLog/Edit/5

        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit(Event e)
        {
            if (ModelState.IsValid)
            {
                db.Entry(e).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(e);
        }

        //
        // GET: /ControlPanel/EventLog/Delete/5

        public ActionResult Delete(int id = 0)
        {
            Event e = db.EventLog.Find(id);
            if (e == null)
            {
                return HttpNotFound();
            }
            return View(e);
        }

        //
        // POST: /ControlPanel/EventLog/Delete

        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles="Administrators")]
        public ActionResult DeleteConfirmed()
        {
            db.Database.ExecuteSqlCommand("TRUNCATE TABLE EventLog");
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            db.Dispose();
            base.Dispose(disposing);
        }
    }
}