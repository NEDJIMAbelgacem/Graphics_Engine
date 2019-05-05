#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Logger.h"

#define LINE_POSITION std::string() + __FILE__ + ":" + std::to_string(__LINE__)
#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) GLClearError();\
	x;\
	ASSERT(!GLPrintErrors(#x, __FILE__, __LINE__))

void GLClearError();

bool GLPrintErrors(std::string function_name, std::string file, int line);

#define DEFAULT_LIGHT_POS glm::vec3(0.0f, 1000.0f, 0.0f)