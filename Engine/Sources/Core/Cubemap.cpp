#include "Core/Cubemap.h"

Cubemap::Cubemap(std::string folder, std::string file) {
	const bool flip = false;
	glCall(glGenTextures(1, &id));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	for (int i = 0; i < 6; ++i) {
		std::string suff = suffixes[i];
		if (!flip) {
			if (suff == "up") suff = "dn";
			else if (suff == "dn") suff = "up";
		}
		std::string file_name = std::string() + "Resources/skyboxes/" + folder + "/" + file + "_" + suff + ".tga";
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(file_name.c_str(), &width, &height, &bitsPerPixel, 0);
		stbi_set_flip_vertically_on_load(false);
		if (data == nullptr) N3D_LOG_FATAL("Failed to load cube map from {}", file_name);

		if (suff == "up") data = RotateImage(data, width, height, bitsPerPixel, flip);
		if (suff == "dn") data = RotateImage(data, width, height, bitsPerPixel, !flip);

		glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
        if (suff == "up" || suff == "dn") delete [] data;
        else stbi_image_free(data);
	}

	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

VertexArray* cubeVAO = nullptr;
VertexBuffer* cubeVBO = nullptr;
void renderCube2()
{
	// initialize (if necessary)
	if (cubeVAO == nullptr) {
		float vertices[]{
			// back face
			-1.0f, -1.0f, -1.0f, // bottom-left
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f, // top-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f, -1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-right
			 1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f,  1.0f, // top-left
			-1.0f, -1.0f,  1.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right         
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f, // top-left
			 1.0f,  1.0f , 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right     
			 1.0f,  1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f,  1.0f,  1.0f  // bottom-left        
		};
		cubeVAO = new VertexArray();
		VertexBuffer* cubeVBO = new VertexBuffer(vertices, sizeof(vertices));
		BufferLayout layout;
		layout.AddElement<float>(3, 0);
		cubeVAO->AddBuffer(*cubeVBO, layout);
	}
	cubeVAO->Bind();
	glCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	cubeVAO->Unbind();
}

ShaderProgram* Cubemap::equirectangular2CubemapShader = nullptr;
Cubemap::Cubemap(std::string file_name) {
	glCall(glEnable(GL_DEPTH_TEST));
	glCall(glDepthFunc(GL_LEQUAL));

	// setup frame buffer
	unsigned int captureFBO, captureRBO;
	glCall(glGenFramebuffers(1, &captureFBO));
	glCall(glGenRenderbuffers(1, &captureRBO));

	glCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
	glCall(glBindRenderbuffer(GL_RENDERBUFFER, captureRBO));
	glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
	glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO));

	// load HDR environment map
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float *data = stbi_loadf(file_name.c_str(), &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glCall(glGenTextures(1, &hdrTexture));
		glCall(glBindTexture(GL_TEXTURE_2D, hdrTexture));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data)); // note how we specify the texture's data value to be float

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		stbi_image_free(data);
	}
	else Logger::N3D_LOG_ERROR("Failed to load HDR image from {}", file_name);

	// setup cubemap to render to and attach to framebuffer
	unsigned int envCubemap;
	glCall(glGenTextures(1, &envCubemap));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	for (unsigned int i = 0; i < 6; ++i) {
		// note that we store each face with 16 bit floating point values
		glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr));
	}

	// set up view matrices for the 6 directions
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[]{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	if (Cubemap::equirectangular2CubemapShader == nullptr) 
		Cubemap::equirectangular2CubemapShader = new ShaderProgram(equirectangular2cubemap_shader_path);
	// convert HDR equirectangular environment map to cubemap equivalent
	Cubemap::equirectangular2CubemapShader->Bind();
	Cubemap::equirectangular2CubemapShader->FillUniform1i("equirectangularMap", 0);
	Cubemap::equirectangular2CubemapShader->FillUniformMat4f("projection", captureProjection);
	glCall(glActiveTexture(GL_TEXTURE0));
	glCall(glBindTexture(GL_TEXTURE_2D, hdrTexture));

	glCall(glViewport(0, 0, 512, 512)); // don't forget to configure the viewport to the capture dimensions.
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangular2CubemapShader->FillUniformMat4f("view", captureViews[i]);
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0));
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		equirectangular2CubemapShader->Bind();
		renderCube2(); // renders a 1x1 cube
	}
	bool is_complete;
	glCall(is_complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	if (!is_complete) N3D_LOG_FATAL("Framebuffer construction at {} is not complete while loading {}", LINE_POSITION, file_name);
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	
	glCall(glDeleteTextures(1, &hdrTexture));
	glCall(glDeleteRenderbuffers(1, &captureRBO));
	glCall(glDeleteFramebuffers(1, &captureFBO));
	this->id = envCubemap;
	glCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
}

ShaderProgram* Cubemap::irradianceShader = nullptr;
Cubemap::Cubemap(Cubemap* cubemap) {
	glCall(glEnable(GL_DEPTH_TEST));
	glCall(glDepthFunc(GL_LEQUAL));
	unsigned int envCubemap = cubemap->id;
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[]{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// setup frame buffer
	unsigned int captureFBO, captureRBO;
	glCall(glGenFramebuffers(1, &captureFBO));
	glCall(glGenRenderbuffers(1, &captureRBO));

	glCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
	glCall(glBindRenderbuffer(GL_RENDERBUFFER, captureRBO));
	glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512));
	glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO));

	// create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	unsigned int irradianceMap;
	glCall(glGenTextures(1, &irradianceMap));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap));
	for (unsigned int i = 0; i < 6; ++i)
	{
		glCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr));
	}
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	glCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
	glCall(glBindRenderbuffer(GL_RENDERBUFFER, captureRBO));
	glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32));

	if (irradianceShader == nullptr) irradianceShader = new ShaderProgram(irradiance_shader_path);
	irradianceShader->Bind();
	irradianceShader->FillUniform1i("environmentMap", 0);
	irradianceShader->FillUniformMat4f("projection", captureProjection);
	glCall(glActiveTexture(GL_TEXTURE0));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap));

	glCall(glViewport(0, 0, 32, 32)); // don't forget to configure the viewport to the capture dimensions.
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, captureFBO));
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader->FillUniformMat4f("view", captureViews[i]);
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0));
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		irradianceShader->Bind();
		renderCube2();
	}
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	glCall(glDeleteRenderbuffers(1, &captureRBO));
	glCall(glDeleteFramebuffers(1, &captureFBO));
	this->id = irradianceMap;
	glCall(glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
}

unsigned char* Cubemap::RotateImage(unsigned char* data, int x, int y, int nb_c, bool anti) {
	if (x != y) N3D_LOG_WARN("cube map face not a square and I haven't implemented that path yet :3");
	unsigned char* data2 = new unsigned char[x * y * nb_c];
	for (int i = 0; i < x; ++i) {
		for (int j = 0; j < y; ++j) {
			int index1 = i * y + j;
			int index2 = (y - 1 - index1 % y) * x + index1 / y;
			for (int k = 0; k < nb_c; ++k) {
				if (anti) data2[nb_c * index1 + k] = data[nb_c * index2 + k];
				else data2[nb_c * index2 + k] = data[nb_c * index1 + k];
			}
		}
	}
	stbi_image_free(data);
	return data2;
}

void Cubemap::Bind(unsigned int slot) {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, id));
}

void Cubemap::Unbind() {
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

Cubemap::~Cubemap() {
	glCall(glDeleteTextures(1, &id));
}