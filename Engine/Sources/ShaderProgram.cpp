#include "Core/ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string path) : shader_path(path) {
	ShaderSource shader = LoadShadersSourceFrom(path);
	this->prg_id = CreateShader(shader);
}

ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader) {
	this->prg_id = CreateShader(ShaderSource{vertexShader, fragmentShader});
}

void ShaderProgram::Bind() const {
	glCall(glUseProgram(this->prg_id));
}

void ShaderProgram::Unbind() const {
	glCall(glUseProgram(0));
}

void ShaderProgram::FillUniform4f(std::string name, float v1, float v2, float v3, float v4) {
	this->Bind();
	glCall(glUniform4f(this->GetUniformLocation(name), v1, v2, v3, v4));
	this->Unbind();
}

void ShaderProgram::FillUniform3f(std::string name, float v1, float v2, float v3) {
	this->Bind();
	glCall(glUniform3f(this->GetUniformLocation(name), v1, v2, v3));
	this->Unbind();
}

void ShaderProgram::FillUniformVec3(std::string name, glm::vec3 v) {
	this->Bind();
	glCall(glUniform3f(this->GetUniformLocation(name), v.x, v.y, v.z));
	this->Unbind();
}

void ShaderProgram::FillUniformVec(std::string name, glm::vec2 v) {
	this->Bind();
	glCall(glUniform2f(this->GetUniformLocation(name), v.x, v.y));
	this->Unbind();
}

void ShaderProgram::FillUniform1i(std::string name, unsigned int value) {
	this->Bind();
	glCall(glUniform1i(this->GetUniformLocation(name), value));
	this->Unbind();
}

void ShaderProgram::FillUnifrom1iv(std::string name, unsigned int count, int* arr) {
	this->Bind();
	glCall(glUniform1iv(this->GetUniformLocation(name), count, arr));
	this->Unbind();
}

void ShaderProgram::FillUniform1f(std::string name, float value) {
	this->Bind();
	int uniform_location = this->GetUniformLocation(name);
	glCall(glUniform1f(uniform_location, value));
	this->Unbind();
}

void ShaderProgram::FillUniformMat4f(std::string name, glm::mat4 m) {
	this->Bind();
	int uniform_location = this->GetUniformLocation(name);
	glCall(glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &m[0][0]));
	this->Unbind();
}

void ShaderProgram::FillUniformMat3f(std::string name, glm::mat3 m) {
	this->Bind();
	int uniform_location = this->GetUniformLocation(name);
	glCall(glUniformMatrix3fv(uniform_location, 1, GL_FALSE, &m[0][0]));
	this->Unbind();
}

void ShaderProgram::FillUniform2f(std::string name, float v1, float v2) {
	this->Bind();
	int uniform_location = this->GetUniformLocation(name);
	glCall(glUniform2f(uniform_location, v1, v2));
	this->Unbind();
}

unsigned int ShaderProgram::GetUniformBlockIndex(std::string uniform_block) {
	unsigned int index;
	glCall(index = glGetUniformBlockIndex(this->prg_id, uniform_block.c_str()));
	return index;
}

void ShaderProgram::BindUniformBlock(unsigned int uniform_block_index, unsigned int binding_point) {
	glCall(glUniformBlockBinding(this->prg_id, uniform_block_index, binding_point));
}

unsigned int ShaderProgram::GetId() const {
	return this->prg_id;
}

ShaderSource ShaderProgram::LoadShadersSourceFrom(const std::string& source_path) {
	enum class shader_type { NONE = -1, VERTEX_SHADER = 0, FRAGMENT_SHADER = 1 };
	shader_type type = shader_type::NONE;

	std::string line;
	std::stringstream shaders[2];
	std::ifstream input(source_path);
	if (!input.is_open()) {
        Logger::N3D_CORE_FATAL("Unable to open \"{}\"", source_path);
		__debugbreak();
	}
	while (std::getline(input, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = shader_type::VERTEX_SHADER;
			else if (line.find("fragment") != std::string::npos)
				type = shader_type::FRAGMENT_SHADER;
		}
		else shaders[(int)type] << line << "\n";
	}
	
	return ShaderSource{ shaders[(int)shader_type::VERTEX_SHADER].str() , shaders[(int)shader_type::FRAGMENT_SHADER].str() };
}

void ShaderProgram::PrintShaderInfoLog(int shader_id, GLenum type) {
	int length;
	glCall(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
	char* message = (char*)alloca(length * sizeof(char));
	glCall(glGetShaderInfoLog(shader_id, length, &length, message));
    Logger::N3D_CORE_FATAL("{} shader : {}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
}

void ShaderProgram::PrintProgramInfoLog(int program_id) {
	int length;
	glCall(glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length));
	char* message = (char*)alloca(length * sizeof(char));
	glCall(glGetProgramInfoLog(program_id, length, &length, message));
	Logger::N3D_CORE_FATAL("{}", message);
}

unsigned int ShaderProgram::CompileShader(unsigned int type, const std::string& source) {
	unsigned int shader_id = glCreateShader(type);
	const char* src = source.c_str();
	glCall(glShaderSource(shader_id, 1, &src, nullptr));
	glCall(glCompileShader(shader_id));

	int result;
	glCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));

	if (!result) {
        Logger::N3D_CORE_FATAL("shader compilation error");
		PrintShaderInfoLog(shader_id, type);
		__debugbreak();
	}

	return shader_id;
}

unsigned int ShaderProgram::CreateShader(ShaderSource shader) {
	unsigned int prg = glCreateProgram();
	unsigned int vi = CompileShader(GL_VERTEX_SHADER, shader.vertex_shader);
	unsigned int fi = CompileShader(GL_FRAGMENT_SHADER, shader.fragment_shader);
	int link_status;
	glCall(glAttachShader(prg, vi));
	glCall(glAttachShader(prg, fi));
	glCall(glLinkProgram(prg));
	glCall(glGetProgramiv(prg, GL_LINK_STATUS, &link_status));
	if (link_status != GL_TRUE) {
        Logger::N3D_CORE_FATAL("shader program linking error");
		PrintProgramInfoLog(prg);
		__debugbreak();
	}
	glCall(glValidateProgram(prg));
	glCall(glGetProgramiv(prg, GL_LINK_STATUS, &link_status));
	if (link_status != GL_TRUE) {
        Logger::N3D_CORE_FATAL("shader program Validation error");
		PrintProgramInfoLog(prg);
		__debugbreak();
	}
	glCall(glDeleteShader(vi));
	glCall(glDeleteShader(fi));
	return prg;
}

int ShaderProgram::GetUniformLocation(std::string name) {
	int uniform_location;
	const char* str = name.c_str();
	if (this->uniforms_cache.find(name) == this->uniforms_cache.end()) {
		glCall(uniform_location = glGetUniformLocation(this->prg_id, str));
		uniforms_cache[name] = uniform_location;
	}
	else uniform_location = uniforms_cache[name];
	if (uniform_location == -1) Logger::N3D_CORE_ERROR("uniform {} location is -1", name);
    return uniform_location;
}

ShaderProgram::~ShaderProgram() {
	glCall(glDeleteProgram(this->prg_id));
}