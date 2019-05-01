#pragma once
#include "Renderer.h"
#include <map>

struct UBO_component {
	unsigned int offset;
	unsigned int size;
};

class UniformBuffer {
private:
	unsigned int ubo;
	unsigned int binding_point;
	unsigned int ubo_size;
	std::map<std::string, UBO_component> components;
public:
	UniformBuffer(unsigned int binding_point, std::map<std::string, UBO_component> components);
	~UniformBuffer();

	void Bind();
	void Unbind();

	void Fill(std::string name, glm::mat4 m);
};