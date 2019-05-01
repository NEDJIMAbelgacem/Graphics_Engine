#pragma once
#include "Renderer.h"
#include "ShaderProgram.h"
#define equirectangular2cubemap_shader_path "Resources/shaders/core/equirectangular2cubemap.shader"
#define irradiance_shader_path "Resources/shaders/core/irradiance.shader"

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