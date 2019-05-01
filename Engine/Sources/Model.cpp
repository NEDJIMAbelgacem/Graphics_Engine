#include "Model.h"

bool operator!=(glm::mat4& m1, glm::mat4& m2) {
	for (int i = 0; i < 16; ++i) {
		if (m1[i / 4][i % 4] != m2[i / 4][i % 4])
			return true;
	}
	return false;
}

Model::Model(std::string model_path, ShaderProgram* shader_ptr) {
	shader = shader_ptr;

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		__debugbreak();
	}
	std::string directory = model_path;
	while (directory.size() != 0 && directory.back() != '/') directory.pop_back();
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshes.push_back(new ModelComponent(directory, scene->mMeshes[i], scene));
	}
	/*std::queue<aiNode*> nodes;
	nodes.push(scene->mRootNode);
	while (nodes.size() != 0) {
		aiNode* node = nodes.back();
		nodes.pop();
		for (int i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(new ModelComponent(directory, mesh, scene));
		}
		for (int i = 0; i < node->mNumChildren; ++i) 
			nodes.push(node->mChildren[i]);
	}*/
}

Model::~Model() { }

void Model::Render() {
    for (ModelComponent* c : this->meshes) c->Render(this->shader);
}

void Model::RenderBatch(Batch& batch) {
    for (ModelComponent* c : this->meshes) c->RenderBatch(batch, this->shader);
}

void Model::SetModelMatrix(glm::mat4 matrix) {
	//if (this->model_matrix != matrix) {
		model_matrix = matrix;
		shader->FillUniformMat4f("u_model", matrix);
	//}
}

void Model::SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor) {
	if (this->reflectivity != reflectivity) {
		this->reflectivity = reflectivity;
		shader->FillUniform1f("u_material.reflectivity", reflectivity);
	}
	
	if (this->shineDamper != shineDamper) {
		this->shineDamper = shineDamper;
		shader->FillUniform1f("u_material.shineDamper", shineDamper);
	}

	if (this->diffuseFactor != diffuseFactor) {
		this->diffuseFactor = diffuseFactor;
		shader->FillUniform1f("u_material.diffuseFactor", diffuseFactor);
	}
}

void Model::SetLightParameters(glm::vec3 light_dir, glm::vec3 light_color) {
    if (this->light_dir != light_dir) {
        this->light_dir = light_dir;
        shader->FillUniformVec3("u_lightDir", light_dir);
    }
    if (this->light_color != light_color) {
        this->light_color = light_color;
        shader->FillUniformVec3("u_lightColor", light_color);
    }
}

ShaderProgram* Model::GetShader() {
	return shader;
}

ModelComponent* Model::GetComponent(int i) {
	return meshes[i];
}

unsigned int Model::GetComponentsCount() {
	return meshes.size();
}