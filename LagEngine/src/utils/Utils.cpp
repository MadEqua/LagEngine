#include "Utils.h"

#include "tinyxml/tinyxml.h"
#include "VertexDescription.h"
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

/*
  Converts a given set of RGB values `r', `g', `b' into HSV
  coordinates. The input RGB values are in the range [0, 1], and the
  output HSV values are in the ranges h = [0, 360], and s, v = [0,
  1], respectively.
*/
glm::vec3 Utils::RGBtoHSV(const glm::vec3 &rgb) {
    glm::vec3 result;
    
    float fCMax = glm::max(glm::max(rgb.r, rgb.g), rgb.b);
    float fCMin = glm::min(glm::min(rgb.r, rgb.g), rgb.b);
    float fDelta = fCMax - fCMin;

    if(fDelta > 0) {
        if(fCMax == rgb.r) {
            result[0] = 60 * (fmod(((rgb.g - rgb.b) / fDelta), 6));
        }
        else if(fCMax == rgb.g) {
            result[0] = 60 * (((rgb.b - rgb.r) / fDelta) + 2);
        }
        else if(fCMax == rgb.b) {
            result[0] = 60 * (((rgb.r - rgb.g) / fDelta) + 4);
        }

        if(fCMax > 0) {
            result[1] = fDelta / fCMax;
        }
        else {
            result[1] = 0;
        }

        result[2] = fCMax;
    }
    else {
        result[0] = 0;
        result[1] = 0;
        result[2] = fCMax;
    }

    if(result[0] < 0) {
        result[0] = 360 + result[0];
    }
    return result;
}


/*! 
  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.
*/
glm::vec3 Utils::HSVtoRGB(const glm::vec3 &hsv) {
    glm::vec3 result;
    
    float fC = hsv[2] * hsv[1]; // Chroma
    float fHPrime = fmod(hsv[0] / 60.0f, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = hsv[2] - fC;

    if(0 <= fHPrime && fHPrime < 1) {
        result[0] = fC;
        result[1] = fX;
        result[2] = 0;
    }
    else if(1 <= fHPrime && fHPrime < 2) {
        result[0] = fX;
        result[1] = fC;
        result[2] = 0;
    }
    else if(2 <= fHPrime && fHPrime < 3) {
        result[0] = 0;
        result[1] = fC;
        result[2] = fX;
    }
    else if(3 <= fHPrime && fHPrime < 4) {
        result[0] = 0;
        result[1] = fX;
        result[2] = fC;
    }
    else if(4 <= fHPrime && fHPrime < 5) {
        result[0] = fX;
        result[1] = 0;
        result[2] = fC;
    }
    else if(5 <= fHPrime && fHPrime < 6) {
        result[0] = fC;
        result[1] = 0;
        result[2] = fX;
    }
    else {
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
    }

    result[0] += fM;
    result[1] += fM;
    result[2] += fM;
    return result;
}