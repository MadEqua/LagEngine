#include "Utils.h"

#include "tinyxml/tinyxml.h"

using namespace Lag;

char Utils::getPathSeparator() {
#if defined _WIN32 || defined __CYGWIN__
    return '\\';
#else
    return '/';
#endif
}

std::string Utils::getFileNameFromPath(const std::string &path, char separator) {
    size_t slash = path.find_last_of(separator);
    if (slash == std::string::npos)
        return std::string();
    else
        return path.substr(slash + 1, std::string::npos);
}

std::string Utils::getDirNameFromPath(const std::string &path, char separator) {
    size_t slash = path.find_last_of(separator);
    if (slash == std::string::npos)
        return path;
    else
        return path.substr(0, slash);
}

std::string Utils::getExtensionFromFile(const std::string &file) {
    size_t dot = file.find_last_of('.');
    if (dot == std::string::npos)
        return std::string();
    else
        return file.substr(dot, std::string::npos);
}

void Utils::copyTinyXmlNode(TiXmlNode *dest, const TiXmlNode *src) {
    if (dest == nullptr || src == nullptr)
        return;

    TiXmlNode *copy = src->Clone();
    if (copy == nullptr)
        return;

    dest->InsertEndChild(*copy);

    for (const TiXmlNode *node = src->FirstChild(); node != nullptr; node = node->NextSibling())
        copyTinyXmlNode(copy, node);
}
