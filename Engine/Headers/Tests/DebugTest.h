#pragma once
#include "Test.h"
#include <map>

class DebugTest : public Test {
private:
	std::map<std::string, std::string> texts;
	std::vector<std::string> texts_order;

	std::map<std::string, std::tuple<float*, float, float>> float_sliders;
	std::map<std::string, std::tuple<glm::vec3*, float, float>> float_3sliders;
	std::map<std::string, std::tuple<int*, int, int>> integer_sliders;
	std::map<std::string, glm::vec3*> color_pickers;
    std::map<std::string, std::tuple<glm::vec3*, float, float, float>> drags_3floats;
    std::map<std::string, std::tuple<glm::vec2*, float, float, float>> drags_2floats;
public:
	DebugTest();
	void AddText(std::string title, std::string value);
	void AddFloatSlider(std::string title, float* ptr, float min = 0.0f, float max = 1.0f);
	void Add3FloatSlider(std::string title, glm::vec3* offset, float min = -1.0f, float max = 1.0f);
	void AddInteger(std::string title, int* ptr, int min, int max);
	void AddColorPicker(std::string title, glm::vec3* c);
    void Add3FloatsDrag(std::string title, glm::vec3* ptr, float min = 0.0f, float max = 1.0f, float speed = 0.01);
    void Add2FloatsDrag(std::string title, glm::vec2* ptr, float min = 0.0f, float max = 1.0f, float speed = 0.01);
	void Display();
	void Render();
	~DebugTest();
};