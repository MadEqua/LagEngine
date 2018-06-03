#pragma once

#include <string>

#include "ManagedObject.h"

namespace Lag {
    /*
    * Resource which metadata can be read from the XML Resources file.
    */
    class XmlResource : public ManagedObject {
    public:
        XmlResource() = default;
        explicit XmlResource(const std::string &path);

        inline void setPath(const std::string &path) { this->path = path; }
        inline const std::string &getPath() const { return path; }

    protected:
        std::string path;
    };
}