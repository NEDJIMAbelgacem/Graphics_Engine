#pragma once
#include "Core/Common.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class AbstractCamera {
public:
    virtual void MoveForward(float distance) { }
    virtual void MoveBackward(float distance) { }
    virtual void MoveUp(float distance) { }
    virtual void MoveDown(float distance) { }
    virtual void MoveRight(float distance) { }
    virtual void MoveLeft(float distance) { }

    virtual void ZoomIn(float zoom_factor) { }
    virtual void ZoomOut(float zoom_factor) { }

    virtual glm::mat4 GetViewMatrix() { return glm::identity<glm::mat4>(); }
    virtual glm::mat4 GetViewMatrixInv() { return glm::inverse(this->GetViewMatrix()); }
    virtual glm::mat4 GetProjectionMatrix() { return glm::identity<glm::mat4>(); }
    virtual void FillShader(ShaderProgram& prg) { }
};

class CameraController : public Layer {
public:
    AbstractCamera& camera;
    bool mouse_enabled = false;
    bool cursor_enabled = false;
    float mouse_x = -1, mouse_y = -1;
    float mouse_scroll_sensitivity = 1.05f;
    float mouse_move_sensitivity = 400.0f;

    bool button_pressed = false;
public:
	CameraController(AbstractCamera& _camera) : Layer("Camera layer"), camera(_camera) { }

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
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(CameraController::OnKeyPressed));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraController::OnMouseMoved));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraController::OnMouseScrolled));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(CameraController::OnMouseButtonPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(CameraController::OnMouseButtonReleased));
	}
};

class Camera_2D : public AbstractCamera {
private:
	glm::vec2 camera_pos;
	glm::vec2 camera_right = glm::vec2(-1.0f, 0.0f);
	glm::vec2 camera_up = glm::vec2(0.0f, -1.0f);
    float zoom_level = 1.0f;
    float screen_width, screen_height;
public:
	Camera_2D(float screen_width = WINDOW_WIDTH, float screen_height = WINDOW_HEIGHT, glm::vec2 position = glm::vec2(0.0f, 0.0f)) {
        this->screen_height = screen_height;
        this->screen_width = screen_width;
        this->camera_pos = position;
    }

	void SetPosition(glm::vec2 pos) { this->camera_pos = pos; }

    void ZoomIn(float zoom_factor) {
        zoom_level *= zoom_factor;
    }

    void ZoomOut(float zoom_factor) {
        zoom_level /= zoom_factor;
    }

    void MoveUp(float distance) {
        camera_pos += camera_up * distance;
    }

    void MoveDown(float distance) {
        camera_pos -= camera_up * distance;
    }

    void MoveRight(float distance) {
        camera_pos += camera_right * distance;
    }

    void MoveLeft(float distance) {
        camera_pos -= camera_right * distance;
    }

    inline glm::vec2 GetCameraPosition() { 
        // return glm::vec2(0.5f * screen_width + camera_pos.x, 0.5 * screen_height + camera_pos.y);
        return this->camera_pos;
    }
    
    inline void SetScreenSize(float width, float height) { this->screen_width = width; this->screen_height = height; }
    inline float GetScreenWidth() { return screen_width; }
    inline float GetScreenHeight() { return screen_height; }

    glm::mat4 GetViewMatrix() override {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(1.0f / zoom_level, 1.0f / zoom_level, 1.0f));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(camera_pos.x, camera_pos.y, 0.0f));
        return translation * scale;
    }

	glm::mat4 GetProjectionMatrix() override {
        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(2.0f / screen_width, -2.0f / screen_height, 1.0f));
        glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(-1.0f, 1.0f, 0.0f));
        glm::mat4 m = translation * scale;
        return m;
    }

    void FillShader(ShaderProgram& prg) override {
        glm::mat4 view_m = this->GetViewMatrix();
        glm::mat4 proj_m = this->GetProjectionMatrix();
        prg.FillUniformMat4f("u_view", view_m);
        prg.FillUniformMat4f("u_proj", proj_m);

    }
};