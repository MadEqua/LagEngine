#pragma once

namespace Lag
{
	constexpr uint8 MAX_POINT_LIGHTS = 8;
	constexpr uint8 MAX_DIRECTIONAL_LIGHTS = 4;

	constexpr const char* const SHADER_XML_TAG = "shader";
	constexpr const char* const MATERIAL_XML_TAG = "material";
	constexpr const char* const MESH_XML_TAG = "mesh";
	constexpr const char* const IMAGE_XML_TAG = "image";
	constexpr const char* const TEXTURE_XML_TAG = "texture";
	constexpr const char* const BLENDING_XML_TAG = "blending";
	constexpr const char* const DEPTH_XML_TAG = "depth";

	constexpr const char* const NAME_XML_ATTR = "name";
	constexpr const char* const FILE_XML_ATTR = "file";

	constexpr const char* const ROOT_SCENE_NODE = "root";

	constexpr const char* const LOG_FILE_NAME = "LagEngineLog.txt";

	constexpr const char* const GLFW_GL4_5 = "GLFW_GL4_5";
}