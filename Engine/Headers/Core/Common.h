#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) GLClearError();\
	x;\
	ASSERT(!GLPrintErrors(#x, __FILE__, __LINE__))

void GLClearError();

bool GLPrintErrors(std::string function_name, std::string file, int line);