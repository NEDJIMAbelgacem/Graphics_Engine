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
#include "Layers/KeyBoardControlLayer.h"
#include "Layers/SceneViewControlLayer.h"
#include "Layers/ImGuiController.h"
#include "Light.h"
#include "Systems/LightingManager.h"
#include "Geometry/LightAnchor.h"

#include "Components/ShaderParameters.h"
#include "Uniforms/Uniforms.h"

#include "3D/Scene_3D.h"
#include "3D/3D_Objects.h"

using namespace N3D;

std::string png_path = "Resources/2k_earth_daymap.jpg";

class SceneApp : public Application {
private:
	Scene_3D* scene = nullptr;
    N3D::TexturedIcosphere* sphere = nullptr;
	N3D::ColorfulIcosphere* globe = nullptr;
public:
	SceneApp() {
        scene = new Scene_3D(*this, glm::vec3(0.2f));
        // sphere = new N3D::Icosphere(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		globe = new N3D::ColorfulIcosphere(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        // scene->AddObject(*sphere);
		scene->AddObject(*globe);
	}

	~SceneApp() {
    }

	bool OnWindowResize(WindowResizeEvent& e) override { 
		// TODO : find more suitable place for this opengl call
		glCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
		scene->SetSize((float)e.GetWidth(), (float)e.GetHeight());
		return true;
	}
};

Application* CreateApplication() {
    return new SceneApp();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();

	app->Run();
	delete app;
}