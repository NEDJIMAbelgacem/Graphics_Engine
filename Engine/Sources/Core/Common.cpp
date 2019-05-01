#include "Core/Common.h"

void GLClearError() {
	while (glGetError());
}

bool GLPrintErrors(std::string function_name, std::string file, int line) {
	bool has_errors = false;
	while (GLenum err = glGetError()) {
		has_errors = true;
		std::cout << "[OpenGL error] : " << err << " " << function_name << " in " << file << ":" << line << std::endl;
	}
	return has_errors;
}