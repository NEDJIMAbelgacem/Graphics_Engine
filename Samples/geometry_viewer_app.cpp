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
#include "Layers/KeyBoardControlLayer.h"
#include "Layers/SceneViewControlLayer.h"
#include "Layers/ImGuiController.h"
#include "Light.h"
#include "Systems/LightingManager.h"
#include "Geometry/LightAnchor.h"

#include "Components/ShaderParameters.h"
#include "Uniforms/Uniforms.h"

const std::string skybox_shader_path = "Shaders/skybox.glsl";
const std::string geometry_objects_shader_path = "Shaders/geometry_objects.glsl";

class GeometryViewer : public Application {
private:
    Camera* camera;
    int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
    bool cursor_enabled = false;
    bool mouse_enabled = false;
    std::vector<SurfaceParameters*> surface_parametered;
    Sphere* sphere;
    Cube* cube;
    Sphere* light;
    Plane* plane;
    LightAnchor* light_anchor;
public:
	GeometryViewer() {
        camera = new Camera();
        camera->SetAngles(0.0f, -45.0f);
        camera->SetPosition(glm::vec3(100.0f, 100.0f, 100.0f));
        // this->GetWindow().SetVSync(false);

        ShaderProgram* sky_box_shader = new ShaderProgram(skybox_shader_path);
        ShaderProgram* cube_shader = new ShaderProgram(geometry_objects_shader_path);
        ShaderProgram* plane_shader = new ShaderProgram(geometry_objects_shader_path);
        ShaderProgram* sphere_shader = new ShaderProgram(geometry_objects_shader_path);
        ShaderProgram* light_shader = new ShaderProgram(geometry_objects_shader_path);

        ImGuiController* imgui_ctl = new ImGuiController("ImGui Controller");
        CameraLayer* camera_layer = new CameraLayer(camera);
        RenderingLayer* rendering_layer = new RenderingLayer(camera);
        KeyBoardControlLayer* keyboard_layer = new KeyBoardControlLayer;
        SceneViewControlLayer* mouse_control_layer = new SceneViewControlLayer(*camera);

        auto func = [&](MouseMovedEvent& e) -> bool {
            N3D_LOG_INFO("mouse mouved called with {}", 1);
            return false;
        };
        auto func2 = [&](MouseButtonPressedEvent& e) -> bool {
            N3D_LOG_INFO("mouse button pressed called with {} {}", e.GetX(), e.GetY());
            return false;
        };
        // mouse_control_layer->AddMouseMovedAction(func);
        // mouse_control_layer->AddMouseButtonPressedAction(func2);

        sphere = new Sphere(sphere_shader, glm::vec3(-150.0f, 100.0f, 0.0f), 100.0f);
        cube = new Cube(cube_shader, glm::vec3(50.0f, 101.0f, 0.0f), glm::vec3(100.0f));
        plane = new Plane(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec3(0.0f), 30.0f, plane_shader);

        SkyBox* skybox = new SkyBox("mp_drakeq", "drakeq", sky_box_shader);
        skybox->Setup(*camera);

        CameraHook* camera_hook = new CameraHook(*camera);
        skybox->AddComponent(camera_hook);
        rendering_layer->SetSkyBox(*skybox);

        ShaderParameters* shader_param = new ShaderParameters;
        FloatUniform* f_uniform = new FloatUniform;
        f_uniform->val = 1.0f;
        shader_param->AddValue("u_test_param", *f_uniform);

        sphere->AddComponent(camera_hook);
        cube->AddComponent(camera_hook);
        plane->AddComponent(camera_hook);

        sphere->AddComponent(shader_param);
        cube->AddComponent(shader_param);
        plane->AddComponent(shader_param);

        rendering_layer->AddSphere(*sphere);
        rendering_layer->AddCube(*cube);
        rendering_layer->AddPlane(*plane);

        Light* point_light = new Light;
        point_light->type =  LightType::POINT_LIGHT;
        point_light->position = glm::vec3(0.0f, 200.0f, 0.0f);
        point_light->color = glm::vec3(1.0f);
        LightingManager::AddLight(*point_light);
        light_anchor = new LightAnchor(light_shader, point_light, 20.0f);
        light_anchor->AddComponent(camera_hook);
        rendering_layer->AddSphere(*light_anchor);
        
        imgui_ctl->AddLightController(*point_light, "light parameters");
        imgui_ctl->AddComponentControl(*cube->GetTransformComponent(), "cube transform");
        imgui_ctl->AddComponentControl(*sphere->GetTransformComponent(), "sphere transform");
        imgui_ctl->AddComponentControl(*plane->GetTransformComponent(), "plane transform");
        imgui_ctl->AddComponentControl(*camera_hook, "camera parameters");
        imgui_ctl->AddComponentControl(*shader_param, "shader parameters");
        // PushOverlay(mouse_control_layer);
        PushLayer(imgui_ctl);
        PushLayer(keyboard_layer);
		PushLayer(camera_layer);
        
        PushLayer(rendering_layer);
	}

	~GeometryViewer() {
        delete camera;
    }
};

Application* CreateApplication() {
    return new GeometryViewer();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();

	app->Run();
	delete app;
}