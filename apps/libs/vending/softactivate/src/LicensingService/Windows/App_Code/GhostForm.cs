using System;
using System.Web.UI;
using System.Web.UI.HtmlControls;

/// <summary>
/// This is a special form that can _not_ render the actual form tag, but always render the contents
/// </summary>
namespace SoftActivate
{
    public class GhostForm : System.Web.UI.HtmlControls.HtmlForm
    {
        protected bool _render;

        public bool RenderFormTag
        {
            get { return _render; }
            set { _render = value; }
        }

        public GhostForm()
        {
            //By default, show the form tag
            _render = true;
        }

        protected override void RenderBeginTag(HtmlTextWriter writer)
        {
            //Only render the tag when _render is set to true
            if (_render)
                base.RenderBeginTag(writer);
        }

        protected override void RenderEndTag(HtmlTextWriter writer)
        {
            //Only render the tag when _render is set to true
            if (_render)
                base.RenderEndTag(writer);
        }
    }
}
