#pragma once

#include "Core/Common.h"
#include "Window/Window.h"
#include "Window/LayerStack.h"
#include "Window/Layer.h"
#include "Window/ImGuiLayer.h"
#include "Window/Events/Event.h"
#include "Window/Events/ApplicationEvent.h"

class Application {
private:
    static Application* s_Instance;

    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
    ImGuiLayer* m_ImGuiLayer;

    bool m_Running = true;
public:
    static float delta_time;
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *m_Window; }

    inline static Application& Get() { return *s_Instance; }
private:
    bool OnWindowClose(WindowCloseEvent& e);

    virtual bool OnMouseMoved(MouseMovedEvent& e) { return false; }
    virtual bool OnMouseScrolled(MouseScrolledEvent& e) { return false; }
    virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e) { return false; }
    virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e) { return false; }
    virtual bool OnKeyPressed(KeyPressedEvent& e) { return false; }
    virtual bool OnKeyReleased(KeyReleasedEvent& e) { return false; }
    virtual bool OnKeyTyped(KeyTypedEvent& e) { return false; }
    virtual bool OnWindowResize(WindowResizeEvent& e) { return false; }
    // TODO : implement other window events
};

// To be defined in CLIENT
Application* CreateApplication();