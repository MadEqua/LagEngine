#pragma once

#include <string>

#include "../core/ManagedObject.h"

namespace Lag
{
	class Resource : public ManagedObject
	{
	public:
		Resource();
		Resource(const std::string &path);
		virtual ~Resource();

		inline void setPath(const std::string &path) { this->path = path; }
		inline const std::string& getPath() const { return path; }

	protected:
		std::string path;
	};
}