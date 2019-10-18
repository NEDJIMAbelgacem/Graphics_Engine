#pragma once
#include <functional>
#include <vector>

#include "Layers/Layer.h"
#include "Logger.h"
#include "2D/Objects/Object_2D.h"
#include "2D/Camera_2D.h"
#include "Layers/BaseSceneController.h"

using namespace N3D;

// SceneViewControlLayer is responsible of handling mouse usage and propagating events to the scene objects
class SceneControl_2D : public BaseSceneController {
private:
    std::vector<Object_2D*> objects;
    Object_2D* grabbed_object = nullptr;
    bool is_object_grabbed = false;
    glm::vec2 grab_pos;
public:
	SceneControl_2D(AbstractCamera& camera) : BaseSceneController(camera, "SceneControl_2D") { }

    void AddObject(Object_2D& obj) {
        objects.push_back(&obj);
    }

    bool OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        if (is_object_grabbed) {
            is_object_grabbed = false;
            e.Handled = true;
        }
        else {
            glm::vec4 p = camera.GetViewMatrixInv() * glm::vec4(e.GetX(), e.GetY(), 0.0f, 1.0f);
            for (Object_2D* obj : objects) {
                if (obj->InBounds(p.x, p.y)) {
                    is_object_grabbed = true;
                    grabbed_object = obj;
                    break;
                }
            }
            if (is_object_grabbed) {
                e.Handled = true;
                grab_pos = glm::vec2(p.x, p.y);
            }
        }
        return true;
    }

    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        return true;
    }

    bool OnMouseMoved(MouseMovedEvent& e) {
        glm::vec4 p = camera.GetViewMatrixInv() * glm::vec4(e.GetX(), e.GetY(), 0.0f, 1.0f);
        if (is_object_grabbed) {
            grabbed_object->SetPosition(grabbed_object->GetPosition() - grab_pos + glm::vec2(p.x, p.y));
            grab_pos = glm::vec2(p.x, p.y);
            e.Handled = true;
        }
        return true;
    }
};