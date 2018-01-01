#pragma once

#include <string>

#include "../core/Manager.h"
#include "../io/tinyxml/tinyxml.h"

class TiXmlElement;

namespace Lag
{
	/*
	* Simple abstract ResourceManager that can iterate and create all the declared Resources on the passed in XML file.
	* Each concrete implementation can handle some Resource type from the XML resources file.
	*/
	template<class V>
	class XmlResourceManager : public Manager<std::string, V>
	{
	public:
		XmlResourceManager(const std::string &name, const std::string &folder);
		explicit XmlResourceManager(const std::string &name);

		//TODO: pass some nicer structure instead of the XML file root?
		//Parse resource file and check if there is something of interest for the manager
		void parseResourceFile(const TiXmlElement &resourceFileRoot);

	protected:

		std::string folder;

		//Handle a specific resource description
		virtual void parseResourceDescription(const TiXmlElement &elem) = 0;
	};


	/*
	* DEFINITION HERE 'CAUSE C++ TEMPLATE COMPILATION...
	*/

	template<class V>
	XmlResourceManager<V>::XmlResourceManager(const std::string &name, const std::string &folder) :
		Manager(name),
		folder(folder)
	{
		static_assert(std::is_base_of<XmlResource, V>::value, "Creating a XmlResourceManager of Values not derived from XmlResource");
	}

	template<class V>
	XmlResourceManager<V>::XmlResourceManager(const std::string &name) :
		Manager(name),
		folder("")
	{
	}

	template<class V>
	void XmlResourceManager<V>::parseResourceFile(const TiXmlElement &resourceFileRoot)
	{
		for (const TiXmlElement* elem = resourceFileRoot.FirstChildElement();
			elem != NULL;
			elem = elem->NextSiblingElement())
		{
			parseResourceDescription(*elem);
		}
	}
}