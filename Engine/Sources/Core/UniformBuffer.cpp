#include "Core/UniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"

UniformBuffer::UniformBuffer(unsigned int binding_point, std::map<std::string, UBO_component> components) : binding_point(binding_point), components(components) {
	glCall(glGenBuffers(1, &ubo));
	this->ubo_size = 0;
	for (std::pair<std::string, UBO_component> c : components) ubo_size += c.second.size;
	glCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
	glCall(glBufferData(GL_UNIFORM_BUFFER, ubo_size, NULL, GL_STATIC_DRAW));
	glCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	glCall(glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, ubo, 0, ubo_size));
}

void UniformBuffer::Bind() {
	glCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
}

void UniformBuffer::Unbind() {
	glCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::Fill(std::string name, glm::mat4 m) {
	if (this->components.find(name) == this->components.end()) {
		std::cerr << "warning : component " << name << " doesn\'t exist in uniform buffer object " << ubo << std::endl;
		return;
	}
	UBO_component c = this->components[name];
	this->Bind();
	glCall(glBufferSubData(GL_UNIFORM_BUFFER, c.offset, c.size, glm::value_ptr(m)));
	this->Unbind();
}

UniformBuffer::~UniformBuffer() {
	glCall(glDeleteBuffers(1, &ubo));
}