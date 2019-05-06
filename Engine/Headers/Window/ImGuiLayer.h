#pragma once

#include "Window/Layer.h"

#include "Window/Events/ApplicationEvent.h"
#include "Window/Events/KeyEvent.h"
#include "Window/Events/MouseEvent.h"

#include "Window/Layer.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

// TODO : make tghis layer accept his events only
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    virtual void OnEvent(Event& event) override;

    // virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    // virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
    // virtual bool OnKeyPressed(KeyPressedEvent& e);
    // virtual bool OnKeyReleased(KeyReleasedEvent& e);
    // virtual bool OnKeyTyped(KeyTypedEvent& e);
    // virtual bool OnMouseMoved(MouseMovedEvent& e);
    // virtual bool OnMouseScrolled(MouseScrolledEvent& e);

    void Begin();
    void End();
private:
    float m_Time = 0.0f;
};