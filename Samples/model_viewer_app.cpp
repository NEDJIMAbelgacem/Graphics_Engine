#include "Core/Common.h"
#include "Window/Application.h"
#include "Logger.h"
#include "Window/Input.h"
#include "Tests/ImGuiParams.h"

#include "Core/ShaderProgram.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "Tests/DebugTest.h"
#include "SkyBox.h"
#include "Model.h"

const std::string skybox_shader_path = "Shaders/skybox.glsl";
const std::string model_shader_path = "Shaders/model.glsl";
const std::string model_path = "Resources/nanosuit/nanosuit.obj";

class TerrainViewerLayer : public ImGuiParams {
public:
    Camera* camera;
    ShaderProgram* sky_box_shader;
    ShaderProgram* model_shader;
    SkyBox* skybox;
    Model* model;

    float camera_speed = 3000.0f;
    float reflectivity = 0.5f;
    float shineDamper = 0.5;
    float diffuseFactor = 0.5;
    glm::vec3 light_color = glm::vec3(1.0f);
    glm::vec3 light_dir = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 proj;

    bool mouse_enabled = false;
    bool cursor_enabled = false;
public:
	TerrainViewerLayer(Camera* camera_ptr) : ImGuiParams("Terrain parameters") {
        this->AddFloatSlider("camera speed", &camera_speed, 0.0f, 10000.0f);
        this->AddFloatSlider("model reflectivity", &reflectivity);
        this->AddFloatSlider("model shineDamper", &shineDamper);
        this->AddFloatSlider("model diffuseFactor", &diffuseFactor);
        this->Add3FloatSlider("light color", &light_color, 0.0f, 1.0f);
        this->Add3FloatSlider("light direction", &light_dir, -1.0f, 1.0f);

        float near_plane = 0.1f, far_plane = 4000.0f;
        float field_of_view = glm::radians(90.0f);
        float aspect_ratio = (float)800.0f / (float)600.0f;
        proj = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);

        camera = camera_ptr;

        sky_box_shader = new ShaderProgram(skybox_shader_path);
        model_shader = new ShaderProgram(model_shader_path);
        model_shader->FillUniformMat4f("u_proj", proj);

        model = new Model(model_path, model_shader);
        glm::mat4 object_model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(100, 100, 100));//glm::translate(glm::mat4(100.0f), glm::vec3(100.0f));
        model->SetModelMatrix(object_model_matrix);

        skybox = new SkyBox("mp_drakeq", "drakeq", sky_box_shader);
        sky_box_shader->FillUniform1f("u_near", near_plane);
        sky_box_shader->FillUniform1f("u_far", far_plane);
    }

	void OnUpdate() override {        
        glm::mat4 view_matrix = camera->getViewMatrix();
        model->SetSurfaceParameters(reflectivity, shineDamper, diffuseFactor);
        model->SetLightColor(light_color);
        model->SetLightDirection(light_dir);
        model_shader->FillUniformMat4f("u_view", view_matrix);
        
        model->Render();
        skybox->Draw(view_matrix, proj);
	}

    bool OnMouseMoved(MouseMovedEvent& e) {
        if (mouse_enabled) camera->ProcessMouseMove(e.GetX(), e.GetY());
        return true;
    }

    bool OnKeyPressed(KeyPressedEvent& e) {
        switch (e.GetKeyCode()) {
            case GLFW_KEY_F1:
                mouse_enabled = !mouse_enabled;
                break;
            case GLFW_KEY_LEFT_CONTROL:
            case GLFW_KEY_RIGHT_CONTROL: {
                GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
                if (cursor_enabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursor_enabled = !cursor_enabled;
                break;
            }
            case GLFW_KEY_UP:
            case GLFW_KEY_W:
                camera->ProcessKeyboard(GLFW_KEY_UP, Application::delta_time);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                camera->ProcessKeyboard(GLFW_KEY_DOWN, Application::delta_time);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                camera->ProcessKeyboard(GLFW_KEY_RIGHT, Application::delta_time);
                break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A:
                camera->ProcessKeyboard(GLFW_KEY_LEFT, Application::delta_time);
                break;
        }
        return true;
    }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(TerrainViewerLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(TerrainViewerLayer::OnMouseMoved));
	}
};

class ModelViewer : public Application {
private:
    Camera* camera;
    int width = 800;
    int height = 600;
    bool cursor_enabled = false;
    bool mouse_enabled = false;
public:
	ModelViewer() {
        camera = new Camera();
        camera->SetAngles(0.0f, 0.0f);
        camera->SetPosition(glm::vec3(0.0f, 1000.0f, 1000.0f));
		PushLayer(new TerrainViewerLayer(camera));
	}

	~ModelViewer() { delete camera; }
};

Application* CreateApplication() {
    return new ModelViewer();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");
	N3D_LOG_TRACE("Hello! Var={0}");

	auto app = CreateApplication();

	app->Run();
	delete app;
}