#pragma once
#include <functional>

#include "Core/Common.h"
#include "TerrainBlock.h"
#include "HeightMap.h"
#include "Core/IndexBuffer.h"
#include "Core/Texture.h"
#include "GameObject.h"
#include "2D/RenderingLayer_2D.h"
#include "2D/Camera_2D.h"
#include "2D/2D_Objects.h"
#include "2D/SceneControl_2D.h"
#include "2D/CameraController_2D.h"
#include "Events/Event.h"


// layers are owned by the layer stack
class Scene_2D {
private:
    RenderingLayer_2D* rendering_layer = nullptr;
    CameraController_2D* controller = nullptr;
    Camera_2D* camera = nullptr;
    KeyBoardControlLayer* keyboard_layer = nullptr;
    SceneControl_2D* scene_control = nullptr;
    std::vector<Object_2D*> objects;
public:
    Scene_2D(Application& app, glm::vec3 background_color = glm::vec3(0.0f)) {
        camera = new Camera_2D;
        controller = new CameraController_2D(*camera);
        rendering_layer = new RenderingLayer_2D(*camera);
        keyboard_layer = new KeyBoardControlLayer;
        scene_control = new SceneControl_2D(*camera);
        app.PushLayer(rendering_layer);
        app.PushLayer(keyboard_layer);
        app.PushLayer(controller);
        app.PushLayer(scene_control);

        rendering_layer->SetBackgroundColor(background_color);
    }

    void SetSize(float width, float height) {
        camera->SetScreenSize(width, height);
    }

    glm::vec2 GetSize() {
        return glm::vec2(camera->GetScreenHeight(), camera->GetScreenHeight());
    }

    void SetBackGroundColor(glm::vec3 color) { rendering_layer->SetBackgroundColor(color); }
    glm::vec3 GetBackgroundColor() { return rendering_layer->GetBackgroundColor(); }

    ~Scene_2D() {
        delete camera;
    }

    void AddObject(Object_2D& obj) {
        scene_control->AddObject(obj);
        rendering_layer->Add2DObject(obj);
        objects.push_back(&obj);
    }
};