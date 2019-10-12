#pragma once
#include "Layers/Layer.h"

class CameraLayer : public Layer {
public:
    Camera* camera;

    bool mouse_enabled = false;
    bool cursor_enabled = false;
    float mouse_x = -1, mouse_y = -1;
public:
	CameraLayer(Camera* camera) : Layer("Camera layer") {
        this->camera = camera;
    }

    bool OnMouseMoved(MouseMovedEvent& e) {
        if (mouse_x == -1 || mouse_y == -1) {
            mouse_x = e.GetX();
            mouse_y = e.GetY();
        }
        if (mouse_enabled) camera->ProcessMouseMove(e.GetX() - mouse_x, e.GetY() - mouse_y, Application::delta_time);
        mouse_x = e.GetX();
        mouse_y = e.GetY();
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
                camera->ProcessKeyboard(GLFW_KEY_UP, Application::delta_time);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                camera->ProcessKeyboard(GLFW_KEY_DOWN, Application::delta_time);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                camera->ProcessKeyboard(GLFW_KEY_RIGHT, Application::delta_time);
                break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A:
                camera->ProcessKeyboard(GLFW_KEY_LEFT, Application::delta_time);
                break;
        }
        return true;
    }

	void OnEvent(Event& event) override {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(CameraLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraLayer::OnMouseMoved));
	}
};