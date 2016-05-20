#pragma once

#include <string>

namespace Lag
{
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