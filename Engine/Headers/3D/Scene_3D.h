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


// layers are owned by the layer stack
class Scene_3D {
private:
    RenderingLayer_3D* rendering_layer = nullptr;
    CameraController_3D* controller = nullptr;
    Camera_3D* camera = nullptr;
    SceneControl_3D* scene_control = nullptr;
    std::vector<Object_3D*> objects;
public:
    Scene_3D(Application& app, glm::vec3 background_color = glm::vec3(0.0f)) {
        camera = new Camera_3D(glm::vec3(0, 0, -300));
        controller = new CameraController_3D(*camera);
        rendering_layer = new RenderingLayer_3D(*camera);
        scene_control = new SceneControl_3D(*camera);
        app.PushLayer(rendering_layer);
        app.PushLayer(controller);
        app.PushLayer(scene_control);

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

    void SetBackGroundColor(glm::vec3 color) { rendering_layer->SetBackgroundColor(color); }
    glm::vec3 GetBackgroundColor() { return rendering_layer->GetBackgroundColor(); }

    void AddObject(Object_3D& obj) {
        // scene_control->AddObject(obj);
        rendering_layer->Add_3D_Object(obj);
        objects.push_back(&obj);
        scene_control->AddObject(obj);
    }
};