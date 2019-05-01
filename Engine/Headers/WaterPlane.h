#pragma once
#include "Core/Common.h"
#include "Core/FrameBuffer.h"

#define DEFAULT_WATERSPEED 0.0001

class VertexBuffer;
class IndexBuffer;
class FrameBuffer;
class ShaderProgram;

class WaterPlane {
private:
	glm::vec3 center_pos;
	glm::vec3 cameraPos;
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float size;
	std::string shader_path;
	ShaderProgram* shader;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	FrameBuffer* below_fbo;
	FrameBuffer* above_fbo;
	Texture* dudvMap;
	Texture* normalMap;
	float move_factor;
	float clamp_factor;
	float waterSpeed;
	float near_plane;
    float far_plane;
	float distortion_strength;
	float depth_parameter;
	float specular_damper;
	float reflectivity;
	float tiling_factor;
public:
	WaterPlane(std::string shader_path, std::string dudv_map, std::string normal_map, glm::vec3 center_pos, float size, float waterSpeed = DEFAULT_WATERSPEED);
	FrameBuffer* GetBelowFrameBuffer();
	FrameBuffer* GetAboveFrameBuffer();
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	ShaderProgram* GetShader();
	Texture* GetDuDvMap();
	Texture* GetNormalMap();
	void SetNearFarPlanes(float near_plane, float far_plane);
	void SetCameraPosition(glm::vec3 pos);
	void SetLightParameters(glm::vec3 pos, glm::vec3 color);
	void SetDistortionStrength(float distortion);
	void SetWaterSpeed(float speed);
	void SetClampFactor(float clampf);
	void SetDepthParameter(float depth);
	void SetSpecularDamper(float damper);
	void SetReflectivity(float reflectivity);
	void SetTiling(float tiling_factor);
	void MoveWater();

	~WaterPlane();
};