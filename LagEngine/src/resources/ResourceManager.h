#pragma once

#include <string>

#include "../core/Manager.h"

class TiXmlElement;

namespace Lag
{
	class IResourceFactory;

	/*
	* Simple abstract ResourceManager that can iterate and create all the declared Resources on the passed in XML file.
	* Each concrete implementation can handle some Resource type from the XML resources file.
	*/
	class ResourceManager : public Manager<std::string>
	{
	public:
		ResourceManager(const std::string &name, const std::string &folder);
		explicit ResourceManager(const std::string &name);

		//TODO: pass some nicer structure instead of the XML file root?
		//Parse resource file and check if there is something of interest for the manager
		void parseResourceFile(const TiXmlElement &resourceFileRoot);

	protected:

		std::string folder;

		//Handle a specific resource description
		virtual void parseResourceDescription(const TiXmlElement &elem) = 0;
	};
}