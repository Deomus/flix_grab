using Licensing.Web.DAL;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Threading;
using System.Web;

namespace Licensing.Web.DAL
{
    public class LicensingDatabaseInitializer : IDatabaseInitializer<LicensingServiceDbContext>
    {
        public void InitializeDatabase(LicensingServiceDbContext context)
        {

        }
    }
}
