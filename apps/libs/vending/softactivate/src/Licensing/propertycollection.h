#pragma once

#include "precomp.h"
#include "uniconv.h"
#include "tinyxml2.h"
#include <map>
#include <list>

#ifndef MAX_XML_BUFFER_SIZE
#define MAX_XML_BUFFER_SIZE 4096
#endif

class PathEntry 
{
	friend class PropertyCollection;

public:
	PathEntry()
	{
		processed = false;
	}

	PathEntry(const PathEntry & copy)
	{
		processed = false;
		path = copy.path;
		nameValueCollection = copy.nameValueCollection;
	}

	PathEntry(const PathEntry * copy)
	{
		processed = false;
		path = copy->path;
		nameValueCollection = copy->nameValueCollection;
	}

private:
	string_t path;
	std::map<string_t, string_t> nameValueCollection;
	bool processed;
};

class PropertyCollection
{
public:
	PropertyCollection()
	{

	}

	void SetProperty(const char_t * path, const char_t * name, const char_t * value)
	{
		string_t propName = (name != NULL) ? name : _T("");
		std::map<string_t, PathEntry>::iterator entry;

		if ((entry = m_properties.find(path)) == m_properties.end())
		{
			PathEntry entry;
			entry.path = path;
			entry.nameValueCollection.insert(std::map<string_t, string_t>::value_type(propName, value));
			m_properties.insert(std::map<string_t, PathEntry>::value_type(path, entry));
		} else
			entry->second.nameValueCollection.insert(std::map<string_t, string_t>::value_type(propName, value));
	}

	const char_t * GetProperty(const char_t * path, const char_t * name)
	{
		std::map<string_t, PathEntry>::iterator entry = m_properties.find(path);
		
		if (entry == m_properties.end())
			return NULL;

		std::map<string_t, string_t>::iterator nv = entry->second.nameValueCollection.find((name != NULL) ? name : _T(""));

		if (nv == entry->second.nameValueCollection.end())
			return NULL;

		return nv->second.c_str();
	}
	
	void SaveXml(std::string & xml)
	{
		PathListToXml("/", &m_properties, xml, 1);
	}

	void LoadXml(tinyxml2::XMLElement * src)
	{
		XmlToPathList("", src);
	}

private: 
	std::map<string_t, PathEntry> m_properties;

	void XmlToPathList(const std::string prefix, const tinyxml2::XMLElement * node)
	{
		USES_CONVERSION;
		USES_CONVERSION_2;

		const char * text = node->GetText();

		if (text != NULL)
			SetProperty(UTF82T_2(prefix.c_str()), _T(""), UTF82T_2(text));

		const tinyxml2::XMLAttribute * attr = node->FirstAttribute();

		// risk of stack overflow because _alloca is called in loop. However the current number of properties
		// is very low so it is acceptable
		while (attr)
		{
			SetProperty(UTF82T_2(prefix.c_str()), UTF82T_2(attr->Name()), UTF82T_2(attr->Value()));

			attr = attr->Next();
		}
		
		tinyxml2::XMLElement * child;

		for (child = (tinyxml2::XMLElement *)node->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			XmlToPathList(prefix + "/" + child->Name(), child);
		}
	}

	void PathListToXml(const std::string pathPrefix, std::map<string_t, PathEntry> * sortedPathList, std::string & xml, int indent)
	{
		USES_CONVERSION;
		USES_CONVERSION_2;

		std::string elementValue;
		bool startElementClosed = false;

		if (pathPrefix != "/")
		{
			if (indent > 0)
				xml.append(indent, '\t');

			xml.append("<"); xml.append(1 + strrchr(pathPrefix.c_str(), '/'));
		}

		for (std::map<string_t, PathEntry>::const_iterator item = sortedPathList->begin(); item != sortedPathList->end(); item++)
		{
			if (strcmp(T2UTF8(item->second.path.c_str()), pathPrefix.c_str()) == 0)
			{
				for (std::map<string_t, string_t>::const_iterator nvItem = item->second.nameValueCollection.begin(); 
					 nvItem != item->second.nameValueCollection.end(); 
					 nvItem++)
				{
					if (nvItem->first.length() == 0)
					{
						elementValue = T2UTF8(nvItem->second.c_str());
					} else
					{
						xml.append(" ");
						xml.append(T2UTF8(nvItem->first.c_str()));
						xml.append("=\"");
						xml.append(T2UTF8_2(nvItem->second.c_str()));
						xml.append("\"");
					}
				}
			}
		}

		if (pathPrefix != "/")
		{
			xml.append(">");

			if (elementValue.length() > 0)
			{
				xml.append(elementValue);
			} else
				if (indent >= 0)
					xml.append("\n");
		}

		bool finished = false;

		while (!finished)
		{
			finished = true;
			std::string firstUnprocessedPrefix;
			
			std::map<string_t, PathEntry> innerPathList;

			for (std::map<string_t, PathEntry>::iterator item = sortedPathList->begin(); item != sortedPathList->end(); item++)
			{
				if (!item->second.processed && item->second.path.length() > pathPrefix.length() && (firstUnprocessedPrefix.empty() || strncmp(T2UTF8(item->second.path.c_str()), firstUnprocessedPrefix.c_str(), firstUnprocessedPrefix.length()) == 0))
				{
                    if (firstUnprocessedPrefix.empty())
                    {
                        int nextSep = item->second.path.find('/', pathPrefix.length() + 1);
                        firstUnprocessedPrefix = T2UTF8(item->second.path.substr(0, (nextSep >= 0) ? nextSep : item->second.path.length()).c_str());
                    }

                    innerPathList.insert(std::map<string_t, PathEntry>::value_type(item->first, item->second));

                    item->second.processed = true;
                    finished = false;
				}
			}
			
			if (!finished)
				PathListToXml(firstUnprocessedPrefix, &innerPathList, xml, (indent >= 0) ? indent + 1 : indent);
		}

		if (pathPrefix != "/")
		{
			if (indent > 0 && elementValue.length() == 0)
				xml.append(indent, '\t');

			xml.append("</"); xml.append(1 + strrchr(pathPrefix.c_str(), '/')); xml.append(">");

			if (indent >= 0) xml.append("\n");
		}
	}
};
