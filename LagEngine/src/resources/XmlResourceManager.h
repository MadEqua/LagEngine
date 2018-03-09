#pragma once

#include <string>

#include "../core/Manager.h"
#include "tinyxml/tinyxml.h"

class TiXmlElement;

namespace Lag
{
	template<class V>
	class XmlResourceBuilder : public IManagedObjectBuilder<std::string, V>
	{
	public:
		XmlResourceBuilder(const std::string &tagToParse, const TiXmlDocument &resourcesXml, const std::string &resourceFolderPath) :
			tagToParse(tagToParse),
			resourcesXml(resourcesXml),
			resourceFolderPath(resourceFolderPath)
		{
			// Compile-time sanity check
			static_assert(std::is_base_of<XmlResource, V>::value, "Creating a XmlResourceBuilder of Vs not derived from XmlResource");
		}

		virtual V* build(const std::string &name) const override
		{
			const TiXmlElement* element = findResourceNameOnXml(name);
			if (element != nullptr)
				return parseAndCreate(name, *element);
			else
				return nullptr;
		}

	protected:
		const std::string parseFileAttribute(const TiXmlElement &element) const
		{
			const char* file = element.Attribute("file");
			if (!file)
			{
				LogManager::getInstance().log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, "XmlResourceBuilder",
					"A <" + tagToParse + "> element on the Resources file does not contain the required attribute <file>.");
				return "";
			}
			return file;
		}

		const TiXmlElement* findResourceNameOnXml(const std::string &name) const;
		virtual V* parseAndCreate(const std::string &name, const TiXmlElement &element) const = 0;

		const TiXmlDocument &resourcesXml;
		std::string tagToParse;
		std::string resourceFolderPath;
	};
	
	
	/*
	* Each concrete implementation can handle some Resource type from the XML resources file.
	*/
	template<class V>
	class XmlResourceManager : public Manager<std::string, V>
	{
	public:
		XmlResourceManager(const std::string &name, XmlResourceBuilder<V> *builder);
	};


	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/

	template<class V>
	const TiXmlElement* XmlResourceBuilder<V>::findResourceNameOnXml(const std::string &name) const
	{
		for (const TiXmlElement* elem = resourcesXml.FirstChildElement()->FirstChildElement();
			elem != NULL;
			elem = elem->NextSiblingElement())
		{
			if (tagToParse == elem->ValueStr() && name == elem->Attribute("name"))
				return elem;
		}
		return nullptr;
	}

	template<class V>
	XmlResourceManager<V>::XmlResourceManager(const std::string &name, XmlResourceBuilder<V> *builder) :
		Manager(name, builder)
	{
		static_assert(std::is_base_of<XmlResource, V>::value, "Creating a XmlResourceManager of Values not derived from XmlResource");
	}
}