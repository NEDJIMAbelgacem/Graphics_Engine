#include "Core/Common.h"
#include "Logger.h"

void GLClearError() {
	while (glGetError());
}

bool GLPrintErrors(std::string function_name, std::string file, int line) {
	bool has_errors = false;
	while (GLenum err = glGetError()) {
		has_errors = true;
        N3D_LOG_FATAL("opengl error code {} executing {} at {}:{}", err, function_name, file, line);
	}
	return has_errors;
}