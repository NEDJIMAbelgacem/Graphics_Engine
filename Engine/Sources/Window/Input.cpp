#include "Window/Input.h"

bool Input::IsKeyPressed(int keycode) {
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS;
}

bool Input::IsKeyReleased(int keycode) {
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button) {
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonReleased(int button) {
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_RELEASE;
}

float Input::GetMouseX() {
    double x, y;
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    glfwGetCursorPos(window, &x, &y);
    return (float)x;
}

float Input::GetMouseY() {
    double x, y;
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    glfwGetCursorPos(window, &x, &y);
    return (float)y;
}

std::pair<float, float> Input::GetMousePosition() {
    double x, y;
    GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();
    glfwGetCursorPos(window, &x, &y);
    return {(float)x, (float)y};
}
