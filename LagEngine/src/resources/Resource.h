#pragma once

#include <string>

namespace Lag
{
	/*
	* A Resource is any object that we don't want duplicates while using them. 
	* They should be reutilized by any number of other objects depending on them. 
	* This is accomplished by a Manager for each Resource type.
	* Notable examples include objects loaded from the hard drive (Textures, GpuProgramStages, Meshes), 
	* but also other "memory only" objects like GpuPrograms.
	*/
	class Resource
	{
	public:
		Resource(const std::string &path);
		Resource();
		virtual ~Resource();

		virtual bool load() = 0;
		virtual void unload() = 0;

		inline void setPath(const std::string &path) { this->path = path; }
		inline const std::string& getPath() const { return path; }

		inline bool isLoaded() const { return loaded; }

	protected:
		bool loaded;
		std::string path;
	};
}