#pragma once
#include <functional>
#include <vector>

#include "Layers/Layer.h"
#include "Logger.h"
#include "2D/Objects/Object_2D.h"
#include "2D/Camera_2D.h"

using namespace N3D;

// TODO : fix events propagation system
// ouseMoved_actions are a function that takes a delta x, y values and returns a boolean which determines if the event will be consumed or propagated to lower layers
// SceneViewControlLayer is responsible of handling mouse usage and propagating events to a scene
class SceneViewControlLayer : public Layer {
private:
    AbstractCamera& camera;
    std::vector<Object_2D*> objects;
    Object_2D* grabbed_object = nullptr;
    bool is_object_grabbed = false;
    glm::vec2 grab_pos;
public:
	SceneViewControlLayer(AbstractCamera& _camera) : Layer("Scene view control layer"), camera(_camera) { }

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

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(SceneViewControlLayer::OnMouseMoved));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(SceneViewControlLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(SceneViewControlLayer::OnMouseButtonReleased));
	}
};