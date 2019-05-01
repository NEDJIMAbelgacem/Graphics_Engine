#pragma once
#include "Renderer.h"

class Quad {
private:
	std::string shader_path;
	std::string diffuseMap_path;
	std::string specularMap_path;
	float reflectivity;
	float shineDamper;
	float diffuseFactor;
	glm::vec3 ambientColor;
	ShaderProgram* shader;
	Texture* diffuseMap;
	Texture* specualrMap;
	VertexBuffer* vbo;
	IndexBuffer* ibo;
	glm::vec3 cameraPos;
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	glm::mat4 model_matrix;
public:
	Quad(std::vector<glm::vec3> vertices, std::vector<glm::vec2> texCoords, std::string shader_path, std::string diffuseMap_path, std::string specularMap_path);
	void SetLightPos(glm::vec3 pos);
	void SetCameraPos(glm::vec3 pos);
	void SetLightColor(glm::vec3 color);
	void SetModelMatrix(glm::mat4 matrix);
	void SetAmbientColor(glm::vec3 ambientC);
	void SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor);

	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	Texture* GetDiffuseMap();
	Texture* GetSpecularMap();
	ShaderProgram* GetShader();
	~Quad();
};