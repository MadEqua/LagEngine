#pragma once

#include <string>
#include <unordered_map>

class TiXmlElement;

namespace Lag
{
	class Resource;

	/*
	* Simple abstract ResourceManager that can iterate and add all the declared Resources on the passed in XML file.
	* Loads Resources on the act of adding them to itself.
	* Each concrete implementation must handle some Resource type from the XML resources file.
	*/
	class ResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		//TODO: pass some nicer structure instead of the XML file root?
		void initalizeFromResourcesFile(const TiXmlElement &resourceFileRoot);

		void add(const std::string &name, Resource *res);
		Resource* get(const std::string &name) const;

	protected:

		//Each particular ResourceManager should handle a specific resource description
		virtual void parseResourceDescription(const TiXmlElement &element) = 0;
		
		std::unordered_map<std::string, Resource*> resources;
	};
}