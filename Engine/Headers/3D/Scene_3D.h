#pragma once
#include <functional>

#include "Core/Common.h"
#include "TerrainBlock.h"
#include "HeightMap.h"
#include "Core/IndexBuffer.h"
#include "Core/Texture.h"
#include "3D/RenderingLayer_3D.h"
#include "3D/Camera_3D.h"
#include "3D/3D_Objects.h"
#include "3D/SceneControl_3D.h"
#include "3D/CameraController_3D.h"
#include "Events/Event.h"
#include "Utilities/Maths.h"


// layers are owned by the layer stack
class Scene_3D {
private:
    RenderingLayer_3D* rendering_layer = nullptr;
    CameraController_3D* controller = nullptr;
    Camera_3D* camera = nullptr;
    SceneControl_3D* scene_control = nullptr;
    N3D::SkyBox* skybox = nullptr;

    std::vector<Object_3D*> objects;

    Font* font = nullptr;
public:
    Scene_3D(Application& app, glm::vec3 background_color = glm::vec3(0.0f)) {
        glm::vec3 camera_pos;
        camera_pos = spherical_to_cartesian_coordinates(300.0f, 0.0f, glm::radians(90.0f));

        camera = new Camera_3D(camera_pos);
        controller = new CameraController_3D(*camera);
        rendering_layer = new RenderingLayer_3D(*camera);
        scene_control = new SceneControl_3D(*camera);
        app.PushLayer(rendering_layer);
        app.PushLayer(controller);
        app.PushLayer(scene_control);

        font = new Font();

        rendering_layer->SetTextFont(*font);
        rendering_layer->SetBackgroundColor(background_color);
    }

    ~Scene_3D() {
        delete camera;
    }

    void SetSize(float width, float height) {
        camera->SetScreenSize(width, height);
    }

    glm::vec2 GetSize() {
        return glm::vec2(camera->GetScreenHeight(), camera->GetScreenHeight());
    }

    void SetSkybox(N3D::SkyBox& _skybox) {
        this->skybox = &_skybox;
        this->rendering_layer->SetSkybox(_skybox);
    }

    void SetBackGroundColor(glm::vec3 color) { rendering_layer->SetBackgroundColor(color); }
    glm::vec3 GetBackgroundColor() { return rendering_layer->GetBackgroundColor(); }

    void AddObject(Object_3D& obj) {
        rendering_layer->Add_3D_Object(obj);
        objects.push_back(&obj);
        scene_control->AddObject(obj);
    }

    void AddText(std::string txt, glm::vec2 pos, float scale, glm::vec3 color = glm::vec3(0.0f)) {
        rendering_layer->AddText(txt, pos, scale, color);
    }

    void Add3DText(std::string txt, glm::vec3 pos, float scale, glm::vec3 normal, glm::vec3 tangent, glm::vec3 color = glm::vec3(0.0f)) {
        Text_3D* t = new Text_3D(pos, scale, txt, *camera, normal, tangent, color);
        rendering_layer->AddText3D(*t);
    }
};