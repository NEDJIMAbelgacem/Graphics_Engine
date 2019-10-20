#pragma once
#include "Core/Common.h"
#include "stb_image.h"
#include "Logger.h"
#include "Core/ShaderProgram.h"

class UniformBuffer {
private:
	unsigned int id;
public:
	UniformBuffer(unsigned int size_in_bytes) {
        glCall(glGenBuffers(1, &id));
	    glCall(glBindBuffer(GL_UNIFORM_BUFFER, id));
	    glCall(glBufferData(GL_UNIFORM_BUFFER, size_in_bytes + 2, nullptr, GL_STATIC_DRAW));

        glCall(glBindBufferBase(GL_UNIFORM_BUFFER, 2, id));
        glCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
        // glCall(glBindBufferRange(GL_UNIFORM_BUFFER, 2, id, 0, size_in_bytes));
    }

    ~UniformBuffer() {
        glCall(glDeleteBuffers(1, &id));
    }

    unsigned int GetId() const {
	    return this->id;
    }

	void Bind(ShaderProgram& prg, std::string uniform_block_name, unsigned int binding_point) {
        glCall(glBindBuffer(GL_UNIFORM_BUFFER, id));
        int index;
        glCall(index = glGetUniformBlockIndex(prg.GetId(), uniform_block_name.c_str()));
        // glCall(glBindBufferBase(GL_UNIFORM_BUFFER, 2, id));
        glCall(glUniformBlockBinding(prg.GetId(), index, binding_point));
    }

	void Unbind() {
        glCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

    void ModifyData(unsigned int offset_in_bytes, unsigned int size, void* data) {
        glCall(glBindBuffer(GL_UNIFORM_BUFFER, id));
        glCall(glBufferSubData(GL_UNIFORM_BUFFER, offset_in_bytes, size, data)); 
        // glCall(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW));
        glCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }
};