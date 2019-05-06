#pragma once
#include "Core/Common.h"
#include "Application.h"
#include "Window/Window.h"

class Input {
public:    
    static bool IsKeyPressed(int keycode);
    static bool IsKeyReleased(int keycode); 

    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonReleased(int button);
    static float GetMouseX();
    static float GetMouseY();
    static std::pair<float, float> GetMousePosition();
};