#pragma once

#include "../graphics/shader/shaderProgram.h"
#include "../graphics/gui/color.h"

class HDRTexture;
class Texture;
class CubeMap;
struct Light;
struct Material;

namespace Application {

struct ExtendedPart;

struct SkyboxShader : public ShaderProgram {
	SkyboxShader() : ShaderProgram() {}
	SkyboxShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "viewMatrix", "projectionMatrix", "skyboxTexture", "lightDirection") {}

	void updateProjection(const Mat4f& viewMatrix, const Mat4f& projectionMatrix);
	void updateCubeMap(CubeMap* skybox);
	void updateLightDirection(const Vec3f& lightDirection);
};

struct MaskShader : public ShaderProgram {
	MaskShader() : ShaderProgram() {}
	MaskShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "viewMatrix", "projectionMatrix", "modelMatrix", "color") {}

	void updateProjection(const Mat4f& viewMatrix, const Mat4f& projectionMatrix);
	void updateModel(const Mat4f& modelMatrix);
	void updateColor(const Color& color);
};

struct BasicShader : public ShaderProgram {
	BasicShader() : ShaderProgram() {}
	BasicShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "modelMatrix", "viewMatrix", "projectionMatrix", "viewPosition", "includeNormals", "includeUvs", "material.ambient", "material.diffuse", "material.specular", "material.reflectance", "material.textured", "material.normalmapped", "textureSampler", "normalSampler", "sunDirection", "sunColor", "gamma", "hdr", "exposure", "uniforms") {}

	void createLightArray(int size);
	void updateSunDirection(const Vec3f& sunDirection);
	void updateSunColor(const Color3& sunColor);
	void updateGamma(float gamma);
	void updateHDR(bool hdr);
	void updateExposure(float exposure);
	void updateProjection(const Mat4f& viewMatrix, const Mat4f& projectionMatrix, const Position& viewPosition);
	void updateLight(Light lights[], int size);
	void updatePart(const ExtendedPart& part);
	void updateMaterial(const Material& material);
	void updateModel(const Mat4f& modelMatrix);
	void updateUniforms(int id);
};

struct DepthShader : public ShaderProgram {
	DepthShader() : ShaderProgram() {}
	DepthShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "modelMatrix", "lightMatrix") {}

	void updateLight(const Mat4f& lightMatrix);
	void updateModel(const Mat4f& modelMatrix);
};

struct PostProcessShader : public ShaderProgram {
	PostProcessShader() : ShaderProgram() {}
	PostProcessShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "textureSampler") {}

	void updateTexture(Texture* texture);
	void updateTexture(HDRTexture* texture);
};

struct OriginShader : public ShaderProgram {
	OriginShader() : ShaderProgram() {}
	OriginShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "viewMatrix", "rotatedViewMatrix", "projectionMatrix", "orthoMatrix", "viewPosition") {}

	void updateProjection(const Mat4f& viewMatrix, const Mat4f& rotatedViewMatrix, const Mat4f& projectionMatrix, const Mat4f& orthoMatrix, const Position& viewPosition);
};

struct FontShader : public ShaderProgram {
	FontShader() : ShaderProgram() {}
	FontShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "projectionMatrix", "color", "text") {}

	void updateColor(const Color& color);
	void updateProjection(const Mat4f& projectionMatrix);
	void updateTexture(Texture* texture);
};

struct VectorShader : public ShaderProgram {
	VectorShader() : ShaderProgram() {}
	VectorShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "viewMatrix", "projectionMatrix", "viewPosition") {}

	void updateProjection(const Mat4f& viewMatrix, const Mat4f& projectionMatrix, const Position& viewPosition);
};

struct PointShader : public ShaderProgram {
	PointShader() : ShaderProgram() {}
	PointShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "viewMatrix", "projectionMatrix", "viewPosition") {}

	void updateProjection(const Mat4f& viewMatrix, const Mat4f& projectionMatrix, const Position& viewPosition);
};

struct TestShader : public ShaderProgram {
	TestShader() : ShaderProgram() {}
	TestShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "displacementMap", "viewMatrix", "modelMatrix", "projectionMatrix", "viewPosition") {}

	void updateProjection(const Mat4f& projectionMatrix);
	void updateView(const Mat4f& viewMatrix);
	void updateModel(const Mat4f& modelMatrix);
	void updateViewPosition(const Position& viewPosition);
	void updateDisplacement(Texture* displacementMap);
};

struct LineShader : public ShaderProgram {
	LineShader() : ShaderProgram() {}
	LineShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "projectionMatrix", "viewMatrix") {}

	void updateProjection(const Mat4f& projectionMatrix, const Mat4f& viewMatrix);
};

struct InstanceBasicShader : public ShaderProgram {
	InstanceBasicShader() : ShaderProgram() {}
	InstanceBasicShader(ShaderSource shaderSource) : ShaderProgram(shaderSource, "viewMatrix", "projectionMatrix", "viewPosition", "sunDirection", "sunColor", "gamma", "hdr", "exposure") {}

	void createLightArray(int size);
	void updateSunDirection(const Vec3f& sunDirection);
	void updateSunColor(const Vec3f& sunColor);
	void updateGamma(float gamma);
	void updateHDR(bool hdr);
	void updateExposure(float exposure);
	void updateProjection(const Mat4f& viewMatrix, const Mat4f& projectionMatrix, const Position& viewPosition);
	void updateLight(Light lights[], int size);
};

namespace ApplicationShaders {
extern BasicShader basicShader;
extern DepthShader depthShader;
extern VectorShader vectorShader;
extern OriginShader originShader;
extern FontShader fontShader;
extern PostProcessShader postProcessShader;
extern SkyboxShader skyboxShader;
extern PointShader pointShader;
extern TestShader testShader;
extern LineShader lineShader;
extern InstanceBasicShader instanceBasicShader;
extern MaskShader maskShader;

void onInit();
void onClose();
}

};