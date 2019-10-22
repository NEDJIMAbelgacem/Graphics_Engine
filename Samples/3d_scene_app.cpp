#pragma once
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include "Core/ShaderProgram.h"
#include "Camera.h"
#include "Logger.h"
#include "Events/Event.h"
#include "Layers/RenderingLayer.h"
#include "Window/Application.h"
#include "Layers/CameraLayer.h"
#include "Systems/LightingManager.h"

#include "Components/ShaderParameters.h"

#include "3D/Scene_3D.h"
#include "3D/3D_Objects.h"

#include "ft2build.h"
#include FT_FREETYPE_H 

using namespace N3D;

std::string png_path = "Resources/2k_earth_daymap.jpg";

class SceneApp : public Application {
private:
	Scene_3D* scene = nullptr;
    N3D::TexturedIcosphere* sphere = nullptr;
	N3D::ColorfulIcosphere* globe = nullptr;
	int selected_skybox = 0;
	std::vector<N3D::SkyBox*> skyboxes;
public:
	SceneApp() {
        scene = new Scene_3D(*this, glm::vec3(0.2f));
        // sphere = new N3D::Icosphere(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		globe = new N3D::ColorfulIcosphere(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        // scene->AddObject(*sphere);
		scene->AddObject(*globe);

		skyboxes.push_back(new N3D::SkyBox("ame_ash", "ashcanyon"));
        skyboxes.push_back(new N3D::SkyBox("ame_emerald", "emeraldfog"));
        skyboxes.push_back(new N3D::SkyBox("ame_flatrock", "flatrock"));
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
		scene->SetSkybox(*skyboxes[0]);
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