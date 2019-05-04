#pragma once
#include "Core/Common.h"
#include "Logger.h"

struct WindowProps {
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string& title, unsigned int width = 1280, unsigned int height = 720)
        : Title(title), Width(width), Height(height) { }
};

class Window {
private:
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
    };

    WindowData m_Data;
    GLFWwindow* m_Window;
public:
    Window(const WindowProps& props);
    ~Window();

    void OnUpdate();

    unsigned int GetWidth();
    unsigned int GetHeight();
    
    void SetEventCallback(const EventCallbackFn& callback);
    void SetVSync(bool enabled);
    bool IsVSync();
};