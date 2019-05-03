#pragma once
#include "Core/Common.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
#include "Logger.h"

struct ShaderSource {
	std::string vertex_shader;
	std::string fragment_shader;
};

class ShaderProgram {
private:
	ShaderProgram* shader;
	std::string shader_path;
	unsigned int prg_id;
	std::unordered_map<std::string, int> uniforms_cache;
public:
	ShaderProgram(std::string path);
	ShaderProgram(std::string vertexShader, std::string fragmentShader);

	void Bind() const;

	void Unbind() const;

	void FillUniform4f(std::string name, float v1, float v2, float v3, float v4);
	void FillUniform3f(std::string name, float v1, float v2, float v3);
	void FillUniformVec3(std::string name, glm::vec3 v);
	void FillUniformVec(std::string name, glm::vec2 v);
	void FillUniform1i(std::string name, unsigned int value);
	void FillUnifrom1iv(std::string name, unsigned int count, int* arr);
	void FillUniform1f(std::string name, float value);
	void FillUniform2f(std::string name, float v1, float v2);
	void FillUniformMat4f(std::string name, glm::mat4 m);
	void FillUniformMat3f(std::string name, glm::mat3 m);

	unsigned int GetUniformBlockIndex(std::string uniform_block);
	void BindUniformBlock(unsigned int uniform_block_index, unsigned int binding_point);

	unsigned int GetId() const;

	~ShaderProgram();

private:
	ShaderSource LoadShadersSourceFrom(const std::string& source_path);

	unsigned int CompileShader(unsigned int type, const std::string& source);

	unsigned int CreateShader(ShaderSource shader);

	void PrintShaderInfoLog(int shader_id, GLenum type);
	void PrintProgramInfoLog(int program_id);

	int GetUniformLocation(std::string name);
};