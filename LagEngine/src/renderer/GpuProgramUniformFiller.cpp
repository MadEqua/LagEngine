#include "GpuProgramUniformFiller.h"

#include "Root.h"
#include "SceneManager.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Camera.h"

#include "Renderer.h"
#include "GpuProgramUniformDescription.h"
#include "GpuProgram.h"
#include "GpuProgramUniform.h"
#include "Viewport.h"
#include "Texture.h"

#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

using namespace Lag;

GpuProgramUniformFiller::GpuProgramUniformFiller() :
        frameEndGpuProgram(nullptr) {
}

void GpuProgramUniformFiller::onGpuProgramBind(const GpuProgram *gpuProgram, const Viewport *viewport,
                                               const TextureBindings &textureBindings) {
    if (!gpuProgram || !viewport) return;
    updateLightUniforms(*gpuProgram);
    updateViewportUniforms(*gpuProgram, *viewport);

    /*for (auto binding : textureBindings.bindings)
        updateTextureUniforms(*gpuProgram, *binding.second, binding.first);*/
}

void GpuProgramUniformFiller::onViewportBind(const GpuProgram *gpuProgram, const Viewport *viewport) {
    if (!gpuProgram || !viewport) return;
    updateViewportUniforms(*gpuProgram, *viewport);
}

void GpuProgramUniformFiller::onTextureBind(const GpuProgram *gpuProgram, const Texture *texture, uint8 unit) {
    /*if (!gpuProgram || !texture) return;
    updateTextureUniforms(*gpuProgram, *texture, unit);*/
}

void GpuProgramUniformFiller::onRenderableRender(const GpuProgram &gpuProgram, const glm::mat4 &modelMatrix,
                                                 const glm::mat3 &normalMatrix, const Viewport &viewport) {
    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::MODEL_MATRIX, &modelMatrix);
    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::VIEW_MATRIX,
                        &viewport.getCamera().getInverseTransform());
    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::PROJECTION_MATRIX,
                        &viewport.getCamera().getProjectionMatrix());

    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::NORMAL_WORLD_MATRIX, &normalMatrix);

    if (programContainsUniform(gpuProgram, GpuProgramUniformSemantic::NORMAL_VIEW_MATRIX)) {
        glm::mat3 n = glm::mat3(viewport.getCamera().getInverseTransform()) * normalMatrix;
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::NORMAL_VIEW_MATRIX, &n);
    }

    if (programContainsUniform(gpuProgram, GpuProgramUniformSemantic::MODELVIEW_MATRIX)) {
        glm::mat4 mv = viewport.getCamera().getInverseTransform() * modelMatrix;
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::MODELVIEW_MATRIX, &mv);
    }
    if (programContainsUniform(gpuProgram, GpuProgramUniformSemantic::VIEWPROJECTION_MATRIX)) {
        const Camera &cam = viewport.getCamera();
        glm::mat4 vp = cam.getProjectionMatrix() * cam.getInverseTransform();
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::VIEWPROJECTION_MATRIX, &vp);
    }
    if (programContainsUniform(gpuProgram, GpuProgramUniformSemantic::MVP_MATRIX)) {
        const Camera &camera = viewport.getCamera();
        glm::mat4 mvp = camera.getProjectionMatrix() *
                        camera.getInverseTransform() *
                        modelMatrix;
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::MVP_MATRIX, &mvp);
    }
}

void GpuProgramUniformFiller::updateLightUniforms(const GpuProgram &gpuProgram) {
    //PointLights
    auto &pointLights = Root::getInstance().getSceneManager().getActiveScene().getPointLights();
    auto pointLightCount = static_cast<uint32>(pointLights.size());
    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::POINT_LIGHT_COUNT, &pointLightCount);

    if (pointLightCount > 0) {
        glm::vec3 ppos[Renderer::MAX_POINT_LIGHTS];
        glm::vec3 pcols[Renderer::MAX_POINT_LIGHTS];
        glm::vec3 patten[Renderer::MAX_POINT_LIGHTS];
        for (uint32 i = 0; i < Renderer::MAX_POINT_LIGHTS && i < pointLightCount; ++i) {
            ppos[i] = pointLights[i]->getWorldPosition();
            const float *floatColors = pointLights[i]->getColor().getRGBAfloat();
            pcols[i].r = floatColors[0];
            pcols[i].g = floatColors[1];
            pcols[i].b = floatColors[2];
            const float *floatAttenuation = pointLights[i]->getAttenuation();
            patten[i].x = floatAttenuation[0];
            patten[i].y = floatAttenuation[1];
            patten[i].z = floatAttenuation[2];
        }
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::POINT_LIGHT_POSITIONS, ppos, pointLightCount);
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::POINT_LIGHT_COLORS, pcols, pointLightCount);
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::POINT_LIGHT_ATTENUATIONS, patten, pointLightCount);
    }

    //DirectionalLights
    auto &directionalLights = Root::getInstance().getSceneManager().getActiveScene().getDirectionalLights();
    auto dirLightCount = static_cast<uint32>(directionalLights.size());
    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::DIR_LIGHT_COUNT, &dirLightCount);

    if (dirLightCount > 0) {
        glm::vec3 ddir[Renderer::MAX_DIRECTIONAL_LIGHTS];
        glm::vec3 dcols[Renderer::MAX_DIRECTIONAL_LIGHTS];
        for (uint32 i = 0; i < Renderer::MAX_DIRECTIONAL_LIGHTS && i < dirLightCount; ++i) {
            ddir[i] = directionalLights[i]->getDirection();
            const float *floatColors = directionalLights[i]->getColor().getRGBAfloat();
            dcols[i].r = floatColors[0];
            dcols[i].g = floatColors[1];
            dcols[i].b = floatColors[2];
        }
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::DIR_LIGHT_DIRECTIONS, ddir, dirLightCount);
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::DIR_LIGHT_COLORS, dcols, dirLightCount);
    }
}

void GpuProgramUniformFiller::updateViewportUniforms(const GpuProgram &gpuProgram, const Viewport &viewport) {
    //Camera-only related matrices
    const glm::mat4 &view = viewport.getCamera().getInverseTransform();
    const glm::mat4 &proj = viewport.getCamera().getProjectionMatrix();

    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::VIEW_MATRIX, static_cast<const void *>(&view));
    setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::PROJECTION_MATRIX, static_cast<const void *>(&proj));

    if (programContainsUniform(gpuProgram, GpuProgramUniformSemantic::VIEWPROJECTION_MATRIX)) {
        glm::mat4 viewProj = proj * view;
        setUniformIfPresent(gpuProgram, GpuProgramUniformSemantic::VIEWPROJECTION_MATRIX, &viewProj);
    }
}

void GpuProgramUniformFiller::updateTextureUniforms(const GpuProgram &gpuProgram, const Texture &texture, uint8 unit) {
    GpuProgramUniformSemantic uniformSemantic;
    switch (texture.getTextureData().semantic) {
        case TextureSemantic::DIFFUSE:
            uniformSemantic = GpuProgramUniformSemantic::TEXTURE_DIFFUSE;
            break;
        case TextureSemantic::NORMAL:
            uniformSemantic = GpuProgramUniformSemantic::TEXTURE_NORMAL;
            break;
        default:
            uniformSemantic = GpuProgramUniformSemantic::TEXTURE_DIFFUSE;
            break;
    }

    auto uniformList = gpuProgram.getUniformBySemantic(uniformSemantic);
    if (uniformList != nullptr && unit < uniformList->size()) {
        auto u = static_cast<uint32>(unit);
        uniformList->at(unit)->setValue(static_cast<void *>(&u));
    }
}

bool GpuProgramUniformFiller::programContainsUniform(const GpuProgram &gpuProgram,
                                                     GpuProgramUniformSemantic semantic) {
    return gpuProgram.getUniformBySemantic(semantic) != nullptr;
}

void GpuProgramUniformFiller::setUniformIfPresent(const GpuProgram &gpuProgram,
                                                  GpuProgramUniformSemantic semantic, const void *value,
                                                  uint32 arraySize) {
    auto uniformList = gpuProgram.getUniformBySemantic(semantic);
    if (uniformList != nullptr)
        for (GpuProgramUniform *uniform : *uniformList)
            uniform->setValue(value, arraySize);
}