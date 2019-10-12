#pragma once
#include "Layers/ImGuiParams.h"

class ModelParams : public ImGuiParams {
public:
    Model* model;

    bool mouse_enabled = false;
    bool cursor_enabled = false;
    float mouse_x = -1, mouse_y = -1;

    bool enabled = true;
public:
	ModelParams(Model* model) : ImGuiParams("Model parameters") {
        this->model = model;
        
        auto reflectivity_callback = [this](Event& event) {
            ImGui1FloatSliderChangedEvent e = *(ImGui1FloatSliderChangedEvent*)&event;
            //this->model->SetReflectivity(e.GetValue());
            N3D_LOG_INFO("reflectivity_callback");
        };
        auto shine_damper_callback = [this](Event& event) {
            ImGui1FloatSliderChangedEvent e = *(ImGui1FloatSliderChangedEvent*)&event;
            //this->model->SetShineDamper(e.GetValue());
            N3D_LOG_INFO("shine_damper_callback");
        };
        auto diffuse_factor_callback = [this](Event& event) {
            ImGui1FloatSliderChangedEvent e = *(ImGui1FloatSliderChangedEvent*)&event;
            //this->model->SetDiffuseFator(e.GetValue());
            N3D_LOG_INFO("diffuse_factor_callback");
        };
        auto light_color_callback = [this](Event& event) {
            ImGuiColorPickerChangedEvent e = *(ImGuiColorPickerChangedEvent*)&event;
            //this->model->SetLightColor(e.GetValue());
            N3D_LOG_INFO("light_color_callback");
        };
        auto light_direction_callback = [this](Event& event) {
            ImGui3FloatSliderChangedEvent e = *(ImGui3FloatSliderChangedEvent*)&event;
            //this->model->SetLightDirection(e.GetValue());
            N3D_LOG_INFO("light_direction_callback");
        };
        
        this->AddFloatSlider(reflectivity_callback, "model reflectivity", 0.5f);
        this->AddFloatSlider(shine_damper_callback, "model shineDamper", 0.5f);
        this->AddFloatSlider(diffuse_factor_callback, "model diffuseFactor", 0.5f);
        
        this->Add3FloatSlider(light_color_callback, "light color", glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f);
        this->Add3FloatSlider(light_direction_callback, "light direction", glm::vec3(0.0f, 1.0f, 0.0f), -1.0f, 1.0f);
    }
};