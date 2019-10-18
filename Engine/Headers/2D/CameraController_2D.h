#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "AbstractCamera.h"


class CameraController_2D : public Layer {
public:
    AbstractCamera& camera;
    bool mouse_enabled = false;
    bool cursor_enabled = false;
    float mouse_x = -1, mouse_y = -1;
    float mouse_scroll_sensitivity = 1.05f;
    float mouse_move_sensitivity = 400.0f;

    bool button_pressed = false;
public:
	CameraController_2D(AbstractCamera& _camera) : Layer("Camera layer"), camera(_camera) { }

    bool OnMouseMoved(MouseMovedEvent& e) {
        if (mouse_x == -1 || mouse_y == -1) {
            mouse_x = e.GetX();
            mouse_y = e.GetY();
            return true;
        }
        if (button_pressed) {
            float dx = e.GetX() - mouse_x;
            float dy = e.GetY() - mouse_y;
            camera.MoveDown(dy);
            camera.MoveLeft(dx);
        }
        // if (mouse_enabled) camera.ProcessMouseMove(e.GetX() - mouse_x, e.GetY() - mouse_y, Application::delta_time);
        mouse_x = e.GetX();
        mouse_y = e.GetY();
        return true;
    }

    bool OnMouseScrolled(MouseScrolledEvent& e) {
        if (e.GetYOffset() < 0) camera.ZoomOut(mouse_scroll_sensitivity);
        if (e.GetYOffset() > 0) camera.ZoomIn(mouse_scroll_sensitivity);
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
            // case GLFW_KEY_F1:
            //     mouse_enabled = !mouse_enabled;
            //     break;
            // case GLFW_KEY_LEFT_CONTROL:
            // case GLFW_KEY_RIGHT_CONTROL: {
            //     GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
            //     if (cursor_enabled) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            //     else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            //     cursor_enabled = !cursor_enabled;
            //     break;
            // }
            case GLFW_KEY_UP:
            case GLFW_KEY_W:
                camera.MoveUp(Application::delta_time * mouse_move_sensitivity);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                camera.MoveDown(Application::delta_time * mouse_move_sensitivity);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                camera.MoveRight(Application::delta_time * mouse_move_sensitivity);
                break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A:
                camera.MoveLeft(Application::delta_time * mouse_move_sensitivity);
                break;
        }
        return true;
    }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(CameraController_2D::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraController_2D::OnMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController_2D::OnMouseScrolled));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(CameraController_2D::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(CameraController_2D::OnMouseButtonReleased));
	}
};
