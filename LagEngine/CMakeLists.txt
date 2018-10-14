project(LagEngine)

set(SRC_FOLDER src)
set(DEPENDENCIES_FOLDER dependencies)

set(SOURCES
        ${SRC_FOLDER}/core/Buffer.cpp
        ${SRC_FOLDER}/core/Buffer.h
        ${SRC_FOLDER}/core/Handle.h
        ${SRC_FOLDER}/core/ManagedObject.cpp
        ${SRC_FOLDER}/core/ManagedObject.h
        ${SRC_FOLDER}/core/Manager.h
        ${SRC_FOLDER}/core/Memory.cpp
        ${SRC_FOLDER}/core/Memory.h
        ${SRC_FOLDER}/core/MemoryBuffer.cpp
        ${SRC_FOLDER}/core/MemoryBuffer.h
        ${SRC_FOLDER}/core/NamedContainer.h
        ${SRC_FOLDER}/core/ObserverPattern.h
        ${SRC_FOLDER}/core/SingletonPattern.h
        ${SRC_FOLDER}/core/Timer.cpp
        ${SRC_FOLDER}/core/Timer.h
        ${SRC_FOLDER}/io/ini/IniParser.cpp
        ${SRC_FOLDER}/io/ini/IniParser.h
        ${SRC_FOLDER}/io/ini/IniSettings.cpp
        ${SRC_FOLDER}/io/ini/IniSettings.h
        ${SRC_FOLDER}/io/log/LogManager.cpp
        ${SRC_FOLDER}/io/log/LogManager.h
        ${SRC_FOLDER}/io/ICursorListener.h
        ${SRC_FOLDER}/io/IKeyboardListener.h
        ${SRC_FOLDER}/io/InputManager.h
        ${SRC_FOLDER}/io/Keys.h
        ${SRC_FOLDER}/io/ResourceFilesWatcher.cpp
        ${SRC_FOLDER}/io/ResourceFilesWatcher.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5Error.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5Error.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuBuffer.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuBuffer.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuBufferBuilder.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgram.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgram.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgramBuilder.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgramStage.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgramStage.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgramStageBuilder.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgramUniform.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GpuProgramUniform.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GraphicsAPI.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5GraphicsAPI.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5InputDescription.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5InputDescription.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5InputDescriptionBuilder.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5RenderTargetBuilder.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5RenderToTexture.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5RenderToTexture.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5Texture.cpp
        ${SRC_FOLDER}/platform/GL4_5/GL4_5Texture.h
        ${SRC_FOLDER}/platform/GL4_5/GL4_5TextureBuilder.h
        ${SRC_FOLDER}/platform/GLFW/GLFWInputManager.cpp
        ${SRC_FOLDER}/platform/GLFW/GLFWInputManager.h
        ${SRC_FOLDER}/platform/GLFW/GLFWRenderTargetBuilder.h
        ${SRC_FOLDER}/platform/GLFW/GLFWRenderWindow.cpp
        ${SRC_FOLDER}/platform/GLFW/GLFWRenderWindow.h
        ${SRC_FOLDER}/platform/GLFW_GL4_5Factory.cpp
        ${SRC_FOLDER}/platform/GLFW_GL4_5Factory.h
        ${SRC_FOLDER}/platform/IPlatformFactory.h
        ${SRC_FOLDER}/platform/PlatformType.h
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuBuffer.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuBuffer.h
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuProgram.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuProgram.h
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuProgramStage.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuProgramStage.h
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuProgramUniform.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/GpuProgramUniform.h
        ${SRC_FOLDER}/renderer/graphicsAPI/IGraphicsAPI.h
        ${SRC_FOLDER}/renderer/graphicsAPI/InputDescription.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/InputDescription.h
        ${SRC_FOLDER}/renderer/graphicsAPI/RenderToTexture.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/RenderToTexture.h
        ${SRC_FOLDER}/renderer/graphicsAPI/Texture.cpp
        ${SRC_FOLDER}/renderer/graphicsAPI/Texture.h
        ${SRC_FOLDER}/renderer/Color.cpp
        ${SRC_FOLDER}/renderer/Color.h
        ${SRC_FOLDER}/renderer/GpuProgramUniformDescription.cpp
        ${SRC_FOLDER}/renderer/GpuProgramUniformDescription.h
        ${SRC_FOLDER}/renderer/GpuProgramUniformFiller.cpp
        ${SRC_FOLDER}/renderer/GpuProgramUniformFiller.h
        ${SRC_FOLDER}/renderer/IFrameListener.h
        ${SRC_FOLDER}/renderer/IndexData.cpp
        ${SRC_FOLDER}/renderer/IndexData.h
        ${SRC_FOLDER}/renderer/IRenderable.h
        ${SRC_FOLDER}/renderer/IRenderTargetListener.h
        ${SRC_FOLDER}/renderer/IWindowListener.h
        ${SRC_FOLDER}/renderer/Material.cpp
        ${SRC_FOLDER}/renderer/Material.h
        ${SRC_FOLDER}/renderer/Mesh.cpp
        ${SRC_FOLDER}/renderer/Mesh.h
        ${SRC_FOLDER}/renderer/Renderer.cpp
        ${SRC_FOLDER}/renderer/Renderer.h
        ${SRC_FOLDER}/renderer/RenderOperation.h
        ${SRC_FOLDER}/renderer/RenderPhase.h
        ${SRC_FOLDER}/renderer/RenderQueue.cpp
        ${SRC_FOLDER}/renderer/RenderQueue.h
        ${SRC_FOLDER}/renderer/RenderTarget.cpp
        ${SRC_FOLDER}/renderer/RenderTarget.h
        ${SRC_FOLDER}/renderer/RenderWindow.cpp
        ${SRC_FOLDER}/renderer/RenderWindow.h
        ${SRC_FOLDER}/renderer/SubMesh.cpp
        ${SRC_FOLDER}/renderer/SubMesh.h
        ${SRC_FOLDER}/renderer/VertexData.h
        ${SRC_FOLDER}/renderer/VertexDescription.cpp
        ${SRC_FOLDER}/renderer/VertexDescription.h
        ${SRC_FOLDER}/renderer/Viewport.cpp
        ${SRC_FOLDER}/renderer/Viewport.h
        ${SRC_FOLDER}/renderer/ComparisionFunction.h
        ${SRC_FOLDER}/renderer/RenderMode.h
        ${SRC_FOLDER}/renderer/DepthSettings.h
        ${SRC_FOLDER}/renderer/DepthSettings.cpp
        ${SRC_FOLDER}/renderer/BlendingSettings.h
        ${SRC_FOLDER}/renderer/BlendingSettings.cpp
        ${SRC_FOLDER}/resources/GpuBufferManager.cpp
        ${SRC_FOLDER}/resources/GpuBufferManager.h
        ${SRC_FOLDER}/resources/GpuProgramManager.cpp
        ${SRC_FOLDER}/resources/GpuProgramManager.h
        ${SRC_FOLDER}/resources/GpuProgramStageManager.cpp
        ${SRC_FOLDER}/resources/GpuProgramStageManager.h
        ${SRC_FOLDER}/resources/Image.cpp
        ${SRC_FOLDER}/resources/Image.h
        ${SRC_FOLDER}/resources/ImageManager.cpp
        ${SRC_FOLDER}/resources/ImageManager.h
        ${SRC_FOLDER}/resources/InputDescriptionManager.cpp
        ${SRC_FOLDER}/resources/InputDescriptionManager.h
        ${SRC_FOLDER}/resources/MaterialManager.cpp
        ${SRC_FOLDER}/resources/MaterialManager.h
        ${SRC_FOLDER}/resources/MeshManager.cpp
        ${SRC_FOLDER}/resources/MeshManager.h
        ${SRC_FOLDER}/resources/RenderTargetManager.cpp
        ${SRC_FOLDER}/resources/RenderTargetManager.h
        ${SRC_FOLDER}/resources/TextureManager.cpp
        ${SRC_FOLDER}/resources/TextureManager.h
        ${SRC_FOLDER}/resources/XmlResource.cpp
        ${SRC_FOLDER}/resources/XmlResource.h
        ${SRC_FOLDER}/resources/XmlResourceManager.h
        ${SRC_FOLDER}/resources/MeshRepository.h
        ${SRC_FOLDER}/resources/MeshRepository.cpp
        ${SRC_FOLDER}/scene/Camera.cpp
        ${SRC_FOLDER}/scene/Camera.h
        ${SRC_FOLDER}/scene/DirectionalLight.cpp
        ${SRC_FOLDER}/scene/DirectionalLight.h
        ${SRC_FOLDER}/scene/Entity.cpp
        ${SRC_FOLDER}/scene/Entity.h
        ${SRC_FOLDER}/scene/Frustum.cpp
        ${SRC_FOLDER}/scene/Frustum.h
        ${SRC_FOLDER}/scene/Light.cpp
        ${SRC_FOLDER}/scene/Light.h
        ${SRC_FOLDER}/scene/OrthographicCamera.cpp
        ${SRC_FOLDER}/scene/OrthographicCamera.h
        ${SRC_FOLDER}/scene/PerspectiveCamera.cpp
        ${SRC_FOLDER}/scene/PerspectiveCamera.h
        ${SRC_FOLDER}/scene/PointLight.cpp
        ${SRC_FOLDER}/scene/PointLight.h
        ${SRC_FOLDER}/scene/Scene.cpp
        ${SRC_FOLDER}/scene/Scene.h
        ${SRC_FOLDER}/scene/SceneGraph.cpp
        ${SRC_FOLDER}/scene/SceneGraph.h
        ${SRC_FOLDER}/scene/SceneManager.cpp
        ${SRC_FOLDER}/scene/SceneManager.h
        ${SRC_FOLDER}/scene/SceneNode.cpp
        ${SRC_FOLDER}/scene/SceneNode.h
        ${SRC_FOLDER}/scene/SceneObject.cpp
        ${SRC_FOLDER}/scene/SceneObject.h
        ${SRC_FOLDER}/scene/SubEntity.cpp
        ${SRC_FOLDER}/scene/SubEntity.h
        ${SRC_FOLDER}/scene/Transform.cpp
        ${SRC_FOLDER}/scene/Transform.h
        ${SRC_FOLDER}/utils/FreeCamera.cpp
        ${SRC_FOLDER}/utils/FreeCamera.h
        ${SRC_FOLDER}/utils/Utils.cpp
        ${SRC_FOLDER}/utils/Utils.h
        ${SRC_FOLDER}/collisions/CollisionManager.h
        ${SRC_FOLDER}/collisions/CollisionManager.cpp
        ${SRC_FOLDER}/collisions/AABB.h
        ${SRC_FOLDER}/collisions/AABB.cpp
        ${SRC_FOLDER}/collisions/ICollisionListener.h
        ${SRC_FOLDER}/Constants.h
        ${SRC_FOLDER}/InitializationParameters.cpp
        ${SRC_FOLDER}/InitializationParameters.h
        ${SRC_FOLDER}/Root.cpp
        ${SRC_FOLDER}/Root.h
        ${SRC_FOLDER}/Types.h)

set(INCLUDE_FOLDERS
        ${SRC_FOLDER}/core/
        ${SRC_FOLDER}/io/
        ${SRC_FOLDER}/io/ini
        ${SRC_FOLDER}/io/log/
        ${SRC_FOLDER}/platform/
        ${SRC_FOLDER}/platform/GL4_5/
        ${SRC_FOLDER}/platform/GLFW/
        ${SRC_FOLDER}/renderer/
        ${SRC_FOLDER}/renderer/graphicsAPI/
        ${SRC_FOLDER}/resources/
        ${SRC_FOLDER}/scene/
        ${SRC_FOLDER}/utils/
        ${SRC_FOLDER}/collisions/
        ${SRC_FOLDER}/)

add_library(LagEngine STATIC ${SOURCES})
target_include_directories(LagEngine PUBLIC ${INCLUDE_FOLDERS})

set(DEPENDENCIES_INCLUDE_FOLDERS ${DEPENDENCIES_FOLDER})

###############################################
#Filewatch, TinyXml, stbi
###############################################
target_sources(LagEngine
        PRIVATE ${DEPENDENCIES_INCLUDE_FOLDERS}/tinyxml/tinyxml.cpp
        PRIVATE ${DEPENDENCIES_INCLUDE_FOLDERS}/tinyxml/tinyxmlerror.cpp
        PRIVATE ${DEPENDENCIES_INCLUDE_FOLDERS}/tinyxml/tinyxmlparser.cpp)

target_include_directories(LagEngine PUBLIC ${DEPENDENCIES_INCLUDE_FOLDERS})

###############################################
#GLFW
###############################################
target_include_directories(LagEngine PRIVATE ${DEPENDENCIES_FOLDER}/glfw-3.2.1/include)

find_library(GLFW_LIBRARY_DEBUG
        NAMES glfw3d.lib
        HINTS ${DEPENDENCIES_FOLDER}/glfw-3.2.1/lib)

find_library(GLFW_LIBRARY_RELEASE
        NAMES glfw3.lib
        HINTS ${DEPENDENCIES_FOLDER}/glfw-3.2.1/lib)

target_link_libraries(LagEngine
        PRIVATE debug ${GLFW_LIBRARY_DEBUG}
        PRIVATE optimized ${GLFW_LIBRARY_RELEASE})

###############################################
#GLEW
###############################################
target_include_directories(LagEngine PRIVATE ${DEPENDENCIES_FOLDER}/glew-2.1.0/include)

add_definitions(-DGLEW_STATIC)

find_library(GLEW_LIBRARY_DEBUG
        NAMES glew32sd.lib
        HINTS ${DEPENDENCIES_FOLDER}/glew-2.1.0/lib)

find_library(GLEW_LIBRARY_RELEASE
        NAMES glew32s.lib
        HINTS ${DEPENDENCIES_FOLDER}/glew-2.1.0/lib)

target_link_libraries(LagEngine
        PRIVATE debug ${GLEW_LIBRARY_DEBUG}
        PRIVATE optimized ${GLEW_LIBRARY_RELEASE})

find_package(OpenGL REQUIRED)
target_link_libraries(LagEngine PRIVATE ${OPENGL_LIBRARIES})

###############################################
#Assimp
###############################################
target_include_directories(LagEngine PRIVATE ${DEPENDENCIES_FOLDER}/assimp-4.1.0/include)

find_library(ASSIMP_LIBRARY_DEBUG
        NAMES assimp-vc140-mtd.lib
        HINTS ${DEPENDENCIES_FOLDER}/assimp-4.1.0/lib)

find_library(ASSIMP_LIBRARY_RELEASE
        NAMES assimp-vc140-mt.lib
        HINTS ${DEPENDENCIES_FOLDER}/assimp-4.1.0/lib)

find_library(ASSIMP_LIBRARY_DYN_DEBUG
        NAMES assimp-vc140-mtd.dll
        HINTS ${DEPENDENCIES_FOLDER}/assimp-4.1.0/lib)

find_library(ASSIMP_LIBRARY_DYN_RELEASE
        NAMES assimp-vc140-mt.dll
        HINTS ${DEPENDENCIES_FOLDER}/assimp-4.1.0/lib)

target_link_libraries(LagEngine
        PRIVATE debug ${ASSIMP_LIBRARY_DEBUG}
        PRIVATE optimized ${ASSIMP_LIBRARY_RELEASE})

###############################################
#GLM
###############################################
target_include_directories(LagEngine PUBLIC ${DEPENDENCIES_FOLDER}/glm-0.9.9.0/include)


###############################################
#Install
###############################################

#Not working for some reason
#install(FILES ${ASSIMP_LIBRARY_DYN_DEBUG} DESTINATION ${INSTALL_FOLDER} CONFIGURATIONS debug)
#install(FILES ${ASSIMP_LIBRARY_DYN_RELEASE} DESTINATION ${INSTALL_FOLDER} CONFIGURATIONS release)

#On Release builds we copy the resources
install(DIRECTORY resources/ DESTINATION ${INSTALL_FOLDER}/LagResources CONFIGURATIONS release)

#On Debug builds we create a symlink for working hot reloading (not working on Windows)
#install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink resources ${INSTALL_FOLDER}/LagResources)" CONFIGURATIONS debug)
