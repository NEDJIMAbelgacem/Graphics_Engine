#pragma once
#include "Core/Common.h"
#include "TerrainBlock.h"
#include "HeightMap.h"
#include <string>

class TerrainBlock;
class HeightMap;
 
#define SEED 1

class Terrain {
private:
	TerrainBlock* terrain_block;
	HeightMap* height_map;
	ShaderProgram* shader;

	std::vector<Texture*> textures;
	std::vector<std::string> textures_paths;
	std::string shader_path;
	float tile_size;
	int tiles_per_side;
    int vertices_per_tile_side;
    glm::vec3 size;
	glm::vec3 center_position;

	float reflectivity;
	float shineDamper;
	float diffuseFactor;
	glm::vec3 lightPos;
	glm::vec3 cameraPos;
	glm::vec3 lightColor;
public:
	Terrain(std::vector<std::string> texture_path, ShaderProgram* shader, glm::vec3 center_pos, 
        glm::vec3 size, int tiles_per_side, int vertices_per_tile, int seed = SEED);
    ~Terrain();

    void Render();

	ShaderProgram* GetShader();
	Texture* GetTexture(int i);
	std::vector<Texture*> GetTextures();

	void SetHeightAmplitude(float height);
	void SetPosition(glm::vec3 pos);
	void SetTilesPerSide(int tiles_per_side);
	void SetVerticesPerTile(int vertices_per_tile);
	void SetTileSize(float tile_size);
	void SetHeightMap(HeightMap* height_map);
	int GetTilesPerSide();
	int GetVerticesPerTile();
	float GetHeightAmplitude();
	HeightMap* GetHeightMap();
	float GetHeight(float x, float z);
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	int GetBlockCount();
	float GetTileSize();
	int GetInstancesCount();

	void SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor);
	void SetLightParameters(glm::vec3 pos, glm::vec3 color);
	void SetCameraPosition(glm::vec3 pos);
};