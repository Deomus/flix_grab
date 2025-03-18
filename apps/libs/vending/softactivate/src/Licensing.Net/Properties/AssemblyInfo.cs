using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System;

// General Information about an assembly is controlled through the following 
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("Licensing.Net")]
[assembly: AssemblyDescription("")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("")]
[assembly: AssemblyProduct("")]
[assembly: AssemblyCopyright("Copyright © 2015")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]
[assembly: CLSCompliant(true)]

[assembly: Obfuscation(Feature = "code control flow obfuscation", Exclude = false)]
[assembly: Obfuscation(Feature = "string encryption", Exclude = true)]
[assembly: Obfuscation(Feature = "runtime key security", Exclude = true)]
[assembly: Obfuscation(Feature = "Apply to *: renaming", Exclude = true, ApplyToMembers = true)]

// Setting ComVisible to false makes the types in this assembly not visible 
// to COM components.  If you need to access a type in this assembly from 
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("6a687017-d62a-4e77-a7f1-bb49e4062c49")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version 
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Build and Revision Numbers 
// by using the '*' as shown below:
// [assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyVersion("3.0.0.0")]
[assembly: AssemblyFileVersion("3.1.18.0")]
[assembly: AllowPartiallyTrustedCallers]
