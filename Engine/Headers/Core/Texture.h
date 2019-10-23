#pragma once
#include "Core/Common.h"
#include "stb_image.h"
#include "Logger.h"

class Texture {
private:
	unsigned int id;
	std::string path;
	unsigned int slot = -1;
	int width, height, bitsPerPixel;
public:
	Texture(std::string path, bool flip_on_load = false);
	Texture(unsigned int _id, int _width, int _height, int _bpp);
    ~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();
};