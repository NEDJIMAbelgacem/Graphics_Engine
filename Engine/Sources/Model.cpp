#include "Model.h"

bool operator!=(glm::mat4& m1, glm::mat4& m2) {
	for (int i = 0; i < 16; ++i) {
		if (m1[i / 4][i % 4] != m2[i / 4][i % 4])
			return true;
	}
	return false;
}

Model::Model(std::string model_path, ShaderProgram* shader_ptr) {
	this->shader = shader_ptr;
    
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
        N3D_LOG_FATAL("assimp asset loading error : {}", importer.GetErrorString());
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

ModelComponent* Model::GetComponent(int index) {
	return meshes[index];
}

unsigned int Model::GetComponentsCount() {
	return meshes.size();
}