#pragma once
#include "Core/Common.h"

class Input {
    GLFWwindow* window = nullptr;
public:
    Input(GLFWwindow* window);
    ~Input();
    
    bool IsKeyPressed(int keycode);
    bool IsKeyReleased(int keycode); 

    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonReleased(int button);
    float GetMouseX();
    float GetMouseY();
    std::pair<float, float> GetMousePosition();
};