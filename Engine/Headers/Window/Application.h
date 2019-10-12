#pragma once

#include "Core/Common.h"
#include "Window/Window.h"
#include "Layers/LayerStack.h"
#include "Layers/Layer.h"
#include "Layers/ImGuiActivationLayer.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

class Application {
private:
    static Application* instance;

    std::unique_ptr<Window> window;
    LayerStack layer_stack;
    ImGuiActivationLayer* imgui_layer;

    bool is_running = true;
public:
    static float delta_time;
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& GetWindow() { return *window; }

    inline static Application& Get() { return *instance; }
    inline static float GetFrameRate() { return 1.0f / delta_time; }
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