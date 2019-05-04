#include "Window/Input.h"

Input::Input(GLFWwindow* window) {
    this->window = window;
}

Input::~Input() { }

bool Input::IsKeyPressed(int keycode) {
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS;
}

bool Input::IsKeyReleased(int keycode) {
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button) {
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonReleased(int button) {
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_RELEASE;
}

float Input::GetMouseX() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return (float)x;
}

float Input::GetMouseY() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return (float)y;
}

std::pair<float, float> Input::GetMousePosition() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {(float)x, (float)y};
}
