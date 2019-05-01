#include "Terrain.h"

Terrain::Terrain(std::vector<std::string> textures_paths,  ShaderProgram* shader, glm::vec3 center_position, glm::vec3 size, int tiles_per_side, int vertices_per_tile, int seed) {
    this->size = size;
    this->shader = shader;
    this->tiles_per_side = tiles_per_side;
    // TODO: make tile size into glm::vec2
    this->tile_size = size.x / tiles_per_side;
    
    terrain_block = new TerrainBlock(vertices_per_tile);
    this->SetVerticesPerTile(vertices_per_tile);
	this->SetTilesPerSide(tiles_per_side);
	this->SetTileSize(tile_size);

    // setup height map and other textures
	this->height_map = HeightMap::GenerateRandomHeightMap(tiles_per_side * vertices_per_tile, tiles_per_side * vertices_per_tile, seed);
    for (std::string path : textures_paths) textures.push_back(new Texture(path));
	int* arr = (int*)alloca(textures.size() * sizeof(int));
	for (unsigned int i = 0; i < textures.size(); ++i) arr[i] = i + 1;
	shader->FillUnifrom1iv("u_material.diffuseMaps", textures.size(), arr);
	shader->FillUniform1i("u_heightMap", 0);
    shader->FillUniformMat4f("u_model", glm::identity<glm::mat4>());
    
    this->size.y -= 1.0f;
	this->SetHeightAmplitude(size.y);

    this->center_position -= glm::vec3(1.0f);
	this->SetPosition(center_position);
}

Terrain::~Terrain() {
	for (Texture* t : textures)
		delete t;
	delete terrain_block;
	delete height_map;
}

float Terrain::GetHeight(float x, float z) {
    float sub_tile_size = this->size.x / vertices_per_tile_side / tiles_per_side;
	return height_map->GetHeight((x + size.x / 2.0f) / sub_tile_size,
        (z + size.z / 2.0f) / sub_tile_size) * size.y / 2.0f + center_position.y;
}

void Terrain::Render() {
    VertexArray* vao = terrain_block->GetVertexArray();
    VertexBuffer* vbo = terrain_block->GetVerticesBuffer();
	IndexBuffer* ibo = terrain_block->GetIndexesBuffer();
	std::vector<Texture*> textures = this->GetTextures();

	int instances_count = this->GetInstancesCount();
	height_map->Bind(0);

	for (unsigned int i = 0; i < textures.size(); ++i) textures[i]->Bind(i + 1);
    
    shader->Bind();
	vao->Bind();
    ibo->Bind();
	glCall(glDrawElementsInstanced(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0, instances_count));
	ibo->Unbind();
	vao->Unbind();
	shader->Unbind();
	for (Texture* t : textures) t->Unbind();
	height_map->Unbind();
}

void Terrain::SetHeightAmplitude(float height) {
	if (this->size.y != height) {
		this->size.y = height;
		shader->FillUniform1f("u_height_amplitude", height / 2.0f);
	}
}

void Terrain::SetHeightMap(HeightMap* height_map) {
	delete this->height_map;
	this->height_map = height_map;
}

void Terrain::SetPosition(glm::vec3 pos) {
    if (this->center_position != pos) {
        this->center_position = pos;
	    glm::vec3 upper_left = this->center_position - this->size / 2.0f;
        upper_left.y = center_position.y;
	    shader->FillUniformMat4f("u_model", glm::translate(glm::identity<glm::mat4>(), upper_left));
    }
}

void Terrain::SetTilesPerSide(int tiles_per_side) {
	this->tiles_per_side = tiles_per_side;
	shader->FillUniform1i("u_tiles_per_side", tiles_per_side);
}

void Terrain::SetVerticesPerTile(int vertices_per_tile) {
    this->vertices_per_tile_side = vertices_per_tile;
	terrain_block->SetVerticesPerSide(vertices_per_tile);
    shader->FillUniform1i("u_vertices_per_tile", vertices_per_tile);
}

void Terrain::SetTileSize(float tile_size) {
	this->tile_size = tile_size;
	shader->FillUniform1f("u_tile_size", tile_size);
}

ShaderProgram* Terrain::GetShader() {
	return shader;
}

Texture* Terrain::GetTexture(int i) {
	ASSERT(i < textures.size() && i >= 0);
	return textures[i];
}

float Terrain::GetHeightAmplitude() {
	return this->size.y;
}

std::vector<Texture*> Terrain::GetTextures() {
	return textures;
}

int Terrain::GetTilesPerSide() {
	return tiles_per_side;
}

int Terrain::GetVerticesPerTile() {
	return terrain_block->GetVerticesCount();
}

float Terrain::GetTileSize() {
	return tile_size;
}

HeightMap* Terrain::GetHeightMap() {
	return this->height_map;
}

VertexBuffer* Terrain::GetVertexBuffer() {
	return terrain_block->GetVerticesBuffer();
}

IndexBuffer* Terrain::GetIndexBuffer() {
	return terrain_block->GetIndexesBuffer();
}

int Terrain::GetBlockCount() {
	int tiles_per_side = this->tiles_per_side;
	return tiles_per_side * tiles_per_side;
}

int Terrain::GetInstancesCount() {
	int vertices_per_tile = this->GetVerticesPerTile();
	return vertices_per_tile * vertices_per_tile * tiles_per_side * tiles_per_side;
}
/*
void Terrain::SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor) {
	this->reflectivity = reflectivity;
	this->shineDamper = shineDamper;
	this->diffuseFactor = diffuseFactor;
	shader->FillUniform1f("u_material.reflectivity", reflectivity);
	shader->FillUniform1f("u_material.shineDamper", shineDamper);
	shader->FillUniform1f("u_material.diffuseFactor", diffuseFactor);
}

void Terrain::SetLightParameters(glm::vec3 pos, glm::vec3 color) {
	lightPos = pos;
	lightColor = color;
	shader->FillUniformVec3("u_lightPos", pos);
	shader->FillUniformVec3("u_lightColor", color);
}
*/
void Terrain::SetCameraPosition(glm::vec3 pos) {
	cameraPos = pos;
	shader->FillUniformVec3("u_cameraPos", pos);
}