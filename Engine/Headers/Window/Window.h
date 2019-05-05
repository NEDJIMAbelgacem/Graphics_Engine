#pragma once
#include "Core/Common.h"
#include "Logger.h"
#include "Window/Event.h"
#include "Window/ApplicationEvent.h"
#include "Window/KeyEvent.h"
#include "Window/MouseEvent.h"

struct WindowProps {
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(const std::string& title, unsigned int width = 1280, unsigned int height = 720)
        : Title(title), Width(width), Height(height) { }
};

class Window {
    using EventCallbackFn = std::function<void(Event&)>;
private:
    struct WindowData {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
    GLFWwindow* m_Window;
public:
    Window(const WindowProps& props);
    ~Window();
    inline GLFWwindow* GetNativeWindow() {return m_Window; }

    void OnUpdate();

    unsigned int GetWidth();
    unsigned int GetHeight();
    
    void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

    void SetVSync(bool enabled);
    bool IsVSync();
};