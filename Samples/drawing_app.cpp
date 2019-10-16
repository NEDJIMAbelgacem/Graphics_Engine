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

#include "2D/Scene_2D.h"
#include "2D/2D_Objects.h"

using namespace N3D;

std::string png_path = "Resources/textures/awesomeface.png";

class DrawingApp : public Application {
private:
	Scene_2D* scene = nullptr;
	N3D::Rectangle* rect = nullptr;
	Triangle* triangle = nullptr;
	Circle* circle = nullptr;
public:
	DrawingApp() {
        scene = new Scene_2D(*this, glm::vec3(0.2f));
		// rect = new N3D::Rectangle(glm::vec2(100.0f, 100.0f), 200, 200, glm::vec3(0, 0, 1));
		rect = new N3D::PixMap(glm::vec2(0.0f, 0.0f), 100.0f, 100.0f, png_path);

		scene->AddObject(*rect);
		triangle = new Triangle(
			glm::vec2(200.0f, 0.0f), glm::vec2(200.0f, 100.0f), glm::vec2(300.0f, 0.0f), 
			glm::vec3(0, 0, 1));

		circle = new Circle(glm::vec2(500.0f, 500.0f), 50.0f, glm::vec3(0, 0, 1));
		// scene->AddObject(*circle);

		Line* line = new Line(glm::vec2(600.0f, 600.0f), glm::vec2(700.0f, 700.0f), 5, glm::vec3(1.0f, 0.0f, 0.0f));
		// scene->AddObject(*line);
		// scene->AddObject(*triangle);

		std::vector<glm::vec2> points = {
			{100, 300}, {300, 100}, {500, 200}, {500, 400}, {300, 500}
		};
		N3D::Polygon* polygon = new N3D::Polygon(points, glm::vec3(0.0f, 0.5f, 0.0f));
		scene->AddObject(*polygon);
	}

	~DrawingApp() {
		delete scene;
		delete rect;
    }

	bool OnWindowResize(WindowResizeEvent& e) override { 
		// TODO : find more suitable place for this opengl call
		glCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
		scene->SetSize((float)e.GetWidth(), (float)e.GetHeight());
		return true;
	}
};

Application* CreateApplication() {
    return new DrawingApp();
}

int main(int argc, char** argv) {
	Logger::Init();
	N3D_LOG_TRACE("Initialized Log!");

	auto app = CreateApplication();

	app->Run();
	delete app;
}