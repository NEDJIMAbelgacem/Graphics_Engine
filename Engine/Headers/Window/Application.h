#pragma once
#include "Core/Common.h"
#include "Window.h"
#include "Window/Event.h"
#include "Window/ApplicationEvent.h"
#include "Window/LayerStack.h"
#include "Window/Layer.h"
#include "Window/ImGuiLayer.h"

class Application {
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

    std::unique_ptr<Window> m_Window;
    LayerStack m_LayerStack;
    ImGuiLayer* m_ImGuiLayer;

    bool m_Running = true;
private:
    static Application* s_Instance;
};

// To be defined in CLIENT
//Application* CreateApplication();