#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "SkyBox.h"

class SkyBox;

class RayTracer {
private:
	ShaderProgram* shader;
	VertexArray* vao;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 inv_proj_view;
	float near_plane, far_plane;
	float movement_factor = 0.0f;
	float movement_speed = 0.05f;
	bool hdr = false;

	glm::vec3 camera_pos;	
	glm::vec3 sun_pos;
	int skybox_slot = -1;
	int irradiance_map_slot = -1;
public:
	RayTracer(std::string shader_path = "resources/shaders/RayTracing/raymarcher_scene1.shader");
	~RayTracer();

	void SetCameraPos(glm::vec3 pos);
	void SetProjectionMatrix(glm::mat4 projection);
	void SetViewMatrix(glm::mat4 view);
	void SetNearFarPlanes(float near_plane, float far_plane);
	void SetSunPosition(glm::vec3 pos);
	void SetHDR(bool hdr);

	float roughness, metallic;
	void SetRoughness(float roughness);
	void SetMetallic(float metallic);
	void SetSkyBoxSlot(int slot);
	void SetIrradianceMapSlot(int slot);

	float light_intensity = 0.0f;
	glm::vec3 emmited_light_color;
	void SetEmmitedLight(glm::vec3 light);

	int reflection_bounces;
	void SetReflectionBounces(int reflections);

	glm::vec3 spheres_albedo;
	void SetSpheresAlbedo(glm::vec3 albedo);

	void Render();
};