#include "Plane.h"

Plane::Plane(glm::vec4 equation, glm::vec3 center, float size, int tiles_per_side, std::string shader_path, glm::vec3 color)
	: equation(equation), center(center), size(size), tiles_per_side(tiles_per_side), shader_path(shader_path), color(color) {
	shader = new ShaderProgram(shader_path);
	shader->FillUniformVec3("u_color", color);
	shader->FillUniformVec3("u_center", center);
	shader->FillUniform1f("u_size", size);
	shader->FillUniform1f("u_line_thickness", 0.05f);
	
	float vertices[3 * 4];
	unsigned int indexes[]{ 0, 1, 2, 3 };
	float p = size / 2.0f;
	for (int i = 0; i < 4; ++i) {
		float theta = (0.25 + 0.5 * i) * glm::pi<float>();
		float xp, yp, zp;
		if (equation[0] != 0) {
			yp = p * cos(theta) + center.y;
			zp = p * sin(theta) + center.z;
			xp = (equation[3] - yp * equation[1] - zp * equation[2]) / equation[0];
		}
		else if (equation[1] != 0) {
			xp = p * cos(theta) + center.x;
			zp = p * sin(theta) + center.z;
			yp = (equation[3] - xp * equation[0] - zp * equation[2]) / equation[1];
		}
		else {
			xp = p * cos(theta) + center.x;
			yp = p * sin(theta) + center.y;
			zp = (equation[3] - xp * equation[0] - yp * equation[1]) / equation[2];
		}
		
		vertices[i * 3] = xp;
		vertices[i * 3 + 1] = yp;
		vertices[i * 3 + 2] = zp;
	}
	vbo = new VertexBuffer(vertices, 4 * 3 * sizeof(float));
}

void Plane::SetZoom(float zoom) {
	shader->FillUniform1f("u_zoom", zoom);
}

void Plane::SetCenterOffset(glm::vec3 offset) {
	shader->FillUniformVec3("u_center_offset", offset);
}

VertexBuffer* Plane::GetVertexBuffer() {
	return vbo;
}

ShaderProgram* Plane::GetShader() {
	return shader;
}

Plane::~Plane() {
	delete vbo;
}