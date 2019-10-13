#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Layers/Layer.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

class ImGuiActivationLayer : public Layer {
public:
    ImGuiActivationLayer();
    ImGuiActivationLayer(std::string name);
    ~ImGuiActivationLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    virtual void OnEvent(Event& event) override;

    void Begin();
    void End();
private:
    float m_Time = 0.0f;
};