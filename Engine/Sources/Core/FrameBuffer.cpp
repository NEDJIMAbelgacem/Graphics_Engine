#include "Core/FrameBuffer.h"

FrameBuffer::FrameBuffer() {
	glCall(glGenFramebuffers(1, &fbo));
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

	// create color texture
	glCall(glGenTextures(1, &screen_texture));
	glCall(glBindTexture(GL_TEXTURE_2D, screen_texture));
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	// create depth texture
	glCall(glGenTextures(1, &depth_texture));
	glCall(glBindTexture(GL_TEXTURE_2D, depth_texture));
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 800, 600, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));

	glCall(bool is_complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
	if (is_complete) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		__debugbreak();
	}

	glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

bool FrameBuffer::IsComplete() {
	this->Bind();
	glCall(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	this->Unbind();
	return status == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::Bind() {
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
}

void FrameBuffer::BindTexture(int slot) {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, screen_texture));
}

void FrameBuffer::UnbindTexture() {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void FrameBuffer::BindDepthTexture(int slot) {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, depth_texture));
}

void FrameBuffer::UnbindDepthTexture() {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void FrameBuffer::Unbind() {
	glCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo));
	//glCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate_fbo));
	//glCall(glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_LINEAR));
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 
}


void FrameBuffer::Draw(VertexArray& vao, ShaderProgram& shader) {
	vao.Bind();
	this->BindTexture(0);
	shader.FillUniform1i("screenTexture", 0);
	shader.Bind();
	glCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	shader.Unbind();
}

FrameBuffer::~FrameBuffer() {
	glCall(glDeleteFramebuffers(1, &fbo));
	glCall(glDeleteRenderbuffers(1, &rbo));
	glCall(glDeleteTextures(1, &screen_texture));
	//glCall(glDeleteTextures(1, &multisample_color_texture));
	glCall(glDeleteFramebuffers(1, &intermediate_fbo));
}