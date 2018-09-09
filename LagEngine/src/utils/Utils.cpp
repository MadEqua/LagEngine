#include "Utils.h"

#include "tinyxml/tinyxml.h"
#include "VertexDescription.h"
#include "glm/glm.hpp"
#include "LogManager.h"

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

bool Utils::parseBool(const std::string &str) {
    return str == "true" || str == "TRUE" || str == "1";
}

int Utils::parseInt(const std::string &str) {
    return std::stoi(str);
}

float Utils::parseFloat(const std::string &str) {
    return std::stof(str);
}

//Signed will map to [-1.0, 1.0]. Unsigned to [0.0, 1.0]
float Utils::convertNormalizedIntegerToFloat(const byte *integerPointer, VertexAttributeType type) {
    switch(type) {
    case VertexAttributeType::INT8:
        return glm::max(static_cast<float>(*reinterpret_cast<const int8*>(integerPointer)) / static_cast<float>(MAX_INT8), -1.0f);
    case VertexAttributeType::UINT8:
        return static_cast<float>(*integerPointer) / static_cast<float>(MAX_UINT8);
    case VertexAttributeType::INT16:
        return glm::max(static_cast<float>(*reinterpret_cast<const int16*>(integerPointer)) / static_cast<float>(MAX_INT16), -1.0f);
    case VertexAttributeType::UINT16:
        return static_cast<float>(*reinterpret_cast<const uint16*>(integerPointer)) / static_cast<float>(MAX_UINT16);
    case VertexAttributeType::INT32:
        return glm::max(static_cast<float>(*reinterpret_cast<const int32*>(integerPointer)) / static_cast<float>(MAX_INT32), -1.0f);
    case VertexAttributeType::UINT32:
        return static_cast<float>(*reinterpret_cast<const uint32*>(integerPointer)) / static_cast<float>(MAX_UINT32);
    //default:
        //TODO Log warning
    }
    return 0.0f;
}

float Utils::convertValueToFloat(const byte *valuePointer, VertexAttributeType type) {
    switch(type) {
    case VertexAttributeType::INT8:
        return static_cast<float>(*reinterpret_cast<const int8*>(valuePointer));
    case VertexAttributeType::UINT8:
        return static_cast<float>(*valuePointer);
    case VertexAttributeType::INT16:
        return static_cast<float>(*reinterpret_cast<const int16*>(valuePointer));
    case VertexAttributeType::UINT16:
        return static_cast<float>(*reinterpret_cast<const uint16*>(valuePointer));
    case VertexAttributeType::INT32:
        return static_cast<float>(*reinterpret_cast<const int32*>(valuePointer));
    case VertexAttributeType::UINT32:
        return static_cast<float>(*reinterpret_cast<const uint32*>(valuePointer));
    case VertexAttributeType::FLOAT:
        return *reinterpret_cast<const float*>(valuePointer);
        //default:
            //TODO Log warning
    }
    return 0.0f;
}