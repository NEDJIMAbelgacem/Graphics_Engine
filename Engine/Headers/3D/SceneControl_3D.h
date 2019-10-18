#pragma once
#include <functional>
#include <vector>

#include "Layers/Layer.h"
#include "Logger.h"
#include "3D/Objects/Object_3D.h"
#include "3D/Camera_3D.h"
#include "Layers/BaseSceneController.h"

using namespace N3D;

// SceneViewControlLayer is responsible of handling mouse usage and propagating events to the scene objects
class SceneControl_3D : public BaseSceneController {
private:
    glm::vec2 last_position;
    std::vector<Object_3D*> objects;
    bool is_pressed = false;
public:
	SceneControl_3D(AbstractCamera& camera) : BaseSceneController(camera, "SceneControl_2D") { }

    bool OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        if (!is_pressed) {
            is_pressed = true;
            last_position = glm::vec2(e.GetX(), e.GetY());
        } 
        return true;
    }

    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        if (is_pressed) {
            is_pressed = false;
        }
        return true;
    }

    bool OnMouseMoved(MouseMovedEvent& e) {
        if (is_pressed) {
            float dx = e.GetX() - last_position.x;
            float dy = e.GetY() - last_position.y;
            last_position = glm::vec2(e.GetX(), e.GetY());
            for (Object_3D* obj : objects) {
                glm::vec3 rotation = obj->GetRotation();
                rotation.y += dx * 0.01f;
                rotation.x += -dy * 0.01f;
                obj->SetRotation(rotation);
            }
        }
        return true;
    }

    void AddObject(Object_3D& obj) {
        this->objects.push_back(&obj);
    }
};