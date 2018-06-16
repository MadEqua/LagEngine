#include "GL4_5GraphicsAPI.h"

#include <sstream>
#include <DepthSettings.h>
#include "BlendingSettings.h"

#include "LogManager.h"
#include "Renderer.h"
#include "IndexData.h"

#include "GL4_5InputDescription.h"
#include "GL4_5GpuProgram.h"
#include "GL4_5GpuBuffer.h"
#include "GL4_5Texture.h"
#include "GL4_5RenderToTexture.h"

#include "GL4_5Error.h"

using namespace Lag;

bool GL4_5GraphicsAPI::initialize() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::string errorString = reinterpret_cast<const char *>(glewGetErrorString(err));
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "GL4_5GraphicsAPI", "Failed to initialize GLEW: " + errorString);
        return false;
    }

    GLint majorV, minorV;
    glGetIntegerv(GL_MAJOR_VERSION, &majorV);
    glGetIntegerv(GL_MINOR_VERSION, &minorV);
    const GLubyte *versionString = glGetString(GL_VERSION);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::stringstream sstream;
    sstream << "OpenGL version: " << majorV << "." << minorV << ". Version string: " << versionString << std::endl;
    if (vendor != nullptr) sstream << "Vendor: " << vendor << std::endl;
    if (renderer != nullptr) sstream << "Renderer: " << renderer << std::endl;
    if (glslVersion != nullptr) sstream << "GLSL Version: " << glslVersion << std::endl;

    LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL, "GL4_5GraphicsAPI", sstream.str());

    GLint res;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &res);

    if (res == GL_LINEAR) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      "GL4_5GraphicsAPI", "Linear RGB Default Framebuffer.");

        //TODO This should be disable. But on some nVidia drivers the return value is always GL_LINEAR.
        glEnable(GL_FRAMEBUFFER_SRGB);
    }
    else if (res == GL_SRGB) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      "GL4_5GraphicsAPI", "sRGB Default Framebuffer.");

        //enable auto Linear RGB to sRGB conversion when writing to sRGB framebuffers
        glEnable(GL_FRAMEBUFFER_SRGB);
    }

    glGetIntegerv(GL_SAMPLES, &res);
    if (res > 0) {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      "GL4_5GraphicsAPI", "Multisampled Default Framebuffer. Samples: " + res);
        glEnable(GL_MULTISAMPLE);
    }
    else {
        LogManager::getInstance().log(LogType::INFO, LogVerbosity::NORMAL,
                                      "GL4_5GraphicsAPI", "Non-Multisampled Default Framebuffer.");
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

#ifdef ENABLE_DEBUG_MACRO
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE); //Enable all messages
#endif

    return true;
}

void GL4_5GraphicsAPI::renderVertices(RenderMode mode, uint32 first, uint32 count) {
    glDrawArrays(convertRenderModeToGLenum(mode), first, count);
}

void GL4_5GraphicsAPI::renderIndexed(RenderMode mode, uint32 first, IndexType indexType, uint32 count, uint32 baseVertex) {
    uint32 indexByteSize;
    GLenum type;
    switch (indexType) {
        case IndexType::UINT8:
            type = GL_UNSIGNED_BYTE;
            indexByteSize = 1;
            break;
        case IndexType::UINT16:
            type = GL_UNSIGNED_SHORT;
            indexByteSize = 2;
            break;
        case IndexType::UINT32:
            type = GL_UNSIGNED_INT;
            indexByteSize = 4;
            break;
    }

    uint32 offset = first * indexByteSize;
    glDrawElementsBaseVertex(convertRenderModeToGLenum(mode), count, type, reinterpret_cast<void *>(offset),
                             baseVertex);
}

void GL4_5GraphicsAPI::clearColorBuffer(const Color &color) {
    glClearBufferfv(GL_COLOR, 0, color.getRGBAfloat());
}

void GL4_5GraphicsAPI::clearDepthBuffer(float value) {
    glClearBufferfv(GL_DEPTH, 0, &value);
}

void GL4_5GraphicsAPI::clearStencilBuffer(int32 value) {
    glClearBufferiv(GL_STENCIL, 0, &value);
}

void GL4_5GraphicsAPI::clearDepthAndStencilBuffer(float depth, int32 stencil) {
    glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
}

void GL4_5GraphicsAPI::bindVertexBuffer(const GpuBuffer &vertexBuffer) {
    GLuint handle = dynamic_cast<const GL4_5GpuBuffer &>(vertexBuffer).getHandle();
    glBindBuffer(GL_ARRAY_BUFFER, handle);
}

void GL4_5GraphicsAPI::bindIndexBuffer(const GpuBuffer &indexBuffer) {
    GLuint handle = dynamic_cast<const GL4_5GpuBuffer &>(indexBuffer).getHandle();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}

void GL4_5GraphicsAPI::bindGpuProgram(const GpuProgram &gpuProgram) {
    GLuint handle = dynamic_cast<const GL4_5GpuProgram &>(gpuProgram).getHandle();
    glUseProgram(handle);
}

void GL4_5GraphicsAPI::bindInputDescription(const InputDescription &inputDescription) {
    GLuint handle = dynamic_cast<const GL4_5InputDescription &>(inputDescription).getHandle();
    glBindVertexArray(handle);
}

void GL4_5GraphicsAPI::bindRenderTarget(const RenderTarget &renderTarget) {
    GLuint handle = 0;
    if (!renderTarget.isMainWindow())
        handle = dynamic_cast<const GL4_5RenderToTexture &>(renderTarget).getHandle();
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void GL4_5GraphicsAPI::bindViewport(uint32 x, uint32 y, uint32 width, uint32 height) {
    glViewport(x, y, width, height);
}

void GL4_5GraphicsAPI::bindTexture(const Texture &texture, uint8 unit) {
    const auto &GL4Tex = dynamic_cast<const GL4_5Texture &>(texture);
    glBindTextureUnit(unit, GL4Tex.getHandle());
}

void GL4_5GraphicsAPI::setPointSizeFromGpuProgramEnabled(bool enabled) {
    if (enabled)
        glEnable(GL_PROGRAM_POINT_SIZE);
    else
        glDisable(GL_PROGRAM_POINT_SIZE);
}

void GL4_5GraphicsAPI::setDepthSettings(const DepthSettings &depthSettings) {
    if(depthSettings.enableDepthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(depthSettings.enableDepthWriting ? GL_TRUE : GL_FALSE);
        glDepthFunc(convertComparisionFunctionToGLenum(depthSettings.comparisionFunction));
        if(depthSettings.enableDepthClamping)
            glEnable(GL_DEPTH_CLAMP);
        else
            glDisable(GL_DEPTH_CLAMP);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void GL4_5GraphicsAPI::setBlendingSettings(const Lag::BlendingSettings &blendingSettings) {
    if(blendingSettings.enableBlending) {
        glEnable(GL_BLEND);

        const BlendingSetting &rgbSetting = blendingSettings.settingRGB;
        const BlendingSetting &alphaSetting = blendingSettings.settingAlpha;

        glBlendFuncSeparate(convertBlendingFunctionToGLenum(rgbSetting.sourceBlendingFunction),
                            convertBlendingFunctionToGLenum(rgbSetting.destinationBlendingFunction),
                            convertBlendingFunctionToGLenum(alphaSetting.sourceBlendingFunction),
                            convertBlendingFunctionToGLenum(alphaSetting.destinationBlendingFunction));
        glBlendEquationSeparate(convertBlendingEquationToGLenum(rgbSetting.blendingEquation),
                                convertBlendingEquationToGLenum(alphaSetting.blendingEquation));

        if(blendingSettings.needsConstantColor()) {
            const Color &cc = blendingSettings.constantColor;
            glBlendColor(cc.r(), cc.g(), cc.b(), cc.alpha());
        }
    }
    else {
        glDisable(GL_BLEND);
    }
}

GLenum GL4_5GraphicsAPI::convertRenderModeToGLenum(RenderMode renderMode) {
    switch (renderMode) {
        case RenderMode::TRIANGLES:
            return GL_TRIANGLES;
        case RenderMode::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case RenderMode::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case RenderMode::LINES:
            return GL_LINES;
        case RenderMode::LINE_STRIP:
            return GL_LINE_STRIP;
        case RenderMode::LINE_LOOP:
            return GL_LINE_LOOP;
        case RenderMode::POINTS:
            return GL_POINTS;
        case RenderMode::PATCHES:
            return GL_PATCHES;
        default:
            return 0;
    }
}

GLenum GL4_5GraphicsAPI::convertComparisionFunctionToGLenum(ComparisionFunction comparisionFunction) {
    switch(comparisionFunction) {
        case ComparisionFunction::NEVER:
            return GL_NEVER;
        case ComparisionFunction::ALWAYS:
            return GL_ALWAYS;
        case ComparisionFunction::LESS:
            return GL_LESS;
        case ComparisionFunction::LESS_OR_EQUAL:
            return GL_LEQUAL;
        case ComparisionFunction::GREATER:
            return GL_GREATER;
        case ComparisionFunction::GREATER_OR_EQUAL:
            return GL_GEQUAL;
        case ComparisionFunction::EQUAL:
            return GL_EQUAL;
        case ComparisionFunction::NOT_EQUAL:
            return GL_NOTEQUAL;
        default:
            return 0;
    }
}

GLenum GL4_5GraphicsAPI::convertBlendingFunctionToGLenum(BlendingFunction blendingFunction) {
    switch (blendingFunction) {
        case BlendingFunction::ZERO:
            return GL_ZERO;
        case BlendingFunction::ONE:
            return GL_ONE;
        case BlendingFunction::SOURCE_COLOR:
            return GL_SRC_COLOR;
        case BlendingFunction::ONE_MINUS_SOURCE_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendingFunction::DESTINATION_COLOR:
            return GL_DST_COLOR;
        case BlendingFunction::ONE_MINUS_DESTINATION_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendingFunction::SOURCE_ALPHA:
            return GL_SRC_ALPHA;
        case BlendingFunction::ONE_MINUS_SOURCE_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendingFunction::DESTINATION_ALPHA:
            return GL_DST_ALPHA;
        case BlendingFunction::ONE_MINUS_DESTINATION_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case BlendingFunction::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case BlendingFunction::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendingFunction::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case BlendingFunction::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case BlendingFunction::ALPHA_SATURATE:
            return GL_SRC_ALPHA_SATURATE;
        case BlendingFunction::SOURCE_1_COLOR:
            return GL_SRC1_COLOR;
        case BlendingFunction::ONE_MINUS_SOURCE_1_COLOR:
            return GL_ONE_MINUS_SRC1_COLOR;
        case BlendingFunction::SOURCE_1_ALPHA:
            return GL_SRC1_ALPHA;
        case BlendingFunction::ONE_MINUS_SOURCE_1_ALPHA:
            return GL_ONE_MINUS_SRC1_ALPHA;
        default:
            return 0;
    }
}

GLenum GL4_5GraphicsAPI::convertBlendingEquationToGLenum(BlendingEquation blendingEquation) {
    switch (blendingEquation) {
        case BlendingEquation::ADD:
            return GL_FUNC_ADD;
        case BlendingEquation::SOURCE_MINUS_DESTINATION:
            return GL_FUNC_SUBTRACT;
        case BlendingEquation::DESTINATION_MINUS_SOURCE:
            return GL_FUNC_REVERSE_SUBTRACT;
        case BlendingEquation::MIN:
            return GL_MIN;
        case BlendingEquation::MAX:
            return GL_MAX;
        default:
            return 0;
    }
}
