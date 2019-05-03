#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"
#include "Core/VertexBuffer.h"
#include "stb_image.h"
#include "Logger.h"

#define equirectangular2cubemap_shader_path "Shaders/Cubemap/equirectangular2cubemap.glsl"
#define irradiance_shader_path "Shaders/Cubemap/irradiance.glsl"

class ShaderProgram;

class Cubemap {
private:
	static ShaderProgram* equirectangular2CubemapShader;
	static ShaderProgram* irradianceShader;
private:
	const std::vector<std::string> suffixes{ "rt", "lf", "dn", "up", "bk", "ft" };
	unsigned char* RotateImage(unsigned char* data, int x, int y, int nb_c, bool anti = false);
private:
	unsigned int id;
	int width, height, bitsPerPixel;
public:
	Cubemap(std::string folder, std::string file);
	Cubemap(std::string file_name);
	Cubemap(Cubemap* cubemap);

	void Bind(unsigned int slot = 0);
	void Unbind();

	~Cubemap();
};