#include "Layers/ImGuiController.h"
#include <ctime>
#include "stb_image_write.h"
#include "Window/Application.h"

ImGuiController::ImGuiController(std::string title) : Layer(title) { }
ImGuiController::~ImGuiController() { }

void ImGuiController::OnImGuiRender() {
	component_index = 0;
    ImGui::Begin(this->debug_name.c_str());
    for (auto& c : this->transform_controls) UpdateControl(c.first, c.second);
    for (auto& c : this->surface_controls) UpdateControl(c.first, c.second);
    for (auto& c : this->lighting_controls) UpdateControl(c.first, c.second);
    for (auto& c : this->camera_hook_controls) UpdateControl(c.first, c.second);
    for (auto& c : this->shader_params_controls) {
        UpdateControl(c.first, c.second);
    }
    for (auto& c : this->light_controls) UpdateControl(c.first, c.second);
    static double time1 = 0.0f, time2 = 0.0f;
    static double avg_delta = 1.0f / 60.0f;
    static int counter = 0;
    const int average_window = 60;
    time1 = time2;
    time2 = ImGui::GetTime();
    static double times_sum = 0.0f;
    if (this->display_frame_rate) {
        ++counter;
        times_sum += time2 - time1;
        if (counter == average_window) {
            avg_delta = times_sum / average_window;
            times_sum = 0.0f;
            counter = 0;
        }
        // if (counter % 100 == 0) frame_rate_mod = Application::GetFrameRate();
        ImGui::Text("frame rate %.1f", 1.0f / avg_delta);
    }
    ImGui::End();
}

void ImGuiController::UpdateControl(Transform* transform, std::string title) {
    if (ImGui::CollapsingHeader(title.c_str())) {
        ImGui::PushID(component_index);
        ImGui::DragFloat3("position##", &transform->position[0]);
        ImGui::DragFloat3("scale##", &transform->scale[0]);

        glm::vec3 rotation = transform->rotation;
        for (int i = 0; i < 3; ++i) rotation[i] = glm::degrees(transform->rotation[i]);
        ImGui::SliderFloat3("rotation##", &rotation[0], -180.0f, 180.0);
        for (int i = 0; i < 3; ++i) rotation[i] = glm::radians(rotation[i]);
        transform->rotation = rotation;

        ImGui::PopID();
        component_index++;
    }
}

void ImGuiController::UpdateControl(Surface* surface, std::string title) {
    if (ImGui::CollapsingHeader(title.c_str())) {
        ImGui::PushID(component_index);
        ImGui::SliderFloat("reflectivity##", &surface->reflectivity, 0.0f, 1.0f);
        ImGui::SliderFloat("diffuse factor##", &surface->diffuse_factor, 0.0f, 1.0f);
        ImGui::SliderFloat("shine damper##", &surface->shine_damper, 0.0f, 1.0f);
        ImGui::PopID();
        component_index++;
    }
}

void ImGuiController::UpdateControl(Lighting* lighting, std::string title) {
    if (ImGui::CollapsingHeader(title.c_str())) {
        ImGui::PushID(component_index);
        ImGui::ColorEdit3("light color##", &lighting->color[0]);
        ImGui::PopID();
        component_index++;
    }
}

void ImGuiController::UpdateControl(CameraHook* camera_hook, std::string title) {
    if (ImGui::CollapsingHeader(title.c_str())) {
        Camera* camera = camera_hook->camera;
        ImGui::PushID(component_index);
        glm::vec3 camera_pos = camera->GetCameraPosition();
        float camera_speed = camera->GetCameraSpeed();
        float mouse_sensivity = camera->GetMouseSensivity();
        float far_plane = camera->GetFarPlane();
        float near_plane = camera->GetNearPlane();
        float pitch = camera->GetPitch();
        float yaw = camera->GetYaw();

        ImGui::InputFloat("move speed##", &camera_speed, 1.0f, 10.0f, 1);
        ImGui::InputFloat("mouse sensivity##", &mouse_sensivity, 5.0f, 20.0f);
        ImGui::InputFloat3("position##", &camera_pos[0], 1);
        ImGui::InputFloat("near plane##", &near_plane);
        ImGui::InputFloat("far plane##", &far_plane);
        ImGui::SliderFloat("pitch##", &pitch, -180.0f, 180.0f);
        ImGui::SliderFloat("yaw##", &yaw, -180.0f, 180.0f);

        ImGui::PopID();
        component_index++;

        camera->SetCameraSpeed(camera_speed);
        camera->SetMouseSensivity(mouse_sensivity);
        camera->SetPosition(camera_pos);
        camera->SetNearFarPlanes(near_plane, far_plane);
        camera->SetAngles(pitch, yaw);
    }
}

void ImGuiController::UpdateControl(Light* light, std::string title) {
    const char light_types_arr[] = "directional light\0point light\0";
    if (ImGui::CollapsingHeader(title.c_str())) {
        ImGui::PushID(component_index);
        int light_type = light->type;
        ImGui::Combo("light type##", &light_type, light_types_arr);
        light->type = static_cast<LightType>(light_type);
        ImGui::ColorEdit3("light color##", &light->color[0]);
        glm::vec3 light_position = light->position;
        switch (light->type) {
        case DIRECTIONAL_LIGHT:
            ImGui::DragFloat3("light direction##", &light_position[0], 0.05f);
            break;
        case POINT_LIGHT:
            ImGui::DragFloat3("light position##", &light_position[0]);
            break;
        }
        light->position = light_position;
        ImGui::PopID();
        component_index++;
    }
}

void ImGuiController::UpdateControl(ShaderParameters* shader_params, std::string title) {
    if (ImGui::CollapsingHeader(title.c_str())) {
        std::unordered_map<std::string, UniformValue*>& uniforms_map = shader_params->GetUniformsMap();
        for (auto& p : uniforms_map) {
            std::string u_name = p.first;
            UniformValue& u_value = *p.second;
            ImGui::PushID(component_index);
            switch (u_value.GetType()) {
            case UniformTypes::Float3 : {
                    Vec3Uniform& u_vec3 = *(Vec3Uniform*)&u_value;
                    glm::vec3 val = u_vec3.val;
                    ImGui::DragFloat3("3 floats ##", &val[0]);
                    u_vec3.val = val;
                }
                break;
            case UniformTypes::Float1 : {
                    FloatUniform& u_float = *(FloatUniform*)&u_value;
                    float val = u_float.val;
                    // TODO : make float drag speed customizable
                    if (p.second->GetLabel().empty()) ImGui::DragFloat("1 float ##", &val, 0.01f);
                    else  ImGui::DragFloat((p.second->GetLabel() + " ##").c_str(), &val, 0.01f);
                    u_float.val = val;
                }
                break;
            default:
                break;
            }
            ImGui::PopID();
            component_index++;
        }
    }
}

void ImGuiController::AddComponentControl(Transform& transform, std::string title) {
    this->transform_controls.push_back(std::make_pair(&transform, title));
}

void ImGuiController::AddComponentControl(Surface& surface, std::string title) {
    this->surface_controls.push_back(std::make_pair(&surface, title));
}

void ImGuiController::AddComponentControl(Lighting& lighting, std::string title) {
    this->lighting_controls.push_back(std::make_pair(&lighting, title));
}

void ImGuiController::AddComponentControl(CameraHook& camera_hook, std::string title) {
    this->camera_hook_controls.push_back(std::make_pair(&camera_hook, title));
}

void ImGuiController::AddLightController(Light& light, std::string title) {
    this->light_controls.push_back(std::make_pair(&light, title));
}

void ImGuiController::AddComponentControl(ShaderParameters& shader_params, std::string title) {
    this->shader_params_controls.push_back(std::make_pair(&shader_params, title));
}