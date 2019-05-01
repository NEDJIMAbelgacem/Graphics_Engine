#include "AlphabetAtlas.h"

#define EntriesFile "chars_models.txt"

AlphabetAtlas::AlphabetAtlas(std::string models_path, std::string shader_path)
	: shader_path(shader_path), models_path(models_path) {

	shader = new ShaderProgram(shader_path);

	std::ifstream input(EntriesFile, std::ifstream::in);
	int lines_count;
	input >> lines_count;
	for (int i = 0; i < lines_count; ++i) {
		std::string s1, s2;
		input >> s1 >> s2;
		this->chars_models[s1[0]] = new Model(models_path + s2, shader);
	}
}

void AlphabetAtlas::Draw(Renderer& r, VertexArray& vao, glm::vec3 pos, std::string str) {
	for (char c : str) {
		if (chars_models.find(c) != chars_models.end()) {
			Model* m = chars_models[c];
			m->SetModelMatrix(glm::translate(glm::mat4(100.0f), pos));
			//r.DrawModel(*m, vao);
		}
		pos.x += 50;
	}
}


void AlphabetAtlas::SetSurfaceParameters(float reflectivity, float shineDamper, float diffuseFactor) {
	shader->FillUniform1f("u_material.reflectivity", reflectivity);
	shader->FillUniform1f("u_material.shineDamper", shineDamper);
	shader->FillUniform1f("u_material.diffuseFactor", diffuseFactor);
}

void AlphabetAtlas::SetLightParameters(glm::vec3 pos, glm::vec3 color) {
	shader->FillUniformVec3("u_lightPos", pos);
	shader->FillUniformVec3("u_lightColor", color);
}

void AlphabetAtlas::SetCameraPosition(glm::vec3 pos) {
	shader->FillUniformVec3("u_cameraPos", pos);
}

AlphabetAtlas::~AlphabetAtlas() {
	for (std::pair<char, Model*> p : chars_models) {
		delete p.second;
	}
}