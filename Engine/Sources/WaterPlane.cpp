#include "WaterPlane.h"

WaterPlane::WaterPlane(std::string shader_path, std::string dudv_map, std::string normal_map, glm::vec3 center_pos, float size, float waterSpeed) : shader_path(shader_path), waterSpeed(waterSpeed) {
	float vertices[]{
		-1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, 0.0f,  1.0f, 0.0f, 1.0f,
		 1.0f, 0.0f, -1.0f, 1.0f, 1.0f
	};
	unsigned int indexes[]{
		0, 1, 2,
		2, 3, 0
	};
	for (int i = 0; i < 4; ++i) {
		vertices[5 * i] = vertices[5 * i] * size / 2.0f + center_pos.x;
		vertices[5 * i + 1] = center_pos.y;
		vertices[5 * i + 2] = vertices[5 * i + 2] * size / 2.0f + center_pos.z;
	}
	vbo = new VertexBuffer(vertices, sizeof(vertices));
	ibo = new IndexBuffer(indexes, 6);
	below_fbo = new FrameBuffer();
	above_fbo = new FrameBuffer();
	shader = new ShaderProgram(shader_path);
	dudvMap = new Texture(dudv_map, "dudv_map");
	normalMap = new Texture(normal_map, "normal_map");
	
}

FrameBuffer* WaterPlane::GetBelowFrameBuffer() {
	return below_fbo;
}

FrameBuffer* WaterPlane::GetAboveFrameBuffer() {
	return above_fbo;
}

VertexBuffer* WaterPlane::GetVertexBuffer() {
	return vbo;
}

IndexBuffer* WaterPlane::GetIndexBuffer() {
	return ibo;
}

ShaderProgram* WaterPlane::GetShader() {
	return shader;
}

Texture* WaterPlane::GetDuDvMap() {
	return this->dudvMap;
}

Texture* WaterPlane::GetNormalMap() {
	return normalMap;
}

void WaterPlane::MoveWater() {
	this->move_factor += waterSpeed;
	shader->FillUniform1f("u_moveFactor", move_factor);
}

void WaterPlane::SetNearFarPlanes(float near_plane, float far_plane) {
	if (this->near_plane != near_plane) {
		this->near_plane = near_plane;
		shader->FillUniform1f("u_near", near_plane);
	}
	if (this->far_plane != far_plane) {
		this->far_plane = far_plane;
		shader->FillUniform1f("u_far", far_plane);
	}
}

void WaterPlane::SetCameraPosition(glm::vec3 pos) {
	if (this->cameraPos != pos) {
		this->cameraPos = pos;
		shader->FillUniformVec3("u_cameraPos", pos);
	}
}

void WaterPlane::SetLightParameters(glm::vec3 pos, glm::vec3 color) {
	if (this->lightPos != pos) {
		this->lightPos = pos;
		shader->FillUniformVec3("u_lightPos", pos);
	}
	if (this->lightColor != color) {
		this->lightColor = color;
		shader->FillUniformVec3("u_lightColor", color);
	}
}

void WaterPlane::SetDistortionStrength(float distortion) {
	if (distortion != this->distortion_strength) {
		this->distortion_strength = distortion;
		shader->FillUniform1f("u_distortionStrength", distortion);
	}
}

void WaterPlane::SetWaterSpeed(float speed) {
	this->waterSpeed = speed;
}

void WaterPlane::SetClampFactor(float clampf) {
	if (clampf != this->clamp_factor) {
		this->clamp_factor = clampf;
		shader->FillUniform1f("u_clampFactor", clamp_factor);
	}
}

void WaterPlane::SetDepthParameter(float depth) {
	if (depth != this->depth_parameter) {
		this->depth_parameter = depth;
		shader->FillUniform1f("u_depthParameter", depth);
	}
}

void WaterPlane::SetSpecularDamper(float damper) {
	if (damper != this->specular_damper) {
		this->specular_damper = damper;
		shader->FillUniform1f("u_specularDamper", damper);
	}
}

void WaterPlane::SetReflectivity(float reflectivity) {
	if (reflectivity != this->reflectivity) {
		this->reflectivity = reflectivity;
		shader->FillUniform1f("u_reflectivity", reflectivity);
	}
}

void WaterPlane::SetTiling(float tiling_factor) {
	if (tiling_factor != this->tiling_factor) {
		this->tiling_factor = tiling_factor;
		shader->FillUniform1f("u_tiling", tiling_factor);
	}
}

WaterPlane::~WaterPlane() {
	delete vbo;
	delete ibo;
	delete below_fbo;
	delete above_fbo;
}