using System;
using System.Collections.Generic;
using System.Data.Entity.Migrations;
using System.Linq;
using System.Web;

namespace Licensing.Web.DAL
{
    public class LicensingDatabaseMigrationConfiguration : DbMigrationsConfiguration<LicensingServiceDbContext>
    {
        private readonly bool _pendingMigrations;

        public LicensingDatabaseMigrationConfiguration()
        {
            AutomaticMigrationsEnabled = true;

            var migrator = new DbMigrator(this);
            _pendingMigrations = migrator.GetPendingMigrations().Any();
        }

        protected override void Seed(LicensingServiceDbContext context)
        {
            //Microsoft comment says "This method will be called after migrating to the latest version."
            //However my testing shows that it is called every time the software starts

            //Exit if there aren't any pending migrations
            if (!_pendingMigrations) return;

            //else run your code to seed the database, e.g.
            
            //context.Foos.AddOrUpdate(new Foo { bar = true });
        }
    }
}
