#pragma once

#include <string>
#include "IniSettings.h"

namespace Lag {
    class IniParser {
    public:
        bool parse(const std::string &filePath);

        inline const IniSettings &getParsedIniSettings() { return out; }

    private:
        IniSettings out;
        std::string trim(const std::string &in) const;
    };
}