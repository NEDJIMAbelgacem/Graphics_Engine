#include "Quad.h"

Quad::Quad(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords, std::string shader_path, std::string diffuseMap_path, std::string specularMap_path) 
	: shader_path(shader_path), diffuseMap_path(diffuseMap_path), specularMap_path(specularMap_path) {
	ASSERT(vertices.size() == 4);
	shader = new ShaderProgram(shader_path);
	diffuseMap = new Texture(diffuseMap_path);
	specualrMap = new Texture(specularMap_path);
	float vertex_buffer[20 * sizeof(float)];
	for (int i = 0; i < 4; ++i) {
		vertex_buffer[5 * i] = vertices[i].x;
		vertex_buffer[5 * i + 1] = vertices[i].y;
		vertex_buffer[5 * i + 2] = vertices[i].z;;
		vertex_buffer[5 * i + 3] = texCoords[i].x;
		vertex_buffer[5 * i + 4] = texCoords[i].y;
	}
	vbo = new VertexBuffer(vertex_buffer, sizeof(vertex_buffer));
	unsigned int indexes[6]{ 0, 1, 2, 0, 2, 3 };
	ibo = new IndexBuffer(indexes, 6);
}

void Quad::SetLightPos(glm::vec3 pos) {
	this->lightPos = pos;
	shader->FillUniformVec3("u_lightPos", pos);
}

void Quad::SetCameraPos(glm::vec3 pos) {
	this->cameraPos = pos;
	shader->FillUniformVec3("u_cameraPos", pos);
}

void Quad::SetLightColor(glm::vec3 color) {
	this->lightColor = color;
	shader->FillUniformVec3("u_lightColor", color);
}

void Quad::SetModelMatrix(glm::mat4 matrix) {
	this->model_matrix = matrix;
	shader->FillUniformMat4f("u_model", matrix);
}

void Quad::SetAmbientColor(glm::vec3 ambientC) {
	this->ambientColor = ambientC;
	shader->FillUniformVec3("u_material.ambientLColor", ambientC);
}

void Quad::SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor) {
	this->reflectivity = reflectivity;
	this->shineDamper = shineDamper;
	this->diffuseFactor = diffuseFactor;
	shader->FillUniform1f("u_material.reflectivity", reflectivity);
	shader->FillUniform1f("u_material.shineDamper", shineDamper);
	shader->FillUniform1f("u_material.diffuseFactor", diffuseFactor);
}

VertexBuffer* Quad::GetVertexBuffer() {
	return vbo;
}

IndexBuffer* Quad::GetIndexBuffer() {
	return ibo;
}

Texture* Quad::GetDiffuseMap() {
	return diffuseMap;
}

Texture* Quad::GetSpecularMap() {
	return specualrMap;
}

ShaderProgram* Quad::GetShader() {
	return shader;
}

Quad::~Quad() {
	delete vbo;
	delete ibo;
	delete diffuseMap;
	delete specualrMap;
}