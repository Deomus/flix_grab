using System;
using System.Security;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Windows.Forms;

namespace SoftActivate.Licensing
{
    internal static class WebBrowserExtensions
    {
        internal static bool LoadHtmlIntoBrowser(WebBrowser webBrowser, string html, string baseUrl)
        {
            bool ret = false;

            IWebBrowser2 axIWebBrowser2 = (IWebBrowser2)webBrowser.ActiveXInstance;

            if ((axIWebBrowser2 == null) || (html.Length == 0))
                return ret;

            IStream stream = null;

            //Use Marshal.StringToHGlobalAnsi to get ANSI
            int hr = WinApis.CreateStreamOnHGlobal(Marshal.StringToHGlobalAuto(html), true, out stream);
            if ((hr != 0) || (stream == null))
                return ret;

            IPersistMoniker pPM = axIWebBrowser2.Document as IPersistMoniker;

            if (pPM == null)
                return ret;

            IBindCtx bindctx = null;

            WinApis.CreateBindCtx((uint)0, out bindctx);
            if (bindctx == null)
                return ret;

            LoadHTMLMoniker loader = new LoadHTMLMoniker();

            if (string.IsNullOrEmpty(baseUrl))
                baseUrl = axIWebBrowser2.LocationURL;

            loader.InitLoader(html, baseUrl);

            pPM.Load(1, loader, bindctx, WinApis.STGM_READ);

            stream = null;

            return true;
        }

        sealed class WinApis
        {
            public const uint MAX_PATH = 512;
            public const uint STGM_READ = 0x00000000;
            public const uint SHDVID_SSLSTATUS = 33;
            public const int GWL_WNDPROC = -4;
            public const uint KEYEVENTF_EXTENDEDKEY = 0x01;
            public const uint KEYEVENTF_KEYUP = 0x02;

            public const short
                //defined inWTypes.h
                // 0 == FALSE, -1 == TRUE
                //typedef short VARIANT_BOOL;
                VAR_TRUE = -1,
                VAR_FALSE = 0;

            [DllImport("ole32.dll", ExactSpelling = true, CharSet = CharSet.Auto)]
            public static extern int CreateBindCtx(
                [MarshalAs(UnmanagedType.U4)] uint dwReserved,
                [Out, MarshalAs(UnmanagedType.Interface)] out IBindCtx ppbc);

            [DllImport("ole32.dll", CharSet = CharSet.Auto)]
            public static extern int CreateStreamOnHGlobal(IntPtr hGlobal, bool fDeleteOnRelease,
              [MarshalAs(UnmanagedType.Interface)] out IStream ppstm);
        }

        [ComImport, SuppressUnmanagedCodeSecurity, TypeLibType(TypeLibTypeFlags.FOleAutomation | (TypeLibTypeFlags.FDual | TypeLibTypeFlags.FHidden)), Guid("D30C1661-CDAF-11d0-8A3E-00C04FC9E26E")]
        interface IWebBrowser2
        {
            [DispId(100)]
            void GoBack();
            [DispId(0x65)]
            void GoForward();
            [DispId(0x66)]
            void GoHome();
            [DispId(0x67)]
            void GoSearch();
            [DispId(0x68)]
            void Navigate([In] string Url, [In] ref object flags, [In] ref object targetFrameName, [In] ref object postData, [In] ref object headers);
            [DispId(-550)]
            void Refresh();
            [DispId(0x69)]
            void Refresh2([In] ref object level);
            [DispId(0x6a)]
            void Stop();
            [DispId(200)]
            object Application { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
            [DispId(0xc9)]
            object Parent { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
            [DispId(0xca)]
            object Container { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
            [DispId(0xcb)]
            object Document { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
            [DispId(0xcc)]
            bool TopLevelContainer { get; }
            [DispId(0xcd)]
            string Type { get; }
            [DispId(0xce)]
            int Left { get; set; }
            [DispId(0xcf)]
            int Top { get; set; }
            [DispId(0xd0)]
            int Width { get; set; }
            [DispId(0xd1)]
            int Height { get; set; }
            [DispId(210)]
            string LocationName { get; }
            [DispId(0xd3)]
            string LocationURL { get; }
            [DispId(0xd4)]
            bool Busy { get; }
            [DispId(300)]
            void Quit();
            [DispId(0x12d)]
            void ClientToWindow(out int pcx, out int pcy);
            [DispId(0x12e)]
            void PutProperty([In] string property, [In] object vtValue);
            [DispId(0x12f)]
            object GetProperty([In] string property);
            [DispId(0)]
            string Name { get; }
            [DispId(-515)]
            int HWND { get; }
            [DispId(400)]
            string FullName { get; }
            [DispId(0x191)]
            string Path { get; }
            [DispId(0x192)]
            bool Visible { get; set; }
            [DispId(0x193)]
            bool StatusBar { get; set; }
            [DispId(0x194)]
            string StatusText { get; set; }
            [DispId(0x195)]
            int ToolBar { get; set; }
            [DispId(0x196)]
            bool MenuBar { get; set; }
            [DispId(0x197)]
            bool FullScreen { get; set; }
            [DispId(500)]
            void Navigate2([In] ref object URL, [In] ref object flags, [In] ref object targetFrameName, [In] ref object postData, [In] ref object headers);
            [DispId(0x1f5)]
            int QueryStatusWB([In] int cmdID);
            [DispId(0x1f6)]
            void ExecWB([In] int cmdID, [In] int cmdexecopt, ref object pvaIn, IntPtr pvaOut);
            [DispId(0x1f7)]
            void ShowBrowserBar([In] ref object pvaClsid, [In] ref object pvarShow, [In] ref object pvarSize);
            [DispId(-525)]
            WebBrowserReadyState ReadyState { get; }
            [DispId(550)]
            bool Offline { get; set; }
            [DispId(0x227)]
            bool Silent { get; set; }
            [DispId(0x228)]
            bool RegisterAsBrowser { get; set; }
            [DispId(0x229)]
            bool RegisterAsDropTarget { get; set; }
            [DispId(0x22a)]
            bool TheaterMode { get; set; }
            [DispId(0x22b)]
            bool AddressBar { get; set; }
            [DispId(0x22c)]
            bool Resizable { get; set; }
        }

        [ComVisible(true), ComImport(),
        Guid("79EAC9D3-BAF9-11CE-8C82-00AA004BA90B"),
        InterfaceTypeAttribute(ComInterfaceType.InterfaceIsIUnknown)]
        interface IAsyncMoniker
        {

        }

        [ComImport, ComVisible(true),
        Guid("79eac9c9-baf9-11ce-8c82-00aa004ba90b"),
        InterfaceTypeAttribute(ComInterfaceType.InterfaceIsIUnknown)]
        interface IPersistMoniker
        {
            void GetClassID(
                [In, Out] ref Guid pClassID);

            [return: MarshalAs(UnmanagedType.I4)]
            [PreserveSig]
            int IsDirty();

            void Load([In] int fFullyAvailable,
                [In, MarshalAs(UnmanagedType.Interface)] IMoniker pmk,
                [In, MarshalAs(UnmanagedType.Interface)] Object pbc,
                [In, MarshalAs(UnmanagedType.U4)] uint grfMode);

            void SaveCompleted(
                [In, MarshalAs(UnmanagedType.Interface)] IMoniker pmk,
                [In, MarshalAs(UnmanagedType.Interface)] Object pbc);

            [return: MarshalAs(UnmanagedType.Interface)]
            IMoniker GetCurMoniker();
        }

        /// <summary>
        /// For loading html into document while having the ability to set the baseUrl
        /// Only two methods of IMoniker are called BindToStorage and GetDisplayName
        /// In BindToStorage, we pass a ref to our stream object to be used for loading page
        /// data. In GetDisplayName, we pass our baseUrl to MSHTML to be used.
        /// BaseUrl must be in form of http://www.sitename.com
        /// <code>
        /// string html = "<HTML><HEAD></Head><Body><p><a href=\"gosearch\">Second link</a></p><p>1 This HTML content is being loaded from a stream.</p><p>2 This HTML content is being loaded from a stream.</p><p><a href=\"goHome\">Second link</a></p></Body></HTML>";
        /// m_CurWB.LoadHtmlIntoBrowser(html, "http://www.google.com");
        /// </code>
        /// </summary>
        class LoadHTMLMoniker : IMoniker, IAsyncMoniker
        {
            private IStream m_stream = null;
            private string m_sBaseName = string.Empty;

            public void InitLoader(string sContent, string sBaseUrl)
            {
                m_sBaseName = sBaseUrl;
                int hr = WinApis.CreateStreamOnHGlobal(Marshal.StringToHGlobalAuto(sContent), true, out m_stream);
                if ((hr != 0) || (m_stream == null))
                    return;
            }

            void IMoniker.BindToObject(IBindCtx pbc, IMoniker pmkToLeft, ref Guid riidResult, out object ppvResult)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.BindToStorage(IBindCtx pbc, IMoniker pmkToLeft, ref Guid riid, out object ppvObj)
            {
                ppvObj = null;
                if (riid.Equals(/* IID_IStream */new Guid("0000000c-0000-0000-C000-000000000046")))
                    ppvObj = (IStream)m_stream;
            }

            void IMoniker.CommonPrefixWith(IMoniker pmkOther, out IMoniker ppmkPrefix)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.ComposeWith(IMoniker pmkRight, bool fOnlyIfNotGeneric, out IMoniker ppmkComposite)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.Enum(bool fForward, out IEnumMoniker ppenumMoniker)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.GetClassID(out Guid pClassID)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.GetDisplayName(IBindCtx pbc, IMoniker pmkToLeft, out string ppszDisplayName)
            {
                ppszDisplayName = m_sBaseName;
            }

            void IMoniker.GetSizeMax(out long pcbSize)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.GetTimeOfLastChange(IBindCtx pbc, IMoniker pmkToLeft, out System.Runtime.InteropServices.ComTypes.FILETIME pFileTime)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.Hash(out int pdwHash)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.Inverse(out IMoniker ppmk)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            int IMoniker.IsDirty()
            {
                throw new Exception("The method or operation is not implemented.");
            }

            int IMoniker.IsEqual(IMoniker pmkOtherMoniker)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            int IMoniker.IsRunning(IBindCtx pbc, IMoniker pmkToLeft, IMoniker pmkNewlyRunning)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            int IMoniker.IsSystemMoniker(out int pdwMksys)
            {
                pdwMksys = 0;
                return 1; // S_FALSE

                /* throw new Exception("The method or operation is not implemented."); */
            }

            void IMoniker.Load(IStream pStm)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.ParseDisplayName(IBindCtx pbc, IMoniker pmkToLeft, string pszDisplayName, out int pchEaten, out IMoniker ppmkOut)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.Reduce(IBindCtx pbc, int dwReduceHowFar, ref IMoniker ppmkToLeft, out IMoniker ppmkReduced)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.RelativePathTo(IMoniker pmkOther, out IMoniker ppmkRelPath)
            {
                throw new Exception("The method or operation is not implemented.");
            }

            void IMoniker.Save(IStream pStm, bool fClearDirty)
            {
                throw new Exception("The method or operation is not implemented.");
            }
        }
    }
}

/*
namespace System.Runtime.CompilerServices
{
    [AttributeUsage(AttributeTargets.Assembly | AttributeTargets.Class
         | AttributeTargets.Method)]
    public sealed class ExtensionAttribute : Attribute { }
}
*/
