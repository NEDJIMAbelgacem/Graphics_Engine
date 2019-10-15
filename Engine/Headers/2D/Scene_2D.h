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
#include "Layers/SceneViewControlLayer.h"
#include "Events/Event.h"

#define EventProcessingLambda(EventType) [&](##EventType& e) -> bool {                  \
    for (Object_2D* obj : objects) {                                                    \
        glm::mat4 view_matrix = camera->GetViewMatrix();                                \
        glm::mat4 inv_view_matrix = glm::inverse(view_matrix);;                         \
        glm::vec4 p = inv_view_matrix * glm::vec4(e.GetX(), e.GetY(), 0.0f, 1.0f);      \
        ##EventType e2{e};                                                              \
        e2.SetX(p.x);                                                                   \
        e2.SetY(p.y);                                                                   \
        e2.Handled = e.Handled;                                                         \
        if (obj->InBounds(e2.GetX(), e2.GetY())) {                                      \
            obj->ProcessEvent(e2);                                                      \
        }                                                                               \
        e.Handled = e2.Handled;                                                         \
        if (e.Handled) break;                                                           \
    }                                                                                   \
    return false;                                                                       \
}

// layers are owned by the layer stack
class Scene_2D {
private:
    RenderingLayer_2D* rendering_layer = nullptr;
    CameraController* controller = nullptr;
    Camera_2D* camera = nullptr;
    KeyBoardControlLayer* keyboard_layer = nullptr;
    SceneViewControlLayer* scene_control = nullptr;
    std::vector<Object_2D*> objects;
public:
    Scene_2D(Application& app, glm::vec3 background_color = glm::vec3(0.0f)) {
        camera = new Camera_2D;
        controller = new CameraController(*camera);
        rendering_layer = new RenderingLayer_2D(*camera);
        keyboard_layer = new KeyBoardControlLayer;
        scene_control = new SceneViewControlLayer(*camera);
        app.PushLayer(rendering_layer);
        app.PushLayer(keyboard_layer);
        app.PushLayer(controller);
        app.PushLayer(scene_control);

        // scene_control->AddMouseButtonPressedAction(EventProcessingLambda(MouseButtonPressedEvent));
        // scene_control->AddMouseButtonReleasedAction(EventProcessingLambda(MouseButtonReleasedEvent));
        // scene_control->AddMouseMovedAction(EventProcessingLambda(MouseMovedEvent));

        rendering_layer->SetBackgroundColor(background_color);
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