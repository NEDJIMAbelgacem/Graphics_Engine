#include "Core/Texture.h"

Texture::Texture(std::string path, bool flip_on_load)
	: path(path), width(0), height(0), bitsPerPixel(4)
{
	if (flip_on_load)
		stbi_set_flip_vertically_on_load(true);

	unsigned char* texture_buffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 0);
	if (texture_buffer == nullptr) N3D_LOG_FATAL("error at {} loading image \"{}\", stbi_failure_reason : {}", LINE_POSITION, path, stbi_failure_reason());

	glCall(glGenTextures(1, &id));
	glCall(glBindTexture(GL_TEXTURE_2D, id));

	GLenum format = GL_RGBA;
	if (bitsPerPixel == 3) format = GL_RGB;
	if (bitsPerPixel == 1) format = GL_ALPHA;
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_buffer));
	glCall(glGenerateMipmap(GL_TEXTURE_2D));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	stbi_image_free(texture_buffer);
}

Texture::Texture(unsigned int _id, int _width, int _height, int _bpp) : id(_id), width(_width), height(_height), bitsPerPixel(_bpp) {

}

void Texture::Bind(unsigned int slot) {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::Unbind() {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	this->slot = -1;
}

Texture::~Texture() {
	glCall(glDeleteTextures(1, &id));
}