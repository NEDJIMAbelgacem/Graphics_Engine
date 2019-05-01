// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Core/ShaderProgram.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "Tests/DebugTest.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "AlphabetAtlas.h"

const int mWidth = 800, mHeight = 600;

void InitImGui(GLFWwindow* window, const char* glsl_version);

void DestroyImGui();

const std::string skybox_shader_path = "Shaders/skybox.shader";
const std::string terrain_shader_path = "Shaders/terrain.shader";
const std::string model_shader_path = "Shaders/model.shader";
const std::string model_path = "Resources/LowPolyTree/lowpolytree.obj";
const std::string terrain_texture1_path = "Resources/textures/red_dirt_mud_01_diff_1k.jpg";
const std::string terrain_texture2_path = "Resources/textures/grassy2.png";
const std::string alphabet_atlas_models_folder_path = "Resources/alphabet/Letters/";
const std::string alphabets_shader_path = "Shaders/alphabet.glsl";

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
		window = glfwCreateWindow(mWidth, mHeight, "model viewer", NULL, NULL);
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
        InitImGui(window, "#version 430");
        
        glCall(glEnable(GL_MULTISAMPLE));
        //glCall(glEnable(GL_CULL_FACE));
        //glCall(glCullFace((GL_BACK)));
	    glCall(glEnable(GL_BLEND));
	    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	    glCall(glEnable(GL_DEPTH_TEST));
        //glCall(glDepthFunc(GL_ALWAYS));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        int q;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES, &q);
        std::cout << "geometry maximum output vertices : " << q << std::endl;
        //glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &q);

        camera_ptr = new Camera();
        Camera& camera = *camera_ptr;
        camera.SetAngles(0.0f, 0.0f);
        camera.SetPosition(glm::vec3(0.0f, 10.0f, 10.0f));

        // imgui passed params
        float camera_speed = 100.0f;
        float reflectivity = 0.5f;
        float shineDamper = 0.5;
        float diffuseFactor = 0.5;
        glm::vec3 light_color(1.0f);
        glm::vec3 light_dir(0.0f, 1.0f, 0.0f);
        glm::vec2 model_pos(0.0f);
        float model_rotation = 0.0f;

        DebugTest test;
        test.AddFloatSlider("camera speed", &camera_speed, 0.0f, 10000.0f);
        test.AddFloatSlider("model reflectivity", &reflectivity);
        test.AddFloatSlider("model shineDamper", &shineDamper);
        test.AddFloatSlider("model diffuseFactor", &diffuseFactor);
        test.Add3FloatSlider("light color", &light_color, 0.0f, 1.0f);
        test.Add3FloatSlider("light direction", &light_dir, -1.0f, 1.0f);
        test.Add2FloatsDrag("model position", &model_pos, -500.0f, 500.0f, 1.0f);
        test.Add1FloatsDrag("model rotation", &model_rotation, -180.0f, 180.0f, 1.0f);

        float near_plane = 0.1f, far_plane = 8000.0f;
        float field_of_view = glm::radians(90.0f);
        float aspect_ratio = (float)mWidth / (float)mHeight;

        glm::mat4 proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
        ShaderProgram sky_box_shader(skybox_shader_path);

        SkyBox sky_box("mp_drakeq", "drakeq", &sky_box_shader);
        sky_box_shader.FillUniform1f("u_near", near_plane);
        sky_box_shader.FillUniform1f("u_far", far_plane);

        std::vector<std::string> terrain_textures;
        terrain_textures.push_back(terrain_texture2_path);
        terrain_textures.push_back(terrain_texture1_path);
        ShaderProgram terrain_shader(terrain_shader_path);
        terrain_shader.FillUniformMat4f("u_proj", proj);
        glm::vec3 terrain_size(1000.0f, 100.0f, 1000.0f);
        glm::vec3 terrain_pos(0.0f);
        int vertices_per_tile = 10;
        int tiles_per_side = 20;
        Terrain terrain(terrain_textures, &terrain_shader, terrain_pos, terrain_size, tiles_per_side, vertices_per_tile);
        terrain.SetLightColor(glm::vec3(1.0f));
        terrain.SetLightPosition(glm::vec3(0, 10000.0f, 0.0f));

        ShaderProgram model_shader = ShaderProgram(model_shader_path);
        Model model_object(model_path, &model_shader);
        model_shader.FillUniformMat4f("u_proj", proj);

        ShaderProgram alphabet_shader(alphabets_shader_path);
        alphabet_shader.FillUniformMat4f("u_proj", proj);
        AlphabetAtlas alphabet_atlas(alphabet_atlas_models_folder_path, &alphabet_shader);
        
        // Rendering Loop
        double time1 = 0.0f;
        double time2 = 0.0f;
        while (!glfwWindowShouldClose(window)) {
            time1 = time2;
            time2 = ImGui::GetTime();
            delta = (float)(time2 - time1);
            camera.SetCameraSpeed(camera_speed);
            // Background Fill Color
            //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // drawing
            proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
            glm::mat4 view_matrix = camera.getViewMatrix();

            //glDepthFunc(GL_LEQUAL);
            //sky_box.Draw(view_matrix, proj);
            //glDepthFunc(GL_LESS);

            glm::mat4 rotation_m = glm::rotate(glm::identity<glm::mat4>(), glm::radians(model_rotation), glm::vec3(1.0f, 0.0f, 0.0f));
            float model_x = model_pos.x, model_z = model_pos.y;
            float model_y = terrain.GetHeight(model_x, model_z);
            glm::mat4 object_model_matrix = glm::identity<glm::mat4>();
            object_model_matrix = glm::scale(object_model_matrix, glm::vec3(1.0f));
            //object_model_matrix = rotation_m * object_model_matrix;
            object_model_matrix = glm::translate(object_model_matrix, glm::vec3(model_x, model_y, model_z));
            //object_model_matrix = glm::inverse(rotation_m) * object_model_matrix;
            model_object.SetModelMatrix(object_model_matrix);

            model_object.SetSurfaceParameters(reflectivity, shineDamper, diffuseFactor);
            model_object.SetLightColor(light_color);
            model_object.SetLightDirection(light_dir);
            model_shader.FillUniformMat4f("u_view", view_matrix);
            model_object.Render();

            terrain_shader.FillUniformMat4f("u_view", view_matrix);
            terrain.Render();

            alphabet_shader.FillUniformMat4f("u_view", view_matrix);
            alphabet_atlas.SetCameraPosition(camera.getCameraPosition());
            alphabet_atlas.SetSurfaceParameters(reflectivity, shineDamper, diffuseFactor);
            alphabet_atlas.Render(glm::vec3(-100.0f, 50.0f, -100.0f), "ABC", 100.0f);

            // imgui stuff
            glm::vec3 pos = camera.getCameraPosition();
            test.AddText("camera pos", std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z));
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