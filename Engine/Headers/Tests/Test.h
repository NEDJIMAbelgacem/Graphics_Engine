#pragma once
#include "Core/Common.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Test {
public:
	Test();
	virtual void Display();
	virtual void Render();
	virtual ~Test();
};