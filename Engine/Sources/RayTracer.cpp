#include "RayTracer.h"

#define epsilon 0.0001f

RayTracer::RayTracer(std::string shader_path) {
	shader = new ShaderProgram(shader_path);
	vao = new VertexArray();
	float vertices[]{
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f
	};
	VertexBuffer vbo(vertices, 8 * sizeof(float));
	BufferLayout layout;
	layout.AddElement<float>(2, 0);
	vao->AddBuffer(vbo, layout);
}

RayTracer::~RayTracer() {
	delete shader;
	delete vao;
}

void RayTracer::SetProjectionMatrix(glm::mat4 projection) {
	if (this->projection_matrix != projection) {
		this->projection_matrix = projection;
		shader->FillUniformMat4f("u_proj_matrix", projection);
	}
}

void RayTracer::SetViewMatrix(glm::mat4 view) {
	if (this->view_matrix != view) {
		this->view_matrix = view;
		shader->FillUniformMat4f("u_view_matrix", view);
	}
}

void RayTracer::SetHDR(bool hdr) {
	if (this->hdr != hdr) {
		this->hdr = hdr;
		this->shader->FillUniform1i("u_isHDR", hdr);
	}
}

void RayTracer::SetCameraPos(glm::vec3 pos) {
	if (this->camera_pos != pos) {
		this->camera_pos = pos;
		this->shader->FillUniformVec3("u_cameraPos", pos);
	}
}


void RayTracer::SetNearFarPlanes(float near_plane, float far_plane) {
	if (this->near_plane != near_plane) {
		this->near_plane = near_plane;
		this->shader->FillUniform1f("u_near", near_plane);
	}
	if (this->far_plane != far_plane) {
		this->far_plane = far_plane;
		this->shader->FillUniform1f("u_far", far_plane);
	}
}

void RayTracer::SetSunPosition(glm::vec3 pos) {
	if (this->sun_pos != pos) {
		this->sun_pos = pos;
		this->shader->FillUniformVec3("u_sunPos", pos);
	}
}

void RayTracer::SetRoughness(float roughness) {
	if (roughness != this->roughness) {
		this->roughness = roughness;
		shader->FillUniform1f("u_roughness", roughness);
	}
}

void RayTracer::SetMetallic(float metallic) {
	if (metallic != this->metallic) {
		this->metallic = metallic;
		shader->FillUniform1f("u_metallic", metallic);
	}
}

void RayTracer::SetSkyBoxSlot(int slot) {
	if (slot != skybox_slot) {
		skybox_slot = slot;
		shader->FillUniform1i("u_skybox", slot);
	}
}

void RayTracer::SetIrradianceMapSlot(int slot) {
	if (slot != irradiance_map_slot) {
		irradiance_map_slot = slot;
		shader->FillUniform1i("u_irradianceMap", slot);
	}
}

void RayTracer::SetReflectionBounces(int reflections) {
	if (this->reflection_bounces != reflections) {
		this->reflection_bounces = reflections;
		this->shader->FillUniform1i("u_reflection_bounces", reflections);
	}
}

void RayTracer::SetSpheresAlbedo(glm::vec3 albedo) {
	if (this->spheres_albedo != albedo) {
		this->spheres_albedo = albedo;
		this->shader->FillUniformVec3("u_sphere_albedo", albedo);
	}
}

void RayTracer::SetEmmitedLight(glm::vec3 light) {
	if (this->emmited_light_color != light) {
		this->emmited_light_color = light;
		this->shader->FillUniformVec3("u_emmited_light", light);
	}
}

void RayTracer::Render() {
	if (movement_factor > 2.0 || movement_factor < 0.0f) movement_speed *= -1.0f;
	movement_factor += movement_speed;
	this->shader->FillUniform1f("u_light_intensity", movement_factor / 2.0f);

	vao->Bind();
	shader->Bind();
	glCall(glDrawArrays(GL_TRIANGLE_FAN, 0, 4));
	shader->Unbind();
	vao->Unbind();
}