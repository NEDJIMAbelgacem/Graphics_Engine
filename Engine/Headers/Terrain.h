#pragma once
#include "Core/Common.h"
#include "TerrainBlock.h"
#include "HeightMap.h"
#include "Core/IndexBuffer.h"
#include "Core/Texture.h"
#include "GameObject.h"

class TerrainBlock;
class HeightMap;
 
#define SEED 1

class Terrain : public GameObject {
private:
	TerrainBlock* terrain_block;
	HeightMap* height_map;

	std::vector<Texture*> textures;
	float tile_size;
	int tiles_per_side;
    int vertices_per_tile_side;
    glm::vec3 size;
	glm::vec3 center_position;

	glm::vec3 cameraPos;
public:
	Terrain(std::vector<std::string> textures_paths, ShaderProgram* shader, glm::vec3 center_pos, 
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

	void SetCameraPosition(glm::vec3 pos);
};