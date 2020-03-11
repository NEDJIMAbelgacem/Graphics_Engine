#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "3D/Camera_3D.h"

const float _SPEED = 3000.0f;
const float _SENSITIVITY = 50.0f;

class FPS_CameraController : public Layer {
public:
    Camera_3D& camera;
    bool mouse_enabled = false;
    bool cursor_enabled = false;
    float mouse_x = -1, mouse_y = -1;
    float movement_speed = 3000.0f;
    float mouse_move_sensitivity = 200.0f;

    bool button_pressed = false;
public:
    FPS_CameraController(Camera_3D& _camera) : Layer("Camera layer"), camera(_camera) { }

    bool OnMouseMoved(MouseMovedEvent& e) {
        if (mouse_x == -1 || mouse_y == -1) {
            mouse_x = e.GetX();
            mouse_y = e.GetY();
            return true;
        }
        if (button_pressed) {
            // float dx = e.GetX() - mouse_x;
            // float dy = e.GetY() - mouse_y;
            // camera.MoveDown(dy);
            // camera.MoveLeft(dx);
        }
        if (mouse_enabled) {
            float delta_x = e.GetX() - mouse_x;
            float delta_y = e.GetY() - mouse_y;
            float pitch_delta = delta_x / camera.GetScreenWidth();
            float yaw_delta = delta_y / camera.GetScreenHeight();
            camera.SetAngles(camera.GetPitch() + mouse_move_sensitivity * pitch_delta, camera.GetYaw() + mouse_move_sensitivity * yaw_delta);
        }
        
        mouse_x = e.GetX();
        mouse_y = e.GetY();
        return true;
    }

    bool OnMouseScrolled(MouseScrolledEvent& e) {
        // if (e.GetYOffset() < 0) camera.MoveForward(10.0f);
        // if (e.GetYOffset() > 0) camera.MoveForward(-10.0f);
        return true;
    }

    bool OnMouseButtonPressed(MouseButtonPressedEvent& e) {
        switch (e.GetMouseButton()) {
        case GLFW_MOUSE_BUTTON_LEFT:
            button_pressed = true;
            break;
        }
        return true;
    }

    bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) {
        switch (e.GetMouseButton()) {
        case GLFW_MOUSE_BUTTON_LEFT:
            button_pressed = false;
            break;
        }
        return true;
    }

    bool OnKeyPressed(KeyPressedEvent& e) {
        switch (e.GetKeyCode()) {
            case GLFW_KEY_F1:
                mouse_enabled = !mouse_enabled;
                break;
            case GLFW_KEY_LEFT_CONTROL:
            case GLFW_KEY_RIGHT_CONTROL: {
                GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
                if (cursor_enabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cursor_enabled = !cursor_enabled;
                break;
            }
            case GLFW_KEY_UP:
            case GLFW_KEY_W:
                // camera.MoveUp(Application::delta_time * mouse_move_sensitivity);
                camera.MoveForward(Application::delta_time * movement_speed);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                // camera.MoveDown(Application::delta_time * mouse_move_sensitivity);
                camera.MoveBackward(Application::delta_time * movement_speed);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                camera.MoveRight(Application::delta_time * movement_speed);
                break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A:
                camera.MoveLeft(Application::delta_time * movement_speed);
                break;
        }
        return true;
    }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(FPS_CameraController::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(FPS_CameraController::OnMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(FPS_CameraController::OnMouseScrolled));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(FPS_CameraController::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(FPS_CameraController::OnMouseButtonReleased));
	}
};
