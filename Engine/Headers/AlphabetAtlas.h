#pragma once
#include "Renderer.h"
#include "Model.h"
#include <map>

class Model;

class AlphabetAtlas {
private:
	std::string shader_path;
	std::string models_path;
	ShaderProgram* shader;
	std::map<char, Model*> chars_models;
public:
	AlphabetAtlas(std::string models_path, std::string shader);
	void SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor);
	void SetLightParameters(glm::vec3 pos, glm::vec3 color);
	void SetCameraPosition(glm::vec3 pos);

	void Draw(Renderer& r, VertexArray& vao, glm::vec3 pos, std::string str);
	~AlphabetAtlas();
};