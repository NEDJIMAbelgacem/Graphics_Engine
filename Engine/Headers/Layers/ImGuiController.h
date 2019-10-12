#pragma once
#include <map>
#include <vector>
#include "Logger.h"
#include "Layers/Layer.h"
#include "Events/ImGuiEvents.h"
#include "Components/Transform.h"
#include "Components/Surface.h"
#include "Components/Lighting.h"
#include "Components/CameraHook.h"
#include "Components/ShaderParameters.h"
#include "Light.h"
#include "Uniforms/UniformTypes.h"
#include "Uniforms/UniformValue.h"
#include "Uniforms/Uniforms.h"

class ImGuiController : public Layer {
private:
    std::vector<std::pair<Transform*, std::string>> transform_controls;
    std::vector<std::pair<Surface*, std::string>> surface_controls;
    std::vector<std::pair<Lighting*, std::string>> lighting_controls;
    std::vector<std::pair<CameraHook*, std::string>> camera_hook_controls;
    std::vector<std::pair<Light*, std::string>> light_controls;
    std::vector<std::pair<ShaderParameters*, std::string>> shader_params_controls;

    bool display_frame_rate = true;
    bool enabled = true;

    int component_index;
private:
    void UpdateControl(Transform* transform, std::string title);
    void UpdateControl(Surface* surface, std::string title);
    void UpdateControl(Lighting* lighting, std::string title);
    void UpdateControl(CameraHook* camera_hook, std::string title);
    void UpdateControl(Light* light, std::string title);
    void UpdateControl(ShaderParameters* light, std::string title);
public:
	ImGuiController(std::string title);
    ~ImGuiController();

    inline bool IsEnabled() { return enabled; }
    inline void Enable() { enabled = true; }
    inline void Disable() { enabled = false; }

    void OnImGuiRender();

    void AddComponentControl(Transform& transform, std::string title);
    void AddComponentControl(Surface& surface, std::string title);
    void AddComponentControl(Lighting& lighting, std::string title);
    void AddComponentControl(CameraHook& camera_hook, std::string title);
    void AddComponentControl(ShaderParameters& shader_params, std::string title);
    void AddLightController(Light& light, std::string title);
};