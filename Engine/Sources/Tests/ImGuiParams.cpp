#include "Tests/ImGuiParams.h"
#include <ctime>
#include "stb_image_write.h"

ImGuiParams::ImGuiParams(std::string title) : Layer(title) { }
ImGuiParams::~ImGuiParams() { }

void ImGuiParams::OnImGuiRender() {
	for (std::string s : texts_order) {
		std::string line = s + " : " + texts[s];
		ImGui::Text(line.c_str());
		ImGui::NewLine();
	}
	texts_order.clear();
	for (std::pair<std::string, std::tuple<float*, float, float>> p : float_sliders) {
        float* ptr = std::get<0>(p.second);
        float min = std::get<1>(p.second);
        float max = std::get<2>(p.second);
		ImGui::SliderFloat(p.first.c_str(), ptr, min, max);
	}

	for (std::pair<std::string, std::tuple<glm::vec3*, float, float>> p : float_3sliders) {
        glm::vec3* ptr = std::get<0>(p.second);
        float min = std::get<1>(p.second);
        float max = std::get<2>(p.second);
		ImGui::SliderFloat3(p.first.c_str(), &ptr->x, min, max);
	}

	for (std::pair<std::string, std::tuple<int*, int, int>> p : integer_sliders) {
		std::string& title = p.first;
		int* ptr = std::get<0>(p.second);
		int min = std::get<1>(p.second);
		int max = std::get<2>(p.second);
		ImGui::SliderInt(title.c_str(), ptr, min, max);
	}
	for (std::pair<std::string, glm::vec3*> p : color_pickers) {
		std::string& title = p.first;
		glm::vec3* v = p.second;
		ImGui::ColorEdit3(title.c_str(), (float*)v);
	}
    for (auto t : drags_3floats) {
        glm::vec3* ptr = std::get<0>(t.second);
        float& min = std::get<1>(t.second);
        float& max = std::get<2>(t.second);
        float& speed = std::get<3>(t.second);
        ImGui::DragFloat3(t.first.c_str(), &ptr->x, speed, min, max);
    }
    for (auto t : drags_2floats) {
        glm::vec2* ptr = std::get<0>(t.second);
        float& min = std::get<1>(t.second);
        float& max = std::get<2>(t.second);
        float& speed = std::get<3>(t.second);
        ImGui::DragFloat2(t.first.c_str(), &ptr->x, speed, min, max);
    }
    for (auto t : drags_1floats) {
        float* ptr = std::get<0>(t.second);
        float& min = std::get<1>(t.second);
        float& max = std::get<2>(t.second);
        float& speed = std::get<3>(t.second);
        ImGui::DragFloat(t.first.c_str(), ptr, speed, min, max);
    }
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void ImGuiParams::AddText(std::string title, std::string value) {
	texts_order.push_back(title);
	texts[title] = value;
}

void ImGuiParams::AddFloatSlider(std::string title, float* ptr, float min, float max) {
	float_sliders[title] = std::make_tuple(ptr, min, max);
}

void ImGuiParams::AddInteger(std::string title, int* ptr, int min, int max) {
	integer_sliders[title] = std::make_tuple(ptr, min, max);
}

void ImGuiParams::Add3FloatSlider(std::string title, glm::vec3* offset, float min, float max) {
	float_3sliders[title] = std::make_tuple(offset, min, max);
}

void ImGuiParams::AddColorPicker(std::string title, glm::vec3* c) {
	color_pickers[title] = c;
}

void ImGuiParams::Add3FloatsDrag(std::string title, glm::vec3* ptr, float min, float max, float speed) {
    this->drags_3floats[title] = std::make_tuple(ptr, min, max, speed);
}

void ImGuiParams::Add2FloatsDrag(std::string title, glm::vec2* ptr, float min, float max, float speed) {
    this->drags_2floats[title] = std::make_tuple(ptr, min, max, speed);
}

void ImGuiParams::Add1FloatsDrag(std::string title, float* ptr, float min, float max, float speed) {
    this->drags_1floats[title] = std::make_tuple(ptr, min, max, speed);
}
