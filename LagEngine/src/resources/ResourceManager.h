#pragma once

#include <string>
#include <unordered_map>

class TiXmlElement;

namespace Lag
{
	class Resource;
	class IResourceFactory;

	/*
	* Simple abstract ResourceManager that can iterate and create all the declared Resources on the passed in XML file.
	* Loads Resources on the act of adding them to itself. (no delayed load yet)
	* Each concrete implementation can handle some Resource type from the XML resources file.
	*/
	class ResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		//TODO: pass some nicer structure instead of the XML file root?
		void initalizeFromResourcesFile(const TiXmlElement &resourceFileRoot);

		Resource* get(const std::string &name) const;
		void create(const std::string &name, const IResourceFactory &factory);

	protected:

		void add(const std::string &name, Resource *res);

		//Handle a specific resource description
		virtual void parseResourceDescription(const TiXmlElement &element) = 0;
		
		std::unordered_map<std::string, Resource*> resources;
	};
}