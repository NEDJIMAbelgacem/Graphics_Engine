#pragma once
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

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
#include "3D/FPS_SceneController.h"
#include "3D/FPS_CameraController.h"
#include "3D/CenteredPoint_SceneController.h"
#include "3D/CenteredPoint_CameraController.h"
#include "3D/3D_Objects.h"
#include "MineField.h"

#include "Font.h"

using namespace N3D;

// using SceneType = Scene_3D<FPS_SceneController, FPS_CameraController>;
using SceneType = Scene_3D<CenteredPoint_SceneController, CenteredPoint_CameraController>;

class SceneApp : public Application {
private:
    SceneType* scene = nullptr;
    int selected_skybox = 0;
    std::vector<N3D::SkyBox*> skyboxes;
    Icosphere* sphere = nullptr;
    N3D::Plane* hz_plane = nullptr;
    N3D::Plane* vt_plane = nullptr;
    N3D::Line* x_axis = nullptr;
    N3D::Line* y_axis = nullptr;
    N3D::Line* z_axis = nullptr;
public:
	SceneApp() {
        scene = new SceneType(*this, glm::vec3(0.2f));

        sphere = new Icosphere(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        hz_plane = new N3D::Plane(
            std::vector<glm::vec3>{
                glm::vec3(300, -100.0, 300.0f),
                glm::vec3(-300.0f, -100.0f, 300.0f),
                glm::vec3(-300.0f, -100.0f, -300.0f),
                glm::vec3(300.0f, -100.0f, -300.0f)
            }, glm::vec3(1.0f, 1.0f, 0.0f));
        vt_plane = new N3D::Plane(
            std::vector<glm::vec3>{
                glm::vec3(300, 300.0f, 0.0f),
                glm::vec3(-300.0f, 300.0f, 0.0f),
                glm::vec3(-300.0f, -300.0f, 0.0f),
                glm::vec3(300.0f, -300.0f, 0.0f)
            }, glm::vec3(0.0f, 1.0f, 1.0f));
        x_axis = new N3D::Line(glm::vec3(-300.0f, 0.0f, 0.0f), glm::vec3(300.0f, 0.0f, 0.0f), 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        y_axis = new N3D::Line(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f, 300.0f, 0.0f), 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        z_axis = new N3D::Line(glm::vec3(0.0f, 0.0f, -300.0f), glm::vec3(0.0f, 0.0f, 300.0f), 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        

        skyboxes.push_back(new N3D::SkyBox("ame_ash", "ashcanyon"));
        skyboxes.push_back(new N3D::SkyBox("ame_emerald", "emeraldfog"));
        skyboxes.push_back(new N3D::SkyBox("ame_flatrock", "flatrock"));
        skyboxes.push_back(new N3D::SkyBox("ame_oasis", "oasisnight"));
        skyboxes.push_back(new N3D::SkyBox("darkskies", "darkskies"));
        // skyboxes.push_back(new N3D::SkyBox("Resources/hdr_skyboxes/modern_buildings_2k.hdr"));
        // skyboxes.push_back(new N3D::SkyBox("ely_darkcity", "darkcity"));
        // skyboxes.push_back(new N3D::SkyBox("hw_entropic", "entropic"));
        // skyboxes.push_back(new N3D::SkyBox("lmcity", "lmcity"));
        // skyboxes.push_back(new N3D::SkyBox("mp_bromene", "bromene-bay"));
        // skyboxes.push_back(new N3D::SkyBox("mp_classm", "classmplanet"));
        // skyboxes.push_back(new N3D::SkyBox("mp_deception", "deception_pass"));
        // skyboxes.push_back(new N3D::SkyBox("mp_deviltooth", "devils-tooth"));
        // skyboxes.push_back(new N3D::SkyBox("mp_drakeq", "drakeq"));
        // skyboxes.push_back(new N3D::SkyBox("mp_midnight", "midnight-silence"));
        // skyboxes.push_back(new N3D::SkyBox("mp_po", "po"));
        // skyboxes.push_back(new N3D::SkyBox("mp_totality", "totality");
        // skyboxes.push_back(new N3D::SkyBox("mp_us", "urbansp"));
		scene->SetSkybox(*skyboxes[selected_skybox]);

        scene->AddText("YES", {10, 10}, 1, glm::vec3(0.0f, 1.0f, 0.0f));
        scene->AddObject(*sphere);
        scene->AddObject(*x_axis);
        scene->AddObject(*y_axis);
        scene->AddObject(*z_axis);
        // scene->AddObject(*hz_plane);
        // scene->AddObject(*vt_plane);
        
        scene->AddKeyAction(GLFW_KEY_TAB, [&]() {
            selected_skybox = (selected_skybox + 1) % skyboxes.size();
            scene->SetSkybox(*skyboxes[selected_skybox]);
        });
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