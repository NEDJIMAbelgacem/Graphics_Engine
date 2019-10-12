#pragma once
#include "Core/Common.h"
#include "Model.h"
#include "GameObject.h"

class AlphabetAtlas : public GameObject {
private:
	std::unordered_map<char, Model*> chars_models;
public:
	AlphabetAtlas(std::string models_path, ShaderProgram* shader);
    ~AlphabetAtlas();

	void SetCameraPosition(glm::vec3 pos);

	void Render(glm::vec3 pos, std::string str, float spacing, glm::vec3 spacing_direction = glm::vec3(1.0f, 0.0f, 0.0f));
};