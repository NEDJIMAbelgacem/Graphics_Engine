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

#include <3D/CenteredPoint_CameraController.h>
#include <3D/CenteredPoint_SceneController.h>

#include "3D/Scene_3D.h"
#include "3D/3D_Objects.h"

#include "Font.h"

using namespace N3D;
using MineField = ColorfulIcosphere;

std::string png_path = "Resources/2k_earth_daymap.jpg";

enum class TileState {Hidden, Flagged, Visible};

class MineSweeper {
private:
    MineField& sphere;
    std::vector<Text_3D*> indicators;
    std::vector<TileState> tile_states;
    std::vector<bool> is_bomb;
    bool first_click = true;

    int BombNeighboursCount(int tile) {
        int count = 0;
        for (int i : sphere.GetSmallestTileNeighbours(tile)) {
            if (is_bomb[i]) count++;
        }
        return count;
    }

    std::function<void(int, int)> tile_clicked = [&](int mouse_button, int tile_index) -> void{
        if (mouse_button == GLFW_MOUSE_BUTTON_LEFT) {
            // toggle
            if (tile_states[tile_index] == TileState::Hidden) {
                if (first_click) {
                    first_click = false;
                    is_bomb[tile_index] = false;
                }
                tile_states[tile_index] = TileState::Visible;
            }
        } else if (mouse_button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (tile_states[tile_index] == TileState::Hidden) {
                tile_states[tile_index] = TileState::Flagged;
            }
            else if (tile_states[tile_index] == TileState::Flagged) {
                tile_states[tile_index] = TileState::Hidden;
            }
        }

        // 0 => blue / Flagged
        // 1 => green / Visible
        // 2 => red / Hidden
        switch(tile_states[tile_index]) {
            case TileState::Flagged:
                sphere.ChangleTriangleColor(tile_index, 0);
                indicators[tile_index]->SetText("F");
                break;
            case TileState::Hidden:
                sphere.ChangleTriangleColor(tile_index, 2);
                indicators[tile_index]->SetText("?");
                break;
            case TileState::Visible:
                sphere.ChangleTriangleColor(tile_index, 1);
                if (is_bomb[tile_index]) {
                    indicators[tile_index]->SetText("*");
                } else {
                    indicators[tile_index]->SetText(std::to_string(BombNeighboursCount(tile_index)));
                }
                break;
        }
    };
public:
    MineSweeper(Scene_3D<CenteredPoint_SceneController, CenteredPoint_CameraController>& scene, MineField& _sphere) : sphere(_sphere) {
        std::vector<glm::vec3> centers = sphere.GetTrianglesCenters();
        std::vector<glm::vec3> normals = sphere.GetTrianglesNormals();
        for (int i = 0; i < centers.size(); ++i) {
            tile_states.push_back(TileState::Hidden);
            is_bomb.push_back(rand() % 2 == 0);
            sphere.ChangleTriangleColor(i, 2);
        }

        for (int i = 0; i < centers.size(); ++i) {
            Text_3D* t = new Text_3D(centers[i], 0.5f, "?", normals[i]);
            scene.Add3DText(*t);
            indicators.push_back(t);
        }

        for (int i = 0; i < sphere.GetTilesCount(); ++i) {
            // sphere.AddTileAction(i, [&](int i) {
            //     std::string txt = indicators[i]->GetText();
            //     int n = (std::stoi(txt) + 1) % 10;
            //     indicators[i]->SetText(std::to_string(n));
            // });
            sphere.AddTileAction(i, tile_clicked);
        }
    }
};

class SceneApp : public Application {
private:
    Scene_3D<CenteredPoint_SceneController, CenteredPoint_CameraController>* scene = nullptr;
    // N3D::TexturedIcosphere* sphere = nullptr;
    MineField* globe = nullptr;
    N3D::Icosphere* sphere = nullptr;
    int selected_skybox = 0;
    std::vector<N3D::SkyBox*> skyboxes;
    std::vector<Text_3D*> indicators;
    MineSweeper* minesweeper = nullptr;
public:
	SceneApp() {
        scene = new Scene_3D<CenteredPoint_SceneController, CenteredPoint_CameraController>(*this, glm::vec3(0.2f));
        // sphere = new N3D::Icosphere(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        globe = new MineField(glm::vec3(0.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f));
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
        
        scene->AddKeyAction(GLFW_KEY_TAB, [&]() {
            selected_skybox = (selected_skybox + 1) % skyboxes.size();
            scene->SetSkybox(*skyboxes[selected_skybox]);
        });

        minesweeper = new MineSweeper(*scene, *globe);
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