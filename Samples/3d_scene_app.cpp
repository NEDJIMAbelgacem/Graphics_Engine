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

#include "Font.h"

using namespace N3D;

std::string png_path = "Resources/2k_earth_daymap.jpg";

class SceneApp : public Application {
private:
	Scene_3D* scene = nullptr;
    N3D::TexturedIcosphere* sphere = nullptr;
	N3D::ColorfulIcosphere* globe = nullptr;
	int selected_skybox = 0;
	std::vector<N3D::SkyBox*> skyboxes;
    std::vector<Text_3D*> indicators;
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
		scene->SetSkybox(*skyboxes[4]);

        scene->AddText("YES", {10, 10}, 1, glm::vec3(0.0f, 1.0f, 0.0f));

        std::vector<glm::vec3> centers = globe->GetTrianglesCenters();
        std::vector<glm::vec3> normals = globe->GetTrianglesNormals();
        std::vector<glm::vec3> tangent = globe->GetTrianglesTangents();
        for (int i = 0; i < centers.size(); ++i) {
            Text_3D* t = new Text_3D(centers[i], 0.5f, "1", normals[i], tangent[i]);
            scene->Add3DText(*t);
            indicators.push_back(t);
        }

        N3D_CORE_TRACE("{} {}", centers.size(), indicators.size());

        scene->AddKeyAction(GLFW_KEY_TAB, [&]() {
            selected_skybox = (selected_skybox + 1) % skyboxes.size();
            scene->SetSkybox(*skyboxes[selected_skybox]);
        });

        for (int i = 0; i < globe->GetTilesCount(); ++i) {
            globe->AddTileAction(i, [&](int i) {
                std::string txt = indicators[i]->GetText();
                int n = (std::stoi(txt) + 1) % 10;
                indicators[i]->SetText(std::to_string(n));
                std::cerr << "---" << std::endl;
            });
        }
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