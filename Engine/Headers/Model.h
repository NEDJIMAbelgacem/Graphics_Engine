#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <queue>

#include "ModelComponent.h"
#include "Batch.h"

class ModelComponent;
class VertexArray;
class Texture;
class Batch;

class Model {
private:
    ShaderProgram* shader;
	std::string model_path;
	std::vector<ModelComponent*> meshes;
	glm::mat4 model_matrix = glm::mat4(1.0f);
    glm::vec3 light_dir;
    glm::vec3 light_color;
	float reflectivity = 0.0f;
	float shineDamper = 1.0f;
	float diffuseFactor = 1.0f;
public:
	Model(std::string model_path, ShaderProgram* shader_prg);
    ~Model();
    
    void Render();
    void RenderBatch(Batch& batch);

	void SetModelMatrix(glm::mat4 matrix);
	void SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor);
    void SetLightParameters(glm::vec3 light_dir, glm::vec3 light_color);

	ShaderProgram* GetShader();
	ModelComponent* GetComponent(int i);
	unsigned int GetComponentsCount();
};