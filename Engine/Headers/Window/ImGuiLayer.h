#pragma once

#include "Window/Layer.h"

#include "Window/ApplicationEvent.h"
#include "Window/KeyEvent.h"
#include "Window/MouseEvent.h"
#include "Window/Layer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();
private:
    float m_Time = 0.0f;
};