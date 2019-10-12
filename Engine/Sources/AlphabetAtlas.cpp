#include "AlphabetAtlas.h"

#define EntriesFile "Resources/alphabet/chars_models.txt"

AlphabetAtlas::AlphabetAtlas(std::string models_path, ShaderProgram* shader) {
    this->shader = shader;

	std::ifstream input(EntriesFile, std::ifstream::in);
	int lines_count;
	input >> lines_count;
	for (int i = 0; i < lines_count; ++i) {
		std::string s1, s2;
		input >> s1 >> s2;
		this->chars_models[s1[0]] = new Model(models_path + s2, shader);
	}
}

void AlphabetAtlas::Render(glm::vec3 pos, std::string str, float spacing, glm::vec3 spacing_direction) {
	for (char c : str) {
		if (chars_models.find(c) != chars_models.end()) {
			Model* m = chars_models[c];
            //shader->FillUniformMat4f("u_model", glm::translate(glm::identity<glm::mat4>(), pos));
			// m->SetModelMatrix(glm::translate(glm::identity<glm::mat4>(), pos));
            m->Render();
		} else N3D_LOG_WARN("character \"{}\" not found", c);
		pos.x += spacing;
	}
}

void AlphabetAtlas::SetCameraPosition(glm::vec3 pos) {
	shader->FillUniformVec3("u_cameraPos", pos);
}

AlphabetAtlas::~AlphabetAtlas() {
	for (std::pair<char, Model*> p : chars_models) delete p.second;
}