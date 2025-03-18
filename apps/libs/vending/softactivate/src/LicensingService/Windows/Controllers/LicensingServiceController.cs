using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Http;
using LicensingService.Filters;

namespace LicensingService.Controllers
{
    [WebApiInitializeLicensingDatabase]
    public class LicensingServiceController : ApiController
    {

    }
}
