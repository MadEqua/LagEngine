#pragma once

#include <string>

#include "../core/Manager.h"
#include "Resource.h"

class TiXmlElement;

namespace Lag
{
	class IResourceFactory;

	/*
	* Simple abstract ResourceManager that can iterate and create all the declared Resources on the passed in XML file.
	* Loads Resources on the act of adding them to itself. (no delayed load yet)
	* Each concrete implementation can handle some Resource type from the XML resources file.
	*/
	class ResourceManager : public Manager<std::string, Resource>
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		//TODO: pass some nicer structure instead of the XML file root?
		void initalizeFromResourcesFile(const TiXmlElement &resourceFileRoot);

		virtual bool add(const std::string &name, Resource *res) override;

	protected:

		//Handle a specific resource description
		virtual void parseResourceDescription(const TiXmlElement &element) = 0;
	};
}