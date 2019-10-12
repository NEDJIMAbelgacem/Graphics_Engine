#pragma once
#include <map>
#include <vector>
#include "Logger.h"
#include "Layers/Layer.h"
#include "Events/ImGuiEvents.h"

class ImGuiParams : public Layer {
    using EventCallbackFn = std::function<void(Event&)>;
private:
	std::map<std::string, std::string> texts;
	std::vector<std::string> texts_order;

	std::map<std::string, std::tuple<EventCallbackFn, float, float, float>> float_sliders;
	std::map<std::string, std::tuple<EventCallbackFn, glm::vec3, float, float>> float_3sliders;
	std::map<std::string, std::tuple<EventCallbackFn, int, int, int>> integer_sliders;
	std::map<std::string, std::tuple<EventCallbackFn, glm::vec3>> color_pickers;
    std::map<std::string, std::tuple<EventCallbackFn, glm::vec3, float, float, float>> drags_3floats;
    std::map<std::string, std::tuple<EventCallbackFn, glm::vec2, float, float, float>> drags_2floats;
    std::map<std::string, std::tuple<EventCallbackFn, float, float, float, float>> drags_1floats;
    std::map<std::string, std::tuple<EventCallbackFn, float, float>> buttons;
    bool enabled = true;
public:
	ImGuiParams(std::string title);
    ~ImGuiParams();

    inline bool IsEnabled() { return enabled; }
    inline void Enable() { enabled = true; }
    inline void Disable() { enabled = false; }

    void OnImGuiRender();

	void AddText(std::string title, std::string value);

	void AddFloatSlider(EventCallbackFn callback, std::string title, float def_v, float min = 0.0f, float max = 1.0f);
    void Add3FloatSlider(EventCallbackFn callback, std::string title, glm::vec3 def_v, float min = -1.0f, float max = 1.0f);
	void AddInteger(EventCallbackFn callback, std::string title, int def_v, int min, int max);
	void AddColorPicker(EventCallbackFn callback, std::string title, glm::vec3 def_v);
    void Add3FloatsDrag(EventCallbackFn callback, std::string title, glm::vec3 def_v, float min = 0.0f, float max = 1.0f, float speed = 0.01);
    void Add2FloatsDrag(EventCallbackFn callback, std::string title, glm::vec2 def_v, float min = 0.0f, float max = 1.0f, float speed = 0.01);
    void Add1FloatsDrag(EventCallbackFn callback, std::string title, float def_V, float min = 0.0f, float max = 1.0f, float speed = 0.01);

    void AddButton(EventCallbackFn callback, std::string title, float size_x, float size_y);
};