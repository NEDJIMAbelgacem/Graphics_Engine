#pragma once
#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "Core/Cubemap.h"
#include "Core/Texture.h"
#include "stb_image.h"
#include "Camera.h"
#include "GameObject.h"

class ShaderProgram;
class VertexArray;
class Cubemap;

class SkyBox : public GameObject {
private:
	const std::vector<std::string> suffixes{ "rt", "lf", "up", "dn", "bk", "ft" };
private:
	VertexArray* vao;
	Cubemap* cubemap;
	Cubemap* irradiance_map;

	unsigned int slot = -1;
	int width, height, bitsPerPixel;
	std::string skybox_name;
	bool is_hdr = false;
public:
	SkyBox(std::string folder, std::string file, ShaderProgram* shader);
	SkyBox(std::string file_path, ShaderProgram* shader);
	~SkyBox();

    void Setup(Camera& camera);

	void Render();

	void Bind(unsigned int slot = 0);
	void BindIrradiance(unsigned int slot = 0);
	void Unbind();

	std::string GetName();
	ShaderProgram* GetShader();
private:
	void PrepareVAO();
};