#pragma once

#include <string>

#include "../core/ManagedObject.h"

namespace Lag
{
	class XmlResource : public ManagedObject
	{
	public:
		XmlResource() = default;
		explicit XmlResource(const std::string &path);

		inline void setPath(const std::string &path) { this->path = path; }
		inline const std::string& getPath() const { return path; }

	protected:
		std::string path;
	};
}