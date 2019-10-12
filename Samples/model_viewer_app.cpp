#pragma once
#include "Core/Common.h"
#include "Window/Application.h"
#include "Logger.h"
#include "Window/Input.h"
#include "Layers/ImGuiActivationLayer.h"
#include "Layers/ImGuiController.h"

#include "Core/ShaderProgram.h"
#include "glm/glm.hpp"
#include "SkyBox.h"
#include "Model.h"
#include "Layers/CameraLayer.h"
#include "Layers/ModelParams.h"
#include "Systems/RenderingManager.h"
#include "Camera.h"

#include "Components/Lighting.h"
#include "Components/Surface.h"
#include "Components/Transform.h"

#include "Layers/RenderingLayer.h"

const std::string skybox_shader_path = "Shaders/skybox.glsl";
const std::string model_shader_path = "Shaders/model.glsl";
const std::string model_path = "Resources/nanosuit/nanosuit.obj";

class ModelViewer : public Application {
private:
    Camera* camera;
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    bool cursor_enabled = false;
    bool mouse_enabled = false;
public:
	ModelViewer() {
        camera = new Camera;
        camera->SetAngles(0.0f, 0.0f);
        camera->SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));
        
        RenderingLayer* rendering_layer = new RenderingLayer(camera);
        CameraLayer* camera_layer = new CameraLayer(camera);
        ImGuiController* imgui_ctl = new ImGuiController("ImGui Controller");

        ShaderProgram* sky_box_shader = new ShaderProgram(skybox_shader_path);
        ShaderProgram* model_shader = new ShaderProgram(model_shader_path);
        Model* model = new Model(model_path, model_shader);

        Transform* model_transform = new Transform(glm::vec3(100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
        CameraHook* camera_hook = new CameraHook(*camera);
        Lighting* lighting = new Lighting(LightingType::DirectionalLight, glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f));
        Surface* surface = new Surface(0.5f, 0.5f, 0.5f);

        imgui_ctl->AddComponentControl(*model_transform, "model transform");
        imgui_ctl->AddComponentControl(*camera_hook, "camera parameters");
        imgui_ctl->AddComponentControl(*lighting, "lighting parameters");
        imgui_ctl->AddComponentControl(*surface, "model surface parameters");
        
        model->AddComponent(model_transform);
        model->AddComponent(lighting);
        model->AddComponent(surface);
        model->AddComponent(camera_hook);
        rendering_layer->AddModel(*model);

        // std::vector<glm::vec3> positions;
        // int nb_instances = 4;
        // for (int i = 0; i < nb_instances; ++i)
        //     positions.push_back(glm::vec3(100 + i * 1000, 100, 100));
        // Batch* batch = new Batch(positions);
        // RenderingManager::RegisterModelBatch(*model, *batch);

        SkyBox* skybox = new SkyBox("mp_drakeq", "drakeq", sky_box_shader);
        skybox->Setup(*camera);
        skybox->AddComponent(new CameraHook(*camera));
        rendering_layer->SetSkyBox(*skybox);

        PushLayer(imgui_ctl);
		PushLayer(camera_layer);
        PushLayer(rendering_layer);
	}

	~ModelViewer() { 
        delete camera;
    }
};

Application* CreateApplication() {
    return new ModelViewer();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();
    
	app->Run();
	delete app;
}