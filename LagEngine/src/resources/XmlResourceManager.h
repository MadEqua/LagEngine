#pragma once

#include <string>

#include "../core/Manager.h"
#include "tinyxml/tinyxml.h"

#include "../utils/Utils.h"
#include "../Constants.h"


class TiXmlElement;

namespace Lag
{
	struct XmlResourceBuilderData
	{
		const TiXmlDocument &appResourcesFile;
		const TiXmlDocument &lagResourcesFile;
		const std::string appResourceFolderPath;
		const std::string lagResourceFolderPath;
		const std::string tagToParse;

		XmlResourceBuilderData(const TiXmlDocument &appResourcesFile, const TiXmlDocument &lagResourcesFile, 
			const std::string &appResourceFolderPath, const std::string &lagResourceFolderPath,
			const std::string tagToParse) :
			appResourcesFile(appResourcesFile), lagResourcesFile(lagResourcesFile),
			appResourceFolderPath(appResourceFolderPath), lagResourceFolderPath(lagResourceFolderPath),
			tagToParse(tagToParse) {}
	};
	
	template<class V>
	class XmlResourceBuilder : public IManagedObjectBuilder<std::string, V>
	{
	public:
		XmlResourceBuilder(const XmlResourceBuilderData &xmlResourceData);
		virtual V* build(const std::string &name) const override;

	protected:
		const TiXmlElement* findResourceNameOnXml(const std::string &name) const;
		virtual V* parseAndCreate(const std::string &path, const TiXmlElement &element) const = 0;
		const std::string parseFileAttribute(const TiXmlElement &element) const;

		const XmlResourceBuilderData xmlResourceData;
	};
	
	
	/*
	* Each concrete implementation can handle some Resource type from the XML resources file.
	*/
	template<class V>
	class XmlResourceManager : public Manager<std::string, V>
	{
	public:
		XmlResourceManager(const std::string &name, XmlResourceBuilder<V> *builder);

		std::string getNameByFileName(const std::string &name) const;
	};



	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/
	template<class V>
	XmlResourceBuilder<V>::XmlResourceBuilder(const XmlResourceBuilderData &xmlResourceData) :
		xmlResourceData(xmlResourceData)
	{
		// Compile-time sanity check
		static_assert(std::is_base_of<XmlResource, V>::value, "Creating a XmlResourceBuilder of Vs not derived from XmlResource");
	}

	template<class V>
	V* XmlResourceBuilder<V>::build(const std::string &name) const
	{
		const TiXmlElement* element = findResourceNameOnXml(name);
		if (element != nullptr) 
		{
			const std::string &path = element->GetDocument() == &xmlResourceData.appResourcesFile ? 
				xmlResourceData.appResourceFolderPath : xmlResourceData.lagResourceFolderPath;
			return parseAndCreate(path, *element);
		}
		else
			return nullptr;
	}

	template<class V>
	const TiXmlElement* XmlResourceBuilder<V>::findResourceNameOnXml(const std::string &name) const
	{
		for (const TiXmlElement* elem = xmlResourceData.appResourcesFile.FirstChildElement()->FirstChildElement();
			elem != NULL;
			elem = elem->NextSiblingElement())
		{
			if (xmlResourceData.tagToParse == elem->ValueStr() && name == elem->Attribute(NAME_XML_ATTR))
				return elem;
		}

		for (const TiXmlElement* elem = xmlResourceData.lagResourcesFile.FirstChildElement()->FirstChildElement();
			elem != NULL;
			elem = elem->NextSiblingElement())
		{
			if (xmlResourceData.tagToParse == elem->ValueStr() && name == elem->Attribute(NAME_XML_ATTR))
				return elem;
		}

		return nullptr;
	}

	template<class V>
	const std::string XmlResourceBuilder<V>::parseFileAttribute(const TiXmlElement &element) const
	{
		const char* file = element.Attribute(FILE_XML_ATTR);
		if (!file)
		{
			LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "XmlResourceBuilder",
				"A <" + xmlResourceData.tagToParse + "> element on the Resources file does not contain the required attribute <file>.");
			return "";
		}
		return file;
	}


	template<class V>
	XmlResourceManager<V>::XmlResourceManager(const std::string &name, XmlResourceBuilder<V> *builder) :
		Manager(name, builder)
	{
		static_assert(std::is_base_of<XmlResource, V>::value, "Creating a XmlResourceManager of Values not derived from XmlResource");
	}

	template<class V>
	std::string XmlResourceManager<V>::getNameByFileName(const std::string &name) const
	{
		for (auto &pair : objects)
		{
			std::string file = Utils::getFileNameFromPath(pair.second->getPath());
			
			if (file == name)
				return pair.first;
		}

		return std::string();
	}
}