#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <queue>
#include "Logger.h"
#include "Batch.h"
#include "ModelComponent.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/ComponentTypes.h"
#include "Components/Lighting.h"
#include "Components/Surface.h"
#include "Components/Transform.h"

class Model : public GameObject {
private:
	std::string model_path;
	std::vector<ModelComponent*> meshes;
public:
	Model(std::string model_path, ShaderProgram* shader_prg);
    ~Model();
    
    void Render();
    void RenderBatch(Batch& batch);

	ModelComponent* GetComponent(int index);
    inline std::vector<ModelComponent*>::iterator ComponentsBegin() { return meshes.begin(); }
    inline std::vector<ModelComponent*>::iterator ComponentsEnd() { return meshes.end(); }
	unsigned int GetComponentsCount();
};