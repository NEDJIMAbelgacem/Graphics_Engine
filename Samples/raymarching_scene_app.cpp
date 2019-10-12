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
#include "Light.h"
#include "Systems/LightingManager.h"
#include "Geometry/LightAnchor.h"
#include "RayMarcher.h"

#include "Components/ShaderParameters.h"
#include "Uniforms/Uniforms.h"

const std::string raymarcher_shader_path = "Shaders/mandelbox.glsl";
const std::string skybox_shader_path = "Shaders/skybox.glsl";
const std::string geometry_objects_shader_path = "Shaders/geometry_objects.glsl";

std::vector<SkyBox*> skyboxes;

class RaymarshingScene : public Application {
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
	RaymarshingScene() {
        camera = new Camera();
        camera->SetFieldOfView(glm::radians(90.0f));
        // camera->SetScreenSize(mWidth, mHeight);
        camera->SetFieldOfView(90.0f);
        // std::cout << camera->GetFieldOfView() << std::endl;
        camera->SetNearFarPlanes(0.1, 10000.0);
        camera->SetAngles(0.0f, 0.0f);
        camera->SetPosition(glm::vec3(-150.0f, 150.0f, 500.0f));
        // float field_of_view = glm::radians(90.0f);
        float near_plane = 0.1f, far_plane = 1000.0f;
        // float aspect_ratio = (float)mWidth / (float)mHeight;
        // glm::mat4 proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
        // this->GetWindow().SetVSync(false);
        

        ShaderProgram* skybox_shader = new ShaderProgram(skybox_shader_path);
        skyboxes.push_back(new SkyBox("ame_ash", "ashcanyon", skybox_shader));
        // skyboxes.push_back(new SkyBox("ame_emerald", "emeraldfog", skybox_shader));
        // skyboxes.push_back(new SkyBox("ame_flatrock", "flatrock", skybox_shader));
        // skyboxes.push_back(new SkyBox("ame_oasis", "oasisnight", skybox_shader));
        // skyboxes.push_back(new SkyBox("darkskies", "darkskies", skybox_shader));
        // skyboxes.push_back(new SkyBox("ely_darkcity", "darkcity", skybox_shader));
        // skyboxes.push_back(new SkyBox("hw_entropic", "entropic", skybox_shader));
        // skyboxes.push_back(new SkyBox("lmcity", "lmcity", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_bromene", "bromene-bay", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_classm", "classmplanet", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_deception", "deception_pass", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_deviltooth", "devils-tooth", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_drakeq", "drakeq", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_midnight", "midnight-silence", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_po", "po", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_totality", "totality", skybox_shader));
        // skyboxes.push_back(new SkyBox("mp_us", "urbansp", skybox_shader));
        SkyBox* selected_skybox = skyboxes[0];
        selected_skybox->Bind(4);
        selected_skybox->BindIrradiance(5);
        

        skybox_shader->FillUniform1f("u_near", near_plane);
        skybox_shader->FillUniform1f("u_far", far_plane);

        ShaderProgram* sky_box_shader = new ShaderProgram(skybox_shader_path);

        ImGuiController* imgui_ctl = new ImGuiController("ImGui Controller");
        CameraLayer* camera_layer = new CameraLayer(camera);
        RenderingLayer* rendering_layer = new RenderingLayer(camera);

        RayMarcher* ray_marcher = new RayMarcher(raymarcher_shader_path);
        Surface* surface = new Surface(0.5, 0.5, 0.5);
        ray_marcher->SetSkyBoxSlot(4);
        ray_marcher->SetIrradianceMapSlot(5);


        float metallicness = 0.5f;
        float roughness = 0.5f;
        glm::vec3 albedo_color(1.0f);
        glm::vec3 emmited_light(0.0f);
        int light_bounces = 3;
        // ray_marcher->SetViewMatrix(view_matrix);
        ray_marcher->SetMetallic(metallicness);
        ray_marcher->SetRoughness(roughness);

        ShaderParameters* shader_params = new ShaderParameters;

        FloatUniform* mettalicness_val = new FloatUniform;
        mettalicness_val->SetLabel("metallicness");
        FloatUniform* roughness_uniform = new FloatUniform;
        mettalicness_val->SetLabel("roughness");
        mettalicness_val->val = 0.5f;
        roughness_uniform->val = 0.5f;
        shader_params->AddValue("u_metallic", *mettalicness_val);
        shader_params->AddValue("u_roughness", *roughness_uniform);
        ray_marcher->AddComponent(shader_params);
        imgui_ctl->AddComponentControl(*shader_params, "shader parameters");

        // ray_marcher->SetEmmitedLight(emmited_light);
        // ray_marcher->SetReflectionBounces(light_bounces);
        // ray_marcher->SetSpheresAlbedo(albedo_color);
        // ray_marcher->SetCameraPos(camera.getCameraPosition());

        // SkyBox* skybox = new SkyBox("mp_drakeq", "drakeq", sky_box_shader);
        // skybox->Setup(*camera);

        CameraHook* camera_hook = new CameraHook(*camera);
        // skybox->AddComponent(camera_hook);
        ray_marcher->AddComponent(camera_hook);

        // rendering_layer->SetSkyBox(*skyboxes[0]);
        rendering_layer->AddGameObject(*ray_marcher);


        // Light* point_light = new Light;
        // point_light->type =  LightType::POINT_LIGHT;
        // point_light->position = glm::vec3(0.0f, 200.0f, 0.0f);
        // point_light->color = glm::vec3(1.0f);
        // LightingManager::AddLight(*point_light);
        // ShaderProgram* light_shader = new ShaderProgram(geometry_objects_shader_path);
        // light_anchor = new LightAnchor(light_shader, point_light, 20.0f);
        // light_anchor->AddComponent(camera_hook);
        // rendering_layer->AddSphere(*light_anchor);
        
        // imgui_ctl->AddLightController(*point_light, "light parameters");
        // imgui_ctl->AddComponentControl(*cube->GetTransformComponent(), "cube transform");
        // imgui_ctl->AddComponentControl(*sphere->GetTransformComponent(), "sphere transform");
        // imgui_ctl->AddComponentControl(*plane->GetTransformComponent(), "plane transform");
        imgui_ctl->AddComponentControl(*camera_hook, "camera parameters");
        // imgui_ctl->AddComponentControl(*shader_param, "shader parameters");

        PushLayer(imgui_ctl);
		PushLayer(camera_layer);
        PushLayer(rendering_layer);
	}

	~RaymarshingScene() {
        delete camera;
    }
};

Application* CreateApplication() {
    return new RaymarshingScene();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();

	app->Run();
	delete app;
}

// int choosed_skybox = 0;
// std::vector<SkyBox*> skyboxes;
// float delta = 0.0f;
// Camera* camera_ptr;
// GLFWwindow* window = nullptr;
// bool cursor_enabled = true;
// bool mouse_enabled = false;
// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
// int main() {
//     {
//         camera_ptr = new Camera();
//         Camera& camera = *camera_ptr;
//         float camera_speed = 3000.0f;

//         // scene parameters
//         float metallicness = 0.5f;
//         float roughness = 0.5f;
//         glm::vec3 albedo_color(1.0f);
//         glm::vec3 emmited_light(0.0f);
//         int light_bounces = 3;

//         DebugTest test;
//         test.AddFloatSlider("camera speed", &camera_speed, 0.0f, 10000.0f);   
//         test.AddColorPicker("sphere albedo", &albedo_color);
//         test.AddFloatSlider("matallicness", &metallicness);
//         test.AddFloatSlider("roughness", &roughness);
//         test.AddInteger("light bounces", &light_bounces, 1, 6);

//         camera.SetAngles(0.0f, 0.0f);
//         RayMarcher rt(raymarcher_shader_path);
//         rt.SetSkyBoxSlot(0);
//         float field_of_view = glm::radians(90.0f);
//         float near_plane = 0.1f, far_plane = 1000.0f;
//         float aspect_ratio = (float)mWidth / (float)mHeight;

//         glm::mat4 proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
//         glm::vec3 camera_pos = camera.getCameraPosition();
        
//         rt.SetNearFarPlanes(near_plane, far_plane);
//         rt.SetProjectionMatrix(proj);
//         rt.SetCameraPos(camera_pos);
//         rt.SetHDR(false);

//         glClearColor(0.25f, 1.0f, 0.25f, 1.0f);

//         ShaderProgram* skybox_shader = new ShaderProgram(skybox_shader_path);
//         skyboxes.push_back(new SkyBox("ame_ash", "ashcanyon", skybox_shader));
//         skyboxes.push_back(new SkyBox("ame_emerald", "emeraldfog", skybox_shader));
//         skyboxes.push_back(new SkyBox("ame_flatrock", "flatrock", skybox_shader));
//         skyboxes.push_back(new SkyBox("ame_oasis", "oasisnight", skybox_shader));
//         skyboxes.push_back(new SkyBox("darkskies", "darkskies", skybox_shader));
//         skyboxes.push_back(new SkyBox("ely_darkcity", "darkcity", skybox_shader));
//         /*skyboxes.push_back(new SkyBox("hw_entropic", "entropic", &skybox_shader));
//         skyboxes.push_back(new SkyBox("lmcity", "lmcity", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_bromene", "bromene-bay", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_classm", "classmplanet", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_deception", "deception_pass", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_deviltooth", "devils-tooth", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_drakeq", "drakeq", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_midnight", "midnight-silence", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_po", "po", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_totality", "totality", &skybox_shader));
//         skyboxes.push_back(new SkyBox("mp_us", "urbansp", &skybox_shader));*/
//         skyboxes[choosed_skybox]->Bind(0);
//         skyboxes[choosed_skybox]->BindIrradiance(1);
//         rt.SetSkyBoxSlot(0);
//         rt.SetIrradianceMapSlot(0);

//         skybox_shader.FillUniform1f("u_near", near_plane);
//         skybox_shader.FillUniform1f("u_far", far_plane);
        
//         // Rendering Loop
//         double time1 = 0.0f;
//         double time2 = 0.0f;
//         while (!glfwWindowShouldClose(window)) {
//             time1 = time2;
//             time2 = ImGui::GetTime();
//             delta = (float)(time2 - time1);
//             camera.SetCameraSpeed(camera_speed);

//             // Background Fill Color
//             glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//             glm::mat4 view_matrix = camera.getViewMatrix();

//             rt.SetViewMatrix(view_matrix);
//             rt.SetMetallic(metallicness);
//             rt.SetRoughness(roughness);
//             rt.SetEmmitedLight(emmited_light);
//             rt.SetReflectionBounces(light_bounces);
//             rt.SetSpheresAlbedo(albedo_color);
//             rt.SetCameraPos(camera.getCameraPosition());

//             rt.Render();
//             // imgui stuff
//             test.Display();
//             test.Render();

//             // Flip Buffers and Draw
//             glfwSwapBuffers(window);
//             glfwPollEvents();
//         }
//         DestroyImGui();
//     }
//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }