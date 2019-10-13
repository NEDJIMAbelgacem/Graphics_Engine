#pragma once
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include "Core/ShaderProgram.h"
#include "Camera.h"
#include "Tests/DebugTest.h"
#include "SkyBox.h"
#include "Model.h"
#include "Geometry/Sphere.h"
#include "Geometry/Cube.h"
#include "Geometry/Plane.h"
#include "Logger.h"
#include "Events/Event.h"
#include "Layers/RenderingLayer.h"
#include "Window/Application.h"
#include "Layers/CameraLayer.h"
#include "Layers/ImGuiController.h"
#include "Layers/KeboardControlLayer.h"
#include "Light.h"
#include "Systems/LightingManager.h"
#include "Geometry/LightAnchor.h"

#include "Components/ShaderParameters.h"
#include "Uniforms/Uniforms.h"
#include "Terrain.h"

const std::string skybox_shader_path = "Shaders/skybox.glsl";
const std::string terrain_shader_path = "Shaders/terrain.glsl";
const std::string model_shader_path = "Shaders/model.glsl";
const std::string model_path = "Resources/LowPolyTree/lowpolytree.obj";
const std::string terrain_texture1_path = "Resources/textures/red_dirt_mud_01_diff_1k.jpg";
const std::string terrain_texture2_path = "Resources/textures/grassy2.png";
const std::string alphabet_atlas_models_folder_path = "Resources/alphabet/Letters/";
const std::string alphabets_shader_path = "Shaders/alphabet.glsl";

class TerrainViewer : public Application {
private:
    Camera* camera;
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    bool cursor_enabled = false;
    bool mouse_enabled = false;
public:
	TerrainViewer() {
        camera = new Camera;
        camera->SetAngles(0.0f, 0.0f);
        camera->SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));
        
        RenderingLayer* rendering_layer = new RenderingLayer(camera);
        CameraLayer* camera_layer = new CameraLayer(camera);
        ImGuiController* imgui_ctl = new ImGuiController("ImGui Controller");
        KeyBoardControlLayer* keyboard_layer = new KeyBoardControlLayer();

        auto func = []() {
            std::cout << "called" << std::endl;
        };
        keyboard_layer->AddAction(GLFW_KEY_LEFT_SHIFT, func);

        ShaderProgram* sky_box_shader = new ShaderProgram(skybox_shader_path);
        // Model* model = new Model(model_path, model_shader);

        CameraHook* camera_hook = new CameraHook(*camera);
        Lighting* lighting = new Lighting(LightingType::DirectionalLight, glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f));
        Surface* surface = new Surface(0.5f, 0.5f, 0.5f);

        imgui_ctl->AddComponentControl(*camera_hook, "camera parameters");
        imgui_ctl->AddComponentControl(*lighting, "lighting parameters");
        imgui_ctl->AddComponentControl(*surface, "model surface parameters");

        std::vector<std::string> terrain_textures;
        terrain_textures.push_back(terrain_texture1_path);
        terrain_textures.push_back(terrain_texture2_path);
        ShaderProgram* terrain_shader = new ShaderProgram(terrain_shader_path);
        
        glm::vec3 terrain_size(1000.0f, 300.0f, 1000.0f);
        glm::vec3 terrain_pos(0.0f);
        int vertices_per_tile = 10;
        int tiles_per_side = 20;
        Terrain* terrain = new Terrain(terrain_textures, terrain_shader, terrain_pos, terrain_size, tiles_per_side, vertices_per_tile);
        Transform* model_transform = new Transform(glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
        terrain->AddComponent(model_transform);
        terrain->AddComponent(camera_hook);
        terrain->AddComponent(lighting);
        terrain->AddComponent(model_transform);

        SkyBox* skybox = new SkyBox("mp_drakeq", "drakeq", sky_box_shader);
        skybox->Setup(*camera);
        skybox->AddComponent(new CameraHook(*camera));

        rendering_layer->AddGameObject(*terrain);
        rendering_layer->SetSkyBox(*skybox);

        PushLayer(imgui_ctl);
		PushLayer(camera_layer);
        PushLayer(keyboard_layer);
        PushLayer(rendering_layer);
	}

	~TerrainViewer() { 
        delete camera;
    }
};


Application* CreateApplication() {
    return new TerrainViewer();
}


int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();
    
	app->Run();
	delete app;
}