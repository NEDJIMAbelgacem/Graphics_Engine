#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <queue>
#include "ModelComponent.h"
#include "Batch.h"
#include "ShaderComponents/DirectionalLight.h"
#include "ShaderComponents/SurfaceParameters.h"
#include "AbstractClasses/Renderable.h"
#include "Logger.h"

class ModelComponent;
class VertexArray;
class Texture;
class Batch;

class Model : public Renderable, public DirectionalLight, public SurfaceParameters {
private:
	std::string model_path;
	std::vector<ModelComponent*> meshes;
	glm::mat4 model_matrix = glm::mat4(0.0f);
public:
	Model(std::string model_path, ShaderProgram* shader_prg);
    ~Model();
    
    void Render();
    void RenderBatch(Batch& batch);

	void SetModelMatrix(glm::mat4 matrix);

	ModelComponent* GetComponent(int i);
	unsigned int GetComponentsCount();
};