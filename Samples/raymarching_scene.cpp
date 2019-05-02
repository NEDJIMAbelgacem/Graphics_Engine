// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Core/Common.h"
#include "Core/ShaderProgram.h"
#include "RayMarcher.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "Tests/DebugTest.h"
#include "SkyBox.h"
#include <vector>

const int mWidth = 800, mHeight = 600;

void InitImGui(GLFWwindow* window, const char* glsl_version);

void DestroyImGui();

const std::string raymarcher_shader_path = "Shaders/raymarcher_scene1.glsl";
const std::string skybox_shader_path = "Shaders/skybox.glsl";

int choosed_skybox = 0;
std::vector<SkyBox*> skyboxes;
float delta = 0.0f;
Camera* camera_ptr;
GLFWwindow* window = nullptr;
bool cursor_enabled = true;
bool mouse_enabled = false;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
int main() {

    // initialization context
    {
		// initialise glfw
		if (!glfwInit()) return -1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		// Create a windowed mode window and its OpenGL context
		window = glfwCreateWindow(mWidth, mHeight, "pbr-ibl", NULL, NULL);
		if (!window) {
			glfwTerminate();
			return -1;
		}

		// Make the window's context current
		glfwMakeContextCurrent(window);
        gladLoadGL();
		//glfwSwapInterval(1);

		// Display opengl current version
		std::cout << "opengl version : " << glGetString(GL_VERSION) << std::endl;
	}

    {
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        InitImGui(window, "#version 330");

        camera_ptr = new Camera();
        Camera& camera = *camera_ptr;
        float camera_speed = 3000.0f;

        // scene parameters
        float metallicness = 0.5f;
        float roughness = 0.5f;
        glm::vec3 albedo_color(1.0f);
        glm::vec3 emmited_light(0.0f);
        int light_bounces = 3;

        DebugTest test;
        test.AddFloatSlider("camera speed", &camera_speed, 0.0f, 10000.0f);   
        test.AddColorPicker("sphere albedo", &albedo_color);
        test.AddFloatSlider("matallicness", &metallicness);
        test.AddFloatSlider("roughness", &roughness);
        test.AddInteger("light bounces", &light_bounces, 1, 6);

        camera.SetAngles(0.0f, 0.0f);
        RayMarcher rt(raymarcher_shader_path);
        rt.SetSkyBoxSlot(0);
        float field_of_view = glm::radians(90.0f);
        float near_plane = 0.1f, far_plane = 1000.0f;
        float aspect_ratio = (float)mWidth / (float)mHeight;

        glm::mat4 proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
        glm::vec3 camera_pos = camera.getCameraPosition();
        
        rt.SetNearFarPlanes(near_plane, far_plane);
        rt.SetProjectionMatrix(proj);
        rt.SetCameraPos(camera_pos);
        rt.SetHDR(false);

        glClearColor(0.25f, 1.0f, 0.25f, 1.0f);

        ShaderProgram skybox_shader(skybox_shader_path);
        skyboxes.push_back(new SkyBox("ame_ash", "ashcanyon", &skybox_shader));
        skyboxes.push_back(new SkyBox("ame_emerald", "emeraldfog", &skybox_shader));
        skyboxes.push_back(new SkyBox("ame_flatrock", "flatrock", &skybox_shader));
        skyboxes.push_back(new SkyBox("ame_oasis", "oasisnight", &skybox_shader));
        skyboxes.push_back(new SkyBox("darkskies", "darkskies", &skybox_shader));
        skyboxes.push_back(new SkyBox("ely_darkcity", "darkcity", &skybox_shader));
        /*skyboxes.push_back(new SkyBox("hw_entropic", "entropic", &skybox_shader));
        skyboxes.push_back(new SkyBox("lmcity", "lmcity", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_bromene", "bromene-bay", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_classm", "classmplanet", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_deception", "deception_pass", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_deviltooth", "devils-tooth", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_drakeq", "drakeq", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_midnight", "midnight-silence", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_po", "po", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_totality", "totality", &skybox_shader));
        skyboxes.push_back(new SkyBox("mp_us", "urbansp", &skybox_shader));*/
        skyboxes[choosed_skybox]->Bind(0);
        skyboxes[choosed_skybox]->BindIrradiance(1);
        rt.SetSkyBoxSlot(0);
        rt.SetIrradianceMapSlot(0);

        skybox_shader.FillUniform1f("u_near", near_plane);
        skybox_shader.FillUniform1f("u_far", far_plane);
        
        // Rendering Loop
        double time1 = 0.0f;
        double time2 = 0.0f;
        while (!glfwWindowShouldClose(window)) {
            time1 = time2;
            time2 = ImGui::GetTime();
            delta = (float)(time2 - time1);
            camera.SetCameraSpeed(camera_speed);

            // Background Fill Color
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glm::mat4 view_matrix = camera.getViewMatrix();

            rt.SetViewMatrix(view_matrix);
            rt.SetMetallic(metallicness);
            rt.SetRoughness(roughness);
            rt.SetEmmitedLight(emmited_light);
            rt.SetReflectionBounces(light_bounces);
            rt.SetSpheresAlbedo(albedo_color);
            rt.SetCameraPos(camera.getCameraPosition());

            rt.Render();
            // imgui stuff
            test.Display();
            test.Render();

            // Flip Buffers and Draw
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        DestroyImGui();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void InitImGui(GLFWwindow* window, const char* glsl_version) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void DestroyImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Camera& camera = *camera_ptr;
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_F1) mouse_enabled = !mouse_enabled;
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            if (cursor_enabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_enabled = !cursor_enabled;
        }
        if (key == GLFW_KEY_M) {
            skyboxes[choosed_skybox]->Unbind();
            choosed_skybox = (choosed_skybox + 1) % skyboxes.size();
            skyboxes[choosed_skybox]->Bind(0);
            skyboxes[choosed_skybox]->BindIrradiance(1);
        }
        if (key == GLFW_KEY_N) {
            skyboxes[choosed_skybox]->Unbind();
            choosed_skybox = (choosed_skybox + skyboxes.size() - 1) % skyboxes.size();
            skyboxes[choosed_skybox]->Bind(0);
            skyboxes[choosed_skybox]->BindIrradiance(1);
        }
    }
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_W || key == GLFW_KEY_W)
            camera.ProcessKeyboard(GLFW_KEY_UP, delta);
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
            camera.ProcessKeyboard(GLFW_KEY_DOWN, delta);
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
            camera.ProcessKeyboard(GLFW_KEY_RIGHT, delta);
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
            camera.ProcessKeyboard(GLFW_KEY_LEFT, delta);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouse_enabled) return;
    Camera& camera = *camera_ptr;
    camera.ProcessMouseMove((float)xpos, (float)ypos);
}