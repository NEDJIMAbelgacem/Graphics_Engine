#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) GLClearError();\
	x;\
	ASSERT(!GLPrintErrors(#x, __FILE__, __LINE__))

void GLClearError();

bool GLPrintErrors(std::string function_name, std::string file, int line);

#define DEFAULT_LIGHT_POS glm::vec3(0.0f, 1000.0f, 0.0f)