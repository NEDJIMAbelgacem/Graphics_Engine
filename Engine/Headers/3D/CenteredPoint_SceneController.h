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
class CenteredPoint_SceneController : public BaseSceneController {
private:
    glm::vec2 last_position;
    std::vector<Object_3D*> objects;
    bool is_pressed = false;
    Camera_3D& cam;
public:
	CenteredPoint_SceneController(Camera_3D& camera) : cam(camera), BaseSceneController(camera, "SceneControl_2D") {
        camera.SetFront(-camera.GetPosition());
        camera.SetAngles(0, 0);
    }

    bool OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        if (!is_pressed && e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT) {
            is_pressed = true;
            last_position = glm::vec2(e.GetX(), e.GetY());
        } 
        if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT || e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT) {
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
            // N3D_CORE_TRACE("{} -> {} {} {}", t, p.x, p.y, p.z);
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
            dx /= cam.GetScreenWidth();
            float dy = e.GetY() - last_position.y;
            dy /= cam.GetScreenHeight();
            last_position = glm::vec2(e.GetX(), e.GetY());
            glm::vec3 pos = cam.GetPosition();
            // glm::vec3 v = cartesian_to_spherical_coordinates(pos.x, pos.y, pos.z);
            // float r = v.x;
            // float phi = v.y;
            // float theta = v.z;
            // if (pos != spherical_to_cartesian_coordinates(r, phi, theta)) {
            //     N3D_CORE_WARN("pos != sph(car(pos))");
            //     N3D_CORE_WARN("r : {}, phi : {}, theta : {}", r, phi, theta);
            //     N3D_CORE_WARN("y : {}, z : {}", pos.y, pos.z);
            // }
            // // phi += dx * 0.01f;
            // // if (glm::degrees(theta + dy * 0.01f) < 90.0f && glm::degrees(theta + dy * 0.01f) > -90.0f)
            //     theta += dx * 0.01f;
            // pos = spherical_to_cartesian_coordinates(r, phi, theta);

            glm::vec3 camera_front = cam.GetCameraFront();
            glm::vec3 camera_up = cam.GetCameraUp();

            const float scale_factor = 4.0f;

            float pitch = cam.GetPitch();
            float yaw = cam.GetYaw();
            glm::mat4 rotation_m = glm::identity<glm::mat4>();

            pitch -= scale_factor * dx;
            rotation_m = glm::rotate(glm::identity<glm::mat4>(), scale_factor * dx, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))) * rotation_m;
            std::cout << yaw << std::endl;
            if (glm::abs((int)glm::degrees(yaw - scale_factor * dy) % 360) < 60) {
                rotation_m = glm::rotate(glm::identity<glm::mat4>(), scale_factor * dy, glm::normalize(cam.GetCameraRight())) * rotation_m;
                yaw -= scale_factor * dy;
            }
            pos = rotation_m * glm::vec4(pos, 1.0f);
            cam.SetAngles(pitch, yaw);


            // glm::vec2 yx_polars = cartesian_to_polar_coordinates(pos.z, pos.y);
            // float r = yx_polars.x;
            // float theta = yx_polars.y;
            // theta += dy * 0.01f;
            // glm::vec2 p = polar_to_cartesian_coordinates(r, theta);
            // pos.z = p.x;
            // pos.y = p.y;

            // glm::vec2 yz_polars = cartesian_to_polar_coordinates(pos.x, pos.z);
            // r = yz_polars.x;
            // theta = yz_polars.y;
            // theta += dx * 0.01f;
            // p = polar_to_cartesian_coordinates(r, theta);
            // pos.x = p.x;
            // pos.z = p.y;

            cam.SetPosition(pos);
            glm::vec3 front = -pos;
            cam.SetFront(front);
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