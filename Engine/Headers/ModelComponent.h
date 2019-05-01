#pragma once
#include "Core/Common.h"
#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Core/Texture.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/IndexBuffer.h"
#include "Core/Common.h"
#include "Batch.h"

class Texture;
class ShaderProgram;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Batch;

#define vertices_location 0
#define uv_coords_location 1
#define normals_location 2
#define batch_instance_offset 3
#define tangent_location 4
#define bitangent_location 5

class ModelComponent {
private:
    VertexArray* vao = nullptr;
    IndexBuffer* ibo = nullptr;
    VertexBuffer* vbo = nullptr;

	VertexBuffer* vertices_vbo = nullptr;
	VertexBuffer** UVCoords_vbo = nullptr;
	VertexBuffer* normals_vbo = nullptr;
	VertexBuffer* tangent_vbo = nullptr;
	VertexBuffer* bitangent_vbo = nullptr;

	unsigned int* UVCoords_count = nullptr;
	unsigned int vertices_count = 0;
	unsigned int normals_count = 0;
	unsigned int tangent_count = 0;
	unsigned int indexes_count = 0;
	unsigned int uv_components_count = 0;
	std::vector<Texture*> diffuseMaps;
	std::vector<Texture*> specularMaps;
	std::vector<Texture*> heightMaps;
	std::vector<Texture*> ambientMaps;
    std::vector<Texture*> bound_textures;
public:
	ModelComponent(std::string directory, const aiMesh* mesh, const aiScene* scene);
    ~ModelComponent();
    
    void Render(ShaderProgram* shader);
    void RenderBatch(Batch& batch, ShaderProgram* shader);

	VertexBuffer* GetVerticesBuffer();
	VertexBuffer* GetTexCoordsBuffer(int i);
	VertexBuffer* GetNormalsBuffer();
	IndexBuffer* GetIndexesBuffer();
	unsigned int GetIndexesCount();
	unsigned int GetVerticesCount();
	unsigned int GetUVcount();
	unsigned int GetUVComponentsCount(int i);
	std::vector<Texture*> GetDiffuseMaps();
	std::vector<Texture*> GetSpecularMaps();
	std::vector<Texture*> GetHeightMaps();
	std::vector<Texture*> GetAmbientMaps();
	void BindTextures(ShaderProgram& shader);
	void UnbindTextures();
private:
	static std::map<std::string, Texture*> textures_map;
	void LoadTextures(aiMaterial* material, std::string directory, aiTextureType type);
};