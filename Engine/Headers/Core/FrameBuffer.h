#pragma once
#include "Core/Common.h"
#include "Core/VertexArray.h"
#include "Core/ShaderProgram.h"

class FrameBuffer {
private:
	unsigned int samples_count = 4;
	unsigned int fbo;
	unsigned int rbo;
	unsigned int multisample_color_texture;
	unsigned int intermediate_fbo;
	unsigned int screen_texture;
	unsigned int depth_texture;
public:
	FrameBuffer();
	bool IsComplete();
	void Bind();
	void BindTexture(int slot);
	void BindDepthTexture(int slot);
	void Unbind();
	void UnbindTexture();
	void UnbindDepthTexture();

	void Draw(VertexArray& vao, ShaderProgram& shader);
	~FrameBuffer();
};