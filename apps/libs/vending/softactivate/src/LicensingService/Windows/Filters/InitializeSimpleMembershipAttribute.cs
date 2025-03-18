using System;
using System.Data.Entity;
using System.Data.Entity.Infrastructure;
using System.Threading;
using System.Web.Mvc;
using WebMatrix.WebData;
using Licensing.ControlPanel.Models;
using System.Web.Security;

namespace Licensing.ControlPanel.Filters
{
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = false, Inherited = true)]
    public sealed class InitializeSimpleMembershipAttribute : ActionFilterAttribute
    {
        private static SimpleMembershipInitializer _initializer;
        private static object _initializerLock = new object();
        private static bool _isInitialized;

        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            // Ensure ASP.NET Simple Membership is initialized only once per app start
            LazyInitializer.EnsureInitialized(ref _initializer, ref _isInitialized, ref _initializerLock);
        }

        private class SimpleMembershipInitializer
        {

            public SimpleMembershipInitializer()
            {
                //Database.SetInitializer<UsersContext>(null);

                try
                {
                    /*
                    using (var context = new UsersContext())
                    {
                        if (!context.Database.Exists())
                        {
                            // Create the SimpleMembership database without Entity Framework migration schema
                            ((IObjectContextAdapter)context).ObjectContext.CreateDatabase();
                        }
                    }
                    */
                     
                    if (!WebSecurity.UserExists("admin"))
                        WebSecurity.CreateUserAndAccount("admin", "SoftActivate");

                    // if (!WebSecurity.UserExists("guest"))
                    //    WebSecurity.CreateUserAndAccount("guest", "SoftActivate");

                    if (!Roles.RoleExists("Administrators"))
                        Roles.CreateRole("Administrators");

                    if (!Roles.RoleExists("Users"))
                        Roles.CreateRole("Users");

                    if (!Roles.IsUserInRole("admin", "Administrators"))
                        Roles.AddUsersToRole(new string[] { "admin" }, "Administrators");

                    //if (!Roles.IsUserInRole("guest", "Users"))
                    //    Roles.AddUsersToRole(new string[] { "guest" }, "Users");
                }
                catch (Exception ex)
                {
                    throw new InvalidOperationException("The ASP.NET Simple Membership database could not be initialized. For more information, please see http://go.microsoft.com/fwlink/?LinkId=256588", ex);
                }
            }
        }
    }
}
