using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace SoftActivate.Licensing
{
    class PropertyCollection
    {
        public void SetProperty(string path, string name, string value)
        {
            if (name == null) name = "";

            if (!propertyCollection.ContainsKey(path) && value != null)
            {
                PathEntry val = new PathEntry();
                val.path = path;
                val.nameValueCollection[name] = value;
                propertyCollection.Add(path, val);
            }
            else
            {
                if (value == null)
                {
                    if (String.IsNullOrEmpty(name))
                        propertyCollection.Remove(path);
                    else
                        propertyCollection[path].nameValueCollection.Remove(name);
                }
                else
                    propertyCollection[path].nameValueCollection[name] = value;
            }
        }

        public string GetProperty(string path, string name)
        {
            PathEntry  entry;
            string val;

            if (!propertyCollection.TryGetValue(path, out entry))
                return null;

            if (name == null) name = "";
            
            return entry.nameValueCollection.TryGetValue(name, out val) ? val : null;
        }

        public string SaveXml()
        {
            TextWriter s = new Utf8StringWriter();

            XmlWriter xw = XmlWriter.Create(s, new XmlWriterSettings { Indent = true });

            xw.WriteStartDocument();

            SaveXml(xw);

            xw.WriteEndDocument();

            xw.Close();

            return s.ToString();
        }

        public void SaveXml(XmlWriter xw)
        {
            foreach (PathEntry entry in propertyCollection.Values)
            {
                entry.processed = false;
            }

            PathListToXml("/", propertyCollection.Values, xw);
        }

        public void LoadXml(XmlNode rootNode)
        {
            XmlToPathList("", rootNode);
        }

        private SortedDictionary<string, PathEntry> propertyCollection = new SortedDictionary<string, PathEntry>();

        private void XmlToPathList(string prefix, XmlNode node)
        {
            XmlNode textNode = node.SelectSingleNode("text()");
            
            if (textNode != null)
                if (!String.IsNullOrEmpty(textNode.Value))
                    SetProperty(prefix, null, textNode.Value);

            XmlAttributeCollection attrList = node.Attributes;

            if (attrList.Count > 0)
            {
                foreach (XmlAttribute attr in attrList)
                {
                    SetProperty(prefix, attr.Name, attr.Value);
                }
            }
            
            foreach (XmlNode child in node.SelectNodes("*"))
            {
                XmlToPathList(prefix + "/" + child.Name, child);
            }
        }

        private void PathListToXml(string pathPrefix, IEnumerable<PathEntry> sortedPathList, XmlWriter xw)
        {
            bool finished = false;

            if (pathPrefix != "/") xw.WriteStartElement(pathPrefix.Substring(pathPrefix.LastIndexOf('/') + 1));

            foreach (PathEntry entry in sortedPathList)
            {
                if (entry.path == pathPrefix)
                {
                    foreach (KeyValuePair<string, string> item in entry.nameValueCollection)
                    {
                        if (String.IsNullOrEmpty(item.Key))
                            xw.WriteString(item.Value);
                        else
                            xw.WriteAttributeString(item.Key, item.Value);
                    }
                }
            }

            while (!finished)
            {
                finished = true;
                string firstUnprocessedPrefix = null;
                List<PathEntry> innerPathList = new List<PathEntry>();

                foreach (PathEntry val in sortedPathList)
                {
                    if (!val.processed && val.path.Length > pathPrefix.Length && (firstUnprocessedPrefix == null || val.path.StartsWith(firstUnprocessedPrefix)))
                    {
                        if (firstUnprocessedPrefix == null)
                        {
                            int nextSep = val.path.IndexOf('/', pathPrefix.Length + 1);
                            firstUnprocessedPrefix = val.path.Substring(0, (nextSep >= 0) ? nextSep : val.path.Length);
                        }

                        innerPathList.Add(new PathEntry(val));

                        val.processed = true;
                        finished = false;
                    }
                }

                if (!finished)
                    PathListToXml(firstUnprocessedPrefix, innerPathList, xw);
            }

            if (pathPrefix != "/") xw.WriteEndElement();
        }

        private class PathEntry
        {
            public PathEntry()
            {

            }

            public PathEntry(PathEntry v)
            {
                this.processed = false;
                this.path = v.path;
                this.nameValueCollection = v.nameValueCollection;
            }

            public bool processed = false;
            public string path;
            public Dictionary<string, string> nameValueCollection = new Dictionary<string, string>();
        }

        private class Utf8StringWriter : StringWriter
        {
            public override Encoding Encoding
            {
                get
                {
                    return Encoding.UTF8;
                }
            }
        }
    }
}
