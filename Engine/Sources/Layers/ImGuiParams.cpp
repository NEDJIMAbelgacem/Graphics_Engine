#include "Layers/ImGuiParams.h"
#include <ctime>
#include "stb_image_write.h"
#include "Window/Application.h"

ImGuiParams::ImGuiParams(std::string title) : Layer(title) { }
ImGuiParams::~ImGuiParams() { }

using EventCallbackFn = std::function<void(Event&)>;

void ImGuiParams::OnImGuiRender() {
	for (std::string s : texts_order) {
		std::string line = s + " : " + texts[s];
		ImGui::Text(line.c_str());
		ImGui::NewLine();
	}
	texts_order.clear();

	for (auto p : float_sliders) {
        EventCallbackFn& callback = std::get<0>(p.second);
        float& v = std::get<1>(p.second);
        float min = std::get<2>(p.second);
        float max = std::get<3>(p.second);

        float new_v = v;
		ImGui::SliderFloat(p.first.c_str(), &new_v, min, max);
        if (new_v != v) {
            ImGui1FloatSliderChangedEvent e(new_v);
            callback(e);
        }
        float_sliders[p.first] = std::make_tuple(callback, new_v, min, max);
	}

	for (auto p : float_3sliders) {
        std::string title = p.first;
        EventCallbackFn& callback = std::get<0>(p.second);
        glm::vec3& v = std::get<1>(p.second);
        float min = std::get<2>(p.second);
        float max = std::get<3>(p.second);
		
        glm::vec3 new_v = v;
        ImGui::SliderFloat3(title.c_str(), &new_v[0], min, max);
        if (new_v != v) {
            ImGui3FloatSliderChangedEvent e(new_v);
            callback(e);
        }
        float_3sliders[title] = std::make_tuple(callback, new_v, min, max);
	}

	for (auto p : integer_sliders) {
        std::string title = p.first;
        EventCallbackFn& callback = std::get<0>(p.second);
		int& v = std::get<1>(p.second);
		int min = std::get<2>(p.second);
		int max = std::get<3>(p.second);

        int new_v = v;
        ImGui::SliderInt(title.c_str(), &new_v, min, max);
        if (new_v != v) {
            ImGui1IntegerSliderChangedEvent e(new_v);
            callback(e);
        }
        v = new_v;
        integer_sliders[title] = std::make_tuple(callback, new_v, min, max);
	}
	for (auto p : color_pickers) {
        std::string title = p.first;
        EventCallbackFn& callback = std::get<0>(p.second);
		glm::vec3& v = std::get<1>(p.second);

        glm::vec3 new_v = v;
        ImGui::ColorEdit3(title.c_str(), &new_v[0]);
        if (new_v != v) {
            ImGuiColorPickerChangedEvent e(new_v);
            callback(e);
        }
        color_pickers[title] = std::make_tuple(callback, new_v);
	}
    for (auto t : drags_3floats) {
        std::string title = t.first;
        EventCallbackFn& callback = std::get<0>(t.second);
        glm::vec3& v = std::get<1>(t.second);
        float min = std::get<2>(t.second);
        float max = std::get<3>(t.second);
        float speed = std::get<4>(t.second);

        glm::vec3 new_v = v;
        ImGui::DragFloat3(title.c_str(), &new_v.x, speed, min, max);
        if (new_v != v) {
            ImGui3FloatDragChangedEvent e(new_v);
            callback(e);
        }
        drags_3floats[title] = std::make_tuple(callback, new_v, min, max, speed);
    }

    for (auto t : drags_2floats) {
        std::string title = t.first;
        EventCallbackFn& callback = std::get<0>(t.second);
        glm::vec2& v = std::get<1>(t.second);
        float min = std::get<2>(t.second);
        float max = std::get<3>(t.second);
        float speed = std::get<4>(t.second);

        glm::vec2 new_v = v;
        ImGui::DragFloat2(title.c_str(), &new_v.x, speed, min, max);
        if (new_v != v) {
            ImGui2FloatDragChangedEvent e(new_v);
            callback(e);
        }
        drags_2floats[title] = std::make_tuple(callback, new_v, min, max, speed);
    }
    for (auto t : drags_1floats) {
        std::string title = t.first;
        EventCallbackFn& callback = std::get<0>(t.second);
        float& v = std::get<1>(t.second);
        float min = std::get<2>(t.second);
        float max = std::get<3>(t.second);
        float speed = std::get<4>(t.second);

        float new_v = v;
        ImGui::DragFloat(title.c_str(), &new_v, speed, min, max);
        if (new_v != v) {
            ImGui1FloatDragChangedEvent e(new_v);
            callback(e);
        }
        drags_1floats[title] = std::make_tuple(callback, new_v, min, max, speed);
    }
    for (auto p : buttons) {
        EventCallbackFn& callback = std::get<0>(p.second);
        float size_x = std::get<1>(p.second);
        float size_y = std::get<2>(p.second);
        bool clicked = ImGui::Button(p.first.c_str(), ImVec2(size_x, size_y));
        if (clicked) callback(ImGuiButtonClickedEvent());
    }
}

void ImGuiParams::AddText(std::string title, std::string value) {
	texts_order.push_back(title);
	texts[title] = value;
}

void ImGuiParams::AddFloatSlider(EventCallbackFn callback, std::string title, float def_v, float min, float max) {
	float_sliders[title] = std::make_tuple(callback, def_v, min, max);
}

void ImGuiParams::AddInteger(EventCallbackFn callback, std::string title, int def_v, int min, int max) {
	integer_sliders[title] = std::make_tuple(callback, def_v, min, max);
}

void ImGuiParams::Add3FloatSlider(EventCallbackFn callback, std::string title, glm::vec3 def_v, float min, float max) {
	float_3sliders[title] = std::make_tuple(callback, def_v, min, max);
}

void ImGuiParams::AddColorPicker(EventCallbackFn callback, std::string title, glm::vec3 def_v) {
	color_pickers[title] = std::make_tuple(callback, def_v);
}

void ImGuiParams::Add3FloatsDrag(EventCallbackFn callback, std::string title, glm::vec3 def_v, float min, float max, float speed) {
    this->drags_3floats[title] = std::make_tuple(callback, def_v, min, max, speed);
}

void ImGuiParams::Add2FloatsDrag(EventCallbackFn callback, std::string title, glm::vec2 def_v, float min, float max, float speed) {
    this->drags_2floats[title] = std::make_tuple(callback, def_v, min, max, speed);
}

void ImGuiParams::Add1FloatsDrag(EventCallbackFn callback, std::string title, float def_v, float min, float max, float speed) {
    this->drags_1floats[title] = std::make_tuple(callback, def_v, min, max, speed);
}

void ImGuiParams::AddButton(EventCallbackFn callback, std::string title, float size_x, float size_y) {
    this->buttons[title] = std::make_tuple(callback, size_x, size_y);
}