using Licensing.Web.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Web;

namespace Licensing.Web.DAL
{
    public class LicensingServiceDbContext : DbContext
    {
        public DbSet<ConfigurationItem> Configuration { get; set; }
        public DbSet<Product> Products { get; set; }
        public DbSet<Order> Orders { get; set; }
        public DbSet<OrderItem> OrderItems { get; set; } 
        public DbSet<LicenseKey> LicenseKeys { get; set; }
        public DbSet<Activation> Activations { get; set; }
        public DbSet<Event> EventLog { get; set; }
        public DbSet<PaymentServiceMapping> PaymentServiceMappings { get; set; }
        public DbSet<Download> Downloads { get; set; }

        public LicensingServiceDbContext()
            : base("LicensingServiceConnection")
        {

        }

        static LicensingServiceDbContext()
        {
            Database.SetInitializer(new LicensingDatabaseInitializer());
        }
    }
}
