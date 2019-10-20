#pragma once
#include <functional>
#include <vector>

#include "Layers/Layer.h"
#include "Logger.h"
#include "3D/Objects/Object_3D.h"
#include "3D/Camera_3D.h"
#include "Layers/BaseSceneController.h"
#include "Utilities/Maths.h"

using namespace N3D;

// SceneViewControlLayer is responsible of handling mouse usage and propagating events to the scene objects
class SceneControl_3D : public BaseSceneController {
private:
    glm::vec2 last_position;
    std::vector<Object_3D*> objects;
    bool is_pressed = false;
    Camera_3D& cam;
public:
	SceneControl_3D(Camera_3D& camera) : cam(camera), BaseSceneController(camera, "SceneControl_2D") { }

    bool OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        if (!is_pressed && e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT) {
            is_pressed = true;
            last_position = glm::vec2(e.GetX(), e.GetY());
        } 
        if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT) {
            auto [origin, ray] = cam.GenerateRayFrom(e.GetX(), e.GetY());
            for (Object_3D* obj : objects) {
                if (obj->DoIntersect(origin, ray)) {
                    obj->HandleMousePressedEvent(e);
                }
            }
        }
        if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_MIDDLE) {
            glm::vec3 origin = glm::vec3(0.5f, 0.5f, -1.0f);
            glm::vec3 ray = glm::vec3(0.0f, 0.0f, 2.0f);
            float t = Ray_Plane_Intersection(origin, ray, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            glm::vec3 p = origin + t * ray;
            N3D_CORE_TRACE("{} -> {} {} {}", t, p.x, p.y, p.z);
        }
        return true;
    }

    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        if (is_pressed && e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT) {
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
        } else {
            auto [origin, ray] = cam.GenerateRayFrom(e.GetX(), e.GetY());
            for (Object_3D* obj : objects) {
                if (obj->DoIntersect(origin, ray)) {
                    obj->HandleMouseMovedEvent(e);
                }
            }
        }
        return true;
    }

    void AddObject(Object_3D& obj) {
        this->objects.push_back(&obj);
    }
};