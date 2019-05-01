#pragma once
#include "Renderer.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <tuple>
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"

#define RESTART_INDEX 99999

class ShaderProgram;
class VertexArray;

class Mesh {
public:
	Mesh(std::string filePath);
	unsigned int getVerticeCount() const;
	float* getVerticesData() const;
	float* getTexCoordsData() const;
	float* getNormalsData() const;
    void Render(glm::mat4 model, glm::mat4 view, glm::mat4 proj, VertexArray& vao, ShaderProgram& shader);

	~Mesh();
private:
	void LoadFromFile(std::string filePath);
	std::vector<std::string> SplitLine(std::string line, char sep = ' ');
private:
	float* vertices;
	float* texCoords;
	float* normals;
	unsigned int vertices_count;
};